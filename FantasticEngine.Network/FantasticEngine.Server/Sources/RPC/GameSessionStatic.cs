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
using FantasticEngine.Collections;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 会话窗口
    /// </summary>
    public partial class GameSession
    {
        /// <summary>
        /// 会话窗口池
        /// </summary>
        internal static ThreadSafeDictionary<Guid, ISession> Sessions;

        /// <summary>
        /// 静态初始化
        /// </summary>
        static GameSession()
        {
            Sessions = new ThreadSafeDictionary<Guid, ISession>();
        }

        ///// <summary>
        ///// 服务器使用添加新的组别
        ///// </summary>
        ///// <param name="serverName"></param>
        //internal static void OnServerStarted(string serverName)
        //{
        //    Sessions[serverName] = new ThreadSafeDictionary<Guid, ISession>();
        //}

        ///// <summary>
        ///// 服务器使用添加新的组别
        ///// </summary>
        ///// <param name="serverName"></param>
        //internal static void CreateSessionGroup(string serverName)
        //{
        //    Sessions[serverName] = new ThreadSafeDictionary<Guid, ISession>();
        //}

        /// <summary>
        /// 创建会话窗口
        /// </summary>
        internal static ISession CreateNew(Guid keyCode, IRemoteSocket socket, IServerSocket server)
        {
            if (socket == null)
            {
                throw new ArgumentNullException("socket", "session create failed because the socket is null");
            }
            if (server == null)
            {
                throw new ArgumentNullException("server", "session create failed because the server socket is null.");
            }
            GameSession session = new GameSession(keyCode, socket, server);
            Sessions[keyCode] = session;
            return session;
        }

        /// <summary>
        /// 通过Guid获取Session
        /// </summary>
        public static ISession Get(Guid? KeyCode)
        {
            ISession session;
            return KeyCode != null && Sessions.TryGetValue(KeyCode.Value, out session) ? session : null;
        }

        /// <summary>
        /// 异步给一堆客户端发消息
        /// </summary>
        /// <param name="actionId">协议id</param>
        /// <param name="message">消息</param>
        /// <param name="sessions">目标Session</param>
        public static void Send(int actionId, MessageWriter message, IEnumerable<GameSession> sessions)
        {
            message.ActionId = actionId;
            foreach (GameSession session in sessions)
            {
                session.Send(actionId, message);
            }
        }

        /// <summary>
        /// 获取同服的session
        /// </summary>
        /// <param name="session"></param>
        public static IReadOnlyList<ISession> GetSessions(ISession session)
        {
            List<ISession> sessions = new List<ISession>();
            lock (Sessions.SyncRoot)
            {
                foreach (ISession ses in Sessions.Values)
                {
                    if (ses.ServerName == session.ServerName)
                    {
                        sessions.Add(ses);
                    }
                }
            }
            return sessions;
        }
    }
}
