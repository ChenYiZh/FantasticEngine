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
using FantasyEngine.Common.Attribute;
using UnityEngine;
using UnityEngine.EventSystems;

namespace FantasyEngine.Tweener
{
    /// <summary>
    /// 按钮点击时播放不同Tween动画
    /// </summary>
    public class Button_Tweeners : MonoBehaviour, IPointerDownHandler, IPointerUpHandler,
        IPointerEnterHandler, IPointerExitHandler,
        ISelectHandler, IDeselectHandler
    {
        /// <summary>
        /// 动画列表
        /// </summary>
        [SerializeField, ReadOnly] private Tweener[] _tweeners;

        /// <summary>
        /// 按钮是否被覆盖者
        /// </summary>
        [SerializeField, ReadOnly] private bool _buttonHovered = false;

        /// <summary>
        /// 按钮是否在按下状态
        /// </summary>
        [SerializeField, ReadOnly] private bool _buttonPressed = false;

        private void Awake()
        {
            _tweeners = GetComponents<Tweener>();
        }

        private void OnEnable()
        {
            RefreshState();
        }

        private void OnDisable()
        {
            _buttonHovered = false;
            _buttonPressed = false;
        }

        public void RefreshState()
        {
            if (_tweeners == null || _tweeners.Length == 0)
            {
                return;
            }

            if (_buttonPressed)
            {
                foreach (Tweener tweener in _tweeners)
                {
                    if (!tweener.IsRevert)
                    {
                        if (!tweener.IsPlaying)
                        {
                            tweener.ResetToEnd();
                        }

                        tweener.PlayBack();
                    }
                }
            }
            else if (_buttonHovered)
            {
                foreach (Tweener tweener in _tweeners)
                {
                    if (!tweener.IsPlaying)
                    {
                        tweener.ResetToBegin();
                    }

                    tweener.PlayForward();
                }
            }
            else
            {
                foreach (Tweener tweener in _tweeners)
                {
                    if (!tweener.IsRevert)
                    {
                        if (!tweener.IsPlaying)
                        {
                            tweener.ResetToEnd();
                        }

                        tweener.PlayBack();
                    }
                }
            }
        }

        public void OnPointerDown(PointerEventData eventData)
        {
            OnPointerDown();
        }

        public void OnPointerDown()
        {
            _buttonPressed = true;
            RefreshState();
        }

        public void OnPointerUp(PointerEventData eventData)
        {
            OnPointerUp();
        }

        public void OnPointerUp()
        {
            _buttonPressed = false;
            RefreshState();
        }

        public void OnPointerEnter(PointerEventData eventData)
        {
            OnPointerEnter();
        }

        public void OnPointerEnter()
        {
            _buttonHovered = true;
            RefreshState();
        }

        public void OnPointerExit(PointerEventData eventData)
        {
            OnPointerExit();
        }

        public void OnPointerExit()
        {
            _buttonHovered = false;
            RefreshState();
        }

        public void OnSelect(BaseEventData eventData)
        {
            OnSelect();
        }

        public void OnSelect()
        {
            _buttonHovered = true;
            RefreshState();
        }

        public void OnDeselect(BaseEventData eventData)
        {
            OnDeselect();
        }

        public void OnDeselect()
        {
            _buttonHovered = false;
            RefreshState();
        }
    }
}