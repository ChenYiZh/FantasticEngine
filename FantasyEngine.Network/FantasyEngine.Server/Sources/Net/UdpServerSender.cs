using System.Net.Sockets;

namespace FantasyEngine.Network.Core
{
    public sealed class UdpServerSender : SocketSender
    {
        public UdpServerSender(ISocket socket) : base(socket)
        {
        }

        protected override void TrySendAsync(SocketAsyncEventArgs ioEventArgs)
        {
            if (!Socket.Socket.SendToAsync(ioEventArgs))
            {
                ProcessSend(ioEventArgs);
            }
        }
    }
}