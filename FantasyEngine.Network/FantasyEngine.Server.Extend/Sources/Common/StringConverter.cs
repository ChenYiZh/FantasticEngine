/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 字符串操作
    /// </summary>
    public static class StringConverter
    {
        /// <summary>
        /// 变小写，并且大写字母前加下划线
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string ToLowerWithDownLine(string str)
        {
            if (str == null) return null;
            if (str.Length == 1) return str.ToLower();
            string oriStr = str;
            string lowStr = str.ToLower();
            string result = "" + lowStr[0];
            for (int i = 1; i < lowStr.Length; i++)
            {
                char lc = lowStr[i];
                char oc = oriStr[i];
                //if (lc == oc)
                //{
                //    result += lc;
                //}
                //else 
                if (lc != oc && (lowStr[i - 1] == oriStr[i - 1] || (i < lowStr.Length - 1 && lowStr[i + 1] == oriStr[i + 1])))
                {
                    result += "_" + lc;
                }
                else
                {
                    result += lc;
                }
            }
            return result;
        }
    }
}
