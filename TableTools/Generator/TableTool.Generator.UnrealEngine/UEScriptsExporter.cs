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
using FantasyEngine.TableTool.Generator.UnrealEngine;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasyEngine.TableTool.Generator
{
    /// <summary>
    /// 导出UE脚本
    /// </summary>
    public class UEScriptsExporter : IScriptsExporter
    {
        internal static readonly string ExportName = "FantasyTable";
        /// <summary>
        /// 导出的项目路径
        /// </summary>
        private string _exportPath;
        /// <summary>
        /// 导出的项目路径
        /// </summary>
        public string ExportPath
        {
            get { return _exportPath; }
            set
            {
                _exportPath = value;
                ProjectName = Path.GetFileName(value);
            }
        }
        /// <summary>
        /// 项目名称
        /// </summary>
        public string ProjectName { set; get; }
        /// <summary>
        /// 清理目录再导出？
        /// </summary>
        public bool ClearFolder { get; set; }
        /// <summary>
        /// 创建管理器
        /// </summary>
        /// <param name="path">导出的项目路径</param>
        /// <param name="clearFolder">清理目录再导出？</param>
        public UEScriptsExporter(string path, bool clearFolder = true)
        {
            ExportPath = path;
            ClearFolder = clearFolder;
        }
        /// <summary>
        /// 创建脚本
        /// </summary>
        /// <param name="database"></param>
        public void Generate(IDatabase database)
        {
            string abstractPath = ExportPath + "/Source/" + ProjectName + "/{0}/Tables";
            string headerPath = string.Format(abstractPath, "Public");
            string bodyPath = string.Format(abstractPath, "Private");
            if (ClearFolder)
            {
                if (Directory.Exists(headerPath))
                {
                    Directory.Delete(headerPath, true);
                }
                if (Directory.Exists(bodyPath))
                {
                    Directory.Delete(bodyPath, true);
                }
            }
            if (!Directory.Exists(headerPath))
            {
                Directory.CreateDirectory(headerPath);
            }
            if (!Directory.Exists(bodyPath))
            {
                Directory.CreateDirectory(bodyPath);
            }
            headerPath += "/";
            bodyPath += "/";
            foreach (KeyValuePair<string, ITable> table in database)
            {
                UEHeaderBuilder headerBuilder = new UEHeaderBuilder();
                headerBuilder.Table = table.Value;
                headerBuilder.ProjectName = ProjectName;
                string tableName = "Table" + char.ToUpper(table.Value.Name[0]) + table.Value.Name.Substring(1);
                FileUtility.SaveFile(Path.Combine(headerPath, tableName + ".h"), headerBuilder.Build().ToString(), Encoding.UTF8);
                UEBodyBuilder bodyBuilder = new UEBodyBuilder();
                bodyBuilder.Table = table.Value;
                FileUtility.SaveFile(Path.Combine(bodyPath, tableName + ".cpp"), bodyBuilder.Build().ToString(), Encoding.UTF8);
            }

            UETableHeaderBuilder headersBuilder = new UETableHeaderBuilder();
            FileUtility.SaveFile(Path.Combine(headerPath, $"{ExportName}.h"), headersBuilder.Build().ToString(), Encoding.UTF8);

            UETableBodyBuilder bodysBuilder = new UETableBodyBuilder();
            bodysBuilder.ReleativePath = "Tables/";
            bodysBuilder.DataBase = database;
            FileUtility.SaveFile(Path.Combine(bodyPath, $"{ExportName}.cpp"), bodysBuilder.Build().ToString(), Encoding.UTF8);
        }
    }
}
