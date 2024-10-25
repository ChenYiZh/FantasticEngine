/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
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
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;

namespace FantasyEngine.Collections
{
    public class ThreadSafeHashSet<T> : IThreadSafeHashSet<T>
    {

        #region HashSet+Lock
        private HashSet<T> _cache;

        public readonly object SyncRoot = new object();

        public ThreadSafeHashSet()
        {
            _cache = new HashSet<T>();
        }

        //public ThreadSafeHashSet(int capacity)
        //{
        //    _cache = new HashSet<T>(capacity);
        //}

        public int Count
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache.Count;
                }
            }
        }

        public bool IsReadOnly { get { return false; } }

        public bool Add(T item)
        {
            lock (SyncRoot)
            {
                return _cache.Add(item);
            }
        }

        public void UnionWith(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                _cache.UnionWith(other);
            }
        }

        public void IntersectWith(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                _cache.IntersectWith(other);
            }
        }

        public void ExceptWith(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                _cache.ExceptWith(other);
            }
        }

        public void SymmetricExceptWith(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                _cache.SymmetricExceptWith(other);
            }
        }

        public bool IsSubsetOf(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.IsSubsetOf(other);
            }
        }

        public bool IsSupersetOf(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.IsSupersetOf(other);
            }
        }

        public bool IsProperSupersetOf(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.IsProperSupersetOf(other);
            }
        }

        public bool IsProperSubsetOf(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.IsProperSubsetOf(other);
            }
        }

        public bool Overlaps(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.Overlaps(other);
            }
        }

        public bool SetEquals(IEnumerable<T> other)
        {
            lock (SyncRoot)
            {
                return _cache.SetEquals(other);
            }
        }

        void ICollection<T>.Add(T item)
        {
            lock (SyncRoot)
            {
                _cache.Add(item);
            }
        }

        public void Clear()
        {
            lock (SyncRoot)
            {
                _cache.Clear();
            }
        }

        public bool Contains(T item)
        {
            lock (SyncRoot)
            {
                return _cache.Contains(item);
            }
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            lock (SyncRoot)
            {
                _cache.CopyTo(array, arrayIndex);
            }
        }

        public bool Remove(T item)
        {
            lock (SyncRoot)
            {
                return _cache.Remove(item);
            }
        }

        public IEnumerator<T> GetEnumerator()
        {
            lock (SyncRoot)
            {
                return _cache.GetEnumerator();
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public void OnDeserialization(object sender)
        {
            lock (SyncRoot)
            {
                _cache.OnDeserialization(sender);
            }
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            lock (SyncRoot)
            {
                _cache.GetObjectData(info, context);
            }
        }
        #endregion
    }
}
