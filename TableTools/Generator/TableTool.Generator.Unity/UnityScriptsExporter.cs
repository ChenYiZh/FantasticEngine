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
using FantasyEngine.TableTool.Generator.Unity;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace FantasyEngine.TableTool.Generator
{
    /// <summary>
    /// Unity的脚本输出
    /// </summary>
    public class UnityScriptsExporter : IScriptsExporter
    {
        /// <summary>
        /// 输出的脚本类型
        /// </summary>
        public ETarget Target { get; set; }
        /// <summary>
        /// 输出的路径
        /// </summary>
        public string ExportPath { get; set; }

        /************* 模板文本 ***************/
        string _clientGameTablesTxt;
        string _serverGameTablesTxt;
        string _tableManagerTxt;
        string _wholeTableTxt;
        string _tableDataTxt;
        string _tableDataPropertyTxt;
        string _tableBodyTxt;
        /**************************************/
        public UnityScriptsExporter(ETarget target, string path)
        {
            Target = target;
            ExportPath = path;
            _clientGameTablesTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/ClientGameTables.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _serverGameTablesTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/ServerGameTables.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _tableManagerTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/TableManager.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _wholeTableTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/WholeTable.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _tableDataTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/TableData.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _tableDataPropertyTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/TableDataProperty.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
            _tableBodyTxt = FileUtility.ReadFileText(Path.Combine(Environment.CurrentDirectory, "templates/unity/TableBody.txt"), Encoding.UTF8).Replace("{", "{{").Replace("}", "}}").Replace("{{{{", "{").Replace("}}}}", "}");
        }
        public void Generate(IDatabase database)
        {
            StringBuilder initList = new StringBuilder();
            StringBuilder tableList = new StringBuilder();
            int index = 0;
            foreach (var table in database)
            {
                if (++index == database.Count)
                {
                    initList.Append(string.Format("\t\tTable{0}.Instance.Initialize();", table.Value.Name));
                }
                else
                {
                    initList.AppendLine(string.Format("\t\tTable{0}.Instance.Initialize();", table.Value.Name));
                }
                StringBuilder properties = new StringBuilder();
                StringBuilder makeProperties = new StringBuilder();
                IHeader[] headers = table.Value.Headers.Values.Where(h => HeaderUtility.IsValid(h, Target)).OrderBy(h => h.Index).ToArray();
                for (int i = 0; i < headers.Length; i++)
                {
                    var header = headers[i];
                    properties.AppendLine(string.Format(_tableDataPropertyTxt, header.Name, header.Description, header.Target, UnityConverter.GetTypeName(header.Type), header.Name));
                    string localName = "_" + header.Name.ToLower()[0] + header.Name.Substring(1);
                    makeProperties.AppendLine(string.Format("\t\t{0} {1};", UnityConverter.GetTypeName(header.Type), localName));
                    string convertStr = string.Format(UnityConverter.GetConvertStr(header.Type), i, localName);
                    makeProperties.AppendLine(string.Format("\t\t_theResult = {0} && _theResult;", convertStr, localName));
                    if (i == headers.Length - 1)
                    {
                        makeProperties.Append(string.Format("\t\tthis.{0} = {1};", header.Name, localName));
                    }
                    else
                    {
                        makeProperties.AppendLine(string.Format("\t\tthis.{0} = {1};", header.Name, localName));
                    }
                }
                string dataBody = string.Format(_tableDataTxt, table.Key, table.Value.Name, properties.ToString(), makeProperties.ToString());
                string tableBady = string.Format(_tableBodyTxt, table.Key, table.Value.Name);
                tableList.AppendLine(string.Format(_wholeTableTxt, table.Key, dataBody, tableBady));
                if (index != database.Count) { tableList.AppendLine(); }
            }
            string tableManager = string.Format(_tableManagerTxt, initList.ToString());
            string wholeText;
            if (Target == ETarget.CLIENT)
            {
                wholeText = string.Format(_clientGameTablesTxt, tableManager, tableList.ToString());
            }
            else
            {
                wholeText = string.Format(_serverGameTablesTxt, "\t" + tableManager.Replace("\n", "\n\t"), "\t" + tableList.ToString().Replace("\n", "\n\t"));
            }
            FileUtility.SaveFile(ExportPath, wholeText, Encoding.UTF8);
        }
    }
}
