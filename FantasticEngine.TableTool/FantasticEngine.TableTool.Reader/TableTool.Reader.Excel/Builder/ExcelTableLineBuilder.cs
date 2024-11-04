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
using FantasticEngine.TableTool.Reader.Excel.Common;
using System;
using System.Collections.Generic;
using System.Data;
using System.Text;

namespace FantasticEngine.TableTool.Reader.Excel.Builder
{
    /// <summary>
    /// 读取Excel一行的数据
    /// </summary>
    internal class ExcelTableLineBuilder
    {
        /// <summary>
        /// 数据读取的中间件
        /// </summary>
        public IDataRecord Reader { get; internal set; }

        /// <summary>
        /// 表结构
        /// </summary>
        public ITable Table { get; internal set; }

        /// <summary>
        /// 行号
        /// </summary>
        public int Line { get; internal set; }

        /// <summary>
        /// 读取
        /// </summary>
        /// <returns></returns>
        public ITableLine Build()
        {
            ExcelTableLine tableLine = new ExcelTableLine();
            tableLine.Line = Line;
            //TODO 防字段数量不一致的情况
            for (int i = 0; i < Reader.FieldCount; i++)
            {
                string value = Convert.ToString(Reader.GetValue(i));
                uint key;
                if (i == 0 && !uint.TryParse(value, out key))
                {
                    return null;
                }
                IHeader header = Table.Headers.ContainsKey(i) ? Table.Headers[i] : null;
                if (header != null)
                {
                    ExcelTableValue tableValue = new ExcelTableValue();
                    tableValue.Index = i;
                    tableValue.Header = header;
                    tableValue.Value = value;
                    tableLine.Add(i, tableValue);
                }
            }
            return tableLine;
        }
    }
}
