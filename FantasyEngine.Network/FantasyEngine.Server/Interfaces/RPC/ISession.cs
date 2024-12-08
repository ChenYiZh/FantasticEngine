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

namespace FantasyEngine.Network
{
    /// <summary>
    /// Session心跳到期处理
    /// </summary>
    public delegate void OnSessionHeartbeatExpired(ISession session);

    /// <summary>
    /// 会话窗口
    /// </summary>
    public interface ISession
    {
        /// <summary>
        /// 标识符
        /// </summary>
        Guid KeyCode { get; }

        /// <summary>
        /// 会话窗口id
        /// </summary>
        string SessionId { get; }

        /// <summary>
        /// 绑定的UserId
        /// </summary>
        long UserId { get; }

        /// <summary>
        /// 远端地址
        /// </summary>
        EndPoint RemoteAddress { get; }

        /// <summary>
        /// 服务器名称
        /// </summary>
        string ServerName { get; }

        /// <summary>
        /// 自身的Socket
        /// </summary>
        IRemoteSocket Socket { get; }

        ///// <summary>
        ///// 当前的Session是否还有效
        ///// </summary>
        //bool IsValid { get; }

        /// <summary>
        /// 是否阻断当前Session
        /// </summary>
        bool Blocked { get; set; }

        /// <summary>
        /// 最近活跃时间
        /// </summary>
        DateTime ActiveTime { get; }

        /// <summary>
        /// 是否过期
        /// </summary>
        bool Expired { get; }

        /// <summary>
        /// 心跳过期
        /// </summary>
        bool HeartbeatExpired { get; }

        /// <summary>
        /// 是否已经关闭
        /// </summary>
        bool Closed { get; }

        /// <summary>
        /// 是否还连接着
        /// </summary>
        bool Connected { get; }

        /// <summary>
        /// 关闭会话窗口
        /// </summary>
        void Close();

        /// <summary>
        /// 异步发送一条数据
        /// </summary>
        void Send(int actionId, MessageWriter message);
    }
}
