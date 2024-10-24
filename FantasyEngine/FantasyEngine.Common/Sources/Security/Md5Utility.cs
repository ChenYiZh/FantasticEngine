using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FantasyEngine.Security
{
    /// <summary>
    /// Md5帮助类
    /// </summary>
    public static class Md5Utility
    {
        /// <summary>
        /// Stream转Md5
        /// </summary>
        public static string GetMd5(Stream stream)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] retVal = md5.ComputeHash(stream);
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < retVal.Length; i++)
            {
                builder.Append(retVal[i].ToString("x2"));
            }
            return builder.ToString();
        }

        /// <summary>
        /// 字节流转Md5
        /// </summary>
        public static string GetMd5(byte[] buffer)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] retVal = md5.ComputeHash(buffer);
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < retVal.Length; i++)
            {
                builder.Append(retVal[i].ToString("x2"));
            }
            return builder.ToString();
        }

        /// <summary>
        /// 通过文件路径读取到的文件转Md5
        /// </summary>
        public static string GetMd5(string path)
        {
            string result = null;
            try
            {
                using (FileStream stream = File.OpenRead(path))
                {
                    result = GetMd5(stream);
                }
            }
            catch
            {
                result = null;
            }
            return result;
        }
    }
}
