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
using System.Text;
using System.Xml;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// Redis设置信息
    /// </summary>
    public class RedisSetting : IRedisSetting
    {
        /// <summary>
        /// Redis链接地址
        /// </summary>
        public string Host { get; private set; }
        /// <summary>
        /// Redis端口
        /// </summary>
        public int Port { get; private set; }
        /// <summary>
        /// 连接密码
        /// </summary>
        public string Password { get; private set; }
        /// <summary>
        /// DbIndex
        /// </summary>
        public int DbIndex { get; private set; }
        /// <summary>
        /// 连接Timeout
        /// </summary>
        public int Timeout { get; private set; }
        /// <summary>
        /// 数据是否有效
        /// </summary>
        public bool IsValid { get; private set; }

        public RedisSetting(XmlNode node)
        {
            IsValid = false;
            if (node == null)
            {
                return;
            }
            Host = node.GetValue("host", "127.0.0.1");
            Port = node.GetValue("port", 6379);
            Password = node.GetValue("password", null);
            DbIndex = node.GetValue("db", 0);
            Timeout = node.SelectSingleNode("timeout").GetValue(5000);
            HostCheck();
            IsValid = true;
        }

        private void HostCheck()
        {
            string[] values = Host.Split('@', ':');
            if (values.Length == 3)
            {
                Password = values[0];
                Host = values[1];
                Port = int.Parse(values[2]);
            }
            else if (values.Length == 2)
            {
                if (Host.Contains("@"))
                {
                    Password = values[0];
                    Host = values[1];
                }
                else
                {
                    Host = values[0];
                    Port = int.Parse(values[2]);
                }
            }
        }
    }
}
