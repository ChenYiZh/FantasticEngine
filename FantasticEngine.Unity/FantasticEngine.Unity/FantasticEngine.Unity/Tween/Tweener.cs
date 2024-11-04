/****************************************************************************
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
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
namespace FantasticEngine.Tweener
{
    /// <summary>
    /// Tween动画基类
    /// </summary>
    public abstract class Tweener : MonoBehaviour
    {
        /// <summary>
        /// 是否自动执行
        /// </summary>
        public bool auto = false;

        /// <summary>
        /// 是否只在相机视野中才执行
        /// </summary>
        public bool onlyPlayInCamera = false;

        /// <summary>
        /// 播放类型
        /// </summary>
        public PlayType Type = PlayType.Once;

        /// <summary>
        /// 等待多久执行
        /// </summary>
        public float delay = 0;

        /// <summary>
        /// 动画时长
        /// </summary>
        public float duration = 1;

        /// <summary>
        /// 间隔触发
        /// </summary>
        public float interval = 0;

        private bool _isVisible;

        /// <summary>
        /// 是否正在播放
        /// </summary>
        public bool IsPlaying { get; private set; }

        private float _waitTime;
        private float _time;
        private bool _revert;

        /// <summary>
        /// 是否是PlayBack
        /// </summary>
        public bool IsRevert
        {
            get { return _revert; }
        }

        protected abstract void Play(float time);

        public virtual void PlayForward()
        {
            if (IsPlaying && _revert && _time > 0)
            {
                _time = duration - _time;
            }

            _revert = false;
            IsPlaying = true;
        }

        public virtual void PlayBack()
        {
            if (IsPlaying && !_revert && _time > 0)
            {
                _time = duration - _time;
            }

            _revert = true;
            IsPlaying = true;
        }

        protected void Update()
        {
            if (onlyPlayInCamera && !_isVisible)
            {
                return;
            }

            if (IsPlaying)
            {
                if (_waitTime > 0)
                {
                    _waitTime -= UnityEngine.Time.deltaTime;
                    return;
                }

                _time -= UnityEngine.Time.deltaTime;
                float rate = Mathf.Clamp01(_time / duration);
                if (!FEUtility.NearBy(interval, 0) && interval > 0)
                {
                    float minTime = 1 / (duration / interval); //倍数
                    int scale = Mathf.FloorToInt(rate / minTime);
                    rate = scale * minTime;
                }

                if (_revert)
                {
                    Play(rate);
                }
                else
                {
                    Play(1 - rate);
                }

                if (_time < 0)
                {
                    OnEnd();
                    return;
                }
            }
        }

        public virtual void ResetToBegin()
        {
            _time = duration;
            _waitTime = delay;
            Play(0);
        }

        public virtual void ResetToEnd()
        {
            _time = duration;
            _waitTime = delay;
            Play(1);
        }

        protected virtual void OnEnable()
        {
            if (auto)
            {
                if (_revert)
                {
                    PlayBack();
                }
                else
                {
                    PlayForward();
                }
            }
        }

        protected virtual void OnDisable()
        {
            if (auto || IsPlaying)
            {
                if (_revert)
                {
                    ResetToBegin();
                }
                else
                {
                    ResetToEnd();
                }
            }

            IsPlaying = false;
        }

        protected virtual void OnEnd()
        {
            IsPlaying = false;
            switch (Type)
            {
                case PlayType.Once: break;
                case PlayType.Loop:
                    {
                        if (_revert)
                        {
                            ResetToEnd();
                            PlayBack();
                        }
                        else
                        {
                            ResetToBegin();
                            PlayForward();
                        }
                    }
                    break;
                case PlayType.PingPong:
                    {
                        if (_revert)
                        {
                            ResetToBegin();
                            PlayForward();
                        }
                        else
                        {
                            ResetToEnd();
                            PlayBack();
                        }
                    }
                    break;
            }
        }

        public virtual void Stop()
        {
            IsPlaying = false;
        }

        public enum PlayType
        {
            Once,
            Loop,
            PingPong,
        }

        private void OnBecameVisible()
        {
            _isVisible = true;
        }

        private void OnBecameInvisible()
        {
            _isVisible = false;
        }
    }
}