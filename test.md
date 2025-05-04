export LD_LIBRARY_PATH=/home/johannes/Playground/cppwithcsharp/momo-heaven/lib
/home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono Class1.exe

export LD_LIBRARY_PATH=/home/johannes/Playground/cppwithcsharp/momo-heaven/lib
g++ main.cpp -o MyApp \
  -I/home/johannes/Playground/cppwithcsharp/momo-heaven/include/mono-2.0 \
  -L/home/johannes/Playground/cppwithcsharp/momo-heaven/lib \
  -lmonosgen-2.0 -ldl -lstdc++

export LD_LIBRARY_PATH=/home/johannes/Playground/cppwithcsharp/momo-heaven/lib
MONO_LOG_LEVEL=debug MONO_LOG_MASK=aot ./MyApp Class1.exe &> app.log 