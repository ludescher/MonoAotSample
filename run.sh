#!/bin/sh

CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

cd $CURRENT_DIR;

export LD_LIBRARY_PATH="$CURRENT_DIR/momo/lib"
# export LD_LIBRARY_PATH="/usr/lib"

$CURRENT_DIR/momo/bin/mcs -target:library -out:Class1.dll Class1.cs
$CURRENT_DIR/momo/bin/mono --aot=full Class1.dll
$CURRENT_DIR/momo/bin/mono Class1.dll

g++ main.cpp -o MyApp \
  -I$CURRENT_DIR/momo/include/mono-2.0 \
  -L$CURRENT_DIR/momo/lib \
  -lmonosgen-2.0 -ldl -lstdc++

MONO_LOG_LEVEL=debug MONO_LOG_MASK=aot ./MyApp