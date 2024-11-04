using System;
using System.Collections.Generic;
using System.Text;
using FantasticEngine.Network.RPC;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 通用基本服务器类
    /// </summary>
    public class ServerProxy
    {
        /// <summary>
        /// 内部使用的服务器
        /// </summary>
        public IServer Server { get; internal set; }

        /// <summary>
        /// 在服务器启动时执行
        /// </summary>
        public virtual void OnStart()
        {
        }

        /// <summary>
        /// 开始接收数据，第一部处理
        /// </summary>
        public virtual void OnReceiveMessage(ISession session, IMessageReader message)
        {
        }

        /// <summary>
        /// 在客户端连接时执行
        /// </summary>
        public virtual void OnSessionConnected(ISession session)
        {
            //在客户端连接时执行
            //FConsole.WriteFormat("Hello {0}!", session.SessionId);
        }

        /// <summary>
        /// 在客户端断开时执行
        /// </summary>
        public virtual void OnSessionDisonnected(ISession session)
        {
            //在客户端断开时执行
            //FConsole.WriteFormat("Bye {0}!", session.SessionId);
        }

        /// <summary>
        /// 收到客户端的心跳包时执行
        /// </summary>
        public virtual void OnSessionHeartbeat(ISession session)
        {
            //收到客户端的心跳包时执行
            //FConsole.WriteFormat("Beat {0}!", session.SessionId);
        }

        /// <summary>
        /// 在客户端心跳包过期时执行
        /// </summary>
        public virtual void OnSessionHeartbeatExpired(ISession session)
        {
            //在客户端心跳包过期时执行
        }

        /// <summary>
        /// 在关闭前处理
        /// </summary>
        public virtual void OnClose()
        {
            //在关闭前处理
        }
    }
}
