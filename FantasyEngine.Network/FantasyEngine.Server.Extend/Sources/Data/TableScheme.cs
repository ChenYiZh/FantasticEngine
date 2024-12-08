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
using FantasyEngine.Log;
using FantasyEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Threading;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 表结构
    /// </summary>
    public sealed class TableScheme : ITableScheme
    {
        /// <summary>
        /// 私有属性
        /// </summary>
        public IEntityTable EntityTable { get; private set; }
        /// <summary>
        /// 结构类型
        /// </summary>
        public Type Type { get; private set; }
        /// <summary>
        /// 列信息，Key: PropertyName
        /// </summary>
        public IReadOnlyDictionary<string, ITableFieldScheme> FieldsByProperty { get; private set; }
        /// <summary>
        /// 列信息，Key: Name
        /// </summary>
        public IReadOnlyDictionary<string, ITableFieldScheme> FieldsByName { get; private set; }
        /// <summary>
        /// 主键列
        /// </summary>
        internal IReadOnlyList<ITableFieldScheme> KeyFields { get; private set; }

        internal TableScheme(Type type)
        {
            EntityTableAttribute entityTable = type.GetCustomAttribute<EntityTableAttribute>();
            Type = type;
            if (entityTable == null)
            {
                entityTable = new EntityTableAttribute();
            }
            EntityTable = entityTable;

            //读取列信息
            Dictionary<string, ITableFieldScheme> fieldsByProperty = new Dictionary<string, ITableFieldScheme>();
            Dictionary<string, ITableFieldScheme> fieldsByName = new Dictionary<string, ITableFieldScheme>();
            FieldsByProperty = fieldsByProperty;
            FieldsByName = fieldsByName;
            List<ITableFieldScheme> keys = new List<ITableFieldScheme>();
            KeyFields = keys;

            //遍历读取
            PropertyInfo[] properties = type.GetProperties();
            bool hasKey = false;
            EntityFieldAttribute firstAttribute = null;
            TableFieldScheme firstField = null;
            foreach (PropertyInfo property in properties)
            {
                EntityFieldAttribute attribute = property.GetCustomAttribute<EntityFieldAttribute>();
                if (attribute == null) continue;
                TableFieldScheme field = new TableFieldScheme(property, attribute);
                fieldsByProperty.Add(field.PropertyName, field);
                fieldsByName.Add(field.Name, field);
                if (firstAttribute == null)
                {
                    firstAttribute = attribute;
                    firstField = field;
                }
                if (field.IsKey)
                {
                    //FConsole.Write(property.Name);
                    hasKey = true;
                    keys.Add(field);
                }
            }
            if (!hasKey)
            {
                firstAttribute.IsKey = true;
                keys.Add(firstField);
            }
        }

        /// <summary>
        /// 映射的数据库链接名称
        /// </summary>
        public string ConnectKey { get { return EntityTable.ConnectKey; } }
        /// <summary>
        /// 表名
        /// </summary>
        public string TableName { get { return GetTableName(); } }
        /// <summary>
        /// {0}为数据库名称，{1:MMdd}为之间名称
        /// </summary>
        public string TableNameFormat { get { return EntityTable.TableNameFormat; } }
        /// <summary>
        /// 是否进入缓存，默认true，但是日志这些结构，需要设置为false
        /// </summary>
        public bool InCache { get { return EntityTable.InCache; } }
        /// <summary>
        ///  是否从不过期,判断是否产生冷数据，默认false
        /// </summary>
        public bool NeverExpired { get { return EntityTable.NeverExpired; } }
        /// <summary>
        /// 存储方案(位运算)
        /// <para>默认 StorageType.WriteToRedis | EStorageType.ReadFromRedis | EStorageType.WriteToDb | EStorageType.ReadFromDb</para>
        /// </summary>
        public EStorageType StorageType { get { return EntityTable.StorageType; } }
        /// <summary>
        /// 缓存的临时表名
        /// </summary>
        private string _privateTempTableName;
        /// <summary>
        /// 缓存全局表名
        /// </summary>
        private string _privateGlobalTableName;
        /// <summary>
        /// 判断表名是否发生变化
        /// </summary>
        private int _tableNameChanged = 0;
        /// <summary>
        /// 事务处理锁
        /// </summary>
        private readonly object _syncRoot = new object();
        /// <summary>
        /// 判断表名是否发生变化，并且重置缓存名称
        /// </summary>
        public bool TableNameChangedAndReset()
        {
            return Interlocked.CompareExchange(ref _tableNameChanged, 0, 1) == 1;
        }
        /// <summary>
        /// 用于存数据库的表名
        /// </summary>
        public string GetTableName()
        {
            string tableName = Type.Name.EndsWith("s") ? Type.Name : Type.Name + "s";
            if (!string.IsNullOrWhiteSpace(EntityTable.TableName))
            {
                tableName = EntityTable.TableName;
            }
            string format = EntityTable.TableNameFormat;
            if (string.IsNullOrEmpty(EntityTable.TableNameFormat) || !EntityTable.TableNameFormat.Contains("{0}"))
            {
                format = "{0}";
            }
            tableName = string.Format(format, tableName, TimeLord.Now);
            //return tableName;
            if (_privateTempTableName != tableName)
            {
                lock (_syncRoot)
                {
                    //锁阻塞后再一次判断是否改变，保证操作只做一次
                    if (_privateTempTableName != tableName)
                    {
                        Interlocked.Exchange(ref _tableNameChanged, 1);
                        _privateTempTableName = tableName;
                        _privateGlobalTableName = StringConverter.ToLowerWithDownLine(tableName);
                    }
                }
            }
            return _privateGlobalTableName;
        }
    }
}
