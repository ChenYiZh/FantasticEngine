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

using FantasyEngine.Common;
using FantasyEngine.IO;
using FantasyEngine.Log;
using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace FantasyEngine.Net
{
    /// <summary>
    /// 消息接收处理类
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs</para>
    /// </summary>
    public abstract class SocketReceiver<TSocket> : IReceiver where TSocket : ISocket
    {
        /// <summary>
        /// 核心Socket
        /// </summary>
        public ISocket Socket { get; private set; }

        public SocketReceiver(ISocket socket)
        {
            Socket = socket;
        }

        /// <summary>
        /// 收发消息处理
        /// </summary>
        public delegate void MessageReceiveEventHandler(IMessageEventArgs<TSocket> e);

        /// <summary>
        /// 接收到数据包事件
        /// </summary>
        public MessageReceiveEventHandler OnMessageReceived;

        private void MessageReceived(MessageReceiverEventArgs<TSocket> args)
        {
            OnMessageReceived?.Invoke(args);
        }

        /// <summary>
        /// 心跳探索事件
        /// </summary>
        public MessageReceiveEventHandler OnPing;

        private void Ping(MessageReceiverEventArgs<TSocket> args)
        {
            OnPing?.Invoke(args);
        }

        /// <summary>
        /// 心跳回应事件
        /// </summary>
        public MessageReceiveEventHandler OnPong;

        private void Pong(MessageReceiverEventArgs<TSocket> args)
        {
            OnPong?.Invoke(args);
        }

        // /// <summary>
        // /// 等待消息接收
        // /// </summary>
        // /// <returns>是否维持等待状态</returns>
        // public abstract bool BeginReceive(bool force = false);

        /// <summary>
        /// 处理数据接收回调
        /// </summary>
        public virtual void ProcessReceive(SocketAsyncEventArgs ioEventArgs)
        {
            if (ioEventArgs == null || ioEventArgs.UserToken == null)
            {
                Socket.OperationCompleted();
                return;
            }

            UserToken userToken = (UserToken)ioEventArgs.UserToken;
            if (ioEventArgs.BytesTransferred == 0)
            {
                Socket.OperationCompleted();
                Close(ioEventArgs, EOpCode.Empty);
                return;
            }

            if (ioEventArgs.SocketError != SocketError.Success)
            {
                Socket.OperationCompleted();
                FConsole.WriteErrorFormatWithCategory(Categories.SOCKET,
                    "Process Receive IP {0} SocketError:{1}, bytes len:{2}",
                    (userToken != null ? userToken.Socket.Address?.ToString() : ""),
                    ioEventArgs.SocketError.ToString(),
                    ioEventArgs.BytesTransferred);
                Close(ioEventArgs, EOpCode.Close);
                return;
            }

            //处理消息
            if (ioEventArgs.BytesTransferred > 0)
            {
                //先缓存数据
                byte[] buffer = new byte[ioEventArgs.BytesTransferred];
                //lock (ioEventArgs)
                {
                    Buffer.BlockCopy(ioEventArgs.Buffer, ioEventArgs.Offset, buffer, 0, buffer.Length);
                }

                byte[] argsBuffer = buffer;
                int argsLength = buffer.Length;

                //从当前位置数据开始解析
                int offset = 0;

                //消息处理的队列
                List<IMessageReader> messages = new List<IMessageReader>();
                try
                {
                    //继续接收上次未接收完毕的数据
                    if (userToken.ReceivedBuffer != null)
                    {
                        //上次连报头都没接收完
                        if (userToken.ReceivedStartIndex < 0)
                        {
                            byte[] data = new byte[argsLength + userToken.ReceivedBuffer.Length];
                            Buffer.BlockCopy(userToken.ReceivedBuffer, 0, data, 0, userToken.ReceivedBuffer.Length);
                            Buffer.BlockCopy(argsBuffer, 0, data, userToken.ReceivedBuffer.Length, argsLength);
                            userToken.ReceivedBuffer = null;

                            argsBuffer = data;
                            offset = 0;
                            argsLength = data.Length;
                        }
                        //数据仍然接收不完
                        else if (userToken.ReceivedStartIndex + argsLength < userToken.ReceivedBuffer.Length)
                        {
                            Buffer.BlockCopy(argsBuffer, 0, userToken.ReceivedBuffer,
                                userToken.ReceivedStartIndex, argsLength);
                            userToken.ReceivedStartIndex += argsLength;
                            offset += argsLength;

                            //buffer = null;
                        }
                        //这轮数据可以接受完
                        else
                        {
                            int deltaLength = userToken.ReceivedBuffer.Length - userToken.ReceivedStartIndex;
                            Buffer.BlockCopy(argsBuffer, 0, userToken.ReceivedBuffer,
                                userToken.ReceivedStartIndex, deltaLength);
                            IMessageReader bigMessage = PackageFactory.Unpack(userToken.ReceivedBuffer,
                                userToken.Socket.MessageOffset, userToken.Socket.Compression,
                                userToken.Socket.CryptoProvider);
                            userToken.ReceivedBuffer = null;
                            messages.Add(bigMessage);
                            offset += deltaLength;
                        }
                    }

                    //针对接收到的数据进行完整解析
                    while (offset < argsLength)
                    {
                        int totalLength =
                            PackageFactory.GetTotalLength(argsBuffer, offset + Socket.MessageOffset);
                        //包头解析不全
                        if (totalLength < 0)
                        {
                            userToken.ReceivedStartIndex = -1;
                            //userToken.ReceivedBuffer = new byte[buffer.Length - offset];
                            userToken.ReceivedBuffer = new byte[argsLength - offset];
                            Buffer.BlockCopy(argsBuffer, offset, userToken.ReceivedBuffer, 0,
                                argsLength - offset);
                            break;
                        }

                        //包体解析不全
                        if (totalLength > argsLength)
                        {
                            userToken.ReceivedStartIndex = argsLength - offset;
                            userToken.ReceivedBuffer = new byte[totalLength - offset];
                            Buffer.BlockCopy(argsBuffer, offset, userToken.ReceivedBuffer, 0, totalLength - offset);
                            break;
                        }

                        offset += Socket.MessageOffset;
                        IMessageReader message = PackageFactory.Unpack(argsBuffer, offset, userToken.Socket.Compression,
                            userToken.Socket.CryptoProvider);
                        messages.Add(message);
                        offset = totalLength;
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteExceptionWithCategory(Categories.SOCKET, "Process Receive error.", e);
                }

                for (int i = 0; i < messages.Count; i++)
                {
                    IMessageReader message = messages[i];
                    try
                    {
                        if (message.IsError)
                        {
                            FConsole.WriteErrorFormatWithCategory(Categories.SOCKET, message.Error);
                            continue;
                        }

                        switch (message.OpCode)
                        {
                            case (sbyte)EOpCode.Close:
                                {
                                    // TODO: 检查关闭协议是否有效
                                    //Close(ioEventArgs, EOpCode.Empty);
                                    Close(ioEventArgs, EOpCode.Empty);
                                }
                                break;
                            case (sbyte)EOpCode.Ping:
                                {
                                    Ping(new MessageReceiverEventArgs<TSocket>
                                    { Socket = (TSocket)userToken.Socket, Message = message });
                                }
                                break;
                            case (sbyte)EOpCode.Pong:
                                {
                                    Pong(new MessageReceiverEventArgs<TSocket>
                                    { Socket = (TSocket)userToken.Socket, Message = message });
                                }
                                break;
                            default:
                                {
                                    MessageReceived(new MessageReceiverEventArgs<TSocket>
                                    { Socket = (TSocket)userToken.Socket, Message = message });
                                }
                                break;
                        }
                    }
                    catch (Exception e)
                    {
                        FConsole.WriteExceptionWithCategory(Categories.SOCKET,
                            "An exception occurred when resolve the message.", e);
                    }
                }
            }
            else if (userToken.ReceivedBuffer != null)
            {
                //数据错乱
                userToken.ReceivedBuffer = null;
                userToken.Reset();
                //return;
            }

            if (userToken.ReceivedBuffer != null)
            {
                PostReceive(ioEventArgs);
            }
            else
            {
                Socket.NextStep(ioEventArgs);
            }
        }

        /// <summary>
        /// 统一从缓存池读取数据
        /// </summary>
        /// <param name="ioEventArgs"></param>
        /// <param name="to"></param>
        /// <param name="offset"></param>
        private void CopySocketBuffer(SocketAsyncEventArgs ioEventArgs, byte[] dis, int offset)
        {
            Buffer.BlockCopy(ioEventArgs.Buffer, 0, dis, offset, ioEventArgs.BytesTransferred);
        }

        /// <summary>
        /// 投递接收数据请求
        /// </summary>
        /// <param name="ioEventArgs"></param>
        public abstract void PostReceive(SocketAsyncEventArgs ioEventArgs);

        /// <summary>
        /// 关闭操作
        /// </summary>
        protected abstract void Close(SocketAsyncEventArgs ioEventArgs, EOpCode opCode);
    }
}