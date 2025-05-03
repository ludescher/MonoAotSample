using System;
using System.Threading;

class Class1
{
    [STAThread]
    public static int Main(string[] args)
    {
        Console.WriteLine("Hello from AOT C# code!");

        return 42;
    }
}
