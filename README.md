# Mono AOT Sample

This Sample demonstrates how to embed a fully AOT-compiled C# assembly into a C++ application using the Mono runtime. It includes the complete workflow: from installing Mono, compiling C# code with AOT, and building the final native application.

---

## Prerequisites

Before you begin, make sure your system meets the following requirements:

* A working installation of Mono
* CMake and a C++ compiler (e.g., g++)

---

## Setup Instructions

### 1. Install Mono

To build Mono from source, you need an existing Mono installation on your system. Follow the instructions on the official Mono site:

👉 [Mono Download Guide](https://www.mono-project.com/download/stable/#download-lin)

### 2. Prepare for Building from Source

Once Mono is installed, refer to the [official compilation guide](https://www.mono-project.com/docs/compiling-mono/linux/#debian-based-distributions) for detailed steps on installing all listed dependencies and packages you need!

### 3. Configure the Build

Run the configuration script provided in the Sample to prepare Mono and related components:

```bash
./configure.sh
```

🛠️ This script downloads and sets up the required build environment.

### 4. Build and Run the Sample

Once configuration is complete, build and run the Sample with:

```bash
./run.cmake.sh
```

This will:

* Compile the `Class1.cs` C# source file
* Apply full AOT compilation
* Build the `main.cpp` C++ application
* Produce a native binary named `MyApp`

---

## Sample Structure

```text
.
├── Class1.cs         # C# source file
├── CMakeLists.txt    # CMake configuration
├── configure.sh      # Script to set up Mono build environment
├── main.cpp          # C++ source file embedding Mono runtime
├── run.cmake.sh      # Script to build everything via CMake
```

---

## Notes

* This Sample assumes you are using Mono with full AOT compilation. LLVM integration can be added later once the basic flow is stable.
* Make sure `mono`, `mcs`, and `mono-sgen` are all accessible in your `PATH`.

---

## Explanation of Key Steps:

This walkthrough shows step-by-step how the Mono embedding API is used to initialize the runtime, load an AOT-compiled assembly, find and invoke a managed method, and clean up when done.

* **Setting the Mono directories:**
  `mono_set_dirs(lib, etc)` tells Mono where its runtime libraries and configuration files are located. This is necessary when embedding if Mono isn’t installed in its default system location.

* **AOT Mode:**
  `mono_jit_set_aot_mode(MONO_AOT_MODE_FULL)` configures the runtime to use full AOT (Ahead-Of-Time) mode. In this mode, the just-in-time compiler is disabled and Mono will only execute methods for which precompiled native code is available.

* **Initializing the Domain:**
  `mono_jit_init("MonoAotSample")` initializes the Mono engine and creates a “domain” (an application context) named "MonoAotSample". The returned `MonoDomain*` is used in all subsequent Mono API calls to refer to this execution environment.

* **Loading the Assembly:**
  `mono_domain_assembly_open(domain, "Class1.dll")` loads the managed DLL into the domain. Internally Mono reads the assembly metadata and code. If successful, it returns a `MonoAssembly*`.

* **Getting the Image:**
  `mono_assembly_get_image(assembly)` retrieves the `MonoImage*` for the loaded assembly. The image contains the definitions of namespaces, types (classes), methods, etc., which you will query to find classes and methods.

* **Finding the Class and Method:**
  `mono_class_from_name(image, "MonoAotSample", "Class1")` looks up the `Class1` class in the `MonoAotSample` namespace, returning a `MonoClass*` if found. Then `mono_class_get_method_from_name(klass, "Bar", 0)` finds the method named `Bar` with 0 parameters in that class.

* **Invoking the Method:**
  `mono_runtime_invoke(method, NULL, args, NULL)` calls the managed method. The second argument is `NULL` because `Bar` is assumed static (if it were an instance method, we would pass a `MonoObject*` instance). The `args` array holds arguments (none in this case) for the method. This returns a `MonoObject*` that represents the method’s return value.

* **Handling the Return Value:**
  If the managed method returns a value type (like `int`), Mono returns a boxed object. We use `mono_object_unbox(result)` to get a pointer to the raw `int` inside the object, then dereference it.

* **Cleanup:**
  Finally, `mono_jit_cleanup(domain)` shuts down the Mono runtime and frees resources. After this point, the domain cannot be reinitialized in the same process.
