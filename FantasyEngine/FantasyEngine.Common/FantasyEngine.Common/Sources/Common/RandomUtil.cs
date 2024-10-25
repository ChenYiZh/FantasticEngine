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

namespace FantasyEngine.Sources.Common
{
    /// <summary>
    /// 随机数管理
    /// </summary>
    public static class RandomUtil
    {
        /// <summary>
        /// 种子
        /// </summary>
        public static Random Seed { get; set; }

        static RandomUtil()
        {
            Seed = new Random((int)(DateTime.Now.Ticks % int.MaxValue));
        }

        /// <summary>
        /// 随机一个字节
        /// </summary>
        /// <returns></returns>
        public static byte RandomByte()
        {
            return (byte)Seed.Next(byte.MinValue, byte.MaxValue);
        }

        /// <summary>
        /// 范围随机
        /// </summary>
        public static int RandomRange(int min, int max)
        {
            return Seed.Next(min, max);
        }

        /// <summary>
        /// 范围随机
        /// </summary>
        public static float RandomRange(float min, float max)
        {
            return (float)((max - min) * Random() + min);
        }

        /// <summary>
        /// 范围随机
        /// </summary>
        public static double RandomRange(double min, double max)
        {
            return (max - min) * Random() + min;
        }

        /// <summary>
        /// 随机数
        /// </summary>
        public static double Random()
        {
            return Seed.NextDouble();
        }
    }
}
