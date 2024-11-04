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
using FantasticEngine.Network.Server.Entity;
using ProtoBuf;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network.Server.Collections
{
    /// <summary>
    /// Model属性使用的列表
    /// </summary>
    [ProtoContract, Serializable]
    public sealed class EntityList<T> : PropertyEntity, IThreadSafeList<T>
    {
        private readonly bool _isPropertyEntity = FEType<T>.Type.IsSubclassOf(FEType<PropertyEntity>.Type);

        private ThreadSafeList<T> _list = new ThreadSafeList<T>();

        public T this[int index]
        {
            get
            {
                return _list[index];
            }
            set
            {
                if (_isPropertyEntity)
                {
                    if (value != null)
                    {
                        _list[index] = value;
                        (value as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    }
                    else
                    {
                        _list.RemoveAt(index);
                    }
                }
                else
                {
                    _list[index] = value;
                }
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public int Count { get { return _list.Count; } }

        public bool IsReadOnly { get { return _list.IsReadOnly; } }

        public void Add(T item)
        {
            if (_isPropertyEntity)
            {
                if (item != null)
                {
                    _list.Add(item);
                    (item as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    NotifyModified(EModifyType.Modify, PropertyNameInParent);
                }
            }
            else
            {
                _list.Add(item);
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public void Clear()
        {
            _list.Clear();
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
        }

        public bool Contains(T item)
        {
            return _list.Contains(item);
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            _list.CopyTo(array, arrayIndex);
        }

        public IEnumerator<T> GetEnumerator()
        {
            return _list.GetEnumerator();
        }

        public int IndexOf(T item)
        {
            return _list.IndexOf(item);
        }

        public void Insert(int index, T item)
        {
            if (_isPropertyEntity)
            {
                if (item != null)
                {
                    _list.Insert(index, item);
                    (item as PropertyEntity)?.SetParent(this, Categories.ENTITY);
                    NotifyModified(EModifyType.Modify, PropertyNameInParent);
                }
            }
            else
            {
                _list.Insert(index, item);
                NotifyModified(EModifyType.Modify, PropertyNameInParent);
            }
        }

        public bool Remove(T item)
        {
            bool result = _list.Remove(item);
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
            return result;
        }

        public void RemoveAt(int index)
        {
            _list.RemoveAt(index);
            NotifyModified(EModifyType.Modify, PropertyNameInParent);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _list.GetEnumerator();
        }

        internal override void SetParent(Entity.Entity parent, string propertyName)
        {
            base.SetParent(parent, propertyName);
            if (_isPropertyEntity)
            {
                lock (SyncRoot)
                {
                    foreach (T child in _list)
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
                foreach (T child in _list)
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
                foreach (T child in _list)
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
