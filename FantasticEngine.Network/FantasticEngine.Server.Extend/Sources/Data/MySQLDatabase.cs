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
using FantasticEngine.Collections;
using FantasticEngine.Network.Server.Entity;
using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// mysql连接管理
    /// set global max_allowed_packet = 1073741824;
    /// </summary>
    public sealed class MySQLDatabase : Database
    {
        /// <summary>
        /// 事务队列
        /// </summary>
        private CachePool<DbCommition> _commitionPool;

        ///// <summary>
        ///// 通讯连接池
        ///// </summary>
        //private ThreadSafeHashSet<MySqlConnection> Connections;

        /// <summary>
        /// 读写连接
        /// </summary>
        private MySqlConnection _writeConnection, _readConnection;

        /// <summary>
        /// 是否正在事务处理
        /// </summary>
        private int _isExecuting = 0;

        /// <summary>
        /// 准备关闭
        /// </summary>
        private int _isClosed = 0;

        /// <summary>
        /// 判断连接状态
        /// </summary>
        public override bool Connected
        {
            get
            {
                //if (Connections != null && Connections.Count > 0)
                //{
                //    lock (Connections.SyncRoot)
                //    {
                //        return Connections.All(c => c.State == ConnectionState.Connecting || c.State == ConnectionState.Executing);
                //    }
                //}
                //return false;
                return IsConnected(_writeConnection) && IsConnected(_readConnection);
            }
            protected set { }
        }

        private bool IsConnected(MySqlConnection connection)
        {
            return connection != null && (connection.State != ConnectionState.Broken && connection.State != ConnectionState.Closed);
        }

        /// <summary>
        /// 创建连接，多并发事务处理使用
        /// </summary>
        /// <returns></returns>
        private MySqlConnection CreateConnection()
        {
            FEConsole.WriteInfoFormatWithCategory(Kind.ToString(), "MySQL[{0}] start connecting...", Setting.ConnectKey);
            MySqlConnection connection = new MySqlConnection(Setting.ConnectionString);
            //Connections.Add(connection);
            connection.Open();
            FEConsole.WriteInfoFormatWithCategory(Kind.ToString(), "MySQL[{0}] is opened.", Setting.ConnectKey);
            return connection;
        }

        /// <summary>
        /// 判断状态并且获取写入的连接对象
        /// </summary>
        private MySqlConnection GetWriteConnection()
        {
            if (!IsConnected(_writeConnection))
            {
                if (_writeConnection != null && _writeConnection.State == ConnectionState.Closed)
                {
                    _writeConnection.Open();
                }
                else
                {
                    Close(_writeConnection);
                    _writeConnection = CreateConnection();
                }
            }
            return _writeConnection;
        }

        /// <summary>
        /// 判断状态并且获取读取的连接对象
        /// </summary>
        private MySqlConnection GetReadConnection()
        {
            if (!IsConnected(_readConnection))
            {
                if (_readConnection != null && _readConnection.State == ConnectionState.Closed)
                {
                    _readConnection.Open();
                }
                else
                {
                    Close(_readConnection);
                    _readConnection = CreateConnection();
                }
            }
            return _readConnection;
        }

        /// <summary>
        /// 关闭连接
        /// </summary>
        private void Close(MySqlConnection connection)
        {
            if (connection == null)
            {
                return;
            }
            try
            {
                connection.Close();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
            }
            //Connections.Remove(connection);
        }

        /// <summary>
        /// 设置配置文件，初始化时执行
        /// </summary>
        public override void SetSettings(IDatabaseSetting setting)
        {
            base.SetSettings(setting);
            _commitionPool = new CachePool<DbCommition>(PushingCommitions);
            //Connections = new ThreadSafeHashSet<MySqlConnection>();
            _writeConnection = CreateConnection();
            _readConnection = CreateConnection();
        }
        int totalCount = 0;
        /// <summary>
        /// 业务处理线程
        /// </summary>
        private void PushingCommitions(IReadOnlyQueue<DbCommition> set)
        {
            StringBuilder sql = new StringBuilder();
            Interlocked.Exchange(ref _isExecuting, set.Count);
            if (_isExecuting > 0)
            {
                sql.Append("BEGIN;");
            }
            int count = 0;
            while (set.Count > 0)
            {
                if (_isClosed > 0)
                {
                    const int cacheCount = 4;
                    float progress = (1.0f - ((float)set.Count / _isExecuting)) * 100.0f / cacheCount;
                    progress += (_isClosed - 1) * 100.0f / cacheCount;
                    Console.Write($"\r{FEConsole.FormatCustomMessage(Kind.ToString(), $"MySQL[{Setting.ConnectKey}] is saving data... {progress.ToString("F2")}%")}");
                }
                DbCommition commition = set.Dequeue();
                CheckTableNameChanged(DataContext.GetTableScheme(commition.Key.Type));
                try
                {
                    GenerateModifySql(commition, sql);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
                totalCount++;
                //最大多少条数据一起提交
                if (count++ >= 100)
                {
                    count = 0;
                    sql.Append("COMMIT;");
                    string commitSql = sql.ToString();
                    sql.Clear();
                    sql.Append("BEGIN;");
                    try
                    {
                        if (Query(commitSql) != 0)
                        {
                            FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), "[Query Error] There are some errors happened on command the sql -> \r\n{0}", commitSql);
                        }
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                    }
                }
            }
            try
            {
                if (_isExecuting > 0)
                {
                    sql.Append("COMMIT;");
                }
                if (Query(sql.ToString()) != 0)
                {
                    FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), "[Query Error] There are some errors happened on command the sql -> \r\n{0}", sql);
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
            }
            Interlocked.Exchange(ref _isExecuting, 0);
            if (_isClosed > 0)
            {
                Interlocked.Increment(ref _isClosed);
            }
            if (totalCount > 1000000)
            {
                FEConsole.WriteWarn(DateTime.Now + ": " + totalCount);
            }
        }

        /// <summary>
        /// 关闭时执行
        /// </summary>
        public override void Close()
        {
            Interlocked.Increment(ref _isClosed);
            while (_isExecuting > 0)
            {
                Thread.Sleep(Settings.LockerTimeout);
            }
            if (_commitionPool != null)
            {
                _commitionPool.Release();
            }
            Console.Write($"\r{FEConsole.FormatCustomMessage(Kind.ToString(), $"MySQL[{Setting.ConnectKey}] is saving data... 100.00%")}");
            Console.WriteLine();
            Console.WriteLine();
            _commitionPool = null;
            List<MySqlConnection> connections = null;
            //lock (Connections.SyncRoot)
            //{
            //    connections = new List<MySqlConnection>(Connections);
            //}
            connections.Add(_readConnection);
            connections.Add(_writeConnection);
            foreach (MySqlConnection connection in connections)
            {
                Close(connection);
            }
        }

        #region CheckTableSchema
        /// <summary>
        /// 解析数据库属性字段
        /// </summary>
        private class FieldInfo : IEntityField
        {
            /// <summary>
            /// 是否是主键
            /// </summary>
            public bool IsKey { get; set; }
            /// <summary>
            /// 在数据库中列名
            /// </summary>
            public string Name { get; set; }
            /// <summary>
            /// 是否可为空
            /// </summary>
            public bool Nullable { get; set; }
            /// <summary>
            /// 是否建立索引，只在主表下有用
            /// </summary>
            public bool IsIndex { get; set; }
            /// <summary>
            /// 默认补全值
            /// </summary>
            public object DefaultValue { get; set; }
            /// <summary>
            /// 数据库中的类型
            /// </summary>
            public string DbFieldType { get; set; }
            /// <summary>
            /// 数据类型
            /// </summary>
            public ETableFieldType FieldType { get; set; }

            public override string ToString()
            {
                return $"{Name} [{DbFieldType}, {FieldType.ToString()}]: IsKey: {IsKey}, IsIndex: {IsIndex}, Nullable: {Nullable}, DefaultValue: {DefaultValue}";
            }
        }

        /// <summary>
        /// MySQL管理表的数据库
        /// </summary>
        private const string INFORMATION_SCHEMA = "information_schema";

        /// <summary>
        /// 需要读取的列表信息
        /// </summary>
        private readonly static string[] _columnReadInfo
            = new string[] {"IC.`COLUMN_NAME`", "IC.`COLUMN_DEFAULT`", "IF(IC.`IS_NULLABLE` = 'YES', 1, 0) AS NULLABLE", "IC.`COLUMN_TYPE`",
                "IF(IC.`COLUMN_NAME` = KCU.`COLUMN_NAME`, 1, 0) AS IS_KEY",//判断是否是主键， >0 是主键
                "IF(IC.`COLUMN_NAME` = S.`COLUMN_NAME`, 1, 0) AS IS_INDEX" //判断是否是索引
            };

        /// <summary>
        /// 对表结构进行调整或创建
        /// </summary>
        public override void GenerateOrUpdateTableScheme(ITableScheme table)
        {
            ((TableScheme)table).TableNameChangedAndReset();
            string tableName = table.TableName;
            string sql = "SHOW TABLES;";
            HashSet<string> tableNames = Query<HashSet<string>, string>(sql, (reader) => { return reader.GetString(0); });//加载所有表

            #region 通过imformation_schema查询列信息，权限问题

            //string returnColumns = string.Join(", ", ColumnReadInfo);
            //string sql = $"SELECT {returnColumns} " +
            //    $"FROM `{INFORMATION_SCHEMA}`.`COLUMNS` AS IC " +
            //    $"LEFT JOIN `{INFORMATION_SCHEMA}`.`KEY_COLUMN_USAGE` AS KCU " +
            //    $"ON IC.`TABLE_SCHEMA` = KCU.`TABLE_SCHEMA` AND IC.`TABLE_NAME` = KCU.`TABLE_NAME` AND IC.`COLUMN_NAME` = KCU.`COLUMN_NAME` " +
            //    $"LEFT JOIN `{INFORMATION_SCHEMA}`.`STATISTICS` AS S " +
            //    $"ON IC.`TABLE_SCHEMA` = S.`TABLE_SCHEMA` AND IC.`TABLE_NAME` = S.`TABLE_NAME` AND IC.`COLUMN_NAME` = S.`COLUMN_NAME` " +
            //    $"WHERE IC.`TABLE_SCHEMA` = '{Setting.Database.ToLower()}' AND IC.`TABLE_NAME` = '{tableName}';";

            //List<FieldInfo> tableFields = Query(sql, (reader) =>
            //{
            //    string fieldType = reader.GetString("COLUMN_TYPE", null);
            //    return new FieldInfo()
            //    {
            //        IsKey = reader.GetInt32("IS_KEY") == 1,
            //        Name = reader.GetString("COLUMN_NAME"),
            //        Nullable = reader.GetInt32("NULLABLE", 0) == 1,
            //        IsIndex = reader.GetInt32("IS_INDEX", 0) == 1,
            //        DefaultValue = reader.GetString("COLUMN_DEFAULT", null),
            //        DbFieldType = fieldType,
            //        FieldType = ConvertFromString(fieldType),
            //    };
            //});

            //sql = null;
            //if (tableFields.Count == 0)
            #endregion

            //表不存在，就创建表
            if (!tableNames.Contains(tableName))
            {
                //创建表
                string fieldSqls = string.Join(", ", table.FieldsByProperty.Values.Select(f => CreateGenerateSQL(f)));
                string keyFieldSqls = string.Join(", ", table.FieldsByProperty.Values.Where(f => f.IsKey).Select(f => $"`{f.Name}`"));
                ITableFieldScheme[] indexs = table.FieldsByProperty.Values.Where(f => f.IsIndex && !f.IsKey).ToArray();
                string indexSqls = indexs.Length > 0 ? $", {string.Join(", ", indexs.Select(f => $"INDEX `INDEX_{f.Name}` (`{f.Name}`)"))}" : "";
                sql = $"CREATE TABLE IF NOT EXISTS `{tableName}`({fieldSqls}, PRIMARY KEY ({keyFieldSqls}){indexSqls} )ENGINE=InnoDB DEFAULT CHARSET=utf8;";
                if (Query(sql) != 0)
                {
                    FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), "[Query Error] There are some errors happened on command the sql -> \r\n{0}", sql);
                }
            }
            //表存在，判断列信息是否一致
            else
            {
                sql = $"DESC {tableName};";//读取表信息
                List<FieldInfo> tableFields = Query<List<FieldInfo>, FieldInfo>(sql, (reader) =>
                {
                    string fieldType = reader.GetString("Type", null);
                    return new FieldInfo()
                    {
                        IsKey = reader.GetString("Key", null) == "PRI",
                        Name = reader.GetString("Field"),
                        Nullable = reader.GetString("Null", "").ToLower() == "yes",
                        IsIndex = !string.IsNullOrWhiteSpace(reader.GetString("Key", null)),
                        DefaultValue = reader.GetString("Default", null),
                        DbFieldType = fieldType,
                        FieldType = ConvertFromString(fieldType),
                    };
                });

                //进行判断
                Dictionary<string, FieldInfo> dbFields = tableFields.ToDictionary(f => f.Name.ToLower(), f => f);
                List<TableFieldComparor> comparors = new List<TableFieldComparor>();
                foreach (ITableFieldScheme field in table.FieldsByProperty.Values)
                {
                    FieldInfo fieldInfo = null;
                    if (dbFields.ContainsKey(field.Name.ToLower()))
                    {
                        fieldInfo = dbFields[field.Name.ToLower()];
                        dbFields.Remove(field.Name.ToLower());
                    }
                    comparors.Add(new TableFieldComparor(field, fieldInfo));
                }
                foreach (FieldInfo fieldInfo in dbFields.Values)
                {
                    comparors.Add(new TableFieldComparor(null, fieldInfo));
                }
                dbFields.Clear();

                //判断结构处理
                StringBuilder sqls = new StringBuilder();
                bool keyChanged = false;
                foreach (TableFieldComparor comparor in comparors)
                {
                    if (comparor.Operation != TableFieldComparor.EOperation.UnModified)
                    {
                        if (comparor.Operation == TableFieldComparor.EOperation.Deleted)
                        {
                            continue;
                        }
                        if (comparor.IsError)
                        {
                            throw new TypeAccessException($"Can't alter the table `{tableName}`, because the field['{comparor.FieldName}' fail to convert {comparor.DbFieldInfo.FieldType.ToString()} to {comparor.TableField.FieldType.ToString()}]");
                        }
                        if (comparor.Operation == TableFieldComparor.EOperation.ToInsert)
                        {
                            sqls.AppendLine($"ALTER TABLE `{tableName}` ADD {CreateGenerateSQL((ITableFieldScheme)comparor.TableField)};");
                        }
                        else if (!comparor.OnlyIsIndexChanged && !comparor.OnlyIsKeyChanged)
                        {
                            sqls.AppendLine($"ALTER TABLE `{tableName}` MODIFY {CreateGenerateSQL((ITableFieldScheme)comparor.TableField)}");
                        }
                        if ((comparor.TableField.IsIndex && comparor.Operation == TableFieldComparor.EOperation.ToInsert)
                            || (comparor.DbFieldInfo != null && comparor.TableField.IsIndex != comparor.DbFieldInfo.IsIndex))
                        {
                            if (comparor.TableField.IsIndex)
                            {
                                sqls.AppendLine($"ALTER TABLE `{tableName}` ADD INDEX `INDEX_{comparor.TableField.Name}`(`{comparor.TableField.Name}`)");
                            }
                        }
                        if ((comparor.TableField.IsKey && comparor.Operation == TableFieldComparor.EOperation.ToInsert)
                            || (comparor.DbFieldInfo != null && comparor.TableField.IsKey != comparor.DbFieldInfo.IsKey))
                        {
                            keyChanged = true;
                        }
                    }
                }

                if (keyChanged)
                {
                    sqls.AppendLine($"ALTER TABLE `{tableName}` DROP PRIMARY KEY;");
                    sqls.AppendLine($"ALTER TABLE `{tableName}` ADD PRIMARY KEY ({string.Join(", ", table.FieldsByProperty.Values.Where(f => f.IsKey).Select(f => $"`{f.Name}`"))});");
                }

                if (sqls.Length > 0)
                {
                    sql = sqls.ToString();// string.Join("; ", sqls);
                    if (Query(sql) != 0)
                    {
                        FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), "[Query Error] There are some errors happened on command the sql -> \r\n{0}", sql);
                    }
                }
            }
        }
        /// <summary>
        /// 判断表名是否发生变化，有变化就生成新的表
        /// </summary>
        private void CheckTableNameChanged(ITableScheme table)
        {
            TableScheme tableScheme = (TableScheme)table;
            if (tableScheme.TableNameChangedAndReset())
            {
                GenerateOrUpdateTableScheme(table);
            }
        }
        #endregion
        /// <summary>
        /// 通过MySqlDataReader解析数据
        /// </summary>
        private TSet Query<TSet, T>(string sql, Func<MySqlDataReader, T> deserialize, IEnumerable<KeyValuePair<string, object>> parameters = null)
            where TSet : ICollection<T>, new()
        {
            FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), sql);
            MySqlConnection connection = GetReadConnection();
            TSet items = new TSet();
            using (MySqlCommand command = connection.CreateCommand())
            {
                command.CommandText = sql;
                if (parameters != null)
                {
                    foreach (KeyValuePair<string, object> param in parameters)
                    {
                        MySqlParameter parameter = command.CreateParameter();
                        parameter.ParameterName = param.Key;
                        parameter.Value = param.Value;
                    }
                }
                MySqlDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    items.Add(deserialize(reader));
                }
                reader.Close();
            }
            //Close(connection);
            return items;
        }
        /// <summary>
        /// 只执行SQL
        /// </summary>
        private int Query(string sql, IEnumerable<KeyValuePair<string, object>> parameters = null)
        {
            FEConsole.WriteTo(LOG_LEVEL, Kind.ToString(), sql);
            MySqlConnection connection = GetWriteConnection();
            int result = -1;
            using (MySqlCommand command = connection.CreateCommand())
            {
                command.CommandText = sql;
                if (parameters != null)
                {
                    foreach (KeyValuePair<string, object> param in parameters)
                    {
                        MySqlParameter parameter = command.CreateParameter();
                        parameter.ParameterName = param.Key;
                        parameter.Value = param.Value;
                    }
                }
                result = command.ExecuteNonQuery();
            }
            //Close(connection);
            return result;
        }

        private static string CreateGenerateSQL(ITableFieldScheme tableField)
        {
            ETableFieldType type = tableField.FieldType;//== ?TableFieldConverter.ConvertFromType(tableField.Type.PropertyType);
            string sql = $"`{tableField.Name}` {ConvertToString(tableField, type)}";
            if (tableField.IsKey || !tableField.Nullable)
            {
                sql += " NOT NULL";
            }
            if (type == ETableFieldType.String || type == ETableFieldType.Text || type == ETableFieldType.LongText)
            {
                if (tableField.DefaultValue != null)
                {
                    sql += $" DEFAULT '{tableField.DefaultValue}'";
                }
            }
            //else if (!tableField.DefaultValue.Equals(FType.GetDefaultValueFromType(tableField.Type.PropertyType)))
            //{
            //    sql += $" default {tableField.DefaultValue}";
            //}
            else if (tableField.DefaultValue != null)
            {
                sql += $" DEFAULT {tableField.DefaultValue}";
            }
            return sql;
        }

        /// <summary>
        /// 从数据库读取的字符串转类型
        /// </summary>
        private static ETableFieldType ConvertFromString(string sqlType)
        {
            if (sqlType.StartsWith("varchar"))
            {
                return ETableFieldType.String;
            }
            else if (sqlType.StartsWith("text"))
            {
                return ETableFieldType.Text;
            }
            else if (sqlType.StartsWith("longtext"))
            {
                return ETableFieldType.LongText;
            }
            else if (sqlType.StartsWith("blob"))
            {
                return ETableFieldType.Blob;
            }
            else if (sqlType.StartsWith("longblob"))
            {
                return ETableFieldType.LongBlob;
            }
            else if (sqlType.StartsWith("bit"))
            {
                return ETableFieldType.Bit;
            }
            else if (sqlType.StartsWith("tinyint"))
            {
                if (sqlType.Contains("unsigned"))
                {
                    return ETableFieldType.Byte;
                }
                else
                {
                    return ETableFieldType.SByte;
                }
            }
            else if (sqlType.StartsWith("smallint"))
            {
                if (sqlType.Contains("unsigned"))
                {
                    return ETableFieldType.UShort;
                }
                else
                {
                    return ETableFieldType.Short;
                }
            }
            else if (sqlType.StartsWith("int"))
            {
                if (sqlType.Contains("unsigned"))
                {
                    return ETableFieldType.UInt;
                }
                else
                {
                    return ETableFieldType.Int;
                }
            }
            else if (sqlType.StartsWith("bigint"))
            {
                if (sqlType.Contains("unsigned"))
                {
                    return ETableFieldType.ULong;
                }
                else
                {
                    return ETableFieldType.Long;
                }
            }
            else if (sqlType.StartsWith("float"))
            {
                return ETableFieldType.Float;
            }
            else if (sqlType.StartsWith("double"))
            {
                return ETableFieldType.Double;
            }
            else if (sqlType.StartsWith("datetime"))
            {
                return ETableFieldType.DateTime;
            }
            return ETableFieldType.Error;
        }

        /// <summary>
        /// 从数据库读取的字符串转类型
        /// </summary>
        public static string ConvertToString(ITableFieldScheme fieldInfo, ETableFieldType type)
        {
            if (type == ETableFieldType.Auto || type == ETableFieldType.Error)
            {
                type = TableFieldConverter.ConvertFromType(fieldInfo.Type.PropertyType);
            }
            if (type == ETableFieldType.Error)
            {
                return null;
            }
            switch (type)
            {
                case ETableFieldType.String: return "varchar(255)";
                case ETableFieldType.Text: return "text";
                case ETableFieldType.LongText: return "longtext";
                case ETableFieldType.Blob: return "blob";
                case ETableFieldType.LongBlob: return "longblob";
                case ETableFieldType.Bit: return "bit";
                case ETableFieldType.Byte: return "tinyint unsigned";
                case ETableFieldType.Short: return "smallint";
                case ETableFieldType.Int: return "int";
                case ETableFieldType.Long: return "bigint";
                case ETableFieldType.SByte: return "tinyint";
                case ETableFieldType.UShort: return "smallint unsigned";
                case ETableFieldType.UInt: return "int unsigned";
                case ETableFieldType.ULong: return "long unsigned";
                case ETableFieldType.Float: return "float";
                case ETableFieldType.Double: return "double";
                case ETableFieldType.DateTime: return "datetime";
            }
            return null;
        }

        /// <summary>
        /// 操作一堆数据
        /// </summary>
        public override bool CommitModifiedEntitys(IEnumerable<DbCommition> commitions)
        {
            foreach (DbCommition commition in commitions)
            {
                _commitionPool.Push(commition);
            }
            return true;
        }

        /// <summary>
        /// 读取表中所有数据
        /// </summary>
        public override IEnumerable<T> LoadAll<T>()
        {
            ITableScheme tableScheme = DataContext.GetTableScheme<T>();
            CheckTableNameChanged(DataContext.GetTableScheme<T>());
            string sql = $"SELECT * FROM {tableScheme.TableName};";
            return Query<List<T>, T>(sql, (reader) => { return Deserialize<T>(tableScheme, reader); });
        }
        /// <summary>
        /// 通过EntityKey，查询某一条数据，没有就返回空
        /// </summary>
        public override T Find<T>(EntityKey key)
        {
            TableScheme tableScheme = (TableScheme)DataContext.GetTableScheme<T>();
            Dictionary<string, string> keys = new Dictionary<string, string>(tableScheme.KeyFields.Count);
            IReadOnlyList<ITableFieldScheme> keyFields = tableScheme.KeyFields;
            for (int i = 0; i < keyFields.Count; i++)
            {
                keys.Add(keyFields[i].Name, key.Keys[i].GetString());
            }
            string sql = $"SELECT * FROM {tableScheme.TableName} WHERE {string.Join(" AND ", keys.Select(kv => $"`{kv.Key}` = '{kv.Value}'"))};";
            CheckTableNameChanged(DataContext.GetTableScheme<T>());
            List<T> result = Query<List<T>, T>(sql, (reader) => { return Deserialize<T>(tableScheme, reader); });
            if (result.Count == 0)
            {
                return null;
            }
            return result[0];
        }
        /// <summary>
        /// 执行修改操作
        /// </summary>
        public bool GenerateModifySql(DbCommition commition, StringBuilder builder)
        {
            TableScheme tableScheme = (TableScheme)DataContext.TableSchemes[commition.Key.Type];
            Dictionary<string, string> keys = new Dictionary<string, string>(tableScheme.KeyFields.Count);
            IReadOnlyList<ITableFieldScheme> keyFields = tableScheme.KeyFields;
            for (int i = 0; i < keyFields.Count; i++)
            {
                keys.Add(keyFields[i].Name, commition.Key.Keys[i].GetString());
            }
            //Where判断sql
            string keySql = string.Join(" AND ", keys.Select(kv => $"`{kv.Key}` = '{kv.Value}'"));
            //删除逻辑
            if (commition.ModifyType == EModifyType.Remove || commition.Entity == null)
            {
                builder.Append($" DELETE FROM `{tableScheme.TableName}` WHERE {keySql};");
                return true;
            }
            //生成修改逻辑有二进制会动到@
            Dictionary<string, string> values = new Dictionary<string, string>(tableScheme.FieldsByProperty.Count);
            foreach (ITableFieldScheme field in tableScheme.FieldsByProperty.Values)
            {
                //获取当前的值
                object value = field.Type.GetValue(commition.Entity);
                if (field.Type.PropertyType.IsSubclassOf(FEType<PropertyEntity>.Type))
                {
                    values.Add(field.Name, $"'{JsonUtility.ToJson(value)}'");
                }
                else if (field.Type.PropertyType == FEType<TimeSpan>.Type)
                {
                    values.Add(field.Name, $"{((TimeSpan)value).Ticks}");
                }
                else if (field.FieldType == ETableFieldType.Blob || field.FieldType == ETableFieldType.LongBlob)
                {
                    if (value == null)
                    {
                        values.Add(field.Name, null);
                    }
                    else
                    {
                        byte[] buff = (byte[])value;
                        StringBuilder buffStr = new StringBuilder();
                        for (int i = 0; i < buff.Length; i++)
                        {
                            buffStr.Append(Convert.ToString(buff[i], 2));
                        }
                        values.Add(field.Name, buffStr.ToString());
                    }
                }
                else if (field.FieldType == ETableFieldType.String || field.FieldType == ETableFieldType.Text || field.FieldType == ETableFieldType.LongText
                    || field.FieldType == ETableFieldType.DateTime)
                {
                    values.Add(field.Name, $"'{value.ToString()}'");
                }
                else
                {
                    values.Add(field.Name, $"{value.ToString()}");
                }
            }
            string keysSql = string.Join(", ", values.Keys.Select(n => $"`{n}`"));
            string valuesSql = string.Join(", ", values.Values);
            string equalsSql = string.Join(", ", values.Where(kv => !keys.ContainsKey(kv.Key)).Select(kv => $"`{kv.Key}` = {kv.Value}"));

            builder.Append($" INSERT INTO `{tableScheme.TableName}` ({keysSql}) VALUES ({valuesSql}) ON DUPLICATE KEY UPDATE {equalsSql};");

            return true;
        }
        /// <summary>
        /// 解析存储的数据
        /// </summary>
        private T Deserialize<T>(ITableScheme tableScheme, MySqlDataReader reader) where T : MajorEntity, new()
        {
            if (reader.FieldCount == 0)
            {
                return null;
            }
            T entity = new T();
            for (int i = 0; i < reader.FieldCount; i++)
            {
                string name = reader.GetName(i);
                object value = null;
                if (!tableScheme.FieldsByName.ContainsKey(name) || reader.IsDBNull(i)) { continue; }
                ITableFieldScheme tableField = tableScheme.FieldsByName[name];
                switch (tableField.FieldType)
                {
                    case ETableFieldType.Bit: { value = reader.GetBoolean(i); } break;
                    case ETableFieldType.Blob:
                    case ETableFieldType.LongBlob:
                        {
                            using (MemoryStream stream = new MemoryStream())
                            {
                                byte[] buff = new byte[1024];
                                int length = 0;
                                int index = 0;
                                while ((length = (int)reader.GetBytes(index, 0, buff, 0, buff.Length)) > 0)
                                {
                                    index += length;
                                    stream.Write(buff, 0, length);
                                }
                                value = stream.ToArray();
                            }
                        }
                        break;
                    case ETableFieldType.Byte: { value = reader.GetByte(i); } break;
                    case ETableFieldType.DateTime: { value = reader.GetDateTime(i); } break;
                    case ETableFieldType.Double: { value = reader.GetDouble(i); } break;
                    case ETableFieldType.Float: { value = reader.GetFloat(i); } break;
                    case ETableFieldType.Int: { value = reader.GetInt32(i); } break;
                    case ETableFieldType.Long: { value = reader.GetInt64(i); } break;
                    case ETableFieldType.LongText:
                    case ETableFieldType.Text:
                    case ETableFieldType.String: { value = reader.GetString(i); } break;
                    case ETableFieldType.SByte: { value = reader.GetSByte(i); } break;
                    case ETableFieldType.Short: { value = reader.GetInt16(i); } break;
                    case ETableFieldType.UInt: { value = reader.GetUInt32(i); } break;
                    case ETableFieldType.ULong: { value = reader.GetUInt64(i); } break;
                    case ETableFieldType.UShort: { value = reader.GetUInt16(i); } break;
                }
                if (tableField.Type.PropertyType.IsSubclassOf(FEType<PropertyEntity>.Type))
                {
                    tableField.Type.SetValue(entity, JsonUtility.ToObject(value.GetString(), tableField.Type.PropertyType));
                }
                else if (tableField.Type.PropertyType == FEType<TimeSpan>.Type)
                {
                    tableField.Type.SetValue(entity, new TimeSpan((long)value));
                }
                else
                {
                    tableField.Type.SetValue(entity, value);
                }
            }
            entity.OnPulledFromDb();
            return entity;
        }
    }
}
