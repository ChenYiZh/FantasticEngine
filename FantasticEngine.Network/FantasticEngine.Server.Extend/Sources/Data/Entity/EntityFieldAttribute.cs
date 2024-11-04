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
    /// 数据库中字段类型
    /// </summary>
    public enum ETableFieldType
    {
        /// <summary>
        /// 默认参数，自动匹配
        /// </summary>
        Auto = 0,
        /// <summary>
        /// TINYTEXT/varchar(255)
        /// </summary>
        String = 1,
        /// <summary>
        /// TEXT/varchar(65,535)
        /// </summary>
        Text = 2,
        /// <summary>
        /// 数据库中最大可存入的数据长度
        /// </summary>
        LongText = 3,
        /// <summary>
        /// 二进制数据：65,535 字节的数据/varbinary
        /// </summary>
        Blob = 5,
        /// <summary>
        /// 二进制数据：LongBlob/varbinary(max)
        /// </summary>
        LongBlob = 6,
        /// <summary>
        /// bit: 字节0/1
        /// </summary>
        Bit = 10,
        /// <summary>
        /// TINYINT
        /// </summary>
        Byte = 11,
        /// <summary>
        /// SMALLINT
        /// </summary>
        Short = 12,
        /// <summary>
        /// INT
        /// </summary>
        Int = 13,
        /// <summary>
        /// BIGINT
        /// </summary>
        Long = 14,
        /// <summary>
        /// TINYINT unsigned/SMALLINT
        /// </summary>
        SByte = 15,
        /// <summary>
        /// SMALLINT unsigned/SMALLINT
        /// </summary>
        UShort = 16,
        /// <summary>
        /// INT  unsigned/INT
        /// </summary>
        UInt = 17,
        /// <summary>
        /// BIGINT unsigned/BIGINT
        /// </summary>
        ULong = 18,
        /// <summary>
        /// FLOAT/float(24)
        /// </summary>
        Float = 20,
        /// <summary>
        /// DOUBLE/float(53)
        /// </summary>
        Double = 21,
        /// <summary>
        /// DATETIME
        /// </summary>
        DateTime = 30,
        /// <summary>
        /// 错误类型，无法解析，也无法保存
        /// </summary>
        Error = 255,
    }
    /// <summary>
    /// 属性字段
    /// String类型默认varchar(255),列表默认LongText
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false)]
    public sealed class EntityFieldAttribute : Attribute, IEntityField
    {
        /// <summary>
        /// 是否是主键
        /// </summary>
        public bool IsKey { get; set; } = false;
        /// <summary>
        /// 列名
        /// </summary>
        public string Name { get; set; }
        /// <summary>
        /// 是否可为空
        /// </summary>
        public bool Nullable { get; set; } = true;
        /// <summary>
        /// 是否建立索引，只在主表下有用
        /// </summary>
        public bool IsIndex { get; set; } = false;
        /// <summary>
        /// 默认补全值
        /// </summary>
        public object DefaultValue { get; set; }
        /// <summary>
        /// 数据类型
        /// </summary>
        public ETableFieldType FieldType { get; set; } = ETableFieldType.Auto;
    }
}
