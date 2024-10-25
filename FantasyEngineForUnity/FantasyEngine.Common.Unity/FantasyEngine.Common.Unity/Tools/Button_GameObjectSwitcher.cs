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
using FantasyEngine.Common.Attribute;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
namespace FantasyEngine.Common
{
    /// <summary>
    /// 按钮事件，在按钮触发的不同阶段显示不同gameObject
    /// </summary>
    public class Button_GameObjectSwitcher : MonoBehaviour, IPointerDownHandler, IPointerUpHandler,
    IPointerEnterHandler, IPointerExitHandler,
    ISelectHandler, IDeselectHandler
    {
        /// <summary>
        /// 管理的Button
        /// </summary>
        public Button button;

        /// <summary>
        /// 正常情况下的gameObject
        /// </summary>
        public GameObject gameObjectNormal;

        /// <summary>
        /// 鼠标覆盖情况下的gameObject
        /// </summary>
        public GameObject gameObjectHovered;

        /// <summary>
        /// 鼠标按下的gameObject
        /// </summary>
        public GameObject gameObjectOnPressed;

        /// <summary>
        /// 按钮无法使用情况下显示的gameObject
        /// </summary>
        public GameObject gameObjectOnDisable;

        /// <summary>
        /// 按钮是否是启用状态
        /// </summary>
        [SerializeField, ReadOnly] private bool _buttonEnabled = true;

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
            if (button == null)
            {
                button = GetComponent<Button>();
            }

            if (button)
            {
                button.transition = Selectable.Transition.None;
            }
        }

        private void OnEnable()
        {
            RefreshState();
        }

        private void OnDisable()
        {
            _buttonHovered = false;
            _buttonPressed = false;
            //RefreshState();
        }

        public void RefreshState()
        {
            if (gameObjectNormal != null)
            {
                gameObjectNormal.SetActive(false);
            }

            if (gameObjectHovered != null)
            {
                gameObjectHovered.SetActive(false);
            }

            if (gameObjectOnPressed != null)
            {
                gameObjectOnPressed.SetActive(false);
            }

            if (gameObjectOnDisable != null)
            {
                gameObjectOnDisable.SetActive(false);
            }

            if (!_buttonEnabled)
            {
                if (gameObjectOnDisable != null)
                {
                    gameObjectOnDisable.SetActive(true);
                }
                else if (gameObjectNormal != null)
                {
                    gameObjectNormal.SetActive(true);
                }
            }
            else if (_buttonPressed)
            {
                if (gameObjectOnPressed != null)
                {
                    gameObjectOnPressed.SetActive(true);
                }
                else if (gameObjectHovered != null)
                {
                    gameObjectHovered.SetActive(true);
                }
                else if (gameObjectNormal != null)
                {
                    gameObjectNormal.SetActive(true);
                }
            }
            else if (_buttonHovered)
            {
                if (gameObjectHovered != null)
                {
                    gameObjectHovered.SetActive(true);
                }
                else if (gameObjectNormal != null)
                {
                    gameObjectNormal.SetActive(true);
                }
            }
            else if (gameObjectNormal != null)
            {
                gameObjectNormal.SetActive(true);
            }
        }

        private void Update()
        {
            if (button && button.enabled != _buttonEnabled)
            {
                _buttonEnabled = button.enabled;
                RefreshState();
            }
        }

        public void OnPointerDown(PointerEventData eventData)
        {
            _buttonPressed = true;
            RefreshState();
        }

        public void OnPointerUp(PointerEventData eventData)
        {
            _buttonPressed = false;
            RefreshState();
        }

        public void OnPointerEnter(PointerEventData eventData)
        {
            _buttonHovered = true;
            RefreshState();
        }

        public void OnPointerExit(PointerEventData eventData)
        {
            _buttonHovered = false;
            RefreshState();
        }

        public void OnSelect(BaseEventData eventData)
        {
            _buttonHovered = true;
            RefreshState();
        }

        public void OnDeselect(BaseEventData eventData)
        {
            _buttonHovered = false;
            RefreshState();
        }
    }
}