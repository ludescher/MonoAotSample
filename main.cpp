#include <iostream>
#include <dlfcn.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
// #include <mono/jit/driver.h> // für mono_main

extern "C" int mono_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    // Mono-Installationspfad (anpassen an lokale Mono-Installation)
    mono_set_dirs("/home/johannes/DEV/MonoAotSample/momo/lib", "/home/johannes/DEV/MonoAotSample/momo/etc"); //: contentReference[oaicite:13]{index=13}

    mono_jit_set_aot_mode(MONO_AOT_MODE_FULL);
    // mono_jit_set_aot_mode(MONO_AOT_MODE_LLVMONLY);
    // mono_jit_set_aot_mode(MONO_AOT_MODE_HYBRID);

    // Mono-Laufzeit starten
    MonoDomain *domain = mono_jit_init("MainDomain");
    if (!domain)
    {
        std::cerr << "Failed to initialize Mono domain\n";
        return 1;
    }

    // after mono_jit_init and before mono_domain_assembly_open:
    // void *lib = dlopen("Class1.exe.so", RTLD_NOW | RTLD_LOCAL);

    // if (!lib)
    // {
    //     std::cerr << "dlopen failed: " << dlerror() << "\n";
    //     return 1;
    // }

    // void *aot_info = dlsym(lib, "mono_aot_module_Class1_info");

    // if (!aot_info)
    // {
    //     std::cerr << "AOT symbol not found\n";
    //     return 1;
    // }

    // mono_aot_register_module(reinterpret_cast<void **>(aot_info));

    // Assembly laden
    // MonoAssembly *assembly1 = mono_domain_assembly_open(domain, "/home/johannes/Playground/cppwithcsharp/momo-heaven/lib/mono/4.5/mscorlib.dll");
    // if (!assembly1)
    // {
    //     std::cerr << "Assembly mscorlib.dll nicht gefunden\n";
    //     return 1;
    // }

    // MonoAssembly *assembly2 = mono_domain_assembly_open(domain, "/home/johannes/Playground/cppwithcsharp/momo-heaven/lib/mono/4.5/System.dll");
    // if (!assembly2)
    // {
    //     std::cerr << "Assembly System.dll nicht gefunden\n";
    //     return 1;
    // }

    // MonoAssembly *assembly3 = mono_domain_assembly_open(domain, "/home/johannes/Playground/cppwithcsharp/momo-heaven/lib/mono/4.5/System.Core.dll");
    // if (!assembly3)
    // {
    //     std::cerr << "Assembly System.Core.dll nicht gefunden\n";
    //     return 1;
    // }

    MonoAssembly *assembly = mono_domain_assembly_open(domain, "Class1.exe");
    if (!assembly)
    {
        std::cerr << "Assembly Class1.exe nicht gefunden\n";
        return 1;
    }

    // Main-Methode der Assembly ausführen
    int exitcode = mono_jit_exec(domain, assembly, 0, nullptr); //: contentReference[oaicite:15]{index=15}

    // int mono_argc = 2;
    // char *mono_argv[] = {(char *)"MyApp", (char *)"Class1.exe"};
    // int exitcode = mono_main(mono_argc, mono_argv);

    mono_jit_cleanup(domain);
    return exitcode;
}
