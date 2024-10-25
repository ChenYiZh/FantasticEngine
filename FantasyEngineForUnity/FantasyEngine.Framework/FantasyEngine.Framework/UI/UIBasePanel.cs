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
using System.Collections;
using System.Collections.Generic;
using FantasyEngine.Common;
using FantasyEngine.Log;
using UnityEngine;
namespace FantasyEngine
{
    /// <summary>
    /// UI基类
    /// </summary>
    [RequireComponent(typeof(CanvasGroup))]
    public abstract class UIBasePanel : MonoBehaviour, IVisual, ICustomAnim
    {
        public abstract EUIID UIID { get; }

        public virtual UIFactory.PanelConfig PanelConfig { get; set; }

        public GameObject GameObject => gameObject;

        public List<UIVisual> Visuals { get; private set; } = new List<UIVisual>();

        /// <summary>
        /// 是否已经执行过Awake
        /// </summary>
        private bool _awaked = false;

        /// <summary>
        /// 是否已经执行过Started
        /// </summary>
        private bool _started = false;

        //添加额外组件
        protected bool Regist(UIVisual visual)
        {
            if (visual == null || Visuals.Contains(visual))
            {
                return false;
            }

            Visuals.Add(visual);
            visual.MainPanel = this;
            if (_awaked)
            {
                visual.Awake();
            }

            if (_started)
            {
                visual.Start();
            }

            return true;
        }

        protected bool Unregist(UIVisual visual)
        {
            return Visuals.Remove(visual);
        }

        public abstract void OnCreate();

        public virtual void OnPanelOpen(PanelParam param = null)
        {
        }

        public virtual void BeforePlayDisplayAnimation()
        {
        }

        public virtual IEnumerator PlayCustomDisplayAnimation()
        {
            yield return null;
        }

        public virtual void OnShow()
        {
        }

        public virtual void OnHide()
        {
        }

        public virtual void BeforePlayHideAnimation()
        {
        }

        public virtual IEnumerator PlayCustomHideAnimation()
        {
            yield return null;
        }

        public virtual void OnPanelClosed()
        {
        }

        public virtual void Close()
        {
            UIFactory.Instance.Pop(UIID);
        }

        float lastTime;

        private void Update()
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                OnUpdate();
                foreach (IVisual visual in Visuals)
                {
                    try
                    {
                        visual.OnUpdate();
                    }
                    catch (Exception e)
                    {
                        FConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FConsole.WriteException(e);
            }

            if ((lastTime += Time.deltaTime) >= 1)
            {
                lastTime = 0;
                try
                {
                    OnSecond();
                    foreach (IVisual visual in Visuals)
                    {
                        try
                        {
                            visual.OnSecond();
                        }
                        catch (Exception e)
                        {
                            FConsole.WriteException(e);
                        }
                    }
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        private void LateUpdate()
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                OnLateUpdate();
                foreach (IVisual visual in Visuals)
                {
                    try
                    {
                        visual.OnLateUpdate();
                    }
                    catch (Exception e)
                    {
                        FConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FConsole.WriteException(e);
            }
        }

        private void FixedUpdate()
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                OnFixedUpdate();
                foreach (IVisual visual in Visuals)
                {
                    try
                    {
                        visual.OnFixedUpdate();
                    }
                    catch (Exception e)
                    {
                        FConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FConsole.WriteException(e);
            }
        }

        public virtual void OnFixedUpdate()
        {
        }

        public virtual void OnLateUpdate()
        {
        }

        public virtual void OnUpdate()
        {
        }

        public virtual void OnSecond()
        {
        }

        public virtual void OnDestroy()
        {
            foreach (IVisual visual in Visuals)
            {
                try
                {
                    visual.OnDestroy();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        public virtual void Awake()
        {
            _awaked = true;
            foreach (IVisual visual in Visuals)
            {
                try
                {
                    visual.Awake();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        public virtual void Start()
        {
            _started = true;
            foreach (IVisual visual in Visuals)
            {
                try
                {
                    visual.Start();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        public virtual void OnEnable()
        {
            foreach (IVisual visual in Visuals)
            {
                try
                {
                    visual.OnEnable();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }

        public virtual void OnDisable()
        {
            foreach (IVisual visual in Visuals)
            {
                try
                {
                    visual.OnDisable();
                }
                catch (Exception e)
                {
                    FConsole.WriteException(e);
                }
            }
        }
    }

    /// <summary>
    /// 拥有Panel生命周期的基类
    /// </summary>
    public interface IVisual : IUpdate
    {
        void OnCreate();

        void OnPanelOpen(PanelParam param = null);

        void OnShow();

        void OnHide();

        void OnPanelClosed();

        void OnSecond();

        void OnDestroy();

        void Awake();

        void Start();

        void OnEnable();

        void OnDisable();
    }

    /// <summary>
    /// 拥有Panel生命周期的基类
    /// </summary>
    public abstract class UIVisual : IVisual
    {
        // private bool _enable = true;
        //
        // public bool enabled
        // {
        //     get { return _enable; }
        //
        //     set
        //     {
        //         if (_enable != value)
        //         {
        //             if (value)
        //             {
        //                 OnEnable();
        //             }
        //             else
        //             {
        //                 OnDisable();
        //             }
        //
        //             _enable = value;
        //         }
        //     }
        // }

        public UIBasePanel MainPanel { get; set; }

        public abstract void OnCreate();

        public virtual void OnPanelOpen(PanelParam param = null)
        {
        }

        public virtual void OnShow()
        {
        }

        public virtual void OnHide()
        {
        }

        public virtual void OnPanelClosed()
        {
        }

        public virtual void OnFixedUpdate()
        {
        }

        public virtual void OnLateUpdate()
        {
        }

        public virtual void OnUpdate()
        {
        }

        public virtual void OnSecond()
        {
        }

        public virtual void OnDestroy()
        {
        }

        public virtual void Awake()
        {
        }

        public virtual void Start()
        {
        }

        public virtual void OnEnable()
        {
        }

        public virtual void OnDisable()
        {
        }
    }
}