/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
using FantasyEngine.Log;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 数据库设置
    /// </summary>
    public class DatabaseSetting : IDatabaseSetting
    {
        /// <summary>
        /// 数据库映射名称
        /// </summary>
        public string ConnectKey { get; private set; }

        /// <summary>
        /// 数据库连接名称
        /// </summary>
        public string ProviderName { get; private set; }

        /// <summary>
        /// 连接字符串
        /// </summary>
        public string ConnectionString { get; private set; }

        /// <summary>
        /// 是什么类型的数据库
        /// </summary>
        public EDatabase Kind { get; private set; }

        /// <summary>
        /// 数据库名
        /// </summary>
        public string Database { get; private set; }
        /// <summary>
        /// 构造函数
        /// </summary>
        public DatabaseSetting(XmlNode node)
        {
            ConnectKey = node.GetValue("key", "default");
            ProviderName = node.GetValue("providerName", null);
            ConnectionString = node.GetValue("connectionString", null);
            switch (ProviderName.ToLower())
            {
                case "mysqldataprovider": Kind = EDatabase.MySQL; break;
                default: Kind = EDatabase.Unknow; break;
            }
            if (!string.IsNullOrEmpty(ConnectionString))
            {
                string[] values = ConnectionString.Split(';');
                foreach (string value in values)
                {
                    string[] data = value.Split('=');
                    if (data.Length == 2 && data[0].ToLower().Contains("database"))
                    {
                        Database = data[1].Trim();
                        break;
                    }
                }
            }
        }
    }
}
