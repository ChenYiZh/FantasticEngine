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
using FantasticEngine.TableTool.Framework.Utilities;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace FantasticEngine.TableTool.Generator.UnrealEngine
{
    /// <summary>
    /// 头文件先生成一些必要数据
    /// </summary>
    internal class UEHeaderBuilder
    {
        internal ITable Table { get; set; }

        public string ProjectName { get; set; }

        public StringBuilder Build()
        {
            IEnumerable<IHeader> headers = Table.Headers.Values.Where(h => HeaderUtility.IsValid(h, ETarget.CLIENT)).OrderBy(h => h.Index);
            string offset = "\t";

            string tableName = char.ToUpper(Table.Name[0]) + Table.Name.Substring(1);

            StringBuilder privateProperties = new StringBuilder();
            StringBuilder publicProperties = new StringBuilder();
            bool isFirst = true;
            foreach (IHeader header in headers)
            {
                string name = header.Name;// NameUtility.GetDefaultPropertyName(header.Name);//char.ToUpper(header.Name[0]) + header.Name.Substring(1);
                EHeaderType headerType = isFirst ? EHeaderType.INT : header.Type;
                isFirst = false;
                bool canUseUEAttribute = !UEConverter.IgnoreProperty(headerType);
                string description = header.Description.Replace("\n", $"; ").Replace("\r", "");
                privateProperties.AppendLine($"{offset}/**");
                privateProperties.AppendLine($"{offset} * {description}");
                privateProperties.AppendLine($"{offset} */");
                if (canUseUEAttribute)
                {
                    privateProperties.AppendLine($"{offset}UPROPERTY()");
                }
                privateProperties.AppendLine($"{offset}{UEConverter.GetTypeString(headerType)} {name};");

                publicProperties.AppendLine($"{offset}/**");
                publicProperties.AppendLine($"{offset} * {description}");
                publicProperties.AppendLine($"{offset} */");
                if (canUseUEAttribute)
                {
                    publicProperties.AppendLine($"{offset}UFUNCTION(BlueprintPure,Category = \"Tables|Table{tableName}\", DisplayName = \"{name}\")");
                }
                publicProperties.AppendLine($"{offset}{UEConverter.GetTypeString(headerType)} Get{name}() const {{ return {name}; }}");
                publicProperties.AppendLine();
            }
            string model = FileUtility.ReadFileText(Path.Combine(System.Environment.CurrentDirectory, "templates/unreal/Header.txt"), Encoding.UTF8);
            return new StringBuilder(
                model
                .Replace("{0}", tableName)
                .Replace("{1}", privateProperties.ToString().TrimEnd('\n').TrimEnd('\r'))
                .Replace("{2}", publicProperties.ToString().TrimEnd('\n').TrimEnd('\r'))
                .Replace("{3}", string.IsNullOrEmpty(ProjectName) ? "" : ProjectName.ToUpper() + "_API"));
        }
    }
}
