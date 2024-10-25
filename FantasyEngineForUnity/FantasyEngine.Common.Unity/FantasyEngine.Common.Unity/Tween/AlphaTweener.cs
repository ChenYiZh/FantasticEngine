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
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
namespace FantasyEngine.Common
{
    /// <summary>
    /// UI上的Alpha过度
    /// </summary>
    public class AlphaTweener : Tweener
    {
        public float from = 0;
        public float to = 1;
        public AnimationCurve curve = new AnimationCurve(new Keyframe(0, 0, 1, 1), new Keyframe(1, 1, 1, 1));

        private Image _image = null;
        private Text _text = null;
        private CanvasGroup _canvasGroup = null;

        private void Awake()
        {
            _canvasGroup = GetComponent<CanvasGroup>();
            _image = GetComponent<Image>();
            _text = GetComponent<Text>();
        }

        protected override void Play(float time)
        {
            float alpha = (to - from) * curve.Evaluate(time) + from;
            if (_canvasGroup != null)
            {
                _canvasGroup.alpha = alpha;
                return;
            }
            if (_image != null)
            {
                Color color = _image.color;
                color.a = alpha;
                _image.color = color;
            }
            if (_text != null)
            {
                Color color = _text.color;
                color.a = alpha;
                _text.color = color;
            }
        }
    }
}