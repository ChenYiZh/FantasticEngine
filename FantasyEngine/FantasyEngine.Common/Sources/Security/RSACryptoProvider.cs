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
using System.Security.Cryptography;
using System.Text;

namespace FantasyEngine.Security
{
    /// <summary>
    /// RSA加密类
    /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/system.security.cryptography.rsacryptoserviceprovider?view=netstandard-2.0</para>
    /// </summary>
    [Obsolete("代码有问题，密钥有问题")]
    public class RSACryptoProvider : ICryptoProvider
    {
        /// <summary>
        /// 非对称性填充
        /// </summary>
        public bool OAEP { get; set; }

        /// <summary>
        /// 密钥
        /// </summary>
        public CspParameters RSAKeyInfo { get; set; }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="key">密钥</param>
        /// <param name="oaep">非对称性填充</param>
        public RSACryptoProvider(string key, bool oaep = true)
        {
            if (!string.IsNullOrEmpty(key))
            {
                RSAKeyInfo = new CspParameters();
                RSAKeyInfo.KeyContainerName = key;
            }
            OAEP = oaep;
        }

        /// <summary>
        /// 加密
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public byte[] Encrypt(byte[] data)
        {
            if (RSAKeyInfo == null)
            {
                using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
                {
                    rsa.ImportParameters(rsa.ExportParameters(false));
                    return rsa.Encrypt(data, OAEP);
                }
            }
            else
            {
                using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(RSAKeyInfo))
                {
                    return rsa.Encrypt(data, OAEP);
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
            if (RSAKeyInfo == null)
            {
                using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
                {
                    rsa.ImportParameters(rsa.ExportParameters(false));
                    return rsa.Decrypt(buffer, OAEP);
                }
            }
            else
            {
                using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider(RSAKeyInfo))
                {
                    return rsa.Decrypt(buffer, OAEP);
                }
            }
        }
    }
}
