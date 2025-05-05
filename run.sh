#!/bin/sh

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

export LD_LIBRARY_PATH="$CURRENT_DIR/momo/lib"

$CURRENT_DIR/momo/bin/mcs -target:exe -out:Class1.exe Class1.cs
$CURRENT_DIR/momo/bin/mono --aot=full Class1.exe
$CURRENT_DIR/momo/bin/mono Class1.exe

g++ main.cpp -o MyApp \
  -I$CURRENT_DIR/momo/include/mono-2.0 \
  -L$CURRENT_DIR/momo/lib \
  -lmonosgen-2.0 -ldl -lstdc++

MONO_LOG_LEVEL=debug MONO_LOG_MASK=aot ./MyApp