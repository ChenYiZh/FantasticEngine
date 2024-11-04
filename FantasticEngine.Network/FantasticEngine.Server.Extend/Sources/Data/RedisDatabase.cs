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
using FantasticEngine.Network.Server.Entity;
using StackExchange.Redis;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// Redis 链接池
    /// </summary>
    public class RedisDatabase : IRawDatabase
    {
        /// <summary>
        /// 解析方案，默认Protobuff
        /// </summary>
        public IEntityConverter Converter { get; set; } = new EntityProtobufConverter();
        /// <summary>
        /// Redis连接池，这是一个池子
        /// </summary>
        public ConnectionMultiplexer Redis { get; private set; } = null;
        /// <summary>
        /// 配置信息
        /// </summary>
        public IRedisSetting Setting { get; private set; }
        /// <summary>
        /// 是否还连接着
        /// </summary>
        public bool Connected { get { return Redis != null && Redis.IsConnected; } }

        /// <summary>
        /// 心跳检测
        /// </summary>
        private Timer _heartbeatTime = null;
        /// <summary>
        /// Redis的连接服务器
        /// </summary>
        private StackExchange.Redis.IServer _redisServer { get; set; }
        /// <summary>
        /// 连接中的数据库
        /// </summary>
        private StackExchange.Redis.IDatabase _database { get; set; }
        /// <summary>
        /// 判断是否正在事务处理
        /// </summary>
        private int _isExecuting = 0;
        /// <summary>
        /// 什么类型的数据库
        /// </summary>
        public EDatabase Kind { get; private set; }
        /// <summary>
        /// 初始化
        /// </summary>
        public RedisDatabase(IRedisSetting setting)
        {
            _isExecuting = 0;
            Setting = setting;
            Kind = EDatabase.Redis;
        }
        /// <summary>
        /// 关闭
        /// </summary>
        public void Close()
        {
            FEConsole.WriteWithCategory(Kind.ToString(), "Redis closing...");
            if (_heartbeatTime != null)
            {
                try
                {
                    _heartbeatTime.Dispose();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
                _heartbeatTime = null;
            }
            //如果有正在执行的事务，等待处理完
            bool executing = false;
            while (_isExecuting > 0)
            {
                executing = true;
                Console.Write("\r" + FEConsole.FormatCustomMessage(Kind.ToString(), "Redis is saving data..."));
                Thread.Sleep(Settings.LockerTimeout);
            }
            if (executing)
            {
                Console.WriteLine();
                Console.WriteLine();
            }
            if (Redis != null)
            {
                if (_redisServer != null)
                {
                    try
                    {
                        //阻塞当前线程，强制Redis落地
                        _redisServer.Save(SaveType.ForegroundSave);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                    }
                }
                try
                {
                    Redis.Close();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
                Redis = null;
                Redis = null;
                _database = null;
            }
        }
        /// <summary>
        /// 连接函数
        /// </summary>
        /// <returns></returns>
        public bool Connect()
        {
            FEConsole.WriteInfoWithCategory(Kind.ToString(), "Redis connecting...");
            Reconnect();
            if (Connected && _heartbeatTime == null)
            {
                _heartbeatTime = new Timer(Ping, this, 60000, 60000);
            }
            return Connected;
        }

        /// <summary>
        /// 检查连接状态
        /// </summary>
        private void CheckRedis()
        {
            if (!Connected)
            {
                Reconnect();
            }
        }

        /// <summary>
        /// 重新连接
        /// </summary>
        public void Reconnect()
        {
            ConfigurationOptions options = new ConfigurationOptions();
            options.get_EndPoints().Add(new IPEndPoint(IPAddress.Parse(Setting.Host), Setting.Port));
            options.Password = Setting.Password;
            options.ConnectTimeout = Setting.Timeout;
            options.DefaultDatabase = Setting.DbIndex;
            //退出时强制读写需要用到
            options.AllowAdmin = true;
            if (Redis == null)
            {
                try
                {
                    Redis = ConnectionMultiplexer.Connect(options);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
            }
            else if (!Connected)
            {
                try
                {
                    Redis.Close();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
                try
                {
                    Redis = ConnectionMultiplexer.Connect(options);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                }
            }
            if (Connected)
            {
                _redisServer = Redis.GetServer(Setting.Host, Setting.Port);
                _database = Redis.GetDatabase(Setting.DbIndex);
                FEConsole.WriteInfoFormatWithCategory(Kind.ToString(), "Redis[{0}] connected.", Setting.DbIndex);
            }
        }
        /// <summary>
        /// 心跳检测
        /// </summary>
        /// <param name="state"></param>
        private void Ping(object state)
        {
            try
            {
                _redisServer.Ping();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                CheckRedis();
            }
        }

        /// <summary>
        /// 操作一堆数据
        /// </summary>
        public bool CommitModifiedEntitys(IEnumerable<DbCommition> commitions)
        {
            Interlocked.Increment(ref _isExecuting);
            CheckRedis();
            try
            {
                IBatch batch = _database.CreateBatch();
                foreach (DbCommition commition in commitions)
                {
                    //FConsole.Write(commition.Key.ToString() + ": " + commition.ModifyType);
                    //FConsole.Write("Redis Count: " + Interlocked.Increment(ref count));
                    if (commition.ModifyType == EModifyType.Remove || commition.Entity == null)
                    {
                        batch.HashDeleteAsync(commition.Key.TableName, commition.Key.KeyName);
                    }
                    else
                    {
                        RedisValue value = ConvertToValue(Converter.Type, commition.Entity);
                        batch.HashSetAsync(commition.Key.TableName, commition.Key.KeyName, value);
                    }
                }
                batch.Execute();
                return true;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Kind.ToString(), e);
                return false;
            }
            finally
            {
                Interlocked.Decrement(ref _isExecuting);
            }
        }

        /// <summary>
        /// 读取表中所有
        /// </summary>
        public IEnumerable<T> LoadAll<T>() where T : MajorEntity, new()
        {
            CheckRedis();
            Task<HashEntry[]> task = _database.HashGetAllAsync(EntityKey.MakeTableName(FEType<T>.Type));
            task.Wait();
            HashEntry[] entries = task.Result;
            T[] entities = new T[entries.Length];
            for (int i = 0; i < entries.Length; i++)
            {
                entities[i] = ConvertToEntity<T>(Converter.Type, entries[i].Value);
            }
            return entities;
        }

        /// <summary>
        /// 通过EntityKey，查询某一条数据，没有就返回空
        /// </summary>
        public T Find<T>(EntityKey key) where T : MajorEntity, new()
        {
            CheckRedis();
            Task<RedisValue> task = _database.HashGetAsync(key.TableName, key.KeyName);
            task.Wait();
            if (task.Result.HasValue)
            {
                return ConvertToEntity<T>(Converter.Type, task.Result);
            }
            return null;
        }

        /// <summary>
        /// 数据解析
        /// </summary>
        private RedisValue ConvertToValue(EConvertType type, MajorEntity entity)
        {
            switch (type)
            {
                case EConvertType.Binary:
                    {
                        EntityConverter<byte[]> converter = (EntityConverter<byte[]>)Converter;
                        return converter.Serialize(entity);
                    }
                case EConvertType.String:
                    {
                        EntityConverter<string> converter = (EntityConverter<string>)Converter;
                        return converter.Serialize(entity);
                    }
                default:
                    throw new Exception("The converter in RedisDatabase is an error type.");
            }
        }

        /// <summary>
        /// 数据转换
        /// </summary>
        private T ConvertToEntity<T>(EConvertType type, RedisValue value) where T : MajorEntity, new()
        {
            switch (type)
            {
                case EConvertType.Binary:
                    {
                        EntityConverter<byte[]> converter = (EntityConverter<byte[]>)Converter;
                        return converter.Deserialize<T>(value);
                    }
                case EConvertType.String:
                    {
                        EntityConverter<string> converter = (EntityConverter<string>)Converter;
                        return converter.Deserialize<T>(value);
                    }
                default:
                    throw new Exception("The converter in RedisDatabase is an error type.");
            }
        }
    }
}
