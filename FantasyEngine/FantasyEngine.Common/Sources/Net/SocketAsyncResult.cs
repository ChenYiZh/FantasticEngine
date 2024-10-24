/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
using FantasyEngine.Log;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Net
{
    /// <summary>
    /// 操作结果
    /// </summary>
    internal enum EResultCode
    {
        /// <summary>
        /// 等待阶段
        /// </summary>
        Wait,
        /// <summary>
        /// 操作成功
        /// </summary>
        Success,
        /// <summary>
        /// Socket已经关闭
        /// </summary>
        Close,
        /// <summary>
        /// Socket 已报错
        /// </summary>
        Error
    }
    /// <summary>
    /// 套接字处理结果
    /// </summary>
    internal class SocketAsyncResult
    {
        /// <summary>
        /// 内部关联的Socket
        /// </summary>
        public ISocket Socket { get; internal set; }

        /// <summary>
        /// 需要发送的消息大小
        /// </summary>
        public byte[] Buffer { get; internal set; }

        /// <summary>
        /// 处理结果
        /// </summary>
        public EResultCode Result { get; internal set; }

        /// <summary>
        /// 报错信息
        /// </summary>
        public Exception Error { get; internal set; } = null;

        /// <summary>
        /// 数据处理完成后的回调
        /// </summary>
        public Action<SocketAsyncResult> OnCallback { get; internal set; }

        /// <summary>
        /// 消息处理完成时执行
        /// </summary>
        public void Execute()
        {
            try
            {
                OnCallback?.Invoke(this);
            }
            catch (Exception e)
            {
                FConsole.WriteExceptionWithCategory(Categories.SOCKET, e);
            }
        }
    }
}
