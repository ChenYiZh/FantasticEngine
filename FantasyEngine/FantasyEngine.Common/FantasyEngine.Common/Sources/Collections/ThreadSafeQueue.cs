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
using System.Text;

namespace FantasyEngine.Collections
{
    public class ThreadSafeQueue<T> : IThreadSafeQueue<T>
    {
        #region ConcurrentQueue
        private ConcurrentQueue<T> queue = new ConcurrentQueue<T>();

        public object SyncRoot { get { return this; } }

        public int Count { get { return queue.Count; } }

        public void Clear()
        {
            while (!queue.IsEmpty)
            {
                T value;
                queue.TryDequeue(out value);
            }
        }

        public T Dequeue()
        {
            T value;
            queue.TryDequeue(out value);
            return value;
        }

        public void Enqueue(T item)
        {
            queue.Enqueue(item);
        }

        public IEnumerator<T> GetEnumerator()
        {
            return queue.GetEnumerator();
        }

        public T Peek()
        {
            T value;
            queue.TryPeek(out value);
            return value;
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return queue.GetEnumerator();
        }
        #endregion

        #region Queue+Lock
        //private Queue<T> _cache;

        //public readonly object SyncRoot = new object();

        //public int Count { get { return _cache.Count; } }

        //public ThreadSafeQueue()
        //{
        //    _cache = new Queue<T>();
        //}

        //public ThreadSafeQueue(int capacity)
        //{
        //    _cache = new Queue<T>(capacity);
        //}

        //public T Dequeue()
        //{
        //    lock (SyncRoot)
        //    {
        //        return _cache.Dequeue();
        //    }
        //}

        //public void Enqueue(T item)
        //{
        //    lock (SyncRoot)
        //    {
        //        _cache.Enqueue(item);
        //    }
        //}

        //public IEnumerator<T> GetEnumerator()
        //{
        //    lock (SyncRoot)
        //    {
        //        return _cache.GetEnumerator();
        //    }
        //}

        //public T Peek()
        //{
        //    lock (SyncRoot)
        //    {
        //        return _cache.Peek();
        //    }
        //}

        //IEnumerator IEnumerable.GetEnumerator()
        //{
        //    lock (SyncRoot)
        //    {
        //        return _cache.GetEnumerator();
        //    }
        //}

        //public void Clear()
        //{
        //    lock (SyncRoot)
        //    {
        //        _cache.Clear();
        //    }
        //}
        #endregion
    }
}
