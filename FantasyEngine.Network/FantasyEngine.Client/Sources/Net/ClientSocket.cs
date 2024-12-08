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
using FantasyEngine.Log;
using FantasyEngine.Proxy;
using FantasyEngine.IO;
using FantasyEngine.Security;
using System;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Threading;
using FantasyEngine.Network.Core;
using FantasyEngine.Network.IO;
using FantasyEngine.Network.Proxy;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 套接字父类
    /// </summary>
    public abstract class ClientSocket : FESocket, IClientSocket
    {
        /// <summary>
        /// 地址
        /// </summary>
        protected IPEndPoint address = null;

        /// <summary>
        /// 地址
        /// </summary>
        public override EndPoint Address
        {
            get { return address; }
        }

        /// <summary>
        /// 标识名称
        /// </summary>
        public virtual string Name { get; private set; }

        /// <summary>
        /// 服务器地址
        /// </summary>
        public string Host { get; private set; }

        /// <summary>
        /// 服务器端口
        /// </summary>
        public int Port { get; private set; }

        /// <summary>
        /// Ping 的往返时间，最大值为Constants.MaxRoundtripTime
        /// </summary>
        private int _roundtripTime = Constants.MaxRoundtripTime;

        /// <summary>
        /// Ping 的往返时间，最大值为Constants.MaxRoundtripTime
        /// </summary>
        public int RoundtripTime
        {
            get
            {
                return _roundtripTime;
            }
        }

        /// <summary>
        /// Ping对象
        /// </summary>
        Ping _ping = null;

        /// <summary>
        /// 是否使用Ping
        /// </summary>
        public bool UsePing { get; private set; }

        /// <summary>
        /// Ping间隔
        /// </summary>
        public int PingInterval { get; private set; }

        /// <summary>
        /// Ping线程
        /// </summary>
        internal protected virtual Timer PingTimer { get; set; } = null;

        /// <summary>
        /// 运行的标识
        /// </summary>
        private int _readyFlag = 0;

        /// <summary>
        /// 数据是否已经初始化了
        /// </summary>
        public virtual bool IsReady
        {
            get { return _readyFlag == 1; }
        }

        /// <summary>
        /// 心跳间隔
        /// </summary>
        public int HeartbeatInterval { get; private set; }

        /// <summary>
        /// 心跳包线程
        /// </summary>
        internal protected virtual Timer HeartbeatTimer { get; set; } = null;

        /// <summary>
        /// 数据处理线程
        /// </summary>
        internal protected virtual Thread LoopingThread { get; set; } = null;

        /// <summary>
        /// 心跳包
        /// </summary>
        private byte[] _heartbeatBuffer;

        ///// <summary>
        ///// 线程循环等待时间
        ///// </summary>
        //private const int THREAD_SLEEP_TIMEOUT = 5;

        ///// <summary>
        ///// 数据接收线程
        ///// </summary>
        //internal protected virtual Thread ThreadProcessReceive { get; set; } = null;

        /// <summary>
        /// 设置类别名称
        /// </summary>
        protected virtual string Category { get; set; }

        /// <summary>
        /// 消息偏移值
        /// </summary>
        private int _messageOffset = 2;

        /// <summary>
        /// 消息偏移值
        /// </summary>
        public override int MessageOffset
        {
            get { return _messageOffset; }
        }

        /// <summary>
        /// 压缩工具
        /// </summary>
        private ICompression _compression = null;

        /// <summary>
        /// 压缩工具
        /// </summary>
        public override ICompression Compression
        {
            get { return _compression; }
        }

        /// <summary>
        /// 加密工具
        /// </summary>
        private ICryptoProvider _cryptoProvider = null;

        /// <summary>
        /// 加密工具
        /// </summary>
        public override ICryptoProvider CryptoProvider
        {
            get { return _cryptoProvider; }
        }

        /// <summary>
        /// 发送的管理类
        /// </summary>
        internal protected SocketSender Sender { get; private set; }

        /// <summary>
        /// 接收管理类
        /// </summary>
        internal protected SocketReceiver<IClientSocket> Receiver { get; private set; }

        /// <summary>
        /// 消息Id
        /// </summary>
        public long MessageNumber
        {
            get { return UserToken.MessageNumber; }
            set { UserToken.MessageNumber = value; }
        }

        /// <summary>
        /// Action生成类
        /// </summary>
        public IClientActionDispatcher ActionProvider { get; set; }

        /// <summary>
        /// 消息处理的中转站
        /// </summary>
        public IBoss MessageContractor { get; set; }

        /// <summary>
        /// 初始化
        /// </summary>
        protected ClientSocket() : base(null)
        {
        }

        /// <summary>
        /// 初始化Socket基本信息
        /// </summary>
        /// <param name="name"></param>
        /// <param name="host"></param>
        /// <param name="port"></param>
        /// <param name="actionClassFullName">Action协议类的完整名称</param>
        /// <param name="heartbeatInterval">心跳间隔</param>
        /// <param name="usePing">是否使用 Ping 延迟</param>
        /// <param name="pingInterval">Ping 间隔</param>
        public virtual void Ready(string name, string host, int port,
            string actionClassFullName,
            int heartbeatInterval = Constants.HeartBeatsInterval,
            bool usePing = true, int pingInterval = Constants.PingInterval)
        {
            Name = name;
            Host = host;
            Port = port;
            address = new IPEndPoint(IPAddress.Parse(host), port);
            Category = string.Format("{0}:{1},{2}", GetType().Name, Host, Port);
            ActionProvider = new ClientActionDispatcher(actionClassFullName);
            HeartbeatInterval = heartbeatInterval;
            UsePing = usePing;
            PingInterval = pingInterval;
            FEConsole.WriteInfoFormatWithCategory(Category, "Socket is ready...");
            Interlocked.Exchange(ref _readyFlag, 1);
        }

        /// <summary>
        /// 自动连接
        /// </summary>
        public virtual void AutoConnect()
        {
            if (!IsRunning)
            {
                ConnectAsync();
            }
        }

        /// <summary>
        /// 连接函数[内部异步实现]
        /// </summary>
        public virtual void ConnectAsync(Action<bool> callback = null)
        {
            IsRunning = true;
            Awake();
            ThreadPool.UnsafeQueueUserWorkItem((state) =>
            {
                bool success = Connect();
                (state as Action<bool>)?.Invoke(success);
            }, callback);
        }

        /// <summary>
        /// 同步连接
        /// </summary>
        protected virtual bool Connect()
        {
            if (!IsReady)
            {
                IsRunning = false;
                FEConsole.WriteErrorFormatWithCategory(Categories.SOCKET, "Socket is not ready!");
                return false;
            }

            IsRunning = true;
            Awake();
            FEConsole.WriteInfoFormatWithCategory(Category, "Socket is starting...");
            try
            {
                BeginConnectImpl();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Category, "Fail to connect the server!", e);
                Close();
                return false;
            }

            if (HeartbeatTimer == null)
            {
                //心跳包线程
                HeartbeatTimer = new Timer(SendHeartbeatPackage, null, HeartbeatInterval, HeartbeatInterval);
                RebuildHeartbeatPackage();
            }

            if (UsePing)
            {
                if (PingTimer == null)
                {
                    PingTimer = new Timer(BeginPing, null, 0, PingInterval);
                }
                if (_ping == null)
                {
                    _ping = new Ping();
                    _ping.PingCompleted += OnPingCompleted;
                }
            }
            else
            {
                if (PingTimer != null)
                {
                    try
                    {
                        PingTimer.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "PingTimer dispose error.", e);
                    }

                    PingTimer = null;
                }

                if (_ping != null)
                {
                    try
                    {
                        _ping.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "PingTimer dispose error.", e);
                    }

                    _ping = null;
                }
            }

            if (LoopingThread == null)
            {
                LoopingThread = new Thread(Looping);
                LoopingThread.Start();
            }

            FEConsole.WriteInfoFormatWithCategory(Category, "Socket connected.");

            return true;
        }

        /// <summary>
        /// 创建连接
        /// </summary>
        internal protected abstract void BeginConnectImpl();

        /// <summary>
        /// 初始化执行
        /// </summary>
        protected virtual void Awake()
        {
            if (Socket == null)
            {
                Socket = MakeSocket();
                //socket.ReceiveTimeout = 50;//此方法只能在同步模式下使用
                EventArgs = MakeEventArgs(Socket);
                //UserToken.Socket = this;
                EventArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnCompleted);
            }

            if (Sender == null)
            {
                Sender = null;
                switch (Type)
                {
                    case ESocketType.Tcp:
                        Sender = new TcpClientSender(this);
                        break;
                    case ESocketType.Udp:
                        Sender = new UdpClientSender(this);
                        break;
                }
            }

            if (Receiver == null)
            {
                Receiver = null;
                switch (Type)
                {
                    case ESocketType.Tcp:
                        Receiver = new TcpClientReceiver(this);
                        break;
                    case ESocketType.Udp:
                        Receiver = new UdpClientReceiver(this);
                        break;
                }

                Receiver.OnMessageReceived = OnMessageReceived;
            }
        }

        private void OnCompleted(object sender, SocketAsyncEventArgs eventArgs)
        {
            try
            {
                switch (EventArgs.LastOperation)
                {
                    case SocketAsyncOperation.Receive:
                    case SocketAsyncOperation.ReceiveFrom:
                        Receiver.ProcessReceive(eventArgs);
                        break;
                    case SocketAsyncOperation.Send:
                    case SocketAsyncOperation.SendTo:
                        Sender.ProcessSend(eventArgs);
                        break;
                    default:
                        throw new ArgumentException(
                            "The last operation completed on the socket was not a receive or send");
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Category, e);
            }
        }

        private void Looping(object state)
        {
            while (IsRunning)
            {
                if (Operating())
                {
                    continue;
                }

                if (Socket.Poll(0, SelectMode.SelectRead))
                {
                    if (TryReceive(true))
                    {
                        Receiver.PostReceive(EventArgs);
                    }

                    continue;
                }

                if (Socket.Poll(0, SelectMode.SelectWrite))
                {
                    if (TrySend(true))
                    {
                        Sender.PostSend(EventArgs);
                    }

                    continue;
                }
            }
        }

        public override void NextStep(SocketAsyncEventArgs eventArgs)
        {
            OperationCompleted();
        }

        /// <summary>
        /// 创建原生套接字
        /// </summary>
        protected abstract Socket MakeSocket();

        /// <summary>
        /// 发送心跳包
        /// </summary>
        internal protected virtual void SendHeartbeatPackage(object state)
        {
            const string error = "Send heartbeat package failed.";
            try
            {
                if (!Socket.Connected)
                {
                    throw new SocketException((int)SocketError.NotConnected);
                }

                // 心跳包
                if (_heartbeatBuffer != null)
                {
                    Sender.Push(this, _heartbeatBuffer, true);
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Category, error, e);
            }
        }

        /// <summary>
        /// 重新构建心跳包数据
        /// </summary>
        public virtual void RebuildHeartbeatPackage()
        {
            _heartbeatBuffer = BuildHeartbeatBuffer();
        }

        /// <summary>
        /// 创建默认心跳包数据
        /// </summary>
        /// <returns></returns>
        protected virtual byte[] BuildHeartbeatBuffer()
        {
            // 创建默认心跳包数据
            MessageWriter msg = new MessageWriter();
            msg.OpCode = (sbyte)EOpCode.Pong;
            return PackageFactory.Pack(msg, MessageOffset, null, null);
        }

        /// <summary>
        /// 关闭函数
        /// </summary>
        public override void Close(EOpCode opCode = EOpCode.Close)
        {
            if (!IsRunning)
            {
                return;
            }

            //FConsole.Write(new System.Diagnostics.StackTrace(true).ToString());
            IsRunning = false;
            lock (this)
            {
                if (Sender != null)
                {
                    MessageWriter msg = new MessageWriter();
                    msg.OpCode = (sbyte)EOpCode.Close;
                    byte[] closeMessage = PackageFactory.Pack(msg, MessageOffset, null, null);
                    //立即发送一条客户端关闭消息
                    try
                    {
                        Sender.Push(this, closeMessage, true);
                    }
                    catch
                    {
                    }
                }
            }

            base.Close(opCode);
            lock (this)
            {
                if (HeartbeatTimer != null)
                {
                    try
                    {
                        HeartbeatTimer.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "HeartbeatTimer dispose error.", e);
                    }

                    HeartbeatTimer = null;
                }

                if (PingTimer != null)
                {
                    try
                    {
                        PingTimer.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "PingTimer dispose error.", e);
                    }

                    PingTimer = null;
                }

                if (_ping != null)
                {
                    try
                    {
                        _ping.Dispose();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "PingTimer dispose error.", e);
                    }

                    _ping = null;
                }

                if (LoopingThread != null)
                {
                    try
                    {
                        LoopingThread.Abort();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Category, "LoopThread abort error.", e);
                    }

                    LoopingThread = null;
                }

                //管理类回收
                if (EventArgs != null)
                {
                    EventArgs.Dispose();
                }

                EventArgs = null;

                Sender = null;
                Receiver = null;
            }

            FEConsole.WriteInfoFormatWithCategory(Category, "Socket Closed.");
        }

        /// <summary>
        /// 设置消息的偏移值
        /// </summary>
        public void SetMessageOffset(int offset)
        {
            _messageOffset = offset;
        }

        /// <summary>
        /// 设置压缩方案
        /// </summary>
        public void SetCompression(ICompression compression)
        {
            this._compression = compression;
        }

        /// <summary>
        /// 设置解密方案
        /// </summary>
        public void SetCryptoProvide(ICryptoProvider cryptoProvider)
        {
            this._cryptoProvider = cryptoProvider;
        }

        /// <summary>
        /// 数据发送<c>异步</c>
        /// </summary>
        /// <param name="message">大宋的消息</param>
        /// <returns>判断有没有发送出去</returns>
        public void Send(MessageWriter message)
        {
            AutoConnect();
            byte[] data = PackageFactory.Pack(message, MessageOffset, Compression, CryptoProvider);
            Sender.Push(this, data, false);
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
            AutoConnect();
            byte[] data = PackageFactory.Pack(message, MessageOffset, Compression, CryptoProvider);
            Sender.Push(this, data, true);
        }

        /// <summary>
        /// 内部函数，直接传bytes，会影响数据解析
        /// </summary>
        internal protected void SendBytes(byte[] data)
        {
            AutoConnect();
            Sender.Push(this, data, false);
        }

        /// <summary>
        /// 内部函数，直接传bytes，会影响数据解析，以及解析顺序
        /// </summary>
        internal protected void SendBytesImmediately(byte[] data)
        {
            AutoConnect();
            Sender.Push(this, data, true);
        }

        /// <summary>
        /// 消息处理
        /// </summary>
        /// <param name="e"></param>
        private void OnMessageReceived(IMessageEventArgs<IClientSocket> args)
        {
            try
            {
                if (MessageContractor != null)
                {
                    MessageContractor.CheckIn(new MessageWorker { Message = args.Message, Socket = this });
                }
                else
                {
                    ThreadPool.QueueUserWorkItem((obj) => { ProcessMessage(args.Message); });
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Category, "Process message error.", e);
            }
        }

        /// <summary>
        /// 消息处理
        /// </summary>
        internal virtual void ProcessMessage(IMessageReader message)
        {
            if (message == null)
            {
                FEConsole.WriteErrorFormatWithCategory(Category, "{0} receive empty message.", Category);
                return;
            }

            try
            {
                ClientAction action = ActionProvider.Provide(message.ActionId);
                try
                {
                    ActionBoss.Exploit(action, message.ActionId, message);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Category, "Action error.", e);
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Category, "ActionProvider error.", e);
            }
        }

        /// <summary>
        /// Ping延迟
        /// </summary>
        public virtual void BeginPing(object state)
        {
            if (_ping == null)
            {
                _ping = new Ping();
            }
            _ping.SendAsync(address.Address, Math.Min(PingInterval, Constants.MaxRoundtripTime), null);
        }

        private void OnPingCompleted(object sender, PingCompletedEventArgs e)
        {
            if (e.Error != null || e.Reply == null)
            {
                Interlocked.Exchange(ref _roundtripTime, Constants.MaxRoundtripTime);
                return;
            }
            PingReply reply = e.Reply;
            if (reply.Status != IPStatus.Success)
            {
                Interlocked.Exchange(ref _roundtripTime, Constants.MaxRoundtripTime);
                return;
            }
            Interlocked.Exchange(ref _roundtripTime, (int)reply.RoundtripTime);
        }
    }
}