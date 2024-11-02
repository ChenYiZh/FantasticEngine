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
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 字节池的信息
    /// </summary>
    public struct PoolInfo
    {
        /// <summary>
        /// 每一个分组长度
        /// </summary>
        public int SingleLength { get; private set; }
        /// <summary>
        /// 整个分组数量
        /// </summary>
        public int GroupCount { get; private set; }
        /// <summary>
        /// 整个字节长度
        /// </summary>
        public int TotalLength { get; private set; }
        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="singleLength">每一个分组长度</param>
        /// <param name="groupCount">整个分组数量</param>
        public PoolInfo(int singleLength, int groupCount)
        {
            SingleLength = singleLength;
            GroupCount = groupCount;
            TotalLength = singleLength * groupCount;
        }
    }
    /// <summary>
    /// 字节流缓存池
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs.setbuffer</para>
    /// </summary>
    public class BytePool
    {
        /// <summary>
        /// 锁
        /// </summary>
        private readonly object _syncRoot = new object();
        /// <summary>
        /// 字节池
        /// </summary>
        public List<byte[]> Pool { get; private set; }
        /// <summary>
        /// 整个字节长度
        /// </summary>
        public PoolInfo PoolInfo { get; private set; }
        /// <summary>
        /// 可用队列的起始id
        /// </summary>
        public Stack<int> Offsets { get; private set; }
        /// <summary>
        /// 初始化
        /// </summary>
        /// <param name="singleLength">每一个分组长度</param>
        /// <param name="groupCount">整个分组数量</param>
        public BytePool(int singleLength, int groupCount)
        {
            Pool = new List<byte[]>();
            PoolInfo = new PoolInfo(singleLength, groupCount);
            Offsets = new Stack<int>(groupCount * 10);
            CreatePool();
        }
        /// <summary>
        /// 创建缓存池
        /// </summary>
        private void CreatePool()
        {
            int count = Pool.Count;
            Pool.Add(new byte[PoolInfo.TotalLength]);
            for (int i = 0; i < PoolInfo.GroupCount; i++)
            {
                Offsets.Push(count * PoolInfo.TotalLength + i * PoolInfo.SingleLength);
            }
        }
        /// <summary>
        /// 获取可使用的字节段
        /// </summary>
        /// <param name="buffer">字节</param>
        /// <param name="offset">索引位置，使用前先和buffer.length取余，求出内部Index</param>
        /// <param name="length"></param>
        public void GetByteBlock(out byte[] buffer, out int offset, out int length)
        {
            lock (_syncRoot)
            {
                if (Offsets.Count == 0)
                {
                    CreatePool();
                }
                offset = Offsets.Pop();
            }
            buffer = Pool[offset / PoolInfo.TotalLength];
            length = PoolInfo.SingleLength;
        }
        /// <summary>
        /// 释放，需要回传原本获取到的offset
        /// </summary>
        /// <param name="offset"></param>
        public void Release(int offset)
        {
            lock (_syncRoot)
            {
                Offsets.Push(offset);
            }
        }
    }
}
