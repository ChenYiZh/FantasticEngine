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
using FantasticEngine.TableTool.Reader.Excel.Common;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.TableTool.Reader.Excel.Builder
{
    internal class ExcelHeaderBuilder
    {
        /// <summary>
        /// 读取到的参数
        /// </summary>
        public string[] InValues { get; set; }
        /// <summary>
        /// 表头顺序
        /// </summary>
        public int Index { get; set; }
        /// <summary>
        /// 解析表头
        /// </summary>
        /// <returns></returns>
        public IHeader Build()
        {
            ExcelHeader header = new ExcelHeader();
            if (Index == 0)
            {
                header.Type = EHeaderType.UINT;
            }
            header.Name = InValues[0];
            header.Description = InValues[1].Replace("\n", "\t");
            if (string.IsNullOrWhiteSpace(InValues[2]))
            {
                header.Target = ETarget.NONE;
            }
            else
            {
                switch (InValues[2].ToLower())
                {
                    case "all": header.Target = ETarget.ALL; break;
                    case "client": header.Target = ETarget.CLIENT; break;
                    case "server": header.Target = ETarget.SERVER; break;
                    default: header.Target = ETarget.NONE; break;
                }
            }
            header.Type = HeaderUtility.ConvertHeaderType(InValues[3]);
            return header;
        }
    }
}
