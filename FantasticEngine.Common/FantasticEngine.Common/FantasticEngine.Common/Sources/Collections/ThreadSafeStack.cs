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
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Collections
{
    public class ThreadSafeStack<T> : IThreadSafeStack<T>
    {
        #region ConcurrentStack
        private ConcurrentStack<T> _stack = null;

        public object SyncRoot { get { return this; } }

        public ThreadSafeStack()
        {
            _stack = new ConcurrentStack<T>();
        }

        public int Count { get { return _stack.Count; } }

        public T Peek()
        {
            T value;
            _stack.TryPeek(out value);
            return value;
        }

        public T Pop()
        {
            T value;
            _stack.TryPop(out value);
            return value;
        }

        public void Push(T item)
        {
            _stack.Push(item);
        }

        public void Clear()
        {
            _stack.Clear();
        }

        public IEnumerator<T> GetEnumerator()
        {
            return _stack.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _stack.GetEnumerator();
        }
        #endregion

        #region Stack+Lock
        //private Stack<T> _cache;

        //public readonly object SyncRoot = new object();

        //public ThreadSafeStack()
        //{
        //    _cache = new Stack<T>();
        //}

        //public ThreadSafeStack(int capacity)
        //{
        //    _cache = new Stack<T>(capacity);
        //}

        //public int Count
        //{
        //    get
        //    {
        //        lock (SyncRoot)
        //        {
        //            return _cache.Count;
        //        }
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

        //public T Pop()
        //{
        //    lock (SyncRoot)
        //    {
        //        return _cache.Pop();
        //    }
        //}

        //public void Push(T item)
        //{
        //    lock (SyncRoot)
        //    {
        //        _cache.Push(item);
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
