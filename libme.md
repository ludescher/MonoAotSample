# in your mono checkout or package root
for dll in mscorlib.dll System.dll System.Core.dll …; do
  /home/johannes/Playground/cppwithcsharp/momo-heaven/bin/mono --aot=llvm,llvmonly,stats $dll
done
