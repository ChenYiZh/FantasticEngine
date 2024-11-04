/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using FantasticEngine.Network.RPC;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using FantasticEngine.Network.Proxy;
using System.Threading;
using FantasticEngine.IO;
using FantasticEngine.Security;
using FantasticEngine.Proxy;
using FantasticEngine.Network.IO;
using FantasticEngine.Log;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 套接字服务器
    /// </summary>
    public abstract class SocketServer : IServer
    {
        /// <summary>
        /// 是否启动
        /// </summary>
        public bool IsRunning { get; private set; } = false;

        /// <summary>
        /// 标识名称
        /// </summary>
        public string Name { get { return Setting.Name; } }

        /// <summary>
        /// 端口号
        /// </summary>
        public int Port { get { return Setting.Port; } }

        /// <summary>
        /// 类型
        /// </summary>
        public ESocketType Type { get { return Setting.Type; } }

        /// <summary>
        /// 监听套接字
        /// </summary>
        public IServerSocket ServerSocket { get; private set; }

        /// <summary>
        /// 压缩工具
        /// </summary>
        protected ICompression Compression
        {
            get { return ServerSocket?.Compression; }
            set { ServerSocket?.SetCompression(value); }
        }

        /// <summary>
        /// 加密工具
        /// </summary>
        protected ICryptoProvider CryptoProvider
        {
            get { return ServerSocket?.CryptoProvider; }
            set { ServerSocket?.SetCryptoProvide(value); }
        }

        /// <summary>
        /// 生成Action
        /// </summary>
        protected IServerActionDispatcher ActionProvider { get; set; }

        /// <summary>
        /// 消息处理的中转站
        /// </summary>
        protected IBoss MessageContractor { get; set; }

        /// <summary>
        /// 配置文件
        /// </summary>
        public IHostSetting Setting { get; private set; }

        /// <summary>
        /// 代理服务器对象
        /// </summary>
        internal ServerProxy ServerProxy { get; set; } = null;

        /// <summary>
        /// 启动结构
        /// </summary>
        public bool Start(IHostSetting setting)
        {
            if (IsRunning) { return false; }
            Setting = setting;
            ActionProvider = new ServerActionDispatcher(setting.ActionClassFullName);
            FEConsole.WriteInfoFormatWithCategory(setting.GetCategory(), "Server is starting...", setting.Name);
            try
            {
                OnStart();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "An error occurred on start.", e);
            }
            try
            {
                if (ServerSocket != null)
                {
                    ServerSocket.Close();
                }
                ServerSocket = BuildSocket();
                ServerSocket.OnConnected += OnSocketConnected;
                ServerSocket.OnDisconnected += OnSocketDisconnected;
                ServerSocket.OnPong += OnSocketReceiveHeartbeat;
                ServerSocket.OnMessageReceived += ProcessMessage;
                ServerSocket.Start(setting);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(setting.GetCategory(), e);
                return false;
            }
            IsRunning = true;
            return true;
        }

        /// <summary>
        /// 创建服务器套接字
        /// </summary>
        internal protected abstract IServerSocket BuildSocket();

        /// <summary>
        /// 消息处理
        /// </summary>
        private void ProcessMessage(IServerSocket socket, IMessageEventArgs<IRemoteSocket> args)
        {
            try
            {
                ISession session = GameSession.Get(args.Socket?.HashCode);
                if (session != null)
                {
                    OnReceiveMessage(session, args.Message);
                    if (MessageContractor != null)
                    {
                        MessageContractor.CheckIn(new MessageWorker { Session = session, Message = args.Message, Server = this });
                    }
                    else
                    {
                        ThreadPool.QueueUserWorkItem((obj) => { ProcessMessage(session, args.Message); });
                    }
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Process message error.", e);
            }
        }

        /// <summary>
        /// 消息处理
        /// </summary>
        internal virtual void ProcessMessage(ISession session, IMessageReader message)
        {
            if (message == null)
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.SESSION, "{0} receive empty message.", session.SessionId);
                return;
            }
            try
            {
                ServerAction action = ActionProvider.Provide(message.ActionId);
                action.Session = session;
                try
                {
                    ActionBoss.Exploit(action, message.ActionId, message);
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.ACTION, "Action error.", e);
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.ACTION, "ActionProvider error.", e);
            }
        }

        /// <summary>
        /// Socket连接时执行
        /// </summary>
        private void OnSocketConnected(IServerSocket socket, IRemoteSocket remoteSocket)
        {
            try
            {
                GameSession session = (GameSession)GameSession.CreateNew(remoteSocket.HashCode, remoteSocket, socket);
                session.OnHeartbeatExpired += OnSessionHeartbeatExpired;
                OnSessionConnected(session);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
            }
        }

        private void OnSocketDisconnected(IServerSocket socket, IRemoteSocket remoteSocket)
        {
            try
            {
                ISession session = GameSession.Get(remoteSocket.HashCode);
                if (session != null)
                {
                    try
                    {
                        OnSessionDisonnected(session);
                    }
                    catch (Exception ex)
                    {
                        FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "OnSessionDisonnected error.", ex);
                    }
                    session.Close();
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
            }
        }

        private void OnSocketReceiveHeartbeat(IServerSocket socket, IMessageEventArgs<IRemoteSocket> args)
        {
            try
            {
                if (GameSession.Get(args.Socket?.HashCode) is GameSession session)
                {
                    session.Refresh();
                    OnSessionHeartbeat(session);
                    // TODO: 是否要做心跳包回复
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
            }
        }

        /// <summary>
        /// 退出
        /// </summary>
        public void Shutdown()
        {
            try
            {
                OnClose();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "An error occurred on close.", e);
            }
            //if (Setting == null)
            //{
            //    return;
            //}
            if (ServerSocket != null)
            {
                try
                {
                    ServerSocket.Close();
                    ServerSocket = null;
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), e);
                }
            }
        }

        /// <summary>
        /// 在服务器启动时执行
        /// </summary>
        protected virtual void OnStart()
        {
            if (ServerProxy != null)
            {
                ServerProxy.OnStart();
            }
        }

        /// <summary>
        /// 开始接收数据，第一部处理
        /// </summary>
        protected virtual void OnReceiveMessage(ISession session, IMessageReader message)
        {
            if (ServerProxy != null)
            {
                ServerProxy.OnReceiveMessage(session, message);
            }
        }

        /// <summary>
        /// 在客户端连接时执行
        /// </summary>
        protected virtual void OnSessionConnected(ISession session)
        {
            //在客户端连接时执行
            if (ServerProxy != null)
            {
                ServerProxy.OnSessionConnected(session);
            }
        }

        /// <summary>
        /// 在客户端断开时执行
        /// </summary>
        protected virtual void OnSessionDisonnected(ISession session)
        {
            //在客户端断开时执行
            if (ServerProxy != null)
            {
                ServerProxy.OnSessionDisonnected(session);
            }
        }

        /// <summary>
        /// 收到客户端的心跳包时执行
        /// </summary>
        protected virtual void OnSessionHeartbeat(ISession session)
        {
            //收到客户端的心跳包时执行
            if (ServerProxy != null)
            {
                ServerProxy.OnSessionHeartbeat(session);
            }
        }

        /// <summary>
        /// 在客户端心跳包过期时执行
        /// </summary>
        protected virtual void OnSessionHeartbeatExpired(ISession session)
        {
            //在客户端心跳包过期时执行
            if (ServerProxy != null)
            {
                ServerProxy.OnSessionHeartbeatExpired(session);
            }
        }

        /// <summary>
        /// 在关闭前处理
        /// </summary>
        protected virtual void OnClose()
        {
            //在关闭前处理
            if (ServerProxy != null)
            {
                ServerProxy.OnClose();
            }
        }
    }
}
