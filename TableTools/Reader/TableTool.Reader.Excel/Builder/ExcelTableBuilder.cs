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
using ExcelDataReader;
using FantasyEngine.TableTool.Framework;
using FantasyEngine.TableTool.Reader.Excel.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasyEngine.TableTool.Reader.Excel.Builder
{
    /// <summary>
    /// Excel单表加载逻辑
    /// </summary>
    internal class ExcelTableBuilder
    {
        /// <summary>
        /// Excel表路径
        /// </summary>
        private string _fullPath = null;

        /// <summary>
        /// Excel单表加载逻辑
        /// </summary>
        /// <param name="fullPath">Excel表路径</param>
        public ExcelTableBuilder(string fullPath)
        {
            _fullPath = fullPath;
        }

        public ITable Build()
        {
            ExcelTable table = new ExcelTable();
            table.Name = Path.GetFileNameWithoutExtension(_fullPath);
            try
            {
                using (IExcelDataReader reader = ExcelReaderFactory.CreateReader(FileUtility.OpenRead(_fullPath)))
                {
                    reader.Reset();
                    ExcelHeadLineBuilder builder = new ExcelHeadLineBuilder();
                    builder.Reader = reader;
                    Dictionary<int, IHeader> headers = builder.Build();
                    table.Headers = headers;
                    int lineIndex = 0;
                    while (reader.Read())
                    {
                        ExcelTableLineBuilder lineBuilder = new ExcelTableLineBuilder();
                        lineBuilder.Reader = reader;
                        lineBuilder.Table = table;
                        lineBuilder.Line = lineIndex++;
                        ITableLine line = lineBuilder.Build();
                        if (line != null)
                        {
                            table.Add(lineBuilder.Line, line);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Load '{_fullPath}' failed.");
                throw e;
            }
            return table;
        }
    }
}
