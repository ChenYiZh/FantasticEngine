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
using System.Data.Common;
using System.Text;
using FantasticEngine.Log;
using FantasticEngine.Network.Server.Entity;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// 数据库类型
    /// </summary>
    public enum EDatabase
    {
        /// <summary>
        /// 不在识别范围的数据库
        /// </summary>
        Unknow,
        /// <summary>
        /// MySQL
        /// </summary>
        MySQL,
        /// <summary>
        /// SQLServer
        /// </summary>
        SQLServer,
        /// <summary>
        /// Redis
        /// </summary>
        Redis,
    }

    /// <summary>
    /// 数据库操作
    /// </summary>
    public abstract class Database : ISQLDatabase
    {
        /// <summary>
        /// 日志等级
        /// </summary>
        public const string LOG_LEVEL = "SQL";

        /// <summary>
        /// 是什么类型的数据库
        /// </summary>
        public EDatabase Kind { get { return Setting.Kind; } }

        /// <summary>
        /// 配置信息
        /// </summary>
        public IDatabaseSetting Setting { get; private set; }

        /// <summary>
        /// 判断连接状态
        /// </summary>
        public abstract bool Connected { get; protected set; }

        /// <summary>
        /// 设置配置文件，初始化时执行
        /// </summary>
        public virtual void SetSettings(IDatabaseSetting setting)
        {
            if (setting == null)
            {
                throw new ArgumentNullException("Can not create connection, because the setting is null.");
            }
            Setting = setting;
        }

        /// <summary>
        /// 对表结构进行调整或创建
        /// </summary>
        public abstract void GenerateOrUpdateTableScheme(ITableScheme table);

        /// <summary>
        /// 断开连接
        /// </summary>
        public abstract void Close();

        /// <summary>
        /// 开始连接
        /// </summary>
        public virtual bool Connect() { return false; }

        /// <summary>
        /// 操作一堆数据
        /// </summary>
        public abstract bool CommitModifiedEntitys(IEnumerable<DbCommition> commitions);

        /// <summary>
        /// 读取表中所有数据
        /// </summary>
        public abstract IEnumerable<T> LoadAll<T>() where T : MajorEntity, new();

        /// <summary>
        /// 通过EntityKey，查询某一条数据，没有就返回空
        /// </summary>
        public abstract T Find<T>(EntityKey key) where T : MajorEntity, new();

        /// <summary>
        /// 创建数据库连接
        /// </summary>
        /// <param name="setting"></param>
        /// <returns></returns>
        public static ISQLDatabase Make(IDatabaseSetting setting)
        {
            switch (setting.Kind)
            {
                case EDatabase.MySQL: return new MySQLDatabase();
                default:
                    {
                        throw new NotSupportedException("The database of '" + setting.Kind.ToString() + "' is not realized.");
                    }
            }
        }
    }
}
