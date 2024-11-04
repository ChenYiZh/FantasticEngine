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
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using FantasticEngine.IO;
using FantasticEngine.Log;
using FantasticEngine.Network.Core;
using FantasticEngine.Proxy;
using FantasticEngine.Security;

namespace FantasticEngine.Network
{
    ///// <summary>
    ///// 数据发送的回调
    ///// </summary>
    ///// <param name="success">操作是否成功，不包含结果</param>
    ///// <param name="result">同步的结果</param>
    //public delegate void SendCallback(bool success, IAsyncResult result);

    /// <summary>
    /// 套接字管理基类
    /// </summary>
    public abstract class FESocket : ISocket
    {
        /// <summary>
        /// 是否在运行
        /// </summary>
        public virtual bool IsRunning { get; protected set; } = false;

        /// <summary>
        /// 0：等待，1：发送，2：接收，3：在循环中
        /// </summary>
        private int _operation = 0;

        /// <summary>
        /// 尝试开始发送
        /// </summary>
        public virtual bool TrySend(bool onlyWait = false)
        {
            if (onlyWait)
            {
                return Interlocked.CompareExchange(ref _operation, 1, 0) == 0;
            }

            //return Interlocked.CompareExchange(ref _operation, 2, 3) != 2;
            return Interlocked.CompareExchange(ref _operation, 1, 0) != 2;
        }

        /// <summary>
        /// 尝试接收
        /// </summary>
        public virtual bool TryReceive(bool onlyWait = false)
        {
            if (onlyWait)
            {
                return Interlocked.CompareExchange(ref _operation, 2, 0) == 0;
            }

            //return Interlocked.CompareExchange(ref _operation, 2, 3) != 1;
            return Interlocked.CompareExchange(ref _operation, 2, 0) != 1;
        }

        /// <summary>
        /// 是否正处于操作
        /// </summary>
        /// <returns></returns>
        public virtual bool Operating()
        {
            return Interlocked.CompareExchange(ref _operation, 0, 0) != 0;
        }

        /// <summary>
        /// 进入循环
        /// </summary>
        public virtual void InLooping()
        {
            Interlocked.Exchange(ref _operation, 3);
        }

        /// <summary>
        /// 移出循环
        /// </summary>
        public virtual void OutLooping()
        {
            Interlocked.CompareExchange(ref _operation, 0, 3);
        }

        /// <summary>
        /// 操作完成时执行
        /// </summary>
        public virtual void OperationCompleted()
        {
            Interlocked.Exchange(ref _operation, 0);
        }

        /// <summary>
        /// 执行下步操作
        /// </summary>
        public abstract void NextStep(SocketAsyncEventArgs eventArgs);

        /// <summary>
        /// 是否已经开始运行
        /// </summary>
        public virtual bool Connected
        {
            get
            {
                return EventArgs != null
                       && Socket != null
                       && Socket.Connected;
            }
        }

        /// <summary>
        /// 地址
        /// </summary>
        public abstract EndPoint Address { get; }

        /// <summary>
        /// 原生套接字
        /// </summary>
        public virtual Socket Socket { get; protected set; }

        /// <summary>
        /// 内部关键原生Socket
        /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs</para>
        /// </summary>
        public virtual SocketAsyncEventArgs EventArgs { get; protected set; }

        /// <summary>
        /// 自定义数据结构
        /// </summary>
        public virtual UserToken UserToken
        {
            get { return (UserToken)EventArgs.UserToken; }
        }

        /// <summary>
        /// 类型
        /// </summary>
        // TODO: 添加新的类型时需要修改构造函数
        public abstract ESocketType Type { get; }

        /// <summary>
        /// 消息偏移值
        /// </summary>
        public abstract int MessageOffset { get; }

        /// <summary>
        /// 压缩工具
        /// </summary>
        public abstract ICompression Compression { get; }

        /// <summary>
        /// 加密工具
        /// </summary>
        public abstract ICryptoProvider CryptoProvider { get; }

        /// <summary>
        /// 消息处理方案
        /// </summary>
        public virtual IBoss MessageEventProcessor { get; set; } = new DirectMessageProcessor();

        /// <summary>
        /// 初始化
        /// </summary>
        protected FESocket(SocketAsyncEventArgs eventArgs)
        {
            if (eventArgs == null)
            {
                //throw new NullReferenceException("SocketAsyncEventArgs is null! Create socket failed.");
                return;
            }

            EventArgs = eventArgs;
            Socket = EventArgs.AcceptSocket;
            UserToken userToken = eventArgs.UserToken as UserToken;
            if (userToken == null)
            {
                userToken = new UserToken(eventArgs);
                eventArgs.UserToken = userToken;
            }

            //UserToken = userToken;
            userToken.Socket = this;
        }

        /// <summary>
        /// 关闭函数
        /// </summary>
        public virtual void Close(EOpCode opCode = EOpCode.Close)
        {
            lock (this)
            {
                IsRunning = false;
                if (EventArgs != null)
                {
                    //((UserToken)EventArgs.UserToken).ResetSendOrReceiveState(0);
                    if (Socket != null)
                    {
                        try
                        {
                            Socket.Shutdown(SocketShutdown.Both);
                            Socket.Close();
                            Socket.Dispose();
                        }
                        catch (Exception e)
                        {
                            FEConsole.WriteExceptionWithCategory(Categories.SOCKET, "Socket close error.", e);
                        }
                        finally
                        {
                            EventArgs.AcceptSocket = null;
                            Socket = null;
                        }
                    }

                    UserToken userToken;
                    if ((userToken = UserToken) != null && userToken.Socket == this)
                    {
                        userToken.Socket = null;
                    }
                }
            }
        }

        protected void DisposeEventArgs()
        {
            if (EventArgs != null)
            {
                try
                {
                    //((UserToken)EventArgs.UserToken).ResetSendOrReceiveState(0);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.SOCKET, "UserToken reset error.", e);
                }

                UserToken userToken;
                if ((userToken = UserToken) != null && userToken.Socket == this)
                {
                    userToken.Socket = null;
                }

                if (EventArgs.ConnectSocket != null)
                {
                    try
                    {
                        EventArgs.ConnectSocket.Close();
                        EventArgs.ConnectSocket.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Categories.SOCKET, "EventArgs close socket error.", e);
                    }
                }

                try
                {
                    EventArgs.Dispose();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.SOCKET, "EventArgs dispose error.", e);
                }

                EventArgs = null;
            }
        }

        /// <summary>
        /// 创建Socket的超类
        /// </summary>
        public static SocketAsyncEventArgs MakeEventArgs(Socket socket, byte[] buffer = null, int offset = 0,
            int bufferSize = 8192)
        {
            if (buffer == null)
            {
                buffer = new byte[offset + bufferSize];
            }

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            // 设置缓冲区大小
            args.SetBuffer(buffer, offset % buffer.Length, bufferSize);
            UserToken userToken = new UserToken(args);
            args.UserToken = userToken;
            userToken.SetOriginalOffset(offset, bufferSize);
            args.AcceptSocket = socket;
            return args;
        }
    }
}