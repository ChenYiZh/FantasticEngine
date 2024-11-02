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
using FantasyEngine.UI;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine
{
    /// <summary>
    /// 工具类
    /// </summary>
    public partial class FETools
    {
        /// <summary>
        /// 选项框
        /// </summary>
        public static void ShowMessageBox(
            EMsgType msgType,
            string content,
            Action yes,
            string yesLabel = null,
            Action no = null,
            string noLabel = null,
            Action cancel = null)
        {
            UI_Tips.Hide(ETip.Tips_MessageBox);
            UI_Tips.Show(ETip.Tips_MessageBox, new TipsParam_MessageBox()
            {
                msgType = msgType,
                content = content,
                yesLabel = yesLabel,
                yes = yes,
                noLabel = noLabel,
                no = no,
                cancel = cancel,
            });
        }

        /// <summary>
        /// Toast弹框
        /// </summary>
        /// <param name="message"></param>
        public static void Toast(string message)
        {
            if (string.IsNullOrEmpty(message))
            {
                return;
            }

            UI_Tips.Show(ETip.Tips_Toast, new TipsParam_Toast { message = message });
        }
    }
}
