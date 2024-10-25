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
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace FantasyEngine.Collections
{
    public class ThreadSafeDictionary<TKey, TValue> : IThreadSafeDictionary<TKey, TValue>
    {
        #region ConcurrentDictionary 
        // private ConcurrentDictionary<TKey, TValue> dictionary = new ConcurrentDictionary<TKey, TValue>();
        //
        // public object SyncRoot { get { return this; } }
        //
        // public ThreadSafeDictionary()
        // {
        //     dictionary = new ConcurrentDictionary<TKey, TValue>();
        // }
        //
        // public ThreadSafeDictionary(int capacity)
        // {
        //     dictionary = new ConcurrentDictionary<TKey, TValue>(Environment.ProcessorCount, capacity);
        // }
        //
        // public ThreadSafeDictionary(IDictionary<TKey, TValue> dictionary) : this(dictionary.Count)
        // {
        //     //this.dictionary = new ConcurrentDictionary<TKey, TValue>(Environment.ProcessorCount, dictionary.Count);
        //     ParallelOptions options = new ParallelOptions();
        //     //以5000条数据为一组任务进行处理
        //     options.MaxDegreeOfParallelism = dictionary.Count / 5000 + 1;
        //     Parallel.ForEach(dictionary, options, (KeyValuePair<TKey, TValue> kv) =>
        //      {
        //          this.dictionary[kv.Key] = kv.Value;
        //      });
        // }
        //
        // public TValue this[TKey key]
        // {
        //     get { return dictionary[key]; }
        //     set { dictionary[key] = value; }
        // }
        //
        // TValue IReadOnlyDictionary<TKey, TValue>.this[TKey key] { get { return dictionary[key]; } }
        //
        // public ICollection<TKey> Keys { get { return dictionary.Keys; } }
        //
        // public ICollection<TValue> Values { get { return dictionary.Values; } }
        //
        // public int Count { get { return dictionary.Count; } }
        //
        // public bool IsReadOnly { get { return false; } }
        //
        // IEnumerable<TKey> IReadOnlyDictionary<TKey, TValue>.Keys { get { return dictionary.Keys; } }
        //
        // IEnumerable<TValue> IReadOnlyDictionary<TKey, TValue>.Values { get { return dictionary.Values; } }
        //
        // public void Add(TKey key, TValue value)
        // {
        //     dictionary.TryAdd(key, value);
        // }
        //
        // public void Add(KeyValuePair<TKey, TValue> item)
        // {
        //     dictionary.TryAdd(item.Key, item.Value);
        // }
        //
        // public void Clear()
        // {
        //     dictionary.Clear();
        // }
        //
        // public bool Contains(KeyValuePair<TKey, TValue> item)
        // {
        //     return dictionary.Contains(item);
        // }
        //
        // public bool ContainsKey(TKey key)
        // {
        //     return dictionary.ContainsKey(key);
        // }
        //
        // public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
        // {
        //     ((ICollection<KeyValuePair<TKey, TValue>>)dictionary).CopyTo(array, arrayIndex);
        // }
        //
        // public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        // {
        //     return dictionary.GetEnumerator();
        // }
        //
        // public void GetObjectData(SerializationInfo info, StreamingContext context)
        // {
        //     throw new NotImplementedException();
        // }
        //
        // public void OnDeserialization(object sender)
        // {
        //     throw new NotImplementedException();
        // }
        //
        // public bool Remove(TKey key)
        // {
        //     TValue value;
        //     return dictionary.TryRemove(key, out value);
        // }
        //
        // public bool Remove(KeyValuePair<TKey, TValue> item)
        // {
        //     TValue value;
        //     return dictionary.TryRemove(item.Key, out value);
        // }
        //
        // public bool TryGetValue(TKey key, out TValue value)
        // {
        //     return dictionary.TryGetValue(key, out value);
        // }
        //
        // IEnumerator IEnumerable.GetEnumerator()
        // {
        //     return dictionary.GetEnumerator();
        // }
        #endregion
        #region Dictionary+Lock
        private Dictionary<TKey, TValue> _cache;

        public readonly object SyncRoot = new object();

        public ThreadSafeDictionary()
        {
            _cache = new Dictionary<TKey, TValue>();
        }

        public ThreadSafeDictionary(int capacity)
        {
            _cache = new Dictionary<TKey, TValue>(capacity);
        }

        public ThreadSafeDictionary(IDictionary<TKey, TValue> dictionary)
        {
            _cache = new Dictionary<TKey, TValue>(dictionary);
        }

        public ICollection<TKey> Keys
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache.Keys.ToList();
                }
            }
        }

        public ICollection<TValue> Values
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache.Values.ToList();
                }
            }
        }

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

        IEnumerable<TKey> IReadOnlyDictionary<TKey, TValue>.Keys
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache.Keys;
                }
            }
        }

        IEnumerable<TValue> IReadOnlyDictionary<TKey, TValue>.Values
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache.Values;
                }
            }
        }

        public TValue this[TKey key]
        {
            get
            {
                lock (SyncRoot)
                {
                    return _cache[key];
                }
            }
            set
            {
                lock (SyncRoot)
                {
                    _cache[key] = value;
                }
            }
        }

        public bool ContainsKey(TKey key)
        {
            lock (SyncRoot)
            {
                return _cache.ContainsKey(key);
            }
        }

        public void Add(TKey key, TValue value)
        {
            lock (SyncRoot)
            {
                _cache.Add(key, value);
            }
        }

        public bool Remove(TKey key)
        {
            lock (SyncRoot)
            {
                return _cache.Remove(key);
            }
        }

        public bool TryGetValue(TKey key, out TValue value)
        {
            lock (SyncRoot)
            {
                return _cache.TryGetValue(key, out value);
            }
        }

        public void Add(KeyValuePair<TKey, TValue> item)
        {
            lock (SyncRoot)
            {
                _cache.Add(item.Key, item.Value);
            }
        }

        public void Clear()
        {
            lock (SyncRoot)
            {
                _cache.Clear();
            }
        }

        public bool Contains(KeyValuePair<TKey, TValue> item)
        {
            lock (SyncRoot)
            {
                return _cache.Contains(item);
            }
        }

        public bool Remove(KeyValuePair<TKey, TValue> item)
        {
            lock (SyncRoot)
            {
                return _cache.Remove(item.Key);
            }
        }

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
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

        public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
        {
            lock (SyncRoot)
            {
                ((ICollection<KeyValuePair<TKey, TValue>>)_cache).CopyTo(array, arrayIndex);
            }
        }
        #endregion
    }
}
