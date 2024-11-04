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

using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;

namespace FantasticEngine.Network
{
    /// <summary>
    /// Tcp连接池
    /// </summary>
    public class UdpClientSocket : ClientSocket
    {
        /// <summary>
        /// 初始化
        /// </summary>
        public UdpClientSocket() : base()
        {
        }

        /// <summary>
        /// 类型
        /// </summary>
        public override ESocketType Type
        {
            get { return ESocketType.Udp; }
        }

        /// <summary>
        /// 等待握手
        /// </summary>
        private bool _waitToAccepted = false;

        /// <summary>
        /// 握手标示
        /// </summary>
        private const string ACCEPT_FLAG = "Author ChenYiZh";

        /// <summary>
        /// 建立原生套接字
        /// </summary>
        /// <returns></returns>
        protected override Socket MakeSocket()
        {
            return new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        }

        /// <summary>
        /// 创建连接
        /// </summary>
        protected internal override void BeginConnectImpl()
        {
            if (_waitToAccepted)
            {
                return;
            }

            _waitToAccepted = true;

            //EventArgs.Completed += new EventHandler<SocketAsyncEventArgs>(MessageSolved);
            IAsyncResult opt = Socket.BeginConnect(Address, null, EventArgs);
            EventArgs.RemoteEndPoint = Address;
            bool success = opt.AsyncWaitHandle.WaitOne(1000, true);
            if (!success || !opt.IsCompleted || !Socket.Connected)
            {
                IsRunning = false;
                throw new Exception(string.Format("Socket connect failed!"));
            }

            byte[] helloWords = Encoding.UTF8.GetBytes(ACCEPT_FLAG);
            Sender.Push(this, helloWords, true);
        }

    }
}