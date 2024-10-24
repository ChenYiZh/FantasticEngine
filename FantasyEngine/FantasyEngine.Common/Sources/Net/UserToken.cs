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
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using FantasyEngine.Collections;
using FantasyEngine.Proxy;

namespace FantasyEngine.Net
{
    /// <summary>
    /// 寄宿在原生Socket的管理类
    /// </summary>
    public sealed class UserToken : IUserToken
    {
        /// <summary>
        /// 嵌套的Socket
        /// </summary>
        public ISocket Socket { get; internal set; }

        ///// <summary>
        ///// 套接字处理结果
        ///// </summary>
        //public SocketAsyncResult AsyncResult { get; internal set; }

        #region socket state

        // /// <summary>
        // /// 收发数据的标识
        // /// <para>0: 无状态; 1: 接收状态; 2: 发送状态</para>
        // /// </summary>
        // private int _isSendingOrReceivingFlag = 0;
        //
        // /// <summary>
        // /// 是否可以进行发送操作
        // /// </summary>
        // internal bool CheckAndSending()
        // {
        //     return Interlocked.CompareExchange(ref _isSendingOrReceivingFlag, 2, 0) != 1;
        // }
        //
        //
        // /// <summary>
        // /// 是否可以进行接收操作
        // /// </summary>
        // internal bool CheckAndReceiving()
        // {
        //     return Interlocked.CompareExchange(ref _isSendingOrReceivingFlag, 1, 0) != 2;
        // }

        #endregion

        /// <summary>
        /// 已经接收的数据长度
        /// </summary>
        public int ReceivedStartIndex { get; set; } = 0;

        /// <summary>
        /// 解析包时解析不完的数据
        /// </summary>
        public byte[] ReceivedBuffer { get; internal set; } = null;

        /// <summary>
        /// 正在发送的数据
        /// </summary>
        public byte[] SendingBuffer { get; internal set; }

        /// <summary>
        /// 已经发送的字节数量
        /// </summary>
        public int SentCount { get; internal set; }

        /// <summary>
        /// 原本的生成时的offset
        /// </summary>
        public int OriginalOffset { get; private set; }

        /// <summary>
        /// 原本缓存的字节长度
        /// </summary>
        public int OriginalLength { get; private set; }

        /// <summary>
        /// 初始化
        /// </summary>
        public UserToken(SocketAsyncEventArgs eventArgs)
        {
            OriginalOffset = eventArgs.Offset;
            ReceivedBuffer = null;
        }

        ///// <summary>
        ///// 析构时执行
        ///// </summary>
        //~UserToken()
        //{
        //    ReceiverStream.Dispose();
        //    ReceiverStream = null;
        //}

        // /// <summary>
        // /// 重置发送状态
        // /// </summary>
        // public void ResetSendAndReceiveState()
        // {
        //     Interlocked.Exchange(ref _isSendingOrReceivingFlag, 0);
        // }

        /// <summary>
        /// 重置数据
        /// </summary>
        public void Reset()
        {
            SendingBuffer = null;
            ReceivedStartIndex = 0;
            ReceivedBuffer = null;
            //ReceiverStream.Flush();
            //AsyncResult = null;
            //Socket = null;
        }

        /// <summary>
        /// 设置原本的生成时的offset
        /// </summary>
        internal void SetOriginalOffset(int offset, int length)
        {
            OriginalOffset = offset;
            OriginalLength = length;
        }

        #region Send数据

        /// <summary>
        /// 消息Id，需要加原子锁
        /// </summary>
        private long _messageNumber = DateTime.Now.Ticks;

        /// <summary>
        /// 消息Id
        /// <para>get 返回时会自动 +1</para>
        /// </summary>
        public long MessageNumber
        {
            get { return Interlocked.Increment(ref _messageNumber); }
            set { Interlocked.Exchange(ref _messageNumber, value); }
        }

        /// <summary>
        /// 待发送的消息列表
        /// </summary>
        private ThreadSafeLinkedList<byte[]> _waitToSendMessages = new ThreadSafeLinkedList<byte[]>();

        /// <summary>
        /// 缓存需要发送的数据
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="immediate"></param>
        public void Push(byte[] msg, bool immediate)
        {
            if (immediate)
            {
                _waitToSendMessages.AddFirst(msg);
            }
            else
            {
                _waitToSendMessages.AddLast(msg);
            }
        }

        /// <summary>
        /// 判断是否有数据并且返回第一个值
        /// </summary>
        /// <param name="msg"></param>
        /// <returns></returns>
        public bool TryDequeueMsg(out byte[] msg)
        {
            lock (_waitToSendMessages.SyncRoot)
            {
                if (_waitToSendMessages.Count > 0)
                {
                    msg = _waitToSendMessages.GetAndRemoveFirst();
                    return true;
                }

                msg = null;
                return false;
            }
        }

        /// <summary>
        /// 是否有消息需要发送
        /// </summary>
        /// <returns></returns>
        public bool HasMsg()
        {
            return _waitToSendMessages.Count > 0;
        }

        #endregion
    }
}