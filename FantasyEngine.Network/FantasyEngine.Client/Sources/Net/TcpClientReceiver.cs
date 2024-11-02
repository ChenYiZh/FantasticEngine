using System.Net.Sockets;

namespace FantasyEngine.Network.Core
{
    /// <summary>
    /// 消息接收处理类
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs</para>
    /// </summary>
    public sealed class TcpClientReceiver : ClientReceiver
    {
        public TcpClientReceiver(IClientSocket socket) : base(socket)
        {
        }

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

            if (!Socket.Socket.ReceiveAsync(ioEventArgs))
            {
                ProcessReceive(ioEventArgs);
            }
        }
    }
}