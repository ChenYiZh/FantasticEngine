/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using FantasticEngine.Log;
using FantasticEngine.Collections;
using System;
using System.Collections.Generic;
using FantasticEngine.Network.Core;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 网络处理类
    /// </summary>
    public static class FNetwork
    {
        /// <summary>
        /// 默认使用的套接字名称
        /// </summary>
        public static string DefaultUsableSocketName { get; set; } = "default";

        /// <summary>
        /// 客户端套接字列表
        /// </summary>
        private static IDictionary<string, IClientSocket> _sockets = new ThreadSafeDictionary<string, IClientSocket>();

        /// <summary>
        /// 创建套接字
        /// </summary>
        /// <param name="name">标识名称</param>
        /// <param name="host"></param>
        /// <param name="port"></param>
        /// <param name="actionClassFullName">Action协议类的完整名称</param>
        /// <param name="heartbeatInterval">心跳间隔</param>
        public static TcpClientSocket MakeTcpSocket(string name, string host, int port,
            string actionClassFullName,
            int heartbeatInterval = Constants.HeartBeatsInterval,
            bool usePing = true, int pingInterval = Constants.PingInterval)
        {
            TcpClientSocket socket = new TcpClientSocket();
            if (!MakeSocket(name, socket))
            {
                throw new Exception("The same key: " + name + " exists in Network sockets.");
            }
            socket.Ready(name, host, port, actionClassFullName, heartbeatInterval, usePing, pingInterval);
            return socket;
        }

        /// <summary>
        /// 创建套接字
        /// </summary>
        /// <param name="name">标识名称</param>
        /// <param name="host"></param>
        /// <param name="port"></param>
        /// <param name="actionClassFullName">Action协议类的完整名称</param>
        /// <param name="heartbeatInterval">心跳间隔</param>
        public static UdpClientSocket MakeUdpSocket(string name, string host, int port,
            string actionClassFullName,
            int heartbeatInterval = Constants.HeartBeatsInterval,
            bool usePing = true, int pingInterval = Constants.PingInterval)
        {
            UdpClientSocket socket = new UdpClientSocket();
            if (!MakeSocket(name, socket))
            {
                throw new Exception("The same key: " + name + " exists in Network sockets.");
            }
            socket.Ready(name, host, port, actionClassFullName, heartbeatInterval, usePing, pingInterval);
            return socket;
        }

        /// <summary>
        /// 创建套接字
        /// </summary>
        private static bool MakeSocket(string name, IClientSocket socket)
        {
            IClientSocket exists;
            if (!_sockets.TryGetValue(name, out exists))
            {
                _sockets[name] = socket;
                return true;
            }
            if (exists.IsRunning)
            {
                return false;
            }
            _sockets[name] = socket;
            return true;
        }

        /// <summary>
        /// 获取指定的Socket
        /// </summary>
        public static IClientSocket GetSocket(string name)
        {
            IClientSocket socket;
            if (_sockets.TryGetValue(name, out socket))
            {
                return socket;
            }
            return null;
        }

        /// <summary>
        /// 使用默认套接字来发送消息
        /// <para>通过DefaultUsableSocketName来配置默认的套接字名称</para>
        /// </summary>
        public static void Send(int actionId, MessageWriter message)
        {
            Send(DefaultUsableSocketName, actionId, message);
        }

        /// <summary>
        /// 使用套接字来发送消息
        /// </summary>
        public static void Send(string socketName, int actionId, MessageWriter message)
        {
            message.ActionId = actionId;
            message.OpCode = 0;
            GetSocket(socketName)?.Send(message);
        }

        /// <summary>
        /// 关闭所有套接字连接
        /// </summary>
        public static void Shutdown()
        {
            foreach (KeyValuePair<string, IClientSocket> socket in _sockets)
            {
                try
                {
                    socket.Value.Close();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.SOCKET, e);
                }
            }
            _sockets.Clear();
        }

        /// <summary>
        /// 获取 Ping 的往返时间，最大值为Constants.MaxRoundtripTime
        /// </summary>
        /// <returns></returns>
        public static int GetRoundtripTime()
        {
            return GetRoundtripTime(DefaultUsableSocketName);
        }

        /// <summary>
        /// 获取 Ping 的往返时间，最大值为Constants.MaxRoundtripTime
        /// </summary>
        /// <returns></returns>
        public static int GetRoundtripTime(string serverName)
        {
            return GetSocket(serverName).RoundtripTime;
        }
    }
}
