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
using FantasyEngine.TableTool.Framework.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace FantasyEngine.TableTool.Framework
{
    /// <summary>
    /// 导出加载的数据
    /// </summary>
    public class DatabaseExporter
    {
        /// <summary>
        /// 不含BOM
        /// </summary>
        public readonly static Encoding UTF8Encoding = new UTF8Encoding(false);
        /// <summary>
        /// 导出路径
        /// </summary>
        public string ExportFullPath { get; set; }
        /// <summary>
        /// 拓展名
        /// </summary>
        public string Extension { get; set; }
        /// <summary>
        /// 是否清空目录再创建
        /// </summary>
        public bool ClearFolder { get; set; }
        /// <summary>
        /// 目标平台
        /// </summary>
        public ETarget Target { get; set; }
        /// <summary>
        /// 首字母大写？
        /// </summary>
        public bool ForceUpFirstChar { get; set; } = true;
        /// <summary>
        /// 创建导出器
        /// </summary>
        /// <param name="exportFullPath">导出路径</param>
        /// <param name="extension">拓展名</param>
        /// <param name="clearFolder">是否清空目录再创建</param>
        public DatabaseExporter(string exportFullPath, string extension, bool clearFolder)
        {
            ClearFolder = clearFolder;
            ExportFullPath = exportFullPath;
            if (!ExportFullPath.EndsWith("\\"))
            {
                ExportFullPath += "\\";
            }
            Extension = extension;
            if (!Extension.StartsWith("."))
            {
                Extension = "." + Extension;
            }
        }

        /// <summary>
        /// 导出操作
        /// </summary>
        public void Export(IDatabase database, params TableContextBuilder[] builders)
        {
            if (builders.Length == 0) { return; }
            if (!Directory.Exists(ExportFullPath))
            {
                Directory.CreateDirectory(ExportFullPath);
            }
            if (ClearFolder)
            {
                string[] files = Directory.GetFiles(ExportFullPath);
                foreach (string file in files)
                {
                    if (file.EndsWith(Extension))
                    {
                        FileUtility.Delete(file);
                    }
                }
            }
            Parallel.ForEach(database, (KeyValuePair<string, ITable> table) =>
            {
                StringBuilder context = new StringBuilder();
                foreach (TableContextBuilder tbuilder in builders)
                {
                    TableContextBuilder builder = (TableContextBuilder)Activator.CreateInstance(tbuilder.GetType());
                    builder.Context = context;
                    builder.Table = table.Value;
                    builder.Target = Target;
                    builder.Build();
                }
                string tableName = ForceUpFirstChar ? char.ToUpper(table.Value.Name[0]) + table.Value.Name.Substring(1) : table.Value.Name;
                FileUtility.SaveFile(Path.Combine(ExportFullPath, tableName + Extension), context.ToString(), UTF8Encoding);
            });
        }
    }
}
