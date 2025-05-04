Perfekt, danke. Ich überprüfe jetzt, wie man Mono mit LLVM korrekt für vollständigen AOT-Betrieb (`llvmonly`) konfiguriert, insbesondere:

* Welche `--aot`-Flags erlaubt/kombinierbar sind (z. B. `full` vs. `llvmonly`),
* Welche Presets die `lib/mono/4.5`-Verzeichnisse verhindern,
* Und wie `mscorlib.dll` korrekt AOT-kompiliert werden muss.

Ich gebe dir gleich Bescheid mit einem klaren Setup und ggf. alternativen Workarounds.


# AOT-Flag Combinations

Mono’s AOT compiler accepts a comma-separated list of options.  Common flags include `llvm`, `full`, `llvmonly`, and `static`.  For example, one can compile an assembly with LLVM and static linking via:

```
mono --aot=llvm,full,static MyAssembly.dll
```

* **`llvm`** – use the LLVM backend for code generation (slower compile, potentially faster code).
* **`full`** – do *full* AOT: generate all wrapper methods and trampolines needed so **no JIT is used at runtime** (enables the `--full-aot` runtime mode).
* **`llvmonly`** – like `full` AOT but **LLVM-only**: the Mono JIT backend is entirely disabled and only the LLVM-generated bitcode is used.  Specifying `llvmonly` *automatically implies* both `full` and `llvm` (so adding `full,llvmonly` is redundant).  Note: this mode is experimental.
* **`static`** – produce a relocatable object file (`.o` or `.s`) for embedding into a native executable.  When using `static`, the object’s AOT image must be registered with the runtime via `mono_aot_register_module(...)` (so that the code is found at runtime).

In practice, to embed an AOT image you might use something like:

```cmake
add_custom_command(OUTPUT Class1.dll.o
    COMMAND mono --aot=llvm,full,static Class1.dll
    DEPENDS Class1.dll)
```

This produces a `Class1.dll.o` which you link into your C++ binary.  Note that combining `full` and `llvmonly` on the CLI is unnecessary: `--aot=llvmonly` alone already does a full AOT with LLVM.

# Role of `/lib/mono/4.5` (BCL Directory)

Mono looks in `PREFIX/lib/mono/4.5` (on Linux) for the base class libraries of the .NET 4.x profile (e.g. **mscorlib.dll**, System.dll, etc).  These managed DLLs (and their AOT images) must be present for execution.  Even in full-AOT mode, Mono still loads the original DLL for metadata and to confirm versions.  If the `4.5` directory is missing or incomplete, you will get load errors such as: “The assembly mscorlib.dll was not found… It should have been installed in `…/mono/4.5`”.

**Why might `/lib/mono/4.5` be missing?**  By default a Mono build will compile all profiles (2.0, 3.5, 4.0, 4.5), but the `make install` step only installs the 2.0 profile unless you explicitly install the others.  For example, on macOS users have discovered that after `./configure; make`, the other profiles reside under `mcs/class/lib/net_4_5/…`, but `make install` placed only the 2.0 assemblies.  The solution is to install the missing profiles manually, e.g.:

```bash
cd mcs
make PROFILE=net_4_5 install
make PROFILE=net_3_5 install
make PROFILE=net_4_0 install
```

This ensures `/usr/local/lib/mono/4.5` (or your prefix’s `lib/mono/4.5`) is populated.  If you used a runtime preset like `fullaot_llvm`, double-check that the base libraries were actually installed or manually copy them into the `lib/mono/4.5` directory.

# AOT-Compiling mscorlib.dll and BCL for LLVM (`llvmonly` mode)

When running in **AOT-only (llvmonly)** mode, *all* code – including the core libraries – must be AOT-compiled with LLVM. In practice, you should AOT-compile **mscorlib.dll** and any other BCL assemblies you use, using the same flags. For example:

```bash
cd /home/user/momo-heaven/lib/mono/4.5
mono --aot=llvm,full,static mscorlib.dll
mono --aot=llvm,full,static System.dll
mono --aot=llvm,full,static System.Core.dll
…
```

Or, since llvmonly implies full, equivalently:

```bash
mono --aot=llvmonly,static mscorlib.dll
```

This generates the bitcode-based AOT images (e.g. `mscorlib.dll.so` or `.o`) for those assemblies.  At runtime, the domain will attempt to load the AOT module `mscorlib.dll.so`; if that image was not compiled with `llvmonly`, you will get an error like
“Failed to load AOT module ‘…/mono/4.5/mscorlib.dll.so’ while running in aot-only mode: not compiled with --aot=llvmonly.”
Thus, be sure to include `llvmonly` (or at least `llvm,full`) in the AOT compile flags for the BCL.  In static-embedding scenarios, each object’s module must be registered (e.g. via `mono_aot_register_module(mono_aot_module_mscorlib_info)`) so that the runtime can find the code.

# Embedding & Execution (`mono_jit_exec` vs `mono_main`)

In your C++ host, initialize the runtime in **LLVM-only mode** before opening assemblies. For example:

```c
mono_set_dirs("/path/to/momo-heaven/lib", "/path/to/momo-heaven/etc");
mono_jit_set_aot_mode(MONO_AOT_MODE_LLVMONLY);
MonoDomain *domain = mono_jit_init("MainDomain");
```

This sets `MONO_AOT_MODE_LLVMONLY`, which disables JIT and only uses LLVM-AOT code. At this point you can either use **`mono_jit_exec`** or call **`mono_main`**.  Both will ultimately run the managed `Main` method.  Using `mono_jit_exec(domain, assembly, argc, argv)` is fine for an embedded domain; it simply invokes the entry point of the opened assembly.  Alternatively, `mono_main(argc, argv)` behaves like the normal Mono executable driver (processing command-line options, etc.).

Neither approach is strictly *required* by llvmonly mode – the key is that the AOT mode is set before domain initialization and that all code is AOT-compiled.  In practice, many embedding examples use `mono_jit_exec` for simplicity.  If you use `static` linking of AOT modules, remember to register each module (including the BCL modules) with `mono_aot_register_module(...)`.  Once set up, the runtime will not generate any new code and will run entirely from your pre-generated LLVM-AOT images.

**Sources:** Mono’s documentation and man-pages on AOT explain the flags and modes.  In particular, note that `--aot=llvmonly` implies `llvm,full`.  Also, after building Mono you may need to install the 4.x profile libraries manually as shown above, otherwise `/lib/mono/4.5` will be missing its DLLs.
