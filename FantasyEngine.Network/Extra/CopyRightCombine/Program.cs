/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2025 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FantasyEngine
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] files = Directory.GetFiles(Environment.CurrentDirectory, "*.cs", SearchOption.AllDirectories);
            ParallelOptions options = new ParallelOptions();
            options.MaxDegreeOfParallelism = files.Length / 100 + 1;
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("/****************************************************************************");
            builder.AppendLine("THIS FILE IS PART OF Fantasy Engine PROJECT");
            builder.AppendLine("THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT");
            builder.AppendLine();
            builder.AppendLine("Copyright (c) 2022-2025 ChenYiZh");
            builder.AppendLine("https://space.bilibili.com/9308172");
            builder.AppendLine();
            builder.AppendLine("Permission is hereby granted, free of charge, to any person obtaining a copy");
            builder.AppendLine("of this software and associated documentation files (the \"Software\"), to deal");
            builder.AppendLine("in the Software without restriction, including without limitation the rights");
            builder.AppendLine("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
            builder.AppendLine("copies of the Software, and to permit persons to whom the Software is");
            builder.AppendLine("furnished to do so, subject to the following conditions:");
            builder.AppendLine();
            builder.AppendLine("The above copyright notice and this permission notice shall be included in all");
            builder.AppendLine("copies or substantial portions of the Software.");
            builder.AppendLine();
            builder.AppendLine("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
            builder.AppendLine("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
            builder.AppendLine("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
            builder.AppendLine("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
            builder.AppendLine("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
            builder.AppendLine("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
            builder.AppendLine("SOFTWARE.");
            builder.AppendLine("****************************************************************************/");
            string header = builder.ToString();
            Parallel.ForEach(files, options, file =>
            {
                string codes = File.ReadAllText(file);
                if (codes.StartsWith("using "))
                {
                    codes = header + codes;
                    File.WriteAllText(file, codes, Encoding.UTF8);
                }
            });
            Console.WriteLine("Finish.");
            Console.ReadLine();
        }
    }
}
