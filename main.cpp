#include <iostream>
#include <dlfcn.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

int main(int argc, char *argv[])
{
    // Mono-Installationspfad (anpassen an lokale Mono-Installation)
    mono_set_dirs("/home/johannes/Playground/cppwithcsharp/momo-heaven/lib", "/home/johannes/Playground/cppwithcsharp/momo-heaven/etc"); //: contentReference[oaicite:13]{index=13}

    // mono_jit_set_aot_mode(MONO_AOT_MODE_LLVMONLY);
    mono_jit_set_aot_mode(MONO_AOT_MODE_HYBRID);

    // Mono-Laufzeit starten
    MonoDomain *domain = mono_jit_init("MainDomain");
    if (!domain)
    {
        std::cerr << "Failed to initialize Mono domain\n";
        return 1;
    }

    // // AOT-Bibliothek (.so) dynamisch laden
    // void *lib = dlopen("Class1.dll.so", RTLD_NOW | RTLD_LOCAL);
    // if (!lib)
    // {
    //     std::cerr << "dlopen failed: " << dlerror() << "\n";
    //     return 1;
    // }

    // // AOT-Info-Symbol finden und registrieren
    // void *aot_info = dlsym(lib, "mono_aot_module_Class1_info");
    // if (!aot_info)
    // {
    //     std::cerr << "AOT symbol not found\n";
    //     return 1;
    // }
    // // mono_aot_register_module((void **)aot_info); //: contentReference[oaicite:14]{index=14}
    // mono_aot_register_module(reinterpret_cast<void **>(aot_info));

    // Assembly laden
    MonoAssembly *assembly = mono_domain_assembly_open(domain, "Class1.exe");
    if (!assembly)
    {
        std::cerr << "Assembly Class1.exe nicht gefunden\n";
        return 1;
    }

    // Main-Methode der Assembly ausführen
    int exitcode = mono_jit_exec(domain, assembly, 0, nullptr); //: contentReference[oaicite:15]{index=15}
    mono_jit_cleanup(domain);
    return exitcode;
}
