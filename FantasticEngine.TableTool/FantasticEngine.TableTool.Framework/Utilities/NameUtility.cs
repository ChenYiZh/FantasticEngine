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
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.TableTool.Framework.Utilities
{
    /// <summary>
    /// 命名管理器
    /// </summary>
    public static class NameUtility
    {
        /// <summary>
        /// 创建一个默认名称
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public static string GetDefaultPropertyName(string name)
        {
            string result = "";
            bool lastCharIsWord = false;
            foreach (char c in name)
            {
                bool currentCharIsWord = IsWord(c) || (result.Length > 0 && (char.IsNumber(c) || c == '_'));
                if (currentCharIsWord)
                {
                    if (!lastCharIsWord)
                    {
                        result += char.ToUpper(c);
                    }
                    else
                    {
                        result += c;
                    }
                }
                lastCharIsWord = currentCharIsWord && !(char.IsNumber(c) || c == '_');
            }

            if (result.Length == 0)
            {
                return "_无效列";
            }
            return result.Split('.')[0];
        }
        /// <summary>
        /// 判断是否是字母
        /// </summary>
        /// <param name="c"></param>
        /// <returns></returns>
        public static bool IsWord(char c)
        {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }
    }
}
