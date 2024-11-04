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
using FantasticEngine.Collections;
using FantasticEngine.Log;
using System;
using System.Collections.Generic;
using System.Text;
using FantasticEngine.Reflection;

namespace FantasticEngine.Network.RPC
{
    /// <summary>
    /// 服务器管理类
    /// </summary>
    public class ServerManager
    {
        private static IDictionary<string, IServer> hosts;

        static ServerManager()
        {
            hosts = new ThreadSafeDictionary<string, IServer>();
        }
        /// <summary>
        /// 开启一个服务器
        /// </summary>
        /// <param name="hostType"></param>
        /// <returns></returns>
        public static bool Start(IHostSetting setting)
        {
            if (!CheckSetting(setting))
            {
                return false;
            }
            IServer host = CreateServer(setting);
            try
            {
                if (host.Start(setting))
                {
                    hosts.Add(host.Name, host);
                    return true;
                }
                return false;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.HOST, e);
                return false;
            }
        }
        /// <summary>
        /// 关闭指定名称的服务器
        /// </summary>
        /// <param name="hostName"></param>
        /// <returns></returns>
        public static bool Shutdown(string hostName)
        {
            if (hosts.ContainsKey(hostName))
            {
                IServer host = hosts[hostName];
                try
                {
                    host.Shutdown();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.HOST, e);
                }
            }
            return hosts.Remove(hostName);
        }
        /// <summary>
        /// 关闭一个服务器
        /// </summary>
        /// <param name="host"></param>
        /// <returns></returns>
        public static bool Shutdown(IServer host)
        {
            try
            {
                host.Shutdown();
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.HOST, e);
            }
            return hosts.Remove(host.Name);
        }
        /// <summary>
        /// 关闭所有服务器
        /// </summary>
        public static void Shutdown()
        {
            foreach (KeyValuePair<string, IServer> kv in hosts)
            {
                IServer host = kv.Value;
                try
                {
                    host.Shutdown();
                }
                catch (Exception e)
                {
                    FEConsole.WriteExceptionWithCategory(Categories.HOST, e);
                }
            }
            hosts.Clear();
        }
        /// <summary>
        /// 创建Host
        /// </summary>
        /// <param name="setting"></param>
        /// <returns></returns>
        private static IServer CreateServer(IHostSetting setting)
        {
            //return new TcpServer();
            IServer host = null;
            ServerProxy serverProxy = null;
            if (!string.IsNullOrEmpty(setting.ClassFullname))
            {
                serverProxy = ObjectFactory.Create<ServerProxy>(setting.ClassFullname);
            }
            switch (setting.Type)
            {
                case ESocketType.Tcp: host = serverProxy != null || string.IsNullOrEmpty(setting.ClassFullname) ? new TcpServer() : ObjectFactory.Create<TcpServer>(setting.ClassFullname); break;
                case ESocketType.Udp: host = serverProxy != null || string.IsNullOrEmpty(setting.ClassFullname) ? new UdpServer() : ObjectFactory.Create<UdpServer>(setting.ClassFullname); break;
            }
            if (host != null && serverProxy != null)
            {
                ((SocketServer)host).ServerProxy = serverProxy;
                serverProxy.Server = host;
            }
            return host;
        }
        /// <summary>
        /// 判断这个配置是否可用
        /// </summary>
        /// <param name="setting"></param>
        /// <returns></returns>
        public static bool CheckSetting(IHostSetting setting)
        {
            if (string.IsNullOrWhiteSpace(setting.Name))
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.HOST, "Fail to start the host, because the name of it is empty.");
                return false;
            }
            if (hosts.ContainsKey(setting.Name))
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.HOST, "The same server: {0} is running!", setting.Name);
                return false;
            }
            foreach (IServer host in hosts.Values)
            {
                if (host.Port == setting.Port)
                {
                    FEConsole.WriteErrorFormatWithCategory(Categories.HOST, "The port of the server: {0}:{1} has been used!", setting.Name, setting.Port);
                    return false;
                }
            }
            return true;
        }
    }
}
