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
using FantasyEngine.Common;

namespace FantasyEngine
{
    /// <summary>
    /// 管理类基类
    /// </summary>
    public abstract class SystemBasis : ISystemBasis, IUpdate
    {
        /// <summary>
        /// Update启动?
        /// </summary>
        public bool enabled { get; set; }
        /// <summary>
        /// 是否在Initialize前执行Tick
        /// </summary>
        public virtual bool UpdateBeforeInitialze
        {
            get { return false; }
        }
        /// <summary>
        /// 是否加载文成？
        /// </summary>
        public virtual bool Ready
        {
            get { return true; }
        }

        public virtual void Prepare()
        {
        }

        public virtual void OnReady()
        {
        }

        #region 游戏生命周期

        //public virtual void BeginPlay(GamePlayParam param)
        public virtual void BeginPlay()
        {
        }

        public virtual void OnPlay()
        {
        }

        public virtual void OnPause()
        {
        }

        public virtual void OnContinue()
        {
        }

        public virtual void OnExit()
        {
        }

        #endregion

        public virtual void OnFixedUpdate()
        {
        }

        /// <summary>
        /// 每帧执行
        /// </summary>
        public virtual void OnLateUpdate()
        {
        }

        /// <summary>
        /// 每帧执行
        /// </summary>
        public virtual void OnUpdate()
        {
        }

        public virtual void Initialize()
        {
        }

        public virtual void OnApplicationFocus(bool hasFocus)
        {
        }

        public virtual void OnApplicationPause(bool pauseStatus)
        {
        }

        public virtual void OnApplicationQuit()
        {
        }

        public virtual void OnLogin()
        {
        }

        public virtual void OnLogout()
        {
        }
    }

    public abstract class SystemBasis<T> : SystemBasis where T : SystemBasis<T>, new()
    {
        public static T Instance
        {
            get { return Singleton<T>.Instance; }
        }
    }
}