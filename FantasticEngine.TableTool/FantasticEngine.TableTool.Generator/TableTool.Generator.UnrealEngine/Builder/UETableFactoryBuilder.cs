/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
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
using FantasticEngine.TableTool.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasticEngine.TableTool.Generator.UnrealEngine.Builder
{
    internal class UETableFactoryBuilder
    {
        public string ReleativePath { get; set; }

        public string ExportFullPath { get; set; }

        internal IDatabase DataBase { get; set; }

        public StringBuilder Build()
        {
            StringBuilder includes = new StringBuilder();
            StringBuilder registers = new StringBuilder();
            string offset = "\t";
            foreach (KeyValuePair<string, ITable> table in DataBase)
            {
                string tableName = char.ToUpper(table.Value.Name[0]) + table.Value.Name.Substring(1);
                includes.AppendLine($"#include \"{ReleativePath}Table{tableName}.h\"");
                registers.AppendLine($"{offset}Tables.Add(Singleton<UTable{tableName}>());");
            }
            string model = File.ReadAllText(Path.Combine(System.Environment.CurrentDirectory, $"templates/unreal/FTableFactory.txt"), Encoding.UTF8);
            return new StringBuilder(
                model
                .Replace("{0}", includes.ToString().TrimEnd('\n').TrimEnd('\r'))
                .Replace("{1}", registers.ToString().TrimEnd('\n').TrimEnd('\r')));
        }
    }
}
