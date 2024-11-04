using System;
using System.Runtime.InteropServices;

namespace FantasticEngine.Test
{
    class Program
    {
        [DllImport("FantasticEngine_common.dll", EntryPoint = "Add", CallingConvention = CallingConvention.StdCall)]//, CharSet = CharSet.Ansi
        extern static int Add(int a, int b);
        static void Main(string[] args)
        {
            Console.WriteLine(Add(5, 20));
            Console.Read();
        }
    }
}
