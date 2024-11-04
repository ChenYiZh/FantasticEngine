/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using System.Runtime.Serialization;

namespace FantasticEngine.Collections
{
    public class ThreadSafeLinkedList<T> :
        ICollection<T>,
        IEnumerable<T>,
        IEnumerable,
        ICollection,
        IReadOnlyCollection<T>,
        ISerializable,
        IDeserializationCallback
    {
        private LinkedList<T> _cache;

        private readonly object _syncRoot = new object();

        public LinkedListNode<T> First
        {
            get
            {
                lock (_syncRoot)
                {
                    return _cache.First;
                }
            }
        }

        public LinkedListNode<T> Last
        {
            get
            {
                lock (_syncRoot)
                {
                    return _cache.Last;
                }
            }
        }

        public T GetAndRemoveFirst()
        {
            lock (_syncRoot)
            {
                T value = _cache.First.Value;
                _cache.RemoveFirst();
                return value;
            }
        }

        public T GetAndRemoveLast()
        {
            lock (_syncRoot)
            {
                T value = _cache.Last.Value;
                _cache.RemoveLast();
                return value;
            }
        }

        public ThreadSafeLinkedList()
        {
            _cache = new LinkedList<T>();
        }

        public ThreadSafeLinkedList(IEnumerable<T> collection)
        {
            _cache = new LinkedList<T>(collection);
        }

        public IEnumerator<T> GetEnumerator()
        {
            lock (_syncRoot)
            {
                return _cache.GetEnumerator();
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public void Add(T item)
        {
            AddLast(item);
        }

        public LinkedListNode<T> AddAfter(LinkedListNode<T> node, T value)
        {
            lock (_syncRoot)
            {
                return _cache.AddAfter(node, value);
            }
        }

        public void AddAfter(LinkedListNode<T> node, LinkedListNode<T> newNode)
        {
            lock (_syncRoot)
            {
                _cache.AddAfter(node, newNode);
            }
        }

        public LinkedListNode<T> AddBefore(LinkedListNode<T> node, T value)
        {
            lock (_syncRoot)
            {
                return _cache.AddBefore(node, value);
            }
        }

        public void AddBefore(LinkedListNode<T> node, LinkedListNode<T> newNode)
        {
            lock (_syncRoot)
            {
                _cache.AddBefore(node, newNode);
            }
        }

        public LinkedListNode<T> AddFirst(T value)
        {
            lock (_syncRoot)
            {
                return _cache.AddFirst(value);
            }
        }

        public void AddFirst(LinkedListNode<T> node)
        {
            lock (_syncRoot)
            {
                _cache.AddFirst(node);
            }
        }

        public LinkedListNode<T> AddLast(T value)
        {
            lock (_syncRoot)
            {
                return _cache.AddLast(value);
            }
        }

        public void AddLast(LinkedListNode<T> node)
        {
            lock (_syncRoot)
            {
                _cache.AddLast(node);
            }
        }

        public void Clear()
        {
            lock (_syncRoot)
            {
                _cache.Clear();
            }
        }

        public bool Contains(T item)
        {
            lock (_syncRoot)
            {
                return _cache.Contains(item);
            }
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            lock (_syncRoot)
            {
                _cache.CopyTo(array, arrayIndex);
            }
        }

        public bool Remove(T item)
        {
            lock (_syncRoot)
            {
                return _cache.Remove(item);
            }
        }

        void ICollection.CopyTo(Array array, int index)
        {
        }

        public int Count
        {
            get
            {
                lock (_syncRoot)
                {
                    return _cache.Count;
                }
            }
        }

        public bool IsSynchronized
        {
            get { return false; }
        }


        public object SyncRoot
        {
            get { return _syncRoot; }
        }

        public bool IsReadOnly
        {
            get { return false; }
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            lock (_syncRoot)
            {
                _cache.GetObjectData(info, context);
            }
        }

        public void OnDeserialization(object sender)
        {
            lock (_syncRoot)
            {
                _cache.OnDeserialization(sender);
            }
        }
    }
}