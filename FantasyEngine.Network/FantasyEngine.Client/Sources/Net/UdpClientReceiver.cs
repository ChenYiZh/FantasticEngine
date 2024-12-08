using System.Net.Sockets;

namespace FantasyEngine.Network.Core
{
    /// <summary>
    /// 消息接收处理类
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.net.sockets.socketasynceventargs</para>
    /// </summary>
    public sealed class UdpClientReceiver : ClientReceiver
    {
        public UdpClientReceiver(IClientSocket socket) : base(socket)
        {
        }

        public override void PostReceive(SocketAsyncEventArgs ioEventArgs)
        {
            if (!Socket.TryReceive())
            {
                return;
            }
            if (!Socket.Socket.ReceiveFromAsync(ioEventArgs))
            {
                ProcessReceive(ioEventArgs);
            }
        }
    }
}