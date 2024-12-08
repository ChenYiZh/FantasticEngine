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

namespace FantasyEngine.Network.Server.Entity
{
    /// <summary>
    /// 存储类型
    /// </summary>
    public enum EStorageType
    {
        ///// <summary>
        ///// 通过默认的策略从Redis和db加载保存
        ///// </summary>
        //Default = 0,
        /// <summary>
        /// 往RawDb写
        /// </summary>
        WriteToRawDb = 10,
        /// <summary>
        /// 从RawDb读
        /// </summary>
        ReadFromRawDb = 11,
        /// <summary>
        /// 往Db写
        /// </summary>
        WriteToDb = 20,
        /// <summary>
        /// 从Db读
        /// </summary>
        ReadFromDb = 21,
    }
    /// <summary>
    /// 实体表映射属性
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
    public sealed class EntityTableAttribute : Attribute, IEntityTable
    {
        /// <summary>
        /// 数据库映射名称
        /// </summary>
        public string ConnectKey { get; set; }

        /// <summary>
        /// 数据表名
        /// </summary>
        public string TableName { get; set; }

        /// <summary>
        /// {0}为数据库名称，{1:MMdd}为之间名称
        /// </summary>
        public string TableNameFormat { get; set; }

        /// <summary>
        /// 是否进入缓存，默认true，但是日志这些结构，需要设置为false
        /// </summary>
        public bool InCache { get; set; } = true;

        /// <summary>
        /// 是否从不过期,判断是否产生冷数据，默认false
        /// </summary>
        public bool NeverExpired { get; set; } = false;

        /// <summary>
        /// 存储方案(位运算)
        /// <para>默认 StorageType.WriteToRedis | EStorageType.ReadFromRedis | EStorageType.WriteToDb | EStorageType.ReadFromDb</para>
        /// </summary>
        public EStorageType StorageType { get; set; } = EStorageType.WriteToRawDb | EStorageType.ReadFromRawDb | EStorageType.WriteToDb | EStorageType.ReadFromDb;

        public EntityTableAttribute()
        {
            ConnectKey = Settings.GetDefaultConnectKey();
            TableName = null;
            TableNameFormat = "{0}";
        }

        public EntityTableAttribute(EStorageType storageType) : this()
        {
            StorageType = storageType;
        }

        public EntityTableAttribute(string connectKey) : this()
        {
            ConnectKey = connectKey;
        }

        public EntityTableAttribute(string connectKey, string tableNameFormat) : this(connectKey)
        {
            TableNameFormat = tableNameFormat;
        }

        public EntityTableAttribute(string connectKey, EStorageType storageType) : this(connectKey)
        {
            StorageType = storageType;
        }

        public EntityTableAttribute(string connectKey, string tableNameFormat, EStorageType storageType) : this(connectKey, tableNameFormat)
        {
            StorageType = storageType;
        }
    }
}
