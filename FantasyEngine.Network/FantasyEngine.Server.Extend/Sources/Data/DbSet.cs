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
using FantasyEngine.Collections;
using FantasyEngine.Log;
using FantasyEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 当DbSet中的数据发生变化时调用
    /// </summary>
    internal delegate void OnDbSetDataModified<T>(EntityKey key, T entity) where T : MajorEntity, new();
    /// <summary>
    /// 缓存中的数据结构
    /// </summary>
    internal sealed class DbSet<T> : IDbSet<T> where T : MajorEntity, new()
    {
        /// <summary>
        /// 数据变化监听
        /// </summary>
        public event OnDbSetDataModified<T> OnDataModified;

        /// <summary>
        /// 表结构配置
        /// </summary>
        public ITableScheme TableScheme { get; private set; }

        /// <summary>
        /// 缓存中的数据，热数据
        /// </summary>
        private ThreadSafeDictionary<EntityKey, T> _rawEntities = new ThreadSafeDictionary<EntityKey, T>();

        /// <summary>
        /// 缓存中的数据，热数据
        /// </summary>
        public IReadOnlyDictionary<EntityKey, T> RawEntities { get { return _rawEntities; } }

        /// <summary>
        /// 现在真在使用的修改缓存池id
        /// </summary>
        private int _modifiedEntitiesPoolIndex = 0;

        /// <summary>
        /// 修改池的锁
        /// </summary>
        private readonly object _modifiedEntitiesPoolLocker = new object();

        /// <summary>
        /// 已经改动过的实例，这些数据应该会出现在热数据中，主要用于推送。
        /// 防止多线程阻塞，所以使用集合池
        /// </summary>
        private IReadOnlyDictionary<EntityKey, T>[] _modifiedEntitiesPool = null;

        /// <summary>
        /// 现在真在使用的修改缓存池id
        /// </summary>
        public int ModifiedEntitiesPoolIndex { get { return _modifiedEntitiesPoolIndex; } }

        /// <summary>
        /// 已经改动过的实例，这些数据应该会出现在热数据中，主要用于推送。
        /// 防止多线程阻塞，所以使用集合池
        /// </summary>
        public IReadOnlyList<IReadOnlyDictionary<EntityKey, T>> ModifiedEntitiesPool { get { return _modifiedEntitiesPool; } }

        /// <summary>
        /// 冷数据
        /// </summary>
        private ThreadSafeDictionary<EntityKey, T> _coldEntities = new ThreadSafeDictionary<EntityKey, T>();

        /// <summary>
        /// 冷数据
        /// </summary>
        public IReadOnlyDictionary<EntityKey, T> ColdEntities { get { return _coldEntities; } }

        /// <summary>
        /// 原子锁
        /// </summary>
        private int _releaseCountdown = 0;

        /// <summary>
        /// 卸载冷数据倒计时
        /// </summary>
        public int ReleaseCountdown { get { return _releaseCountdown; } set { Interlocked.Exchange(ref _releaseCountdown, value); } }

        /// <summary>
        /// 推送倒计时
        /// </summary>
        public int CommitCountdown { get; set; } = Settings.DataCommitInterval;

        /// <summary>
        /// 用于锁的对象
        /// </summary>
        public object SyncRoot { get; private set; } = new object();

        ///// <summary>
        ///// 提交的线程管理
        ///// </summary>
        //internal Thread CommitThread { get; private set; }

        /// <summary>
        /// 提交判断标示
        /// </summary>
        private int _commitFlag = 0;

        public DbSet()
        {
            _modifiedEntitiesPool = new IReadOnlyDictionary<EntityKey, T>[3];
            for (int i = 0; i < _modifiedEntitiesPool.Length; i++)
            {
                _modifiedEntitiesPool[i] = new Dictionary<EntityKey, T>();
            }
            TableScheme = DataContext.GetTableScheme<T>();
            //CommitThread = new Thread(OnCommitModifiedData);
            //CommitThread.Priority = ThreadPriority.Highest;
            //CommitThread.Start();
        }

        /// <summary>
        /// 非泛型修改数据通知，内部强制转换
        /// </summary>
        public void OnModified(EntityKey key, MajorEntity entity)
        {
            try
            {
                OnModified(key, entity as T);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.ENTITY, "Cast error.", e);
            }
        }

        /// <summary>
        /// 当数据修改时执行
        /// </summary>
        public void OnModified(EntityKey key, T entity)
        {
            //判空
            if (string.IsNullOrEmpty(key))
            {
                FEConsole.WriteExceptionWithCategory(Categories.ENTITY, new NullReferenceException("DbSet receive an empty-key entity!"));
                return;
            }

            //实例为空，就只做移除，通知事件在Remove里
            if (entity == null)
            {
                Remove(key);
                return;
            }

            //判断Key是否发生变化
            if (entity.KeyIsModified())
            {
                //老Key有效的话移除
                if (!string.IsNullOrEmpty(entity.GetOldEntityKey()))
                {
                    FEConsole.WriteWarnFormatWithCategory(Categories.ENTITY, "DbSet<{0}>: A key of entity has been changed from {0} to {1}.", entity.GetOldEntityKey(), entity.GetEntityKey());
                    Remove(entity.GetOldEntityKey());
                }
            }

            //刷新Key
            entity.RefreshEntityKey();

            //如果当前的Key为空，就不会加入更新列表
            if (string.IsNullOrEmpty(entity.GetEntityKey()))
            {
                return;
            }

            //更新
            lock (_modifiedEntitiesPoolLocker)
            {
                ((Dictionary<EntityKey, T>)_modifiedEntitiesPool[_modifiedEntitiesPoolIndex])[key] = entity;
            }

            Modify(entity);
            //通知事件
            OnDataModified?.Invoke(key, entity);
        }

        /// <summary>
        /// 冷热数据库查询
        /// </summary>
        public T Find(EntityKey key)
        {
            // 从数据库中加载
            T result;
            if (TableScheme.StorageType.HasFlag(EStorageType.ReadFromRawDb))
            {
                if (!_rawEntities.TryGetValue(key, out result)) { return result; }
            }
            if (TableScheme.StorageType.HasFlag(EStorageType.ReadFromDb))
            {
                if (ReleaseCountdown > 0 && _coldEntities.TryGetValue(key, out result)) { return result; }
                if (!DataContext.Databases.ContainsKey(TableScheme.ConnectKey)) { return null; }
                return DataContext.Databases[TableScheme.ConnectKey].Find<T>(key);
            }
            return null;
        }

        /// <summary>
        /// 需要从两个列表中删除
        /// </summary>
        /// <param name="key"></param>
        private void Remove(EntityKey key)
        {
            T entity;
            if (_rawEntities.TryGetValue(key, out entity))
            {
                entity.SetState(EStorageState.Removed);
            }
            _rawEntities.Remove(key);
            if (_coldEntities.TryGetValue(key, out entity))
            {
                entity.SetState(EStorageState.Removed);
            }
            _coldEntities.Remove(key);

            //提交更改
            lock (_modifiedEntitiesPoolLocker)
            {
                ((Dictionary<EntityKey, T>)_modifiedEntitiesPool[_modifiedEntitiesPoolIndex])[key] = null;
            }
            //通知
            OnDataModified?.Invoke(key, null);
        }

        /// <summary>
        /// 修改集合
        /// </summary>
        private void Modify(T entity)
        {
            //只有配置进入缓存才会缓存
            if (TableScheme.InCache)
            {
                _rawEntities[entity.GetEntityKey()] = entity;
            }
            _coldEntities.Remove(entity.GetEntityKey());
        }

        /// <summary>
        /// 卸载冷数据
        /// </summary>
        public void ReleaseColdEntities()
        {
            Thread.Sleep(100);
            if (ReleaseCountdown <= 0)
            {
                _coldEntities.Clear();
                FEConsole.Write(GetType().Name + " ReleaseColdEntities.");
            }
        }

        /// <summary>
        /// 将所有修改过的数据进行提交
        /// </summary>
        public void CommitModifiedData()
        {
            OnCommitModifiedData(null);
            //Interlocked.Exchange(ref commitFlag, 1);
        }
        /// <summary>
        /// 提交操作
        /// </summary>
        /// <param name="state"></param>
        private void OnCommitModifiedData(object state)
        {
            //while (true)
            //{
            //    if (commitFlag > 0)
            //    {
            //        Interlocked.Exchange(ref commitFlag, 0);
            if (_commitFlag > 0)
            {
                return;
            }
            Interlocked.Exchange(ref _commitFlag, 1);
            try
            {
                int pushIndex = _modifiedEntitiesPoolIndex - 1;
                if (pushIndex < 0)
                {
                    pushIndex = _modifiedEntitiesPool.Length - 1;
                }
                int nextIndex = _modifiedEntitiesPoolIndex + 1;
                if (nextIndex >= _modifiedEntitiesPool.Length)
                {
                    nextIndex = 0;
                }
                Interlocked.Exchange(ref _modifiedEntitiesPoolIndex, nextIndex);
                Dictionary<EntityKey, T> entities = (Dictionary<EntityKey, T>)_modifiedEntitiesPool[pushIndex];
                ForceCommitModifiedData(entities);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
            }
            Interlocked.Exchange(ref _commitFlag, 0);
            //    }
            //    Thread.Sleep(10);
            //}
        }

        /// <summary>
        /// 提交所有修改
        /// </summary>
        public void ForceCommitAllModifiedData()
        {
            lock (_modifiedEntitiesPoolLocker)
            {
                foreach (Dictionary<EntityKey, T> entities in _modifiedEntitiesPool)
                {
                    ForceCommitModifiedData(entities);
                }
            }
        }

        /// <summary>
        /// 强制提交修改
        /// </summary>
        private void ForceCommitModifiedData(IDictionary<EntityKey, T> entities)
        {
            try
            {
                CommitModifiedEntitys(entities, () => { entities.Clear(); });
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.ENTITY, e);
            }
        }

        /// <summary>
        /// 拉取所有热数据
        /// </summary>
        public void PullAllRawData()
        {
            if (TableScheme.InCache)
            {
                if (TableScheme.StorageType.HasFlag(EStorageType.ReadFromRawDb))
                {
                    if (DataContext.RawDatabase != null)
                    {
                        IEnumerable<T> iterator = DataContext.RawDatabase.LoadAll<T>();
                        _rawEntities.Clear();
                        foreach (T entity in iterator)
                        {
                            entity.OnPulledFromDb();
                            _rawEntities[entity.GetEntityKey()] = entity;
                        }
                    }
                }
            }
            else
            {
                _rawEntities.Clear();
            }
        }

        /// <summary>
        /// 将所有的缓存的热数据全部推送出去
        /// </summary>
        public void PushAllRawData()
        {
            lock (_rawEntities.SyncRoot)
            {
                CommitModifiedEntitys(_rawEntities);
            }
        }

        /// <summary>
        /// 推送集合中的数据
        /// </summary>
        private void CommitModifiedEntitys(IDictionary<EntityKey, T> entities, System.Action onCopied = null)
        {
            if (TableScheme.StorageType.HasFlag(EStorageType.WriteToDb) || TableScheme.StorageType.HasFlag(EStorageType.WriteToRawDb))
            {
                if (entities.Count > 0)
                {
                    List<DbCommition> commitions = new List<DbCommition>();
                    Dictionary<EntityKey, T> dic = null;
                    bool lockToken = true;//false 原本是为了线程安全，现在使用的缓存池，在读取时可以不用加锁
                    try
                    {
                        //Monitor.TryEnter(entities.SyncRoot, Settings.LockerTimeout, ref lockToken);
                        if (lockToken)
                        {
                            //中间缓存池，防死锁
                            dic = new Dictionary<EntityKey, T>(entities);
                            onCopied?.Invoke();
                        }
                    }
                    finally
                    {
                        if (lockToken)
                        {
                            //Monitor.Exit(entities.SyncRoot);
                        }
                    }
                    if (lockToken)
                    {
                        string database = TableScheme.ConnectKey;
                        foreach (KeyValuePair<EntityKey, T> kv in dic)
                        {
                            commitions.Add(new DbCommition(kv.Key, kv.Value == null ? EModifyType.Remove : kv.Value.ModifiedType, kv.Value));
                            kv.Value?.ResetModifiedType();
                        }
                        if (TableScheme.StorageType.HasFlag(EStorageType.WriteToRawDb) && DataContext.RawDatabase != null)
                        {
                            DataContext.RawDatabase.CommitModifiedEntitys(commitions);
                        }
                        if (TableScheme.StorageType.HasFlag(EStorageType.WriteToDb))
                        {
                            if (!string.IsNullOrEmpty(database) && DataContext.Databases.ContainsKey(database))
                            {
                                DataContext.Databases[database].CommitModifiedEntitys(commitions);
                            }
                        }
                    }
                }
            }
        }
        /// <summary>
        /// 执行读取全部数据的函数锁
        /// </summary>
        private readonly object _loadingAllFlag = new object();
        /// <summary>
        /// 读取全部数据，只要有一个查询其他全部等待，只为了合批只做一次查询
        /// </summary>
        public IReadOnlyDictionary<EntityKey, T> LoadAll()
        {
            Dictionary<EntityKey, T> result;
            //如果有标识从数据库中读
            if (TableScheme.StorageType.HasFlag(EStorageType.ReadFromDb))
            {
                // LoadAll;
                lock (_loadingAllFlag)
                {
                    //还未卸载直接加载
                    if (_releaseCountdown <= 0 || ColdEntities.Count == 0)
                    {
                        //数据库加载
                        _coldEntities.Clear();
                        if (!DataContext.Databases.ContainsKey(TableScheme.ConnectKey)) { return _coldEntities; }
                        IEnumerable<T> data = DataContext.Databases[TableScheme.ConnectKey].LoadAll<T>();
                        foreach (T entity in data)
                        {
                            EntityKey key = entity.GetEntityKey();
                            T value;
                            if (RawEntities.TryGetValue(key, out value))
                            {
                                continue;
                            }
                            else
                            {
                                _coldEntities[key] = entity;
                            }
                        }
                    }

                    result = new Dictionary<EntityKey, T>(_coldEntities);
                    ReleaseCountdown = Settings.DataReleasePeriod;
                }
            }
            else
            {
                result = new Dictionary<EntityKey, T>();
            }
            lock (_rawEntities.SyncRoot)
            {
                foreach (KeyValuePair<EntityKey, T> kv in _rawEntities)
                {
                    result[kv.Key] = kv.Value;
                }
            }
            return result;
        }

        /// <summary>
        /// 执行释放
        /// </summary>
        public void Release()
        {
            //CommitThread.Abort();
        }

        /// <summary>
        /// 移出冷数据
        /// </summary>
        public void CheckOutColdEntities()
        {
            if (TableScheme.NeverExpired) { return; }
            bool lockToken = false;
            List<T> entities = null;
            try
            {
                Monitor.TryEnter(_rawEntities.SyncRoot, Settings.LockerTimeout, ref lockToken);
                if (lockToken)
                {
                    entities = new List<T>(_rawEntities.Values);
                }
            }
            finally
            {
                if (lockToken)
                {
                    Monitor.Exit(_rawEntities.SyncRoot);
                }
            }
            if (lockToken && entities != null)
            {
                foreach (T entity in entities)
                {
                    if (entity.IsExpired)
                    {
                        _rawEntities.Remove(entity.GetEntityKey());
                    }
                    if (ReleaseCountdown > 0)
                    {
                        _coldEntities.Add(entity.GetEntityKey(), entity);
                    }
                }
            }
        }
    }
}
