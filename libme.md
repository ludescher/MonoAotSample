for dll in *.dll; do
  /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=full $dll
done