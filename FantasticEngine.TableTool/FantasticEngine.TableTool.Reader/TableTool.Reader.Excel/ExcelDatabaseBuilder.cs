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
using FantasticEngine.TableTool.Framework.Common;
using FantasticEngine.TableTool.Reader.Excel.Builder;
using FantasticEngine.TableTool.Reader.Excel.Common;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace FantasticEngine.TableTool.Reader.Excel
{
    /// <summary>
    /// 加载Excel数据
    /// </summary>
    public class ExcelDatabaseBuilder : DatabaseBuilder
    {
        /// <summary>
        /// 读取Excel数据
        /// </summary>
        protected override IDatabase Make(string fullPath)
        {
            if (!Directory.Exists(fullPath))
            {
                return null;
            }
            string[] files = Directory.GetFiles(fullPath, "*.xlsx", SearchOption.TopDirectoryOnly);
            ConcurrentDictionary<string, ITable> databaseAsync = new ConcurrentDictionary<string, ITable>();
            foreach (string file in files)
            {
                databaseAsync.TryAdd(Path.GetFileNameWithoutExtension(file), null);
            }
            Parallel.ForEach(files, (file) =>
            {
                ExcelTableBuilder builder = new ExcelTableBuilder(file);
                databaseAsync[Path.GetFileNameWithoutExtension(file)] = builder.Build();
            });

            ExcelDatabase database = new ExcelDatabase();
            database.FullPath = fullPath;
            foreach (KeyValuePair<string, ITable> kv in databaseAsync)
            {
                database.Add(kv.Key, kv.Value);
            }
            return database;
        }
    }
}
