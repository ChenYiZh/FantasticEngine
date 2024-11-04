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
using FantasticEngine.Network.IO;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network
{
    /// <summary>
    /// 通讯数据读取
    /// </summary>
    public interface IMessageReader : IMessageHeader
    {
        /// <summary>
        /// 读取Boolean
        /// </summary>
        /// <returns></returns>
        bool ReadBool();

        /// <summary>
        /// 读取Char
        /// </summary>
        /// <returns></returns>
        char ReadChar();

        /// <summary>
        /// 读取Float
        /// </summary>
        /// <returns></returns>
        float ReadFloat();

        /// <summary>
        /// 读取Double
        /// </summary>
        /// <returns></returns>
        double ReadDouble();

        /// <summary>
        /// 读取Decimal
        /// </summary>
        /// <returns></returns>
        decimal ReadDecimal();

        /// <summary>
        /// 读取SByte
        /// </summary>
        /// <returns></returns>
        sbyte ReadSByte();

        /// <summary>
        /// 读取Short
        /// </summary>
        /// <returns></returns>
        short ReadShort();

        /// <summary>
        /// 读取Int
        /// </summary>
        /// <returns></returns>
        int ReadInt();

        /// <summary>
        /// 读取Long
        /// </summary>
        /// <returns></returns>
        long ReadLong();

        /// <summary>
        /// 读取Byte
        /// </summary>
        /// <returns></returns>
        byte ReadByte();

        /// <summary>
        /// 读取UShort
        /// </summary>
        /// <returns></returns>
        ushort ReadUShort();

        /// <summary>
        /// 读取UInt
        /// </summary>
        /// <returns></returns>
        uint ReadUInt();

        /// <summary>
        /// 读取ULong
        /// </summary>
        /// <returns></returns>
        ulong ReadULong();

        /// <summary>
        /// 读取时间
        /// </summary>
        /// <returns></returns>
        DateTime ReadDateTime();

        /// <summary>
        /// 读取字符串
        /// </summary>
        /// <returns></returns>
        string ReadString();
    }
}
