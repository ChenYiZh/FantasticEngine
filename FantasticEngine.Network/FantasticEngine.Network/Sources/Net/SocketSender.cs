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
using FantasticEngine.IO;
using FantasticEngine.Log;
using FantasticEngine.Proxy;
using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using FantasticEngine.Collections;

namespace FantasticEngine.Network.Core
{
    /// <summary>
    /// 消息发送处理类
    /// </summary>
    public abstract class SocketSender //: ISender
    {
        /// <summary>
        /// 套接字管理类
        /// </summary>
        protected ISocket Socket { get; private set; }

        /// <summary>
        /// 消息发送处理类
        /// </summary>
        public SocketSender(ISocket socket)
        {
            Socket = socket;
        }

        public void Push(ISocket socket, byte[] msg, bool immediate)
        {
            ((UserToken) socket.EventArgs.UserToken).Push(msg, immediate);
        }

        /// <summary>
        /// 消息发送处理
        /// </summary>
        public void ProcessSend(SocketAsyncEventArgs ioEventArgs)
        {
            if (ioEventArgs == null)
            {
                Socket.OperationCompleted();
                return;
            }

            UserToken userToken = (UserToken) ioEventArgs.UserToken;
            if (userToken.SendingBuffer == null)
            {
                SendCompleted(ioEventArgs);
                return;
            }

            byte[] argsBuffer = ioEventArgs.Buffer;
            int argsCount = ioEventArgs.Count;
            int argsOffset = ioEventArgs.Offset;
            if (argsCount >= userToken.SendingBuffer.Length - userToken.SentCount)
            {
                int length = userToken.SendingBuffer.Length - userToken.SentCount;
                Buffer.BlockCopy(userToken.SendingBuffer, userToken.SentCount, argsBuffer, argsOffset, length);
                ioEventArgs.SetBuffer(argsOffset, length);
                userToken.Reset();
            }
            else
            {
                Buffer.BlockCopy(userToken.SendingBuffer, userToken.SentCount, argsBuffer, argsOffset, argsCount);
                ioEventArgs.SetBuffer(argsOffset, argsCount);
                userToken.SentCount += argsCount;
            }

            if (Socket == null || Socket.Socket == null)
            {
                Socket.OperationCompleted();
                return;
            }

            TrySendAsync(ioEventArgs);
        }

        protected abstract void TrySendAsync(SocketAsyncEventArgs ioEventArgs);

        /// <summary>
        /// 开始执行发送
        /// </summary>
        public void PostSend(SocketAsyncEventArgs ioEventArgs)
        {
            UserToken usertoken = ioEventArgs.UserToken as UserToken;
            if (usertoken == null)
            {
                Socket.OperationCompleted();
                return;
            }

            //没有消息就退出
            if (Socket.TrySend())
            {
                byte[] msg;
                if (usertoken.TryDequeueMsg(out msg))
                {
                    usertoken.SendingBuffer = msg;
                    usertoken.SentCount = 0;
                    ProcessSend(ioEventArgs);
                }
                else
                {
                    Socket.NextStep(ioEventArgs);
                }
            }
        }

        /// <summary>
        /// 消息执行完后，判断还有没有需要继续发送的消息
        /// </summary>
        private void SendCompleted(SocketAsyncEventArgs ioEventArgs)
        {
            ioEventArgs.SetBuffer(ioEventArgs.Offset, ((UserToken) ioEventArgs.UserToken).OriginalLength);
            Socket.NextStep(ioEventArgs);
        }
    }
}