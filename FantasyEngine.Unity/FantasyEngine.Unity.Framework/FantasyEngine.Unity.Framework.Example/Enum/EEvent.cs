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

namespace FantasyEngine
{
    /// <summary>
    /// EventManager使用的指令
    /// </summary>
    public enum EEvent
    {
        /// <summary>
        /// 无效指令
        /// </summary>
        Invalid,
        /// <summary>
        /// 加载进度
        /// </summary>
        LoadingProgress,
        /// <summary>
        /// 场景加载完成
        /// </summary>
        SceneLoaded,
        /// <summary>
        /// 场景移除
        /// </summary>
        SceneUnloaded,


        #region 游戏生命周期
        /***
         * 暂时用不到，原混编时，由需要外部调用
         * */


        /// <summary>
        /// 游戏开始
        /// </summary>
        BeginPlay,

        /// <summary>
        /// 游戏暂停
        /// </summary>
        GamePause,
        /// <summary>
        /// 游戏继续
        /// </summary>
        GameContinue,
        /// <summary>
        /// 游戏结束
        /// </summary>
        GameFinish,

        /// <summary>
        /// 登录
        /// </summary>
        Login, 
        /// <summary>
        /// 退出
        /// </summary>
        Logout,

        #endregion
    }
}
