using System.Net;
using System.Net.Sockets;

namespace FantasyEngine.Network.Core
{
    public sealed class UdpServerReceiver : ServerReceiver
    {
        /// <summary>
        /// 投递接收数据请求
        /// </summary>
        /// <param name="ioEventArgs"></param>
        public override void PostReceive(SocketAsyncEventArgs ioEventArgs)
        {
            if (!Socket.TryReceive())
            {
                return;
            }

            ioEventArgs.RemoteEndPoint = new IPEndPoint(IPAddress.Any, ((ServerSocket) Socket).Port);
            if (!Socket.Socket.ReceiveFromAsync(ioEventArgs))
            {
                ProcessReceive(ioEventArgs);
            }
        }

        /// <summary>
        /// 关闭操作
        /// </summary>
        protected override void Close(SocketAsyncEventArgs ioEventArgs, EOpCode opCode)
        {
            base.Close(ioEventArgs, opCode);
            if (ioEventArgs != null)
            {
                ioEventArgs.AcceptSocket = null;
            }
        }

        public override void ProcessReceive(SocketAsyncEventArgs ioEventArgs)
        {
            RemoteSocket socket;
            if (((ServerSocket) Socket).sockets.TryGetValue(ioEventArgs.RemoteEndPoint, out socket))
            {
                ioEventArgs.UserToken = socket.UserToken;
            }
            else
            {
                ioEventArgs.UserToken = null;
                ((ServerSocket) Socket).AcceptCompleted(Socket.Socket, ioEventArgs);
                Socket.OperationCompleted();
                return;
            }

            base.ProcessReceive(ioEventArgs);
        }

        public UdpServerReceiver(ServerSocket serverSocket) : base(serverSocket)
        {
        }
    }
}