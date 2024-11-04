using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FantasticEngine.NativeAOT
{
    public class HelloAOT
    {
        [UnmanagedCallersOnly(EntryPoint = "HelloNativeAOT")]
        public static int HelloNativeAOT(int a, int b)
        {
            return a + b;
        }
    }
}
