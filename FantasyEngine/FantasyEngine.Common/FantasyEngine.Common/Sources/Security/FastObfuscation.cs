using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Security
{
    /// <summary>
    /// 文本快速加密/解密
    /// </summary>
    public static class FastObfuscation
    {
        /// <summary>
        /// 加密/解密后的字符串
        /// </summary>
        /// <param name="text">需要加密/解密的文本</param>
        /// <param name="offset">字符偏移值</param>
        /// <returns></returns>
        public static string Shift(ref string text, int offset)
        {
            StringBuilder result = new StringBuilder();
            result.Length = 0;
            for (int i = 0; i < text.Length; i++)
                result.Append((char)((text[i] + offset) % char.MaxValue));
            return result.ToString();
        }
    }
}
