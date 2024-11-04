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
    /// 通讯包解析
    /// </summary>
    public class MessageReader : MessageInfo, IMessageHeader, IMessageReader
    {
        /// <summary>
        /// 消息Id
        /// </summary>
        public long MsgId { get; private set; }

        /// <summary>
        /// 操作码
        /// </summary>
        public sbyte OpCode { get; private set; }

        /// <summary>
        /// 通讯协议Id
        /// </summary>
        public int ActionId { get; private set; }

        /// <summary>
        /// 是否数据压缩
        /// </summary>
        public bool Compress { get; private set; }

        /// <summary>
        /// 是否加密
        /// </summary>
        public bool Secret { get; private set; }

        /// <summary>
        /// 是否有报错
        /// </summary>
        public bool IsError { get { return !string.IsNullOrEmpty(Error); } }

        /// <summary>
        /// 错误信息
        /// </summary>
        public string Error { get; private set; }

        /// <summary>
        /// 内容长度
        /// </summary>
        public int ContextLength { get; private set; } = 0;

        /// <summary>
        /// 读取的数据指针
        /// </summary>
        private int _readIndex = 0;

        /// <summary>
        /// 通信包内容
        /// </summary>
        private byte[] _context = null;

        /// <summary>
        /// 内容信息
        /// </summary>
        public byte[] GetContext()
        {
            return _context;
        }

        /// <summary>
        /// 包体长度
        /// </summary>
        private int _packetLength = 0;

        /// <summary>
        /// 包体长度
        /// </summary>
        public int GetPacketLength()
        {
            return _packetLength;
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="package"></param>
        /// <param name="offset"></param>
        public MessageReader(byte[] package, int offset, bool compress, bool secret)
        {
            Compress = compress;
            Secret = secret;
            _packetLength = package.Length - offset;
            if (_packetLength < HeaderLength)
            {
                Error = "The message's length is error.";
                return;
            }

            //读取报头
            ReadHeader(package, offset);

            //内容获取
            ContextLength = _packetLength - HeaderLength;
            _context = new byte[ContextLength];
            Buffer.BlockCopy(package, offset + HeaderLength, _context, 0, ContextLength);
        }

        /// <summary>
        /// 消息的基本数据读取
        /// </summary>
        protected virtual void ReadHeader(byte[] package, int offset)
        {
            int index = offset;
            MsgId = BitConverter.ToInt64(package, index);
            index += SizeUtil.LongSize;
            OpCode = (sbyte)package[index];
            index += 1;
            ActionId = BitConverter.ToInt32(package, index);
        }

        /// <summary>
        /// 读取Boolean
        /// </summary>
        /// <returns></returns>
        public bool ReadBool()
        {
            return ReadByte() == ByteUtil.ONE;
        }

        /// <summary>
        /// 读取Byte
        /// </summary>
        /// <returns></returns>
        public byte ReadByte()
        {
            byte b = _context[_readIndex];
            _readIndex += 1;
            return b;
        }

        /// <summary>
        /// 读取Char
        /// </summary>
        /// <returns></returns>
        public char ReadChar()
        {
            char c = BitConverter.ToChar(_context, _readIndex);
            _readIndex += SizeUtil.CharSize;
            return c;
        }

        /// <summary>
        /// 读取时间
        /// </summary>
        /// <returns></returns>
        public DateTime ReadDateTime()
        {
            return new DateTime(ReadLong());
        }

        /// <summary>
        /// 读取Decimal
        /// </summary>
        /// <returns></returns>
        public decimal ReadDecimal()
        {
            int[] bits = new int[4];
            for (int i = 0; i < bits.Length; i++)
            {
                bits[i] = ReadInt();
            }
            return new decimal(bits);
        }

        /// <summary>
        /// 读取Double
        /// </summary>
        /// <returns></returns>
        public double ReadDouble()
        {
            double d = BitConverter.ToDouble(_context, _readIndex);
            _readIndex += SizeUtil.DoubleSize;
            return d;
        }

        /// <summary>
        /// 读取Float
        /// </summary>
        /// <returns></returns>
        public float ReadFloat()
        {
            float f = BitConverter.ToSingle(_context, _readIndex);
            _readIndex += SizeUtil.FloatSize;
            return f;
        }

        /// <summary>
        /// 读取Int
        /// </summary>
        /// <returns></returns>
        public int ReadInt()
        {
            int i = BitConverter.ToInt32(_context, _readIndex);
            _readIndex += SizeUtil.IntSize;
            return i;
        }

        /// <summary>
        /// 读取Long
        /// </summary>
        /// <returns></returns>
        public long ReadLong()
        {
            long l = BitConverter.ToInt64(_context, _readIndex);
            _readIndex += SizeUtil.LongSize;
            return l;
        }

        /// <summary>
        /// 读取SByte
        /// </summary>
        /// <returns></returns>
        public sbyte ReadSByte()
        {
            return (sbyte)ReadByte();
        }

        /// <summary>
        /// 读取Short
        /// </summary>
        /// <returns></returns>
        public short ReadShort()
        {
            short s = BitConverter.ToInt16(_context, _readIndex);
            _readIndex += SizeUtil.ShortSize;
            return s;
        }

        /// <summary>
        /// 读取字符串
        /// </summary>
        /// <returns></returns>
        public string ReadString()
        {
            int length = ReadInt();
            string str = Encoding.UTF8.GetString(_context, _readIndex, length);
            _readIndex += length;
            return str;
        }

        /// <summary>
        /// 读取UInt
        /// </summary>
        /// <returns></returns>
        public uint ReadUInt()
        {
            uint ui = BitConverter.ToUInt32(_context, _readIndex);
            _readIndex += SizeUtil.UIntSize;
            return ui;
        }

        /// <summary>
        /// 读取ULong
        /// </summary>
        /// <returns></returns>
        public ulong ReadULong()
        {
            ulong ul = BitConverter.ToUInt64(_context, _readIndex);
            _readIndex += SizeUtil.ULongSize;
            return ul;
        }

        /// <summary>
        /// 读取UShort
        /// </summary>
        /// <returns></returns>
        public ushort ReadUShort()
        {
            ushort us = BitConverter.ToUInt16(_context, _readIndex);
            _readIndex += SizeUtil.UShortSize;
            return us;
        }
    }
}
