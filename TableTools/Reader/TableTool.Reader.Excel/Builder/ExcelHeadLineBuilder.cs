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
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.TableTool.Reader.Excel.Builder
{
    /// <summary>
    /// 读取Excel表头
    /// </summary>
    internal class ExcelHeadLineBuilder
    {
        /// <summary>
        /// Excel加载接口
        /// </summary>
        public IExcelDataReader Reader { get; set; }
        /// <summary>
        /// 读取表头
        /// </summary>
        /// <returns></returns>
        public virtual Dictionary<int, IHeader> Build()
        {
            if (!Reader.Read())
            {
                return null;
            }
            int fieldCount = Reader.FieldCount;
            string[][] tHeaders = new string[fieldCount][];
            int dataLength = 4;//头上4列都是表头数据
            for (int i = 0; i < Reader.FieldCount; i++)
            {
                tHeaders[i] = new string[dataLength];
            }
            for (int i = 0; i < Reader.FieldCount; i++)
            {
                tHeaders[i][0] = Convert.ToString(Reader.GetValue(i));
            }
            for (int l = 1; l < dataLength; l++)
            {
                if (!Reader.Read())
                {
                    return null;
                }
                for (int i = 0; i < fieldCount; i++)
                {
                    tHeaders[i][l] = Convert.ToString(Reader.GetValue(i));
                }
            }
            Dictionary<int, IHeader> headers = new Dictionary<int, IHeader>();
            for (int i = 0; i < fieldCount; i++)
            {
                ExcelHeaderBuilder builder = new ExcelHeaderBuilder();
                builder.InValues = tHeaders[i];
                builder.Index = i;
                var header = builder.Build();
                if (header == null)
                {
                    return null;
                }
                headers.Add(i, header);
            }
            return headers;
        }
    }
}
