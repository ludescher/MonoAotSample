using System;

namespace MonoAotSample
{
    class Class1
    {
        public static int Foo(string[] args)
        {
            foreach (string arg in args)
            {
                Console.WriteLine("Foo: Hello from AOT C# code with arg: \"{0}\"!", arg);
            }

            return 4201;
        }

        public static int Bar()
        {
            Console.WriteLine("Bar: Hello from AOT C# code!");

            return 1024;
        }
    }
}
