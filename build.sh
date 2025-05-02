#!/bin/sh
# Verzeichnis der lokalen Mono-Installation (anpassen)
# MONO_ROOT="${MONO_ROOT:-$HOME/momo-heaven}"

export LD_LIBRARY_PATH=/home/johannes/Playground/cppwithcsharp/momo-heaven/lib:$LD_LIBRARY_PATH

# C#-Bibliothek kompilieren und AOT
# mcs -target:library -out:Class1.dll Class1.cs
# mono --aot=llvm,full Class1.dll

# C++-Anwendung kompilieren
# gcc main.cpp -o main \
#     -I"/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2.0" -I"/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2" \
#     -L"/home/johannes/Playground/cppwithcsharp/momo-heaven/lib" -lmonosgen-2.0 -ldl

g++ main.cpp -o MyApp \
  -I/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2.0 \
  -L/home/johannes/Playground/cppwithcsharp/momo-heaven/lib \
  -lmonosgen-2.0 -ldl -lstdc++
