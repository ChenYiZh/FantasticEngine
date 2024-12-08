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
using ProtoBuf;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Text;

namespace FantasyEngine.Network.Server.Collections
{
    /// <summary>
    /// Model层使用的数据字典
    /// </summary>
    [ProtoContract, Serializable]
    public sealed class EntityDictionary<TKey, TValue> : PropertyEntity, IThreadSafeDictionary<TKey, TValue> where TKey : struct
    {
        private readonly bool _isPropertyEntity = FEType<TValue>.Type.IsSubclassOf(FEType<PropertyEntity>.Type);

        private ThreadSafeDictionary<TKey, TValue> _dictionary = new ThreadSafeDictionary<TKey, TValue>();

        public TValue this[TKey key]
        {
            get { return _dictionary[key]; }
            set
            {
                if (_isPropertyEntity)
                {
                    if (value != null)
                    {
                        _dictionary[key] = value;
                        (value as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    }
                    else
                    {
                        _dictionary.Remove(key);
                    }
                }
                else
                {
                    _dictionary[key] = value;
                }
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public ICollection<TKey> Keys { get { return _dictionary.Keys; } }

        public ICollection<TValue> Values { get { return _dictionary.Values; } }

        public int Count { get { return _dictionary.Count; } }

        public bool IsReadOnly { get { return _dictionary.IsReadOnly; } }

        IEnumerable<TKey> IReadOnlyDictionary<TKey, TValue>.Keys { get { return _dictionary.Keys; } }

        IEnumerable<TValue> IReadOnlyDictionary<TKey, TValue>.Values { get { return _dictionary.Values; } }

        public void Add(TKey key, TValue value)
        {
            if (_isPropertyEntity)
            {
                if (value != null)
                {
                    _dictionary.Add(key, value);
                    (value as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    NotifyModified(EModifyType.Modify, PropertyNameInParent);
                }
            }
            else
            {
                _dictionary.Add(key, value);
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public void Add(KeyValuePair<TKey, TValue> item)
        {
            if (_isPropertyEntity)
            {
                if (item.Value != null)
                {
                    _dictionary.Add(item);
                    (item.Value as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    NotifyModified(EModifyType.Modify, PropertyNameInParent);
                }
            }
            else
            {
                _dictionary.Add(item);
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public void Clear()
        {
            _dictionary.Clear();
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
        }

        public bool Contains(KeyValuePair<TKey, TValue> item)
        {
            return _dictionary.Contains(item);
        }

        public bool ContainsKey(TKey key)
        {
            return _dictionary.ContainsKey(key);
        }

        public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
        {
            _dictionary.CopyTo(array, arrayIndex);
        }

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            _dictionary.GetObjectData(info, context);
        }

        public void OnDeserialization(object sender)
        {
            _dictionary.OnDeserialization(sender);
        }

        public bool Remove(TKey key)
        {
            bool result = _dictionary.Remove(key);
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
            return result;
        }

        public bool Remove(KeyValuePair<TKey, TValue> item)
        {
            bool result = _dictionary.Remove(item);
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
            return result;
        }

        public bool TryGetValue(TKey key, out TValue value)
        {
            return _dictionary.TryGetValue(key, out value);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _dictionary.GetEnumerator();
        }

        internal override void SetParent(Entity.Entity parent, string propertyName)
        {
            base.SetParent(parent, propertyName);
            if (_isPropertyEntity)
            {
                lock (SyncRoot)
                {
                    foreach (TValue child in _dictionary.Values)
                    {
                        PropertyEntity entity;
                        if ((entity = child as PropertyEntity) != null)
                        {
                            entity.ModifiedTime = TimeLord.Now;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 修改已经提交时执行
        /// </summary>
        internal override void OnModificationCommitted()
        {
            base.OnModificationCommitted();
            lock (SyncRoot)
            {
                foreach (TValue child in _dictionary.Values)
                {
                    PropertyEntity entity;
                    if ((entity = child as PropertyEntity) != null)
                    {
                        entity.OnModificationCommitted();
                    }
                }
            }
        }

        /// <summary>
        /// 数据数据库中拉取下来
        /// </summary>
        internal override void OnPulledFromDb()
        {
            base.OnPulledFromDb();
            lock (SyncRoot)
            {
                foreach (TValue child in _dictionary.Values)
                {
                    PropertyEntity entity;
                    if ((entity = child as PropertyEntity) != null)
                    {
                        entity.OnPulledFromDb();
                    }
                }
            }
        }
    }
}
