/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
using FantasyEngine.TableTool.Framework;
using FantasyEngine.TableTool.Framework.Utilities;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace FantasyEngine.TableTool.Generator.UnrealEngine
{
    /// <summary>
    /// CPP文件生成一些必要字段
    /// </summary>
    internal class UEBodyBuilder
    {
        internal ITable Table { get; set; }

        public StringBuilder Build()
        {
            IEnumerable<IHeader> headers = Table.Headers.Values.Where(h => HeaderUtility.IsValid(h, ETarget.CLIENT)).OrderBy(h => h.Index);
            string offset = "\t";

            StringBuilder functions = new StringBuilder();
            int index = 0;
            bool isFirst = true;
            foreach (IHeader header in headers)
            {
                string name = header.Name;// NameUtility.GetDefaultPropertyName(header.Name);// char.ToUpper(header.Name[0]) + header.Name.Substring(1);
                EHeaderType headerType = isFirst ? EHeaderType.INT : header.Type;
                isFirst = false;
                string line = string.Format(UEConverter.GetMethodString(headerType),/* "m" +*/ name, "Line[" + index++ + "]");
                functions.AppendLine($"{offset}{line}");
            }
            string model = FileUtility.ReadFileText(Path.Combine(System.Environment.CurrentDirectory, "templates/unreal/Body.txt"), Encoding.UTF8);
            string tableName = char.ToUpper(Table.Name[0]) + Table.Name.Substring(1);
            return new StringBuilder(
                model
                .Replace("{0}", tableName)
                .Replace("{1}", Table.Name)
                .Replace("{2}", functions.ToString().TrimEnd('\n').TrimEnd('\r')));
        }
    }
}
