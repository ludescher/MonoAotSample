#!/bin/sh
# Verzeichnis der lokalen Mono-Installation (anpassen)
# MONO_ROOT="${MONO_ROOT:-$HOME/momo-heaven}"

export LD_LIBRARY_PATH=/home/johannes/Playground/cppwithcsharp/momo-heaven/lib:$LD_LIBRARY_PATH

# C#-Bibliothek kompilieren und AOT
# mcs -target:library -out:Class1.dll Class1.cs
# mono --aot=llvm,full Class1.dll

# UPDATED COMMANDS
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mcs -target:library Class1.cs
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full, Class1.dll
# gcc -shared -o libClass1.so Class1.dll.o -Wl,--export-dynamic -lmono-2.0

# C++-Anwendung kompilieren
# gcc main.cpp -o main \
#     -I"/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2.0" -I"/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2" \
#     -L"/home/johannes/Playground/cppwithcsharp/momo-heaven/lib" -lmonosgen-2.0 -ldl

g++ main.cpp -o MyApp \
  -I/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2.0 \
  -L/home/johannes/Playground/cppwithcsharp/momo-heaven/lib \
  -lmonosgen-2.0 -ldl -lstdc++

# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,autoreg Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,print-skipped-methods Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,bind-to-runtime-version Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,llvmonly Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,noload Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,mono_install_load_aot_data_hook Class1.dll

# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,llvmonly,bind-to-runtime-version,print-skipped-methods,stats,write-symbols,no-opt,verbose Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,static,llvmonly,bind-to-runtime-version,print-skipped-methods,stats,write-symbols,verbose Class1.dll
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=full Class1.dll

# nodebug Disables the generation of debug information. Using this will typically yield a smaller, leaner output.

# mono_aot_module_Class1_info


# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mcs -target:exe -out:Class1.exe Class1.cs
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,hybrid,llvmonly Class1.exe
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,hybrid Class1.exe


# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mcs -target:exe -out:Class1.exe Class1.cs
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,full,llvmonly Class1.exe

# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mcs -target:exe -out:Class1.exe Class1.cs
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=full Class1.exe


mono --aot=llvm,full,static mscorlib.dll
mono --aot=llvm,full,static System.dll
mono --aot=llvm,full,static System.Core.dll

# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mcs -target:exe -out:Class1.exe Class1.cs
# /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=full Class1.exe

# 1a) Compile your C# into Class1.exe
mcs -target:exe -out:Class1.exe Class1.cs

# 1b) AOT‑compile mscorlib (so that corlib methods exist in AOT image)
mono --aot=full,asmonly /usr/lib/mono/4.5/mscorlib.dll

# 1c) AOT‑compile your own assembly
mono --aot=full,asmonly Class1.exe

# ,direct-icalls
# hybridaot_llvm