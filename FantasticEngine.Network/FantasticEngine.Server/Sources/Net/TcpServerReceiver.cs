using System.Net.Sockets;

namespace FantasticEngine.Network.Core
{
    public sealed class TcpServerReceiver : ServerReceiver
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

            if (!ioEventArgs.AcceptSocket.ReceiveAsync(ioEventArgs))
            {
                ProcessReceive(ioEventArgs);
            }
        }

        public TcpServerReceiver(ServerSocket serverSocket) : base(serverSocket)
        {
        }
    }
}