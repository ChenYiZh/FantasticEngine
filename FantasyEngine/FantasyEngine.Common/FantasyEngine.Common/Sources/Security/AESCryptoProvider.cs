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
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace FantasyEngine.Security
{
    /// <summary>
    /// AES加密
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.security.cryptography.aescryptoserviceprovider?view=netstandard-2.0</para>
    /// </summary>
    public class AESCryptoProvider : ICryptoProvider
    {
        private const int KEY_LENGTH = 16;// TODO: 是否需要长度限制？

        private byte[] _key = new byte[KEY_LENGTH];

        private byte[] _iv = new byte[KEY_LENGTH];

        public string Key
        {
            set
            {
                if (string.IsNullOrEmpty(value))
                {
                    throw new ArgumentException("The key of AESCryptoProvider is null.");
                }
                byte[] bits = Encoding.UTF8.GetBytes(value);
                Buffer.BlockCopy(bits, 0, _key, 0, bits.Length < KEY_LENGTH ? bits.Length : KEY_LENGTH);
            }
        }

        public string IV
        {
            set
            {
                if (string.IsNullOrEmpty(value))
                {
                    throw new ArgumentException("The iv of AESCryptoProvider is null.");
                }
                byte[] bits = Encoding.UTF8.GetBytes(value);
                Buffer.BlockCopy(bits, 0, _iv, 0, bits.Length < KEY_LENGTH ? bits.Length : KEY_LENGTH);
            }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="key"></param>
        /// <param name="iv"></param>
        public AESCryptoProvider(string key, string iv)
        {
            Key = key;
            IV = iv;
        }

        /// <summary>
        /// 加密
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public byte[] Encrypt(byte[] data)
        {
            //using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            using (Aes aes = Aes.Create())
            {
                aes.Key = _key;
                aes.IV = _iv;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.Zeros;
                using (MemoryStream stream = new MemoryStream())
                {
                    //using (CryptoStream crypto = new CryptoStream(stream, aes.CreateEncryptor(), CryptoStreamMode.Write))
                    using (CryptoStream crypto = new CryptoStream(stream, aes.CreateEncryptor(aes.Key, aes.IV), CryptoStreamMode.Write))
                    {
                        crypto.Write(data, 0, data.Length);
                    }
                    return stream.GetBuffer();
                }
            }
        }

        /// <summary>
        /// 解密
        /// </summary>
        /// <param name="buffer"></param>
        /// <returns></returns>
        public byte[] Decrypt(byte[] buffer)
        {
            //using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            using (Aes aes = Aes.Create())
            {
                aes.Key = _key;
                aes.IV = _iv;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.Zeros;
                using (MemoryStream stream = new MemoryStream())
                {
                    using (MemoryStream ms = new MemoryStream(buffer))
                    {
                        //using (CryptoStream crypto = new CryptoStream(ms, aes.CreateDecryptor(), CryptoStreamMode.Read))
                        using (CryptoStream crypto = new CryptoStream(ms, aes.CreateDecryptor(aes.Key, aes.IV), CryptoStreamMode.Read))
                        {
                            byte[] reader = new byte[1024];
                            int length;
                            while ((length = crypto.Read(reader, 0, reader.Length)) > 0)
                            {
                                stream.Write(reader, 0, length);
                            }
                        }
                    }
                    return stream.GetBuffer();
                }
            }
        }
    }
}
