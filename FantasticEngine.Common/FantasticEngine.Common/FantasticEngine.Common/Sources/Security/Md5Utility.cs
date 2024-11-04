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
using System.IO;
using System.Text;

namespace FantasticEngine.Security
{
    /// <summary>
    /// Md5帮助类
    /// </summary>
    public static class Md5Utility
    {
        /// <summary>
        /// Stream转Md5
        /// </summary>
        public static string GetMd5(Stream stream)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] retVal = md5.ComputeHash(stream);
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < retVal.Length; i++)
            {
                builder.Append(retVal[i].ToString("x2"));
            }
            return builder.ToString();
        }

        /// <summary>
        /// 字节流转Md5
        /// </summary>
        public static string GetMd5(byte[] buffer)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] retVal = md5.ComputeHash(buffer);
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < retVal.Length; i++)
            {
                builder.Append(retVal[i].ToString("x2"));
            }
            return builder.ToString();
        }

        /// <summary>
        /// 通过文件路径读取到的文件转Md5
        /// </summary>
        public static string GetMd5(string path)
        {
            string result = null;
            try
            {
                using (FileStream stream = FileUtility.OpenRead(path))
                {
                    result = GetMd5(stream);
                }
            }
            catch
            {
                result = null;
            }
            return result;
        }
    }
}
