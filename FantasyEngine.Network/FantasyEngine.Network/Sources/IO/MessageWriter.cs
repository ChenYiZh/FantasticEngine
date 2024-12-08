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
using FantasyEngine.Network.IO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasyEngine.Network
{
    /// <summary>
    /// 通讯数据构建
    /// </summary>
    public sealed class MessageWriter : MessageInfo, IMessageHeader
    {
        /// <summary>
        /// 消息Id
        /// </summary>
        public long MsgId { get; set; }

        /// <summary>
        /// 操作码
        /// </summary>
        public sbyte OpCode { get; set; } = (sbyte)EOpCode.Binary;

        /// <summary>
        /// 通讯协议Id
        /// </summary>
        public int ActionId { get; set; }

        /// <summary>
        /// 是否数据压缩
        /// </summary>
        public bool Compress { get; set; } = false;

        /// <summary>
        /// 是否加密
        /// </summary>
        public bool Secret { get; set; } = false;

        /// <summary>
        /// 是否有报错
        /// </summary>
        public bool IsError { get { return !string.IsNullOrEmpty(Error); } }

        /// <summary>
        /// 错误信息
        /// </summary>
        public string Error { get; } = null;

        /// <summary>
        /// 内容长度
        /// </summary>
        public int ContextLength { get; private set; } = 0;

        /// <summary>
        /// 包体长度
        /// </summary>
        public int GetPacketLength()
        {
            return HeaderLength + ContextLength;
        }

        /// <summary>
        /// 内容信息
        /// </summary>
        public byte[] GetContext()
        {
            return _stream.GetBuffer();
        }

        /// <summary>
        /// 缓存池
        /// </summary>
        private MemoryStream _stream = new MemoryStream();

        /// <summary>
        /// 析构函数
        /// </summary>
        ~MessageWriter()
        {
            _stream.Dispose();
        }

        /// <summary>
        /// 写入消息头数据
        /// </summary>
        public void WriteHeader(byte[] buffer, int offset)
        {
            for (int i = 0; i < offset; i++)
            {
                buffer[i] = RandomUtil.RandomByte();
            }
            int index = offset;
            byte[] data = BitConverter.GetBytes(MsgId);
            for (int i = 0; i < data.Length; i++)
            {
                buffer[index + i] = data[i];
            }
            index += data.Length;
            buffer[index] = (byte)OpCode;
            index += 1;
            data = BitConverter.GetBytes(ActionId);
            for (int i = 0; i < data.Length; i++)
            {
                buffer[index + i] = data[i];
            }
        }

        /// <summary>
        /// 写入Boolean
        /// </summary>
        public void WriteBool(bool value)
        {
            WriteByte(value ? ByteUtil.ONE : ByteUtil.ZERO);
        }

        /// <summary>
        /// 写入Byte
        /// </summary>
        public void WriteByte(byte value)
        {
            _stream.WriteByte(value);
            ContextLength += 1;
        }

        /// <summary>
        /// 写入Char
        /// </summary>
        public void WriteChar(char value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入时间
        /// </summary>
        public void WriteDateTime(DateTime value)
        {
            WriteBytes(BitConverter.GetBytes(value.Ticks));
        }

        /// <summary>
        /// 写入Decimal
        /// </summary>
        public void WriteDecimal(decimal value)
        {
            int[] bits = Decimal.GetBits(value);
            foreach (int bit in bits)
            {
                WriteInt(bit);
            }
        }

        /// <summary>
        /// 写入Double
        /// </summary>
        public void WriteDouble(double value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入Float
        /// </summary>
        public void WriteFloat(float value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入Int
        /// </summary>
        public void WriteInt(int value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入Long
        /// </summary>
        public void WriteLong(long value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入SByte
        /// </summary>
        public void WriteSByte(sbyte value)
        {
            WriteByte((byte)value);
        }

        /// <summary>
        /// 写入Short
        /// </summary>
        public void WriteShort(short value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入字符串
        /// </summary>
        public void WriteString(string value)
        {
            byte[] bytes = Encoding.UTF8.GetBytes(value);
            WriteInt(bytes.Length);
            WriteBytes(bytes);
        }

        /// <summary>
        /// 写入UInt
        /// </summary>
        public void WriteUInt(uint value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入ULong
        /// </summary>
        public void WriteULong(ulong value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }

        /// <summary>
        /// 写入UShort
        /// </summary>
        public void WriteUShort(ushort value)
        {
            WriteBytes(BitConverter.GetBytes(value));
        }
        /// <summary>
        /// 写入字节流
        /// </summary>
        /// <param name="bytes"></param>
        private void WriteBytes(byte[] bytes)
        {
            ContextLength += bytes.Length;
            _stream.Write(bytes, 0, bytes.Length);
        }
    }
}
