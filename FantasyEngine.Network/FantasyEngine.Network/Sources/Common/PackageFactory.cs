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
using FantasyEngine.IO;
using FantasyEngine.Security;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Text;

namespace FantasyEngine.Network.IO
{
    /// <summary>
    /// 消息类型
    /// </summary>
    internal enum EMessageType : byte
    {
        /// <summary>
        /// 什么都不处理
        /// </summary>
        NoProcess = 0,
        /// <summary>
        /// 压缩过
        /// </summary>
        OnlyCompress = 10,
        /// <summary>
        /// 加密过
        /// </summary>
        OnlyCrypto = 15,
        /// <summary>
        /// 解压+加密
        /// </summary>
        CompressAndCrypto = 50,
    }
    /// <summary>
    /// 消息处理类
    /// </summary>
    public static class PackageFactory
    {
        /// <summary>
        /// 打包
        /// </summary>
        public static byte[] Pack(MessageWriter message, int offset, ICompression compression, ICryptoProvider cryptography)
        {
            byte[] context = message.GetContext();
            byte[] buffer = new byte[MessageInfo.HeaderLength + message.ContextLength];
            message.WriteHeader(buffer, 0);
            Buffer.BlockCopy(context, 0, buffer, MessageInfo.HeaderLength, message.ContextLength);
            bool compress = message.Compress && compression != null;
            bool crypto = message.Secret && cryptography != null;
            EMessageType type = EMessageType.NoProcess;
            if (compress && crypto)
            {
                type = EMessageType.CompressAndCrypto;
            }
            else if (compress)
            {
                type = EMessageType.OnlyCompress;
            }
            else if (crypto)
            {
                type = EMessageType.OnlyCrypto;
            }
            if (crypto)
            {
                buffer = cryptography.Encrypt(buffer);
            }
            if (compress)
            {
                buffer = compression.Compress(buffer);
            }

            int length = buffer.Length;
            byte[] result = new byte[offset + SizeUtil.IntSize + 1 + length];
            for (int i = 0; i < offset; i++)
            {
                result[i] = RandomUtil.RandomByte();
            }
            byte[] lenBs = BitConverter.GetBytes(length);
            for (int i = 0; i < lenBs.Length; i++)
            {
                result[offset + i] = lenBs[i];
            }
            result[offset + SizeUtil.IntSize] = (byte)type;
            Buffer.BlockCopy(buffer, 0, result, result.Length - length, buffer.Length);

            return result;
        }

        /// <summary>
        /// 解包
        /// </summary>
        public static IMessageReader Unpack(byte[] package, int offset, ICompression compression, ICryptoProvider cryptography)
        {
            int pos = 0;

            pos += offset;
            int length = BitConverter.ToInt32(package, pos);
            pos += SizeUtil.IntSize;
            EMessageType type = (EMessageType)package[pos];
            pos += 1;

            byte[] data = new byte[length];
            Buffer.BlockCopy(package, pos, data, 0, length);
            pos += length;

            bool compress = type == EMessageType.OnlyCompress || type == EMessageType.CompressAndCrypto;
            bool crypto = type == EMessageType.OnlyCrypto || type == EMessageType.CompressAndCrypto;
            if (compress)
            {
                data = compression.Uncompress(data);
            }
            if (crypto)
            {
                data = cryptography.Decrypt(data);
            }
            return new MessageReader(data, 0, compress, crypto);
        }

        /// <summary>
        /// 预解析整体包体大小
        /// </summary>
        public static int GetTotalLength(byte[] package, int offset)
        {
            int headerLength = offset + SizeUtil.IntSize + 1;
            if (package.Length < headerLength)
            {
                return -1;
            }
            int length = BitConverter.ToInt32(package, offset);
            //偏移值，包体大小，消息格式
            return headerLength + length;
        }
    }
}
