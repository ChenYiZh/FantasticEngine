/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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

namespace FantasticEngine.Network.Server.Entity
{
    /// <summary>
    /// 列信息
    /// </summary>
    public interface IEntityField
    {
        /// <summary>
        /// 是否是主键
        /// </summary>
        bool IsKey { get; }
        /// <summary>
        /// 在数据库中列名
        /// </summary>
        string Name { get; }
        /// <summary>
        /// 是否可为空
        /// </summary>
        bool Nullable { get; }
        /// <summary>
        /// 是否建立索引，只在主表下有用
        /// </summary>
        bool IsIndex { get; }
        /// <summary>
        /// 默认补全值
        /// </summary>
        object DefaultValue { get; }
        /// <summary>
        /// 数据类型
        /// </summary>
        ETableFieldType FieldType { get; }
    }
}
