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

namespace FantasticEngine
{
    /// <summary>
    /// 文件操作类
    /// </summary>
    public static class FileUtility
    {
        /// <summary>
        /// 判断文件目录存在不存在，不存在则创建
        /// </summary>
        /// <param name="filename"></param>
        private static void CheckOrAdd(string path)
        {
            string folder = path.Substring(0, path.Length - (Path.GetFileName(path).Length + 1));
            if (!Directory.Exists(folder))
            {
                Directory.CreateDirectory(folder);
            }
        }

        /// <summary>
        /// 文件删除
        /// </summary>
        /// <param name="path"></param>
        public static void Delete(string path)
        {
            File.Delete(path);
        }

        /// <summary>
        /// 读取流
        /// </summary>
        public static FileStream OpenRead(string path)
        {
            return File.OpenRead(path);
        }

        /// <summary>
        /// 读取文件
        /// </summary>
        public static string ReadFileText(string path)
        {
            return ReadFileText(path, Encoding.UTF8);
        }

        /// <summary>
        /// 读取文件
        /// </summary>
        public static string ReadFileText(string path, Encoding encoding)
        {
            if (File.Exists(path))
            {
                return File.ReadAllText(path, encoding);
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// 保存二进制
        /// </summary>
        public static void SaveFile(string path, byte[] buffer)
        {
            CheckOrAdd(path);
            File.WriteAllBytes(path, buffer);
        }

        /// <summary>
        /// 保存文本
        /// </summary>
        public static void SaveFile(string path, string contents)
        {
            SaveFile(path, contents, Encoding.UTF8);
        }
        /// <summary>
        /// 保存文本
        /// </summary>
        public static void SaveFile(string path, string contents, Encoding encoding)
        {
            CheckOrAdd(path);
            File.WriteAllText(path, contents, encoding);
        }
    }
}
