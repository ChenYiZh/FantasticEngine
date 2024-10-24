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
using FantasyEngine.Collections;
using FantasyEngine.Log;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace FantasyEngine.Common
{
    /// <summary>
    /// 缓存队列池
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public sealed class CachePool<T>
    {
        /// <summary>
        /// 队列操作
        /// </summary>
        private Action<IReadOnlyQueue<T>> _execution = null;
        /// <summary>
        /// 缓存池
        /// </summary>
        private TQueue<T>[] _pools;
        /// <summary>
        /// 当前压入的缓存池索引
        /// </summary>
        private int _currentPoolIndex = 0;
        /// <summary>
        /// 线程对象
        /// </summary>
        public Thread Thread { get; private set; }
        /// <summary>
        /// 线程锁
        /// </summary>
        public object SyncRoot { get; private set; }
        /// <summary>
        /// 线程循环的间隔
        /// </summary>
        public int DeltaMilliseconds { get; private set; }
        /// <summary>
        /// 循环中锁的最长等待时间
        /// </summary>
        public int LockerTimeout { get; private set; }
        /// <summary>
        /// 工作状态标识
        /// </summary>
        private int _isReleased = 0;
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="execution">回调函数</param>
        /// <param name="lockerTimeout">循环中锁的最长等待时间</param>
        /// <param name="deltaMilliseconds">线程循环的间隔</param>
        public CachePool(Action<IReadOnlyQueue<T>> execution, int lockerTimeout = 100, int deltaMilliseconds = 10)
        {
            if (execution == null)
            {
                throw new ArgumentNullException("Can not create cache pool, because the exection is null.");
            }
            _isReleased = 0;
            SyncRoot = new object();
            _execution = execution;
            LockerTimeout = lockerTimeout;
            _pools = new TQueue<T>[3];
            for (int i = 0; i < _pools.Length; i++)
            {
                _pools[i] = new TQueue<T>();
            }
            _currentPoolIndex = 0;
            DeltaMilliseconds = deltaMilliseconds;
            Thread = new Thread(Processing);
            Thread.Start();
        }
        /// <summary>
        /// 数据推入
        /// </summary>
        public void Push(T entity)
        {
            lock (SyncRoot)
            {
                _pools[_currentPoolIndex].Enqueue(entity);
            }
        }
        /// <summary>
        /// 线程执行的操作
        /// </summary>
        private void Processing()
        {
            while (_isReleased == 0)
            {
                //bool lockToken = false;
                try
                {
                    //Monitor.TryEnter(SyncRoot, LockerTimeout, ref lockToken);
                    //if (lockToken)
                    //{
                    int nextIndex = _currentPoolIndex + 1;
                    if (nextIndex >= _pools.Length)
                    {
                        nextIndex = 0;
                    }
                    int commitIndex = _currentPoolIndex - 1;
                    if (commitIndex < 0)
                    {
                        commitIndex = _pools.Length - 1;
                    }
                    Interlocked.Exchange(ref _currentPoolIndex, nextIndex);
                    TryCommit(_pools[commitIndex]);
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
                Thread.Sleep(DeltaMilliseconds);
            }
        }

        private void TryCommit(TQueue<T> set)
        {
            if (set.Count > 0)
            {
                try
                {
                    _execution?.Invoke(set);
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
                finally
                {
                    set.Clear();
                }
            }
        }
        /// <summary>
        /// 线程释放，所有数据释放
        /// </summary>
        public void Release()
        {
            if (Thread != null)
            {
                try
                {
                    Interlocked.Exchange(ref _isReleased, 1);
                    while (Thread.IsAlive)
                    {
                        Thread.Sleep(10);
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
                Thread = null;
            }
            for (int i = 0; i < _pools.Length; i++)
            {
                TryCommit(_pools[i]);
            }
        }
    }
}
