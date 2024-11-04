using FantasticEngine.Log;
using FantasticEngine.Network.Core;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace FantasticEngine.Network
{
    /// <summary>
    /// Tcp 套接字管理类
    /// </summary>
    public sealed class TcpSocket : ServerSocket
    {
        /// <summary>
        /// 创建套接字
        /// </summary>
        protected internal override void BuildSocket()
        {
            socket = new Socket(Address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            //相同端口可以重复绑定
            Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            //绑定端口
            Socket.Bind(Address);
            //设置最大挂载连接数量
            Socket.Listen(Setting.Backlog);
        }

        /// <summary>
        /// 开始接受连接
        /// </summary>
        protected internal override void OnPostAccept()
        {
            //对象池里拿结构
            SocketAsyncEventArgs acceptEventArgs = acceptEventArgsPool.Pop() ?? CreateAcceptEventArgs();
            //https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socket.acceptasync?view=net-6.0
            if (!Socket.AcceptAsync(acceptEventArgs))
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
                acceptEventArgs.RemoteEndPoint = acceptEventArgs.AcceptSocket.RemoteEndPoint;
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

                foreach (KeyValuePair<EndPoint, RemoteSocket> kv in sockets)
                {
                    RemoteSocket remoteSocket = kv.Value;
                    if (remoteSocket.EventArgs.AcceptSocket.Poll(0, SelectMode.SelectRead))
                    {
                        if (TryReceive(true))
                        {
                            Receiver.PostReceive(remoteSocket.EventArgs);
                            break;
                        }
                    }

                    if (remoteSocket.EventArgs.AcceptSocket.Poll(0, SelectMode.SelectWrite))
                    {
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