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
using FantasticEngine.TableTool.Framework.Utilities;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.TableTool.Framework.Common
{
    /// <summary>
    /// 表格内容输出的抽象类
    /// </summary>
    public abstract class TableContextBuilder
    {
        /// <summary>
        /// 表格结构
        /// </summary>
        public ITable Table { get; internal set; }
        /// <summary>
        /// 目标平台
        /// </summary>
        public ETarget Target { get; internal set; }
        /// <summary>
        /// 输出的内容
        /// </summary>
        internal StringBuilder Context { get; set; }
        /// <summary>
        /// 创建调用
        /// </summary>
        internal void Build()
        {
            Build(Context);
        }
        /// <summary>
        /// 需要实现的输出函数
        /// </summary>
        protected abstract void Build(StringBuilder context);
        /// <summary>
        /// 判断某个表头是否有效
        /// </summary>
        protected bool IsValid(IHeader header)
        {
            return HeaderUtility.IsValid(header, Target);
        }
    }
}
