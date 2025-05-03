extern void *mono_aot_module_Class1_info;
__attribute__((used)) void *_force_export_mono_aot_class1_info()
{
    return &mono_aot_module_Class1_info;
}

// gcc -shared -o libClass1.so Class1.dll.o export_aot.c -Wl,--export-dynamic -lmono-2.0
