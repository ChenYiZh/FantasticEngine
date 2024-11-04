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
using FantasticEngine.Network.Core;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 服务器套接字接口定义
    /// </summary>
    public interface IServerSocket : ISocket, IMsgSocket
    {
        /// <summary>
        /// 对应Host的名称
        /// </summary>
        string ServerName { get; }

        /// <summary>
        /// 绑定的端口
        /// </summary>
        int Port { get; }

        /// <summary>
        /// 获取配置信息
        /// </summary>
        IHostSetting Setting { get; }

        /// <summary>
        /// 连接事件
        /// </summary>
        event ConnectionEventHandler OnConnected;

        /// <summary>
        /// 握手事件
        /// </summary>
        event ConnectionEventHandler OnHandshaked;

        /// <summary>
        /// 断开连接事件
        /// </summary>
        event ConnectionEventHandler OnDisconnected;

        /// <summary>
        /// 接收到数据包事件
        /// </summary>
        event MessageEventHandler OnMessageReceived;

        /// <summary>
        /// 心跳探索事件
        /// </summary>
        event MessageEventHandler OnPing;

        /// <summary>
        /// 心跳回应事件
        /// </summary>
        event MessageEventHandler OnPong;

        /// <summary>
        /// 启动函数
        /// </summary>
        void Start(IHostSetting setting);

        /// <summary>
        /// 当远端连接关闭时，执行一些回收代码
        /// </summary>
        void OnRemoteSocketClosed(IRemoteSocket socket, EOpCode opCode);
    }
}
