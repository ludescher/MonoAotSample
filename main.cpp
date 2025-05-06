#include <iostream>
#include <dlfcn.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[])
{
    std::filesystem::path mono_lib_path = std::filesystem::current_path() / "momo/lib";
    std::filesystem::path mono_etc_path = std::filesystem::current_path() / "momo/etc";

    mono_set_dirs(mono_lib_path.c_str(), mono_etc_path.c_str());

    mono_jit_set_aot_mode(MONO_AOT_MODE_FULL);

    MonoDomain *domain = mono_jit_init("MainDomain");

    if (!domain)
    {
        std::cerr << "Failed to initialize Mono domain\n";
        return 1;
    }

    MonoAssembly *assembly = mono_domain_assembly_open(domain, "Class1.dll");

    if (!assembly)
    {
        std::cerr << "Assembly Class1.dll nicht gefunden\n";
        return 1;
    }

    MonoImage *image = mono_assembly_get_image(assembly);

    if (!image)
    {
        std::cerr << "Cannot create a image off the assembly!\n";
        return 1;
    }

    MonoClass *klass = mono_class_from_name(image, "VitaGL", "Class1");

    if (!klass)
    {
        fprintf(stderr, "Can't find Class1 in assembly %s\n", mono_image_get_filename(image));
        return 1;
    }

    MonoMethod *method = mono_class_get_method_from_name(klass, "Peter", 0); // 0 args
    // MonoMethod *method = mono_class_get_method_from_name(klass, "Main", 1);  // 1 argument: string[] args

    if (!method)
    {
        fprintf(stderr, "Can't find Peter in assembly %s\n", mono_image_get_filename(image));
        return 1;
    }

    void *args[1] = {nullptr}; // or a properly created MonoArray* for Main

    MonoObject *result = mono_runtime_invoke(method, nullptr, args, nullptr);

    int return_value = *(int *)mono_object_unbox(result);

    fprintf(stdout, "Mono returned %d\n", return_value);

    mono_jit_cleanup(domain);

    return return_value;
}
