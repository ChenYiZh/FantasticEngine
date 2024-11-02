/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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

using FantasyEngine.IO;
using FantasyEngine.Log;
using FantasyEngine.Network.IO;
using FantasyEngine.Security;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace FantasyEngine.Network.Core
{
    /// <summary>
    /// 套接字嵌套层
    /// </summary>
    public sealed class RemoteSocket : FSocket, IRemoteSocket
    {
        /// <summary>
        /// 尝试开始发送
        /// </summary>
        public override bool TrySend(bool onlyWait = false)
        {
            return Server.TrySend(onlyWait);
        }

        /// <summary>
        /// 尝试接收
        /// </summary>
        public override bool TryReceive(bool onlyWait = false)
        {
            return Server.TryReceive(onlyWait);
        }

        public override bool Operating()
        {
            return Server.Operating();
        }

        /// <summary>
        /// 操作完成时执行
        /// </summary>
        public override void OperationCompleted()
        {
            Server.OperationCompleted();
        }
        
        public override void InLooping()
        {
            Server.InLooping();
        }

        public override void OutLooping()
        {
            Server.OutLooping();
        }

        public override void NextStep(SocketAsyncEventArgs eventArgs)
        {
        }

        /// <summary>
        /// 是否在运行
        /// </summary>
        internal void SetIsRunning(bool value)
        {
            IsRunning = value;
        }

        /// <summary>
        /// 所属服务器
        /// </summary>
        public IServerSocket Server { get; private set; }

        /// <summary>
        /// 唯一id
        /// </summary>
        public Guid HashCode { get; private set; }

        /// <summary>
        /// 获取时间
        /// </summary>
        public DateTime AccessTime { get; internal set; }

        /// <summary>
        /// 地址
        /// </summary>
        private EndPoint _address = null;

        /// <summary>
        /// 地址
        /// </summary>
        public override EndPoint Address
        {
            get { return _address; }
        }

        /// <summary>
        /// 类型
        /// </summary>
        public override ESocketType Type
        {
            get { return Server.Type; }
        }

        /// <summary>
        /// 消息偏移值
        /// </summary>
        public override int MessageOffset
        {
            get { return Server.MessageOffset; }
        }

        /// <summary>
        /// 压缩工具
        /// </summary>
        public override ICompression Compression
        {
            get { return Server.Compression; }
        }

        /// <summary>
        /// 加密工具
        /// </summary>
        public override ICryptoProvider CryptoProvider
        {
            get { return Server.CryptoProvider; }
        }

        // /// <summary>
        // /// 发送的管理类
        // /// </summary>
        // internal SocketSender Sender { get; private set; }


        /// <summary>
        /// 消息Id
        /// </summary>
        public long MessageNumber
        {
            get { return UserToken.MessageNumber; }
            set { UserToken.MessageNumber = value; }
        }

        /// <summary>
        /// 上次心跳时间
        /// </summary>
        internal DateTime RefreshTime { get; set; } = TimeLord.Now;

        /// <summary>
        /// 构造函数
        /// </summary>
        public RemoteSocket(IServerSocket server, SocketAsyncEventArgs eventArgs) : base(eventArgs)
        {
            if (server == null)
            {
                throw new NullReferenceException("Fail to create remove socket, because the server is null.");
            }

            //Sender = new SocketSender(this);
            // Receiver = null;
            // switch (server.Type)
            // {
            //     case ESocketType.Tcp: Receiver = new TcpSocketReceiver<IRemoteSocket>(this); break;
            //     case ESocketType.Udp: Receiver = new UdpSocketReceiver<IRemoteSocket>(this); break;
            // }
            // Receiver.OnMessageReceived = OnMessageReceived;
            // Receiver.OnPing = OnPing;
            // Receiver.OnPong = OnPong;
            Server = server;
            IsRunning = true;
            HashCode = Guid.NewGuid();
            try
            {
                //_address = Socket.RemoteEndPoint as IPEndPoint;
                _address = EventArgs.RemoteEndPoint;// as IPEndPoint;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.SOCKET, e);
            }
        }

        /// <summary>
        /// 关闭
        /// </summary>
        public override void Close(EOpCode opCode = EOpCode.Close)
        {
            bool isRuning = IsRunning;
            //Sender = null;
            //Receiver = null;
            if (isRuning)
            {
                Server.OnRemoteSocketClosed(this, opCode);
            }

            base.Close(opCode);
        }

        /// <summary>
        /// 重置唯一id
        /// </summary>
        /// <param name="key">新的id</param>
        public void ResetHashset(Guid key)
        {
            HashCode = key;
        }

        // /// <summary>
        // /// 开始执行发送
        // /// </summary>
        // internal bool BeginSend()
        // {
        //     return Sender.BeginSend();
        // }

        /// <summary>
        /// 数据发送<c>异步</c>
        /// </summary>
        /// <param name="message">大宋的消息</param>
        /// <returns>判断有没有发送出去</returns>
        public void Send(MessageWriter message)
        {
            byte[] data = PackageFactory.Pack(message, MessageOffset, Compression, CryptoProvider);
            ((ServerSocket) Server).Sender.Push(this, data, false);
        }

        /// <summary>
        /// 立即发送消息，会打乱消息顺序。只有类似心跳包这种及时的需要用到。一般使用Send就满足使用
        /// </summary>
        /// <param name="message">发送的消息</param>
        [Obsolete(
            "Only used in important message. This method will confuse the message queue. You can use 'Send' instead.",
            false)]
        public void SendImmediately(MessageWriter message)
        {
            byte[] data = PackageFactory.Pack(message, MessageOffset, Compression, CryptoProvider);
            ((ServerSocket) Server).Sender.Push(this, data, true);
        }

        /// <summary>
        /// 内部函数，直接传bytes，会影响数据解析
        /// </summary>
        internal void SendBytes(byte[] data)
        {
            ((ServerSocket) Server).Sender.Push(this, data, false);
        }

        /// <summary>
        /// 内部函数，直接传bytes，会影响数据解析，以及解析顺序
        /// </summary>
        internal void SendBytesImmediately(byte[] data)
        {
            ((ServerSocket) Server).Sender.Push(this, data, true);
        }

        // /// <summary>
        // /// 等待消息接收
        // /// </summary>
        // internal bool BeginReceive()
        // {
        //     return Receiver.BeginReceive();
        // }

        // /// <summary>
        // /// 当消息处理完执行
        // /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs</para>
        // /// </summary>
        // internal bool MessageSolved(object sender, SocketAsyncEventArgs e)
        // {
        //     // 等待操作处理
        //     bool bWaiting = false;
        //     // determine which type of operation just completed and call the associated handler
        //     switch (e.LastOperation)
        //     {
        //         case SocketAsyncOperation.Receive:
        //             bWaiting = !Receiver.ProcessReceive();
        //             break;
        //         case SocketAsyncOperation.Send:
        //             bWaiting = !Sender.ProcessSend();
        //             break;
        //         default:
        //             throw new ArgumentException("The last operation completed on the socket was not a receive or send");
        //     }
        //     return bWaiting;
        // }

        /// <summary>
        /// 定时处理消息
        /// </summary>
        // internal bool CheckSendOrReceive()
        // {
        //     return Receiver == null || Sender == null || !Receiver.BeginReceive((TimeLord.Now - RefreshTime).TotalMilliseconds > Constants.HeartBeatsInterval) || !Sender.BeginSend();
        // }
        
        // private void OnMessageReceived(IMessageEventArgs<IRemoteSocket> e)
        // {
        //     ((IServerMessageProcessor) Server).MessageReceived(e);
        // }
        //
        // private void OnPong(IMessageEventArgs<IRemoteSocket> e)
        // {
        //     RefreshTime = TimeLord.Now;
        //     ((IServerMessageProcessor) Server).Pong(e);
        // }
        //
        // private void OnPing(IMessageEventArgs<IRemoteSocket> e)
        // {
        //     ((IServerMessageProcessor) Server).Ping(e);
        // }
    }
}