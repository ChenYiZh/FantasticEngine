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
using FantasyEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 转换的数据类型
    /// </summary>
    public enum EConvertType
    {
        /// <summary>
        /// 无效，抛出异常
        /// </summary>
        InValid = 0,
        /// <summary>
        /// string
        /// </summary>
        String = 1,
        /// <summary>
        /// byte[]
        /// </summary>
        Binary = 2,
    }
    /// <summary>
    /// 数据存储和读取时使用，基类,暂时只支持string和byte[]
    /// </summary>
    public abstract class EntityConverter<T> : IEntityConverter
    {
        /// <summary>
        /// 转换的数据类型
        /// </summary>
        public EConvertType Type { get; protected set; }
            = FEType<T>.Type == FEType<string>.Type ? EConvertType.String : 
            (FEType<T>.Type == FEType<byte[]>.Type ? EConvertType.Binary : EConvertType.InValid);
        /// <summary>
        /// 反序列化
        /// </summary>
        public abstract TEntity Deserialize<TEntity>(T data) where TEntity : MajorEntity, new();

        /// <summary>
        /// 序列化
        /// </summary>
        public abstract T Serialize(MajorEntity entity);
    }
}
