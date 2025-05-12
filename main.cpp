#include <iostream>
#include <dlfcn.h>
#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[])
{
    // Construct paths to Mono library and configuration directories.
    std::filesystem::path mono_lib_path = std::filesystem::current_path() / "../momo/lib";
    std::filesystem::path mono_etc_path = std::filesystem::current_path() / "../momo/etc";

    fprintf(stderr, "Mono-Lib-Path: %s\n", mono_lib_path.c_str());

    // Certain features of the runtime like Dll remapping depend on a configuration file, to load the configuration file, just add:
    mono_config_parse(NULL);
    // Which will load the Mono configuration file (typically /etc/mono/config).

    // Tell the Mono runtime where to find its “lib” and “etc” directories.
    // This is needed if Mono is not installed in a standard location.
    // The first argument is the path to Mono's library folder; the second is the path to its config directory.
    mono_set_dirs(mono_lib_path.c_str(), mono_etc_path.c_str());

    // Enable AOT (Ahead-Of-Time) compilation mode.
    // MONO_AOT_MODE_FULL means use only precompiled AOT code and disable the JIT compiler.
    mono_jit_set_aot_mode(MONO_AOT_MODE_FULL);

    // Initialize the Mono runtime and create a root execution domain named "MonoAotSample".
    // This sets up the Mono virtual machine and returns a MonoDomain* for subsequent API calls.
    MonoDomain *domain = mono_jit_init("MonoAotSample");

    if (!domain)
    {
        std::cerr << "Failed to initialize Mono domain\n";
        return 1;
    }

    // Load the managed assembly (Class1.dll) into the domain.
    // The assembly must be present in the current working directory or in Mono’s library path.
    MonoAssembly *assembly = mono_domain_assembly_open(domain, "Class1.dll");

    if (!assembly)
    {
        std::cerr << "Assembly Class1.dll not found\n";
        return 1;
    }

    // Get the MonoImage for the loaded assembly.
    // The MonoImage represents the metadata (types, methods, etc.) of the assembly and is needed to find classes.
    MonoImage *image = mono_assembly_get_image(assembly);

    if (!image)
    {
        std::cerr << "Cannot create an image from the assembly!\n";
        return 1;
    }

    // Look up the class named "Class1" in the namespace "MonoAotSample" within the assembly.
    // This returns a MonoClass* representing the managed class if found.
    MonoClass *klass = mono_class_from_name(image, "MonoAotSample", "Class1");

    if (!klass)
    {
        fprintf(stderr, "Can't find class \"MonoAotSample.Class1\" in assembly %s\n", mono_image_get_filename(image));
        return 1;
    }

    // Look up the method named "Foo" that takes 1 argument in the class we found.
    // mono_class_get_method_from_name searches the class's methods by name and parameter count.
    MonoMethod *foo_method = mono_class_get_method_from_name(klass, "Foo", 1); // 1 args

    if (!foo_method)
    {
        fprintf(stderr, "Can't find method \"Foo\" in class %s\n", mono_image_get_filename(image));
        return 1;
    }

    MonoArray *foo_args_array = mono_array_new(domain, mono_get_string_class(), 2);

    // Create MonoString objects (convert C strings to managed strings)
    MonoString *arg0 = mono_string_new(domain, "Hello");
    MonoString *arg1 = mono_string_new(domain, "World");

    // Set them into the array
    mono_array_set(foo_args_array, MonoString *, 0, arg0);
    mono_array_set(foo_args_array, MonoString *, 1, arg1);

    // Prepare arguments for the method invocation.
    // Since "Foo" takes one argument, we pass a pointer array with multiple string values.
    void *foo_args[1] = {foo_args_array};

    // Invoke the method.
    // - The first parameter is the MonoMethod* we found.
    // - The second is the "this" object (NULL for a static method).
    // - The third is an array of argument pointers (NULL or args if no parameters).
    // - The fourth is an exception return (NULL here to ignore exceptions).
    //
    // The call returns a MonoObject* which represents the return value (boxed if it’s a value type).
    // This call actually executes the managed code of Class1.Foo().
    MonoObject *foo_result = mono_runtime_invoke(foo_method, nullptr, foo_args, nullptr);

    // Unbox the return value. Since "Foo" returns an int (a value type),
    // mono_object_unbox returns a pointer to the raw int inside the MonoObject.
    // We then dereference to get the int value.
    int foo_return_value = *(int *)mono_object_unbox(foo_result);

    fprintf(stdout, "Mono return - Foo: %d\n", foo_return_value);

    // Look up the method named "Bar" that takes 0 arguments in the class we found.
    // mono_class_get_method_from_name searches the class's methods by name and parameter count.
    MonoMethod *bar_method = mono_class_get_method_from_name(klass, "Bar", 0); // 0 args

    if (!bar_method)
    {
        fprintf(stderr, "Can't find method \"Bar\" in class %s\n", mono_image_get_filename(image));
        return 1;
    }

    // Prepare arguments for the method invocation.
    // Since "Bar" takes no arguments, we pass a pointer array with a null pointer (or simply NULL).
    void *bar_args[1] = {nullptr};

    // Invoke the method.
    // - The first parameter is the MonoMethod* we found.
    // - The second is the "this" object (NULL for a static method).
    // - The third is an array of argument pointers (NULL or args if no parameters).
    // - The fourth is an exception return (NULL here to ignore exceptions).
    //
    // The call returns a MonoObject* which represents the return value (boxed if it’s a value type).
    // This call actually executes the managed code of Class1.Bar().
    MonoObject *bar_result = mono_runtime_invoke(bar_method, nullptr, bar_args, nullptr);

    // Unbox the return value. Since "Bar" returns an int (a value type),
    // mono_object_unbox returns a pointer to the raw int inside the MonoObject.
    // We then dereference to get the int value.
    int bar_return_value = *(int *)mono_object_unbox(bar_result);

    fprintf(stdout, "Mono return - Bar: %d\n", bar_return_value);

    // Clean up and shut down the Mono runtime and domain.
    // This should be done before exiting so that Mono can free resources.
    mono_jit_cleanup(domain);

    return 0;
}
