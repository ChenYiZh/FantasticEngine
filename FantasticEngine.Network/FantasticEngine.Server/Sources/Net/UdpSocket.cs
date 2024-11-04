using FantasticEngine.Log;
using FantasticEngine.Network.Core;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace FantasticEngine.Network
{
    /// <summary>
    /// Udp 套接字管理类
    /// </summary>
    public sealed class UdpSocket : ServerSocket
    {
        /// <summary>
        /// 初始化缓冲区大小
        /// </summary>
        private const int BUFFER_SIZE = 8192;

        /// <summary>
        /// 握手标示
        /// </summary>
        private const string ACCEPT_FLAG = "Author ChenYiZh";

        /// <summary>
        /// 创建套接字
        /// </summary>
        protected internal override void BuildSocket()
        {
            socket = new Socket(Address.AddressFamily, SocketType.Dgram, ProtocolType.Udp);
            //相同端口可以重复绑定
            Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            //绑定端口
            Socket.Bind(Address);
        }

        /// <summary>
        /// 开始接受连接
        /// </summary>
        protected internal override void OnPostAccept()
        {
            return;
            //对象池里拿结构
            SocketAsyncEventArgs acceptEventArgs = acceptEventArgsPool.Pop() ?? CreateAcceptEventArgs();
            acceptEventArgs.SetBuffer(new byte[BUFFER_SIZE], 0, BUFFER_SIZE);
            acceptEventArgs.RemoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
            if (!Socket.ReceiveFromAsync(acceptEventArgs))
            {
                ProcessAccept(acceptEventArgs);
            }
        }

        /// <summary>
        /// 收到连接时需要做的事情
        /// </summary>
        protected internal override void AcceptCompleted(object sender, SocketAsyncEventArgs acceptEventArgs)
        {
            try
            {
                if (acceptEventArgs.RemoteEndPoint == null || acceptEventArgs.BytesTransferred == 0)
                {
                    throw new NullReferenceException("RemoteEndPoint is none or bytes transferred is 0.");
                }

                byte[] buffer = new byte[acceptEventArgs.BytesTransferred];
                Buffer.BlockCopy(acceptEventArgs.Buffer, 0, buffer, 0, buffer.Length);
                if (Encoding.UTF8.GetString(buffer) != ACCEPT_FLAG)
                {
                    throw new AccessViolationException("The accept message is wrong.");
                }

                //acceptEventArgs.AcceptSocket = sender as Socket;
                ProcessAccept(acceptEventArgs);
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Setting.GetCategory(), "Accept Completed method error:", e);
                if (acceptEventArgs.AcceptSocket != null)
                {
                    try
                    {
                        acceptEventArgs.AcceptSocket.Close();
                    }
                    catch
                    {
                    }

                    acceptEventArgs.AcceptSocket = null;
                }

                ReleaseAccept(acceptEventArgs);
            }
            finally
            {
                PostAccept();
            }
        }

        protected internal override void Looping(object state)
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
                }

                if (Socket.Poll(0, SelectMode.SelectWrite))
                {
                    foreach (KeyValuePair<EndPoint, RemoteSocket> kv in sockets)
                    {
                        RemoteSocket remoteSocket = kv.Value;
                        if (remoteSocket.UserToken.HasMsg() && TrySend(true))
                        {
                            Sender.PostSend(remoteSocket.EventArgs);
                            break;
                        }
                    }
                }
            }
        }
    }
}