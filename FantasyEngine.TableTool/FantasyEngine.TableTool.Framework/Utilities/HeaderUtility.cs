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
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.TableTool.Framework.Utilities
{
    /// <summary>
    /// 表头工具
    /// </summary>
    public class HeaderUtility
    {
        public static bool IsValid(IHeader header, ETarget target)
        {
            return header.Target != ETarget.NONE && (header.Target == ETarget.ALL || header.Target == target);
        }

        static Dictionary<string, EHeaderType> HeaderTypes;

        public static void InitDic()
        {
            if (HeaderTypes == null)
            {
                HeaderTypes = new Dictionary<string, EHeaderType>();
                foreach (EHeaderType type in System.Enum.GetValues(typeof(EHeaderType)))
                {
                    string str = type.ToString().ToLower();
                    if (str.StartsWith("array_array_"))
                    {
                        HeaderTypes.Add(str.Substring("ARRAY_ARRAY_".Length) + "[][]", type);
                    }
                    else if (str.StartsWith("array_"))
                    {
                        HeaderTypes.Add(str.Substring("ARRAY_".Length) + "[]", type);
                    }
                    else
                    {
                        HeaderTypes.Add(str, type);
                    }
                }
            }
        }

        public static EHeaderType ConvertHeaderType(string headerType)
        {
            if (HeaderTypes == null)
            {
                InitDic();
            }
            if (string.IsNullOrEmpty(headerType))
            {
                return EHeaderType.STRING;
            }
            headerType = headerType.ToLower();
            if (HeaderTypes.ContainsKey(headerType))
            {
                return HeaderTypes[headerType];
            }
            return EHeaderType.STRING;
        }
    }
}
