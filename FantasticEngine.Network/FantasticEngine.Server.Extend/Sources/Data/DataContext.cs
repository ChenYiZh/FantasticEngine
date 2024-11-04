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
using FantasticEngine.Log;
using FantasticEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// 数据读取
    /// </summary>
    public static class DataContext
    {
        /// <summary>
        /// Redis管理类
        /// </summary>
        public static IRawDatabase RawDatabase { get; set; }

        /// <summary>
        /// 连接着的数据库
        /// </summary>
        public static IReadOnlyDictionary<string, ISQLDatabase> Databases { get; private set; }

        /// <summary>
        /// 表结构
        /// </summary>
        private static Dictionary<Type, ITableScheme> _tableSchemes = new Dictionary<Type, ITableScheme>();

        /// <summary>
        /// 表结构映射
        /// </summary>
        internal static IReadOnlyDictionary<Type, ITableScheme> TableSchemes { get { return _tableSchemes; } }

        /// <summary>
        /// 缓存中的数据
        /// </summary>
        private static Dictionary<Type, IDbSet> _entityPool = new Dictionary<Type, IDbSet>();

        /// <summary>
        /// 缓存中的数据
        /// </summary>
        internal static IReadOnlyDictionary<Type, IDbSet> EntityPool { get { return _entityPool; } }

        /// <summary>
        /// 计时器间隔设为100ms
        /// </summary>
        private const int TIMER_INTERVAL = 100;

        /// <summary>
        /// 计时器
        /// </summary>
        private static Timer _timer;

        /// <summary>
        /// 读取数据
        /// </summary>
        public static IEntitySet<T> GetEntity<T>() where T : MajorEntity, new()
        {
            Type type = FEType<T>.Type;
            if (!EntityPool.ContainsKey(type))
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.ENTITY, "There is no EntitySet type of {0}.", type.FullName);
                return null;
            }
            IDbSet<T> dbSet = (IDbSet<T>)EntityPool[type];
            return new EntitySet<T>(dbSet);
        }

        /// <summary>
        /// 获取表名
        /// </summary>
        public static ITableScheme GetTableScheme(Type type)
        {
            if (_tableSchemes.ContainsKey(type))
            {
                return _tableSchemes[type];
            }
            return null;
        }

        /// <summary>
        /// 获取表名
        /// </summary>
        public static ITableScheme GetTableScheme<T>() where T : MajorEntity, new()
        {
            return GetTableScheme(FEType<T>.Type);
        }

        /// <summary>
        /// 初始化
        /// </summary>
        internal static void Initialize()
        {
            //生成表的映射关系
            Assembly assembly = AssemblyService.Assemblies.FirstOrDefault();
            Type majorType = FEType<MajorEntity>.Type;
            if (assembly != null)
            {
                Type[] types = assembly.GetTypes();
                foreach (Type type in types)
                {
                    if (type.IsSubclassOf(majorType) && type.GetCustomAttribute<EntityTableAttribute>() != null)
                    {
                        InitializeDataContainer(type);
                    }
                }
            }

            //建立Redis连接
            if (RawDatabase == null && Settings.RedisSetting != null && Settings.RedisSetting.IsValid)
            {
                RawDatabase = new RedisDatabase(Settings.RedisSetting);
            }
            else
            {
                FEConsole.WriteWarnFormat("No raw database exists.");
            }

            //建立数据库连接
            if (Databases == null)
            {
                Databases = new Dictionary<string, ISQLDatabase>();
            }
            if (Settings.DatabaseSettings != null)
            {
                IDictionary<string, ISQLDatabase> batabases = (IDictionary<string, ISQLDatabase>)Databases;
                foreach (KeyValuePair<string, IDatabaseSetting> setting in Settings.DatabaseSettings)
                {
                    if (batabases.ContainsKey(setting.Key))
                    {
                        FEConsole.WriteWarnFormatWithCategory(Categories.FOOLISH_SERVER, "The connectkey: '{0}' is exists, and database connection will not create.", setting.Key);
                        continue;
                    }
                    batabases[setting.Key] = Database.Make(setting.Value);
                    batabases[setting.Key].SetSettings(setting.Value);
                }
            }
        }

        /// <summary>
        /// 检查数据库结构是否变更
        /// </summary>
        public static void CheckTableSchemes()
        {
            FEConsole.WriteInfoFormatWithCategory(Categories.ENTITY, "Check whether tables in the dbs have been changed...");
            foreach (ITableScheme tableScheme in _tableSchemes.Values)
            {
                ISQLDatabase database;
                if (Databases.TryGetValue(tableScheme.ConnectKey, out database))
                {
                    database.GenerateOrUpdateTableScheme(tableScheme);
                }
                else
                {
                    FEConsole.WriteWarnFormat("No database of '{0}' exists.", tableScheme.ConnectKey);
                }
            }
        }

        /// <summary>
        /// 开始读取数据
        /// </summary>
        internal static void Start()
        {
            if (RawDatabase != null)
            {
                // Redis 连接
                RawDatabase.Connect();
            }
            // 数据库连接
            foreach (KeyValuePair<string, ISQLDatabase> database in Databases)
            {
                database.Value.Connect();
            }

            //检查数据库结构是否变更
            CheckTableSchemes();

            //加载所有热数据
            FEConsole.WriteInfoFormatWithCategory(Categories.ENTITY, "Start loading hot data...");
            Parallel.ForEach(_entityPool.Values, (IDbSet dbSet) =>
            {
                dbSet.PullAllRawData();
            });

            //开启计时器
            _timer = new Timer(Tick, null, TIMER_INTERVAL, TIMER_INTERVAL);
        }

        /// <summary>
        /// 反射用的
        /// </summary>
        private static Type _dbSetType = typeof(DbSet<>);
        /// <summary>
        /// 初始化数据容器
        /// </summary>
        /// <param name="type"></param>
        private static void InitializeDataContainer(Type type)
        {
            _tableSchemes.Add(type, new TableScheme(type));
            Type dbSetType = _dbSetType.MakeGenericType(type);
            IDbSet dbSet = (IDbSet)Activator.CreateInstance(dbSetType);
            _entityPool.Add(type, dbSet);
        }

        /// <summary>
        /// 计时器事件
        /// </summary>
        private static void Tick(object sender)
        {
            //Parallel.ForEach(EntityPool.Values, (IDbSet dbSet) =>
            foreach (IDbSet dbSet in EntityPool.Values)
            {
                //FConsole.WriteWarn("Tick: ");
                try
                {
                    bool commit = false;
                    bool release = false;
                    lock (dbSet.SyncRoot)
                    {
                        dbSet.CommitCountdown -= TIMER_INTERVAL;
                        //是否需要提交修改
                        commit = dbSet.CommitCountdown <= 0;
                        if (commit)
                        {
                            dbSet.CommitCountdown = Settings.DataCommitInterval;
                        }
                        //是否需要释放冷数据
                        if (dbSet.ReleaseCountdown > 0)
                        {
                            dbSet.ReleaseCountdown -= TIMER_INTERVAL;
                            release = dbSet.ReleaseCountdown <= 0;
                        }
                    }
                    if (commit)
                    {
                        ThreadPool.UnsafeQueueUserWorkItem((obj) =>
                        {
                            Thread.CurrentThread.Priority = ThreadPriority.Highest;
                            dbSet.CommitModifiedData();
                        }, null);
                    }
                    if (release)
                    {
                        ThreadPool.UnsafeQueueUserWorkItem((obj) => { dbSet.ReleaseColdEntities(); }, null);
                    }
                    //检查需要移出的冷数据
                    ThreadPool.UnsafeQueueUserWorkItem((obj) => { dbSet.CheckOutColdEntities(); }, null);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
                }
            }
            //});
        }

        /// <summary>
        /// 退出时调用
        /// </summary>
        internal static void Shutdown()
        {
            //关闭计时器
            if (_timer != null)
            {
                try
                {
                    _timer.Dispose();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
                }
                _timer = null;
            }
            //推送数据
            Parallel.ForEach(EntityPool.Values, (IDbSet dbSet) =>
            {
                try
                {
                    dbSet.ForceCommitAllModifiedData();
                    dbSet.Release();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
                }
            });
            //强制Redis落地
            if (RawDatabase != null)
            {
                RawDatabase.Close();
            }
            foreach (ISQLDatabase database in Databases.Values)
            {
                database.Close();
            }
        }

        /// <summary>
        /// 将缓存的数据全部提交
        /// </summary>
        public static void PushAllRawData()
        {
            Parallel.ForEach(EntityPool.Values, (IDbSet dbSet) =>
            {
                try
                {
                    dbSet.PushAllRawData();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
                }
            });
        }
    }
}
