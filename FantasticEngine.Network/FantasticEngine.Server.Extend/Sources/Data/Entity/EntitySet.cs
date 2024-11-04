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
using FantasticEngine.Log;
using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FantasticEngine.Network.Server.Entity
{
    /// <summary>
    /// 数据对象集合
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public sealed class EntitySet<T> :/* Struct.Entity,*/ IEntitySet<T> where T : MajorEntity, new()
    {
        /// <summary>
        /// 全局管理的数据对象池
        /// </summary>
        private IDbSet<T> _dbSet;

        /// <summary>
        /// 泛型的类
        /// </summary>
        public Type EntityType { get; private set; } = FEType<T>.Type;

        /// <summary>
        /// 全数据
        /// </summary>
        private ThreadSafeDictionary<EntityKey, T> _fullData = null;

        /// <summary>
        /// 这个类的缓存数据
        /// </summary>
        private ThreadSafeDictionary<EntityKey, T> Dictionary
        {
            get { return _fullData != null ? _fullData : (ThreadSafeDictionary<EntityKey, T>)_dbSet.RawEntities; }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        internal EntitySet(IDbSet<T> dbSet)
        {
            _dbSet = dbSet;
            ThreadSafeDictionary<EntityKey, T> source = (ThreadSafeDictionary<EntityKey, T>)dbSet.RawEntities;

            lock (source.SyncRoot)
            {
                _fullData = new ThreadSafeDictionary<EntityKey, T>(source);
            }

            lock (dbSet.SyncRoot)
            {
                dbSet.OnDataModified += OnDbSetDataModified;
            }
        }

        /// <summary>
        /// 析构时注销
        /// </summary>
        ~EntitySet()
        {
            _dbSet.OnDataModified -= OnDbSetDataModified;
        }

        ///// <summary>
        ///// 通过唯一主键查询
        ///// </summary>
        //public T Find(long entityId)
        //{
        //    return Find(entityId);
        //}

        /// <summary>
        /// 主键查找，如果缓存中找不到，会从数据库中查询
        /// </summary>
        public T Find(params object[] keys)
        {
            EntityKey entityKey = new EntityKey(EntityType, keys);
            return Find(entityKey);
        }

        /// <summary>
        /// 主键类查询
        /// </summary>
        public T Find(EntityKey key)
        {
            if (key.Type == null)
            {
                key.Type = EntityType;
            }
            T result;
            if (Dictionary.TryGetValue(key, out result)) { return result; }
            //if (FullData != null && FullData.TryGetValue(key, out result)) { return result; }
            return _dbSet.Find(key);
        }

        /// <summary>
        /// 根据Lamda返回新的列表，不会影响内部数据列表
        /// <para>如果要遍历所有，需要预先使用LoadAll</para>
        /// </summary>
        public IList<T> Find(Func<T, bool> condition)
        {
            ThreadSafeDictionary<EntityKey, T> dic = Dictionary;
            ConcurrentDictionary<EntityKey, T> entities = new ConcurrentDictionary<EntityKey, T>(Environment.ProcessorCount, dic.Count);
            lock (dic.SyncRoot)
            {
                ParallelOptions options = new ParallelOptions();
                //以5000条数据为一组任务进行处理
                options.MaxDegreeOfParallelism = dic.Count / 5000 + 1;
                Parallel.ForEach(dic, options, (KeyValuePair<EntityKey, T> kv) =>
                 {
                     if (condition(kv.Value))
                     {
                         entities[kv.Key] = kv.Value;
                     }
                 });
            }
            return entities.Values.ToList();
        }

        /// <summary>
        /// 添加或保存
        /// </summary>
        public bool AddOrUpdate(T entity)
        {
            if (entity == null)
            {
                FEConsole.WriteWarnFormatWithCategory(Categories.ENTITY, "DbSet<{0}> add or update an empty data.", EntityType.FullName);
                return false;
            }
            entity.GetEntityKey().RefreshKeyName();
            if (string.IsNullOrEmpty(entity.GetEntityKey()))
            {
                //当Key发生变化是，删除原数据
                if (entity.GetOldEntityKey() != entity.GetEntityKey())
                {
                    FEConsole.WriteWarnFormatWithCategory(Categories.ENTITY, "DbSet<{0}> origin-key:{1} is using an empty key and old key will be removed.", EntityType.FullName, entity.GetOldEntityKey());
                    Remove(entity.GetOldEntityKey());
                }
                return false;
            }
            //如果这个实例已经时删除状态，将不再退入
            if (entity.ModifiedType == EModifyType.Remove || entity.State == EStorageState.Removed)
            {
                Remove(entity);
                return false;
            }

            ThreadSafeDictionary<EntityKey, T> dictionary = Dictionary;
            lock (dictionary.SyncRoot)
            {
                EntityKey key = entity.GetEntityKey();
                if (entity.ModifiedType == EModifyType.Add && dictionary.ContainsKey(key))
                {
                    entity.ModifiedType = dictionary[key].ModifiedType;
                }
                dictionary[key] = entity;
            }
            //Dictionary.Add(entity.GetEntityKey(), entity);

            entity.SetState(EStorageState.Stored);
            if (entity.ModifiedType == EModifyType.Modify && entity.KeyIsModified())
            {
                FEConsole.WriteWarnFormatWithCategory(Categories.ENTITY, "DbSet<{0}>: A key of entity has been changed from {0} to {1}.", entity.GetOldEntityKey(), entity.GetEntityKey());
                Remove(entity.GetOldEntityKey());
            }
            //刷新Key
            entity.RefreshEntityKey();
            //通知数据中心更新数据
            _dbSet.OnModified(entity.GetEntityKey(), entity);
            return true;
        }

        /// <summary>
        /// 删除实例
        /// </summary>
        public bool Remove(T entity)
        {
            if (entity == null)
            {
                FEConsole.WriteWarnFormatWithCategory(Categories.ENTITY, "DbSet<{0}> remove an empty data.", EntityType.FullName);
                return false;
            }
            return Remove(entity.GetEntityKey());
        }

        /// <summary>
        /// 通过Key删除
        /// </summary>
        public bool Remove(EntityKey key)
        {
            bool result = Dictionary.Remove(key);
            //通知数据中心更新数据
            _dbSet.OnModified(key, null);
            return result;
        }

        /// <summary>
        /// 迭代器
        /// </summary>
        public IEnumerator<T> GetEnumerator()
        {
            return Dictionary.Values.GetEnumerator();
        }

        /// <summary>
        /// 迭代器
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        /// <summary>
        /// 拉取所有数据
        /// </summary>
        public void LoadAll()
        {
            lock (this)
            {
                IReadOnlyDictionary<EntityKey, T> entities = _dbSet.LoadAll();
                //Dictionary.Clear();
                //foreach (KeyValuePair<EntityKey, T> kv in entities)
                //{
                //    Dictionary.Add(kv.Key, kv.Value);
                //}
                _fullData = new ThreadSafeDictionary<EntityKey, T>((IDictionary<EntityKey, T>)entities);
            }
        }

        /// <summary>
        /// 监听DbSet的数据变化
        /// </summary>
        void OnDbSetDataModified(EntityKey key, T entity)
        {
            if (entity == null)
            {
                Dictionary.Remove(key);
                //FConsole.Write("DbSet removed " + key + ".");
            }
            else if (entity.ModifiedType != EModifyType.Remove)
            {
                Dictionary[entity.GetEntityKey()] = entity;
                //FConsole.Write("DbSet modified " + key + ".");
            }
        }
    }
}
