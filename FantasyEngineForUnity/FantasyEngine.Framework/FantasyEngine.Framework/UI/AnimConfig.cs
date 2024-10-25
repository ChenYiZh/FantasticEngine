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

namespace FantasyEngine.UI
{
    /// <summary>
    /// UI动画配置
    /// </summary>
    public struct AnimConfig
    {
        /// <summary>
        /// 显示时动画类型
        /// </summary>
        public EUIAnim AnimIn { get; set; }
        /// <summary>
        /// 显示时动画时长
        /// </summary>
        public float AnimInSeconds { get; set; }
        /// <summary>
        /// 消失时动画类型
        /// </summary>
        public EUIAnim AnimOut { get; set; }
        /// <summary>
        /// 消失时动画时长
        /// </summary>
        public float AnimOutSeconds { get; set; }

        public static readonly AnimConfig TipsDefault = new AnimConfig()
        {
            AnimIn = EUIAnim.None,
            AnimInSeconds = 0.3f,
            AnimOut = EUIAnim.None,
            AnimOutSeconds = 0.3f,
        };
    }
}
