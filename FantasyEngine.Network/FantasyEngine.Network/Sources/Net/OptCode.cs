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
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 操作码
    /// </summary>
    public enum EOpCode : sbyte
    {
        /// <summary>
        /// 空数据
        /// </summary>
        Empty = -1,
        ///// <summary>
        ///// 连续性数据
        ///// </summary>
        //Continuation = 0,
        /// <summary>
        /// 文本数据
        /// </summary>
        Text = 1,
        /// <summary>
        /// 二进制数据
        /// </summary>
        Binary = 2,
        /// <summary>
        /// 关闭操作数据
        /// </summary>
        Close = 8,
        /// <summary>
        /// Ping数据
        /// </summary>
        Ping = 9,
        /// <summary>
        /// Pong数据
        /// </summary>
        Pong = 10
    }
}
