/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
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

namespace FantasticEngine.Network.IO
{
    /// <summary>
    /// 消息构造接口
    /// </summary>
    public interface IMessageHeader
    {
        /// <summary>
        /// 消息Id
        /// </summary>
        long MsgId { get; }

        /// <summary>
        /// 操作码
        /// </summary>
        sbyte OpCode { get; }

        /// <summary>
        /// 通讯协议Id
        /// </summary>
        int ActionId { get; }

        /// <summary>
        /// 是否数据压缩
        /// </summary>
        bool Compress { get; }

        /// <summary>
        /// 是否加密
        /// </summary>
        bool Secret { get; }

        /// <summary>
        /// 是否有报错
        /// </summary>
        bool IsError { get; }

        /// <summary>
        /// 错误信息
        /// </summary>
        string Error { get; }

        /// <summary>
        /// 包体长度
        /// </summary>
        int GetPacketLength();

        /// <summary>
        /// 内容信息
        /// </summary>
        byte[] GetContext();

        /// <summary>
        /// 内容长度
        /// </summary>
        int ContextLength { get; }
    }
}
