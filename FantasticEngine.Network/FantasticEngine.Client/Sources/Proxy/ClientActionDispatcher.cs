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
using FantasticEngine.Reflection;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network.Proxy
{
    /// <summary>
    /// 客户端Action协议生成类
    /// </summary>
    public class ClientActionDispatcher : IClientActionDispatcher
    {
        /// <summary>
        /// Action名称的格式
        /// </summary>
        public string ActionNameFormat { get; private set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="actionNameFormat">Action名称的格式</param>
        public ClientActionDispatcher(string actionNameFormat)
        {
            ActionNameFormat = actionNameFormat;
        }

        /// <summary>
        /// 生成Action协议
        /// </summary>
        public virtual ClientAction Provide(int actionId)
        {
            string typeName = string.Format(ActionNameFormat, actionId);
            return ObjectFactory.Create<ClientAction>(typeName);
        }
    }
}
