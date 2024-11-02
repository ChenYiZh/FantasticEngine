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
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using FantasyEngine.Log;
using FantasyEngine.Network.Core;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 会话窗口
    /// </summary>
    public partial class GameSession : ISession
    {
        /// <summary>
        /// 标识符
        /// </summary>
        public Guid KeyCode { get; private set; }

        /// <summary>
        /// 会话窗口id
        /// </summary>
        public string SessionId { get; private set; }

        /// <summary>
        /// 绑定的UserId
        /// </summary>
        public long UserId { get; set; }

        /// <summary>
        /// 远端地址
        /// </summary>
        public EndPoint RemoteAddress { get { return Socket?.Address; } }

        /// <summary>
        /// 自身的Socket
        /// </summary>
        public IRemoteSocket Socket { get; private set; }

        /// <summary>
        /// 服务器名称
        /// </summary>
        public string ServerName { get { return Server?.ServerName; } }

        /// <summary>
        /// 所属服务器
        /// </summary>
        private IServerSocket Server { get; set; }

        /// <summary>
        /// 当前的Session是否还有效
        /// </summary>
        public bool IsValid { get { return Connected && !Closed && !Expired && !HeartbeatExpired; } }

        /// <summary>
        /// 是否阻断当前Session
        /// </summary>
        public bool Blocked { get; set; }

        /// <summary>
        /// 最近活跃时间
        /// </summary>
        public DateTime ActiveTime { get; private set; }

        /// <summary>
        /// 是否过期
        /// </summary>
        public bool Expired { get; private set; }

        /// <summary>
        /// 心跳过期
        /// </summary>
        public bool HeartbeatExpired { get; private set; }

        /// <summary>
        /// 是否已经关闭
        /// </summary>
        public bool Closed { get { return Socket == null || !Socket.IsRunning; } }

        /// <summary>
        /// 是否还连接着
        /// </summary>
        public bool Connected { get { return Socket != null && Socket.Socket != null && Socket.Socket.Connected; } }

        /// <summary>
        /// 心跳到期
        /// </summary>
        internal event OnSessionHeartbeatExpired OnHeartbeatExpired;

        /// <summary>
        /// 内部构造函数
        /// </summary>
        private GameSession(Guid keyCode, IRemoteSocket socket, IServerSocket server)
        {
            KeyCode = keyCode;
            Socket = socket;
            Server = server;
            SessionId = GenerateSessionId();
        }

        /// <summary>
        /// 生成会话窗口名称
        /// </summary>
        /// <returns></returns>
        private string GenerateSessionId()
        {
            return string.Format("{0}_{1}", ServerName, KeyCode.ToString("N"));
        }

        /// <summary>
        /// 心跳到期
        /// </summary>
        private void HeartbeatTimeout()
        {
            try
            {
                HeartbeatExpired = true;
                OnHeartbeatExpired?.Invoke(this);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.SESSION, "Heartbeat expired process error.", e);
            }
        }

        /// <summary>
        /// 刷新信息
        /// </summary>
        internal void Refresh()
        {
            Expired = false;
            HeartbeatExpired = false;
            ActiveTime = TimeLord.Now;
        }

        /// <summary>
        /// 关闭Session
        /// </summary>
        public void Close()
        {
            ISession session;
            if (Sessions.TryGetValue(KeyCode, out session) && session.Socket != null)
            {
                //设置Socket为Closed的状态, 并未将物理连接马上中断
                ((RemoteSocket)session.Socket).SetIsRunning(false);
            }
        }
        /// <summary>
        /// 异步发送一条数据
        /// </summary>
        public void Send(int actionId, MessageWriter message)
        {
            message.ActionId = actionId;
            message.OpCode = 0;
            try
            {
                Socket.Send(message);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.SOCKET, "GameSession failed to send message.", e);
            }
        }
    }
}
