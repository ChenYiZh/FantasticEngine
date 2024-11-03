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

using FantasyEngine.Collections;
using FantasyEngine.IO;
using FantasyEngine.Network.Core;
using FantasyEngine.Network.IO;
using FantasyEngine.Network.Server;
using FantasyEngine.Security;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 连接消息代理
    /// </summary>
    public delegate void ConnectionEventHandler(IServerSocket socket, IRemoteSocket remoteSocket);

    /// <summary>
    /// 收发消息处理
    /// </summary>
    public delegate void MessageEventHandler(IServerSocket socket, IMessageEventArgs<IRemoteSocket> e);

    /// <summary>
    /// 服务器套接字管理
    /// </summary>
    public abstract class ServerSocket : FESocket, IServerSocket
    {
        /// <summary>
        /// 连接事件
        /// </summary>
        public event ConnectionEventHandler OnConnected;

        private void AfterConnected(IRemoteSocket remoteSocket)
        {
            OnConnected?.Invoke(this, remoteSocket);
        }

        /// <summary>
        /// 握手事件
        /// </summary>
        public event ConnectionEventHandler OnHandshaked;

        private void Handshaked(IRemoteSocket remoteSocket)
        {
            OnHandshaked?.Invoke(this, remoteSocket);
        }

        /// <summary>
        /// 断开连接事件
        /// </summary>
        public event ConnectionEventHandler OnDisconnected;

        private void Disconnected(IRemoteSocket remoteSocket)
        {
            OnDisconnected?.Invoke(this, remoteSocket);
        }

        /// <summary>
        /// 接收到数据包事件
        /// </summary>
        public event MessageEventHandler OnMessageReceived;

        private void MessageReceived(IMessageEventArgs<IRemoteSocket> args)
        {
            OnMessageReceived?.Invoke(this, args);
        }

        /// <summary>
        /// 心跳探索事件
        /// </summary>
        public event MessageEventHandler OnPing;

        private void Ping(IMessageEventArgs<IRemoteSocket> args)
        {
            ((RemoteSocket)args.Socket).RefreshTime = TimeLord.Now;
            OnPing?.Invoke(this, args);
        }

        /// <summary>
        /// 心跳回应事件
        /// </summary>
        public event MessageEventHandler OnPong;

        private void Pong(IMessageEventArgs<IRemoteSocket> args)
        {
            ((RemoteSocket)args.Socket).RefreshTime = TimeLord.Now;
            OnPong?.Invoke(this, args);
        }

        /// <summary>
        /// 内部关键原生Socket
        /// </summary>
        internal protected Socket socket = null;

        /// <summary>
        /// 内部关键原生Socket
        /// </summary>
        public override Socket Socket
        {
            get { return socket; }
        }

        /// <summary>
        /// 封装的地址
        /// </summary>
        private IPEndPoint _address = null;

        /// <summary>
        /// 封装的地址
        /// </summary>
        public override EndPoint Address
        {
            get { return _address; }
        }

        /// <summary>
        /// 绑定的端口
        /// </summary>
        public int Port
        {
            get { return Setting.Port; }
        }

        /// <summary>
        /// 对应Host的名称
        /// </summary>
        public string ServerName
        {
            get { return Setting.Name; }
        }

        /// <summary>
        /// 配置信息
        /// </summary>
        public IHostSetting Setting { get; private set; }

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
        /// 消息偏移值
        /// </summary>
        public void SetMessageOffset(int offset)
        {
            _messageOffset = offset;
        }

        /// <summary>
        /// 消息偏移值
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
        /// 压缩工具
        /// </summary>
        public void SetCompression(ICompression compression)
        {
            this._compression = compression;
        }

        /// <summary>
        /// 消息偏移值
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
        /// 加密工具
        /// </summary>
        public void SetCryptoProvide(ICryptoProvider cryptoProvider)
        {
            this._cryptoProvider = cryptoProvider;
        }

        /// <summary>
        /// 类型
        /// </summary>
        public override ESocketType Type
        {
            get { return Setting.Type; }
        }

        /// <summary>
        /// 状态类
        /// </summary>
        private SummaryStatus _summary = new SummaryStatus();

        /// <summary>
        /// 输出统计的线程
        /// </summary>
        private Timer _summaryTask;

        /// <summary>
        /// 并发管理锁
        /// </summary>
        private Semaphore _maxConnectionsEnforcer;

        /// <summary>
        /// 接受连接并发对象池
        /// </summary>
        internal protected ThreadSafeStack<SocketAsyncEventArgs> acceptEventArgsPool;

        /// <summary>
        /// 回复并发对象池
        /// </summary>
        private ThreadSafeStack<SocketAsyncEventArgs> _ioEventArgsPool;

        /// <summary>
        /// 等待消息处理的缓存列表，主要用于单线程处理
        /// </summary>
        internal protected ThreadSafeDictionary<EndPoint, RemoteSocket> sockets =
            new ThreadSafeDictionary<EndPoint, RemoteSocket>();

        /// <summary>
        /// 生成的所有套接字管理对象都缓存在这里
        /// </summary>
        private ThreadSafeList<SocketAsyncEventArgs> _allEventArgsPool = new ThreadSafeList<SocketAsyncEventArgs>();

        /// <summary>
        /// 字节流池
        /// </summary>
        private BytePool bytePool { get; set; }

        /// <summary>
        /// 心跳检测线程
        /// </summary>
        private Timer _heartBeatsCheckingTimer;

        /// <summary>
        /// 待处理的Socket的等待线程
        /// </summary>
        private Thread _loopingThread;

        /// <summary>
        /// 接收管理类
        /// </summary>
        internal SocketReceiver<IRemoteSocket> Receiver { get; private set; }

        /// <summary>
        /// 发送的管理类
        /// </summary>
        internal SocketSender Sender { get; private set; }

        /// <summary>
        /// 初始化
        /// </summary>
        public ServerSocket() : base(null)
        {
        }

        /// <summary>
        /// 入口函数
        /// </summary>
        /// <param name="setting"></param>
        public void Start(IHostSetting setting)
        {
            if (IsRunning)
            {
                return;
            }

            //默认参数赋值
            IsRunning = true;
            Setting = setting;
            bytePool = new BytePool(Setting.BufferSize, Setting.MaxIOCapacity);
            //Test测试代码，发布前去除
            //Compression = new GZipCompression();
            //CryptoProvide = new AESCryptoProvider("FantasyEngine", "ChenYiZh");

            //对象池初始化
            acceptEventArgsPool = new ThreadSafeStack<SocketAsyncEventArgs>();
            for (int i = 0; i < setting.MaxAcceptCapacity; i++)
            {
                acceptEventArgsPool.Push(CreateAcceptEventArgs());
            }

            _ioEventArgsPool = new ThreadSafeStack<SocketAsyncEventArgs>();
            for (int i = 0; i < setting.MaxIOCapacity; i++)
            {
                _ioEventArgsPool.Push(MakeIOEventArgs());
            }

            //并发锁初始化
            _maxConnectionsEnforcer = new Semaphore(setting.MaxConnections, setting.MaxConnections);
            //生成套接字
            _address = new IPEndPoint(IPAddress.Any, Port);
            EventArgs = new SocketAsyncEventArgs();
            EventArgs.SetBuffer(new byte[bytePool.PoolInfo.SingleLength], 0, bytePool.PoolInfo.SingleLength);
            EventArgs.RemoteEndPoint = new IPEndPoint(IPAddress.Any, Port);
            BuildSocket();

            Receiver = null;
            switch (Type)
            {
                case ESocketType.Tcp:
                    Receiver = new TcpServerReceiver(this);
                    Sender = new TcpServerSender(this);
                    break;
                case ESocketType.Udp:
                    Receiver = new UdpServerReceiver(this);
                    Sender = new UdpServerSender(this);
                    break;
            }


            Receiver.OnMessageReceived = MessageReceived;
            Receiver.OnPing = Ping;
            Receiver.OnPong = Pong;

            //服务器状态输出周期
            _summaryTask = new Timer(WriteSummary, null, 60000, 60000);

            //心跳检测
            int waitingInterval = Constants.HeartBeatsInterval;
            _heartBeatsCheckingTimer = new Timer(HeartBeatsChecking, null, waitingInterval, waitingInterval);

            //待接收消息的套接字管理线程
            _loopingThread = new Thread(Looping);
            _loopingThread.Start();
            //启动监听
            PostAccept();
        }

        /// <summary>
        /// 创建套接字
        /// </summary>
        internal protected abstract void BuildSocket();

        /// <summary>
        /// 开始接受连接
        /// </summary>
        internal protected void PostAccept()
        {
            //while (IsRunning)
            //{
            try
            {
                if (!IsRunning) return;
                OnPostAccept();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Post accept listen error:", e);
            }
            //}
        }

        /// <summary>
        /// 开始接受连接
        /// </summary>
        internal protected abstract void OnPostAccept();

        /// <summary>
        /// 处理接收到的连接
        /// </summary>
        /// <param name="acceptEventArgs"></param>
        internal protected virtual void ProcessAccept(SocketAsyncEventArgs acceptEventArgs)
        {
            try
            {
                Interlocked.Increment(ref _summary.TotalConnectCount);
                _maxConnectionsEnforcer.WaitOne();

                if (acceptEventArgs.SocketError != SocketError.Success)
                {
                    Interlocked.Increment(ref _summary.RejectedConnectCount);
                    HandleBadAccept(acceptEventArgs);
                }
                else
                {
                    Interlocked.Increment(ref _summary.CurrentConnectCount);

                    // 生成连接对象
                    SocketAsyncEventArgs ioEventArgs = null;
                    RemoteSocket socket = null;
                    lock (sockets.SyncRoot)
                    {
                        if (!sockets.ContainsKey(acceptEventArgs.RemoteEndPoint) && _ioEventArgsPool.Count > 0)
                        {
                            ioEventArgs = _ioEventArgsPool.Pop();
                            ioEventArgs.AcceptSocket = acceptEventArgs.AcceptSocket;
                            ioEventArgs.RemoteEndPoint = acceptEventArgs.RemoteEndPoint;
                            //IUserToken userToken = (IUserToken)ioEventArgs.UserToken;
                            //ArrangeSocketBuffer(ioEventArgs);
                            socket = new RemoteSocket(this, ioEventArgs);
                            socket.AccessTime = TimeLord.Now;
                            sockets.Add(acceptEventArgs.RemoteEndPoint, socket);
                            //((UserToken)ioEventArgs.UserToken).Socket = socket;
                        }
                    }

                    acceptEventArgs.AcceptSocket = null;

                    //release connect when socket has be closed.
                    ReleaseAccept(acceptEventArgs, false);

                    // 连接后处理
                    try
                    {
                        AfterConnected(socket);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "OnConnected error:", e);
                    }

                    //Receiver.PostReceive(ioEventArgs);
                    //PostReceive(ioEventArgs);
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
            }
            finally
            {
                PostAccept();
            }
        }

        /// <summary>
        /// 收到连接时需要做的事情
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="acceptEventArgs"></param>
        internal protected abstract void AcceptCompleted(object sender, SocketAsyncEventArgs acceptEventArgs);

        /// <summary>
        /// 释放并发锁
        /// </summary>
        /// <param name="acceptEventArgs"></param>
        /// <param name="isRelease"></param>
        internal protected void ReleaseAccept(SocketAsyncEventArgs acceptEventArgs, bool isRelease = true)
        {
            if (acceptEventArgs != EventArgs)
            {
                acceptEventArgsPool.Push(acceptEventArgs);
            }

            if (isRelease)
            {
                try
                {
                    _maxConnectionsEnforcer.Release();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Release accept error.", e);
                }
            }
        }

        /// <summary>
        /// 消息接收处理
        /// </summary>
        private void IOCompleted(object sender, SocketAsyncEventArgs ioEventArgs)
        {
            try
            {
                IUserToken userToken = UserToken;
                // if (userToken == null)
                // {
                //     return;
                // } 

                RemoteSocket socket = (RemoteSocket)userToken.Socket;
                socket.AccessTime = TimeLord.Now;
                switch (ioEventArgs.LastOperation)
                {
                    case SocketAsyncOperation.Receive:
                    case SocketAsyncOperation.ReceiveFrom:
                        Receiver.ProcessReceive(ioEventArgs);
                        break;
                    case SocketAsyncOperation.Send:
                    case SocketAsyncOperation.SendTo:
                        Sender.PostSend(ioEventArgs);
                        break;
                    default:
                        throw new ArgumentException(
                            "The last operation completed on the socket was not a receive or send");
                }
                // if (socket.MessageSolved(sender, ioEventArgs))
                // {
                //     _sockets.Add(socket);
                // }
            }
            catch (ObjectDisposedException)
            {
                OperationCompleted();
                ReleaseIOEventArgs(ioEventArgs);
            }
            catch (Exception e)
            {
                OperationCompleted();
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), $"IOCompleted unkown error:", e);
            }
        }

        internal protected abstract void Looping(object state);

        public override void NextStep(SocketAsyncEventArgs eventArgs)
        {
            OperationCompleted();
            return;
            while (true)
            {
                if (Socket.Poll(-1, SelectMode.SelectRead))
                {
                    if (TryReceive(true))
                    {
                        Receiver.PostReceive(eventArgs);
                    }

                    break;
                }
                else if (Socket.Poll(-1, SelectMode.SelectWrite))
                {
                    if (TrySend(true))
                    {
                        Sender.PostSend(eventArgs);
                    }

                    break;
                }
            }
        }

        /// <summary>
        /// 心跳检测
        /// </summary>
        private void HeartBeatsChecking(object state)
        {
            Dictionary<EndPoint, RemoteSocket> tSockets;
            lock (sockets.SyncRoot)
            {
                tSockets = new Dictionary<EndPoint, RemoteSocket>(sockets);
            }

            Parallel.ForEach(tSockets, (KeyValuePair<EndPoint, RemoteSocket> kv) =>
            {
                if ((TimeLord.Now - kv.Value.RefreshTime).TotalMilliseconds > 2 * Constants.HeartBeatsInterval)
                {
                    //心跳超时2倍，直接关闭
                    kv.Value.Close();
                }
            });
        }

        /// <summary>
        /// 释放消息事件
        /// </summary>
        private void ReleaseIOEventArgs(SocketAsyncEventArgs ioEventArgs)
        {
            if (ioEventArgs == null) return;
            IUserToken userToken = ioEventArgs.UserToken as IUserToken;
            if (userToken != null)
            {
                RemoveSocket((RemoteSocket)userToken.Socket);
                userToken.Reset();
                //userToken.Socket = null;
            }

            ioEventArgs.AcceptSocket = null;
            _ioEventArgsPool.Push(ioEventArgs);
        }

        /// <summary>
        /// 异常连接处理
        /// </summary>
        /// <param name="acceptEventArgs"></param>
        private void HandleBadAccept(SocketAsyncEventArgs acceptEventArgs)
        {
            try
            {
                ResetSocketAsyncEventArgs(acceptEventArgs);
                acceptEventArgs.AcceptSocket = null;
                ReleaseAccept(acceptEventArgs);
            }
            catch
            {
            }
        }

        /// <summary>
        /// 重置Socket连接对象
        /// </summary>
        /// <param name="eventArgs"></param>
        private void ResetSocketAsyncEventArgs(SocketAsyncEventArgs eventArgs)
        {
            try
            {
                if (eventArgs.AcceptSocket != null)
                {
                    eventArgs.AcceptSocket.Close();
                }
            }
            catch (Exception)
            {
            }

            eventArgs.AcceptSocket = null;
        }

        /// <summary>
        /// 当远端连接关闭时，执行一些回收代码
        /// </summary>
        public virtual void OnRemoteSocketClosed(IRemoteSocket socket, EOpCode opCode)
        {
            Interlocked.Decrement(ref _summary.CurrentConnectCount);
            Interlocked.Increment(ref _summary.CloseConnectCount);
            try
            {
                throw new Exception("OnRemoteSocketClosed");
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            RemoveSocket(socket);
            if (socket.EventArgs != null)
            {
                IUserToken token = socket.UserToken;
                if (opCode != EOpCode.Empty)
                {
                    try
                    {
                        // TODO: 处理关闭握手协议 CloseHandshake(dataToken.Socket, reason);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Closing error:", e);
                    }
                }

                if (socket.EventArgs.AcceptSocket != null)
                {
                    try
                    {
                        socket.EventArgs.AcceptSocket.Shutdown(SocketShutdown.Both);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Closing error:", e);
                    }
                }
            }

            try
            {
                _maxConnectionsEnforcer.Release();
            }
            catch (Exception e)
            {
                string errorMessage = string.Format(
                    "Closed error, connect status: TotalCount={0}, CurrentCount={1}, CloseCount={2}, RejectedCount={3}",
                    _summary.TotalConnectCount, _summary.CurrentConnectCount, _summary.CloseConnectCount,
                    _summary.RejectedConnectCount);
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), errorMessage, e);
            }

            try
            {
                Disconnected(socket);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "OnDisconnected error:", e);
            }

            if (socket.EventArgs != null)
            {
                ResetSocketAsyncEventArgs(socket.EventArgs);
                ReleaseIOEventArgs(socket.EventArgs);
            }
        }

        /// <summary>
        /// 输出状态
        /// </summary>
        /// <param name="state"></param>
        private void WriteSummary(object state)
        {
            try
            {
                FEConsole.WriteFormatWithCategory(Setting.GetCategory(),
                    "Socket connect status: Total Count = {0}, Current Count = {1}, Closed Count = {2}, Rejected Count = {3}",
                    _summary.TotalConnectCount, _summary.CurrentConnectCount, _summary.CloseConnectCount,
                    _summary.RejectedConnectCount);
            }
            catch
            {
            }
        }

        internal void RemoveSocket(IRemoteSocket remoteSocket)
        {
            lock (sockets.SyncRoot)
            {
                RemoteSocket value;
                if (sockets.TryGetValue(remoteSocket.Address, out value) && value == remoteSocket)
                {
                    sockets.Remove(remoteSocket.Address);
                    return;
                }

                EndPoint key = null;
                foreach (KeyValuePair<EndPoint, RemoteSocket> kv in sockets)
                {
                    if (kv.Value == remoteSocket)
                    {
                        key = kv.Key;
                        break;
                    }
                }

                if (key != null)
                {
                    sockets.Remove(key);
                }
            }
        }

        /// <summary>
        /// 设置缓冲区大小
        /// </summary>
        private SocketAsyncEventArgs MakeIOEventArgs()
        {
            //int bufferSize = Setting.BufferSize;
            //ioEventArgs.SetBuffer(new byte[bufferSize], 0, bufferSize);
            byte[] buffer;
            int offset;
            int bufferSize;
            bytePool.GetByteBlock(out buffer, out offset, out bufferSize);
            SocketAsyncEventArgs ioEventArgs = MakeEventArgs(null, buffer, offset, bufferSize);
            ioEventArgs.Completed += new EventHandler<SocketAsyncEventArgs>(IOCompleted);
            _allEventArgsPool.Add(ioEventArgs);
            return ioEventArgs;
        }

        /// <summary>
        /// 创建待建立连接的套接字缓存类
        /// </summary>
        /// <returns></returns>
        internal protected SocketAsyncEventArgs CreateAcceptEventArgs()
        {
            SocketAsyncEventArgs acceptEventArg = new SocketAsyncEventArgs();
            acceptEventArg.Completed += new EventHandler<SocketAsyncEventArgs>(AcceptCompleted);
            _allEventArgsPool.Add(acceptEventArg);
            return acceptEventArg;
        }

        /// <summary>
        /// 关闭操作
        /// </summary>
        public override void Close(EOpCode opCode = EOpCode.Close)
        {
            IsRunning = false;
            DisposeEventArgs();
            List<RemoteSocket> remoteSockets = new List<RemoteSocket>();
            foreach (KeyValuePair<EndPoint, RemoteSocket> kv in sockets)
            {
                remoteSockets.Add(kv.Value);
            }

            foreach (RemoteSocket remoteSocket in remoteSockets)
            {
                try
                {
                    remoteSocket.Close();
                }
                catch
                {
                }
            }

            sockets.Clear();
            if (_heartBeatsCheckingTimer != null)
            {
                _heartBeatsCheckingTimer.Dispose();
                _heartBeatsCheckingTimer = null;
            }

            if (_loopingThread != null)
            {
                try
                {
                    _loopingThread.Abort();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
                }

                _loopingThread = null;
            }

            if (_summaryTask != null)
            {
                try
                {
                    _summaryTask.Dispose();
                }
                catch
                {
                }

                _summaryTask = null;
            }

            base.Close();


            if (socket != null)
            {
                //try
                //{
                //    socket.Shutdown(SocketShutdown.Both);
                //}
                //catch (Exception e)
                //{
                //    FConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
                //}
                try
                {
                    socket.Close();
                    socket.Dispose();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
                }

                socket = null;
            }

            foreach (SocketAsyncEventArgs eventArgs in _allEventArgsPool)
            {
                eventArgs.Dispose();
            }

            _allEventArgsPool.Clear();
            acceptEventArgsPool.Clear();
            _ioEventArgsPool.Clear();
        }
    }
}