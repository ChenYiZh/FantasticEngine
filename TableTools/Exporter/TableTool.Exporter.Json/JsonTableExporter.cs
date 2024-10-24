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
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasyEngine.TableTool.Exporter.Json
{
    /// <summary>
    /// 通过Json导出表格的基本信息
    /// </summary>
    public static class JsonTableExporter
    {
        /// <summary>
        /// 导出操作
        /// </summary>
        public static void Export(IDatabase database, string path)
        {
            List<JsonTable> tables = new List<JsonTable>();
            foreach (ITable table in database.Values)
            {
                List<JsonHeader> headers = new List<JsonHeader>();
                foreach (IHeader header in table.Headers.Values)
                {
                    if (HeaderUtility.IsValid(header, ETarget.CLIENT))
                    {
                        headers.Add(new JsonHeader
                        {
                            Name = header.Name,
                            Type = header.Type,
                            Description = header.Description
                        });
                    }
                }
                if (headers.Count > 1)
                {
                    tables.Add(new JsonTable
                    {
                        Name = table.Name,
                        Headers = headers
                    });
                }
            }
            File.WriteAllText(path, JsonConvert.SerializeObject(tables), new UTF8Encoding(false));
        }
    }
}
