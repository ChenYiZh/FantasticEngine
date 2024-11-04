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
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

namespace FantasyEngine
{
    /// <summary>
    /// 工具类
    /// </summary>
    public partial class FETools
    {
        /// <summary>
        /// 点击时播放的音效
        /// </summary>
        /// <param name="button"></param>
        /// <param name="onclick"></param>
        public static void BindClick(Button button, UnityAction onclick)
        {
            BindClick(button, "click", onclick);//TODO: 修改点击时播放的音效，对应Resource目录
        }
        /// <summary>
        /// UI动画过渡
        /// </summary>
        public static IEnumerator PlayAnim(ICustomAnim obj, AnimConfig config, bool show)
        {
            EUIAnim anim = show ? config.AnimIn : config.AnimOut;
            float duration = show ? config.AnimInSeconds : config.AnimOutSeconds;
            var canvas = obj.GameObject.GetComponent<CanvasGroup>();
            canvas.alpha = 1;
            switch (anim)
            {
                case EUIAnim.Opacity:
                    {
                        if (canvas != null)
                        {
                            float from = show ? 0 : 1;
                            float to = show ? 1 : 0;
                            canvas.alpha = from;
                            float startTime = UnityEngine.Time.time;
                            float time = 0;
                            while ((time = (UnityEngine.Time.time - startTime) / duration) < 1)
                            {
                                yield return null;
                                time = GameDefines.Instance.PanelOpacityAnimCurve.Evaluate(time);
                                canvas.alpha = time * (to - from) + from;
                            }

                            canvas.alpha = to;
                        }
                    }
                    break;
                case EUIAnim.Scale:
                    {
                        var rect = obj.GameObject.GetComponent<RectTransform>();
                        if (rect != null)
                        {
                            float fromSize = show ? 0 : 1;
                            float fromPosition = show ? -GameDefines.Instance.uiAbsHeight : 0;
                            float toSize = show ? 1 : 0;
                            float toPosition = show ? 0 : -GameDefines.Instance.uiAbsHeight;
                            rect.localScale = Vector3.one * fromSize;
                            float startTime = Time.time;
                            float time = 0;
                            while ((time = (Time.time - startTime) / duration) < 1)
                            {
                                yield return null;
                                time = GameDefines.Instance.PanelScaleAnimCurve.Evaluate(time);
                                rect.localScale = Vector3.one * (time * (toSize - fromSize) + fromSize);
                                rect.anchoredPosition = Vector3.up * (time * (toPosition - fromPosition) + fromPosition);
                            }

                            rect.localScale = Vector3.one * toSize;
                            rect.anchoredPosition = Vector3.up * toPosition;
                        }
                    }
                    break;
                case EUIAnim.Round:
                    {
                        var rect = obj.GameObject.GetComponent<RectTransform>();
                        if (rect != null)
                        {
                            Vector2 fromMin = show ? Vector2.one * -1 : Vector2.zero;
                            Vector2 fromMax = show ? Vector2.one * 2 : Vector2.one;
                            Vector2 toMin = show ? Vector2.zero : Vector2.one * -1;
                            Vector2 toMax = show ? Vector2.one : Vector2.one * 2;
                            rect.anchorMin = fromMin;
                            rect.anchorMax = fromMax;
                            float startTime = Time.time;
                            float time = 0;
                            while ((time = (Time.time - startTime) / duration) < 1)
                            {
                                yield return null;
                                time = GameDefines.Instance.PanelRoundAnimCurve.Evaluate(time);
                                rect.anchorMin = (time * (toMin - fromMin) + fromMin);
                                rect.anchorMax = (time * (toMax - fromMax) + fromMax);
                            }

                            rect.anchorMin = toMin;
                            rect.anchorMax = toMax;
                        }
                    }
                    break;
                case EUIAnim.Custom:
                    {
                        yield return show ? obj.PlayCustomDisplayAnimation() : obj.PlayCustomHideAnimation();
                    }
                    break;
            }
        }



        public static void BindClick(Transform button, UnityAction onclick)
        {
            BindClick(button?.gameObject, onclick);
        }

        public static void BindClick(GameObject button, UnityAction onclick)
        {
            BindClick(button?.GetComponent<Button>(), onclick);
        }

        public static void BindClick(Transform button, string fileName, UnityAction onclick)
        {
            BindClick(button.gameObject, fileName, onclick);
        }

        public static void BindClick(GameObject button, string fileName, UnityAction onclick)
        {
            BindClick(button?.GetComponent<Button>(), fileName, onclick);
        }

        public static void BindClick(Button button, string fileName, UnityAction onclick)
        {
            if (button == null || onclick == null)
            {
                return;
            }

            button.onClick.RemoveAllListeners();
            button.onClick.AddListener(() =>
            {
                if (InputSystem.Instance.CanClick())
                {
                    InputSystem.Instance.StartStopwatch();
                    AudioSystem.Instance.PlayAudio(fileName);
                    onclick.Invoke();
                }
            });
        }



        /// <summary>
        /// 当前线程是否在主线程
        /// </summary>
        /// <returns></returns>
        public static bool IsRunningOnGameThread()
        {
            return GameRoot.ThreadId == Thread.CurrentThread.ManagedThreadId;
        }


        /***************需要引入图集插件**********************/
        //public static void SetSprite(Transform transform, string spriteName)
        //{
        //    if (transform == null) return;
        //    SetSprite(transform.GetComponent<Image>(), spriteName);
        //}

        //public static void SetSprite(GameObject gameObject, string spriteName)
        //{
        //    if (gameObject == null) return;
        //    SetSprite(gameObject.GetComponent<Image>(), spriteName);
        //}

        //public static void SetSprite(Image image, string spriteName)
        //{
        //    if (image == null || string.IsNullOrEmpty(spriteName)) return;
        //    image.sprite = GameDefines.Instance.GetSprite(spriteName);
        //}

        //public static Sprite GetSprite(string spriteName)
        //{
        //    return GameDefines.Instance.GetSprite(spriteName);
        //}
        /******************************************************/

    }
}