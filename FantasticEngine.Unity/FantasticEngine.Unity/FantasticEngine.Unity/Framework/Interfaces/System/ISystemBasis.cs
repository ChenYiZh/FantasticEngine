﻿/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
namespace FantasticEngine.Common
{
    public interface ISystemBasis : IInitialize
    {
        void Prepare();

        #region 游戏生命周期
        void BeginPlay();

        /// <summary>
        /// Unity已经开始游戏
        /// </summary>
        void OnPlay();

        /// <summary>
        /// 游戏暂停Ex.完成;暂停
        /// </summary>
        void OnPause();

        /// <summary>
        /// 继续游戏
        /// </summary>
        void OnContinue();

        /// <summary>
        /// 退出游戏
        /// </summary>
        void OnExit();
        #endregion

        void OnLogin();

        void OnLogout();


        void OnApplicationQuit();


        void OnApplicationFocus(bool hasFocus);

        void OnApplicationPause(bool pauseStatus);
    }
}