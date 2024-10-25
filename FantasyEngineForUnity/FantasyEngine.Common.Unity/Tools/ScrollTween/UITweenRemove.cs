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
namespace FantasyEngine.Common
{
    public abstract class UITweenRemove<T> : UITweenRemove where T : UITweenRemove<T>
    {
        public static T Get(Transform item)
        {
            return Get(item.gameObject);
        }
        public static T Get(GameObject item)
        {
            return FEUtility.GetOrAddComponent<T>(item);
        }
    }

    public abstract class UITweenRemove : UITweenCustom
    {
        public System.Action onFinish;

        private float _time;
        protected int _running;

        public virtual void ResetValues()
        {
            if (Duration < 0.0001f) Duration = 1.0f;
            if (AnimationCurve == null || AnimationCurve.length == 0) AnimationCurve = new AnimationCurve(new Keyframe(0, 0, 0, 1), new Keyframe(1, 1, 1, 0));
        }

        public override void Replay()
        {
            //Debug.LogError("Replay");
            ResetValues();
            RefreshItems();
            _time = 0;
            _running = 1;
            Animate(0);
        }

        public override void Play()
        {
            //Debug.LogError("Play");
            ResetValues();
            RefreshItems();
            _running = 1;
        }

        private void Update()
        {
            if (_running > 0)
            {
                _time += Time.deltaTime;
                float rate = Mathf.Clamp01(_time / Duration);
                Animate(AnimationCurve.Evaluate(rate));
                if (rate >= 1)
                {
                    _running = 0;
                    if (onFinish != null) onFinish();
                }
            }
        }

        private void RefreshItems()
        {
            foreach (Transform item in transform.parent)
            {
                GameObject obj = item.gameObject;
                if (obj.GetComponent<UITweenRemove>())
                {
                    obj.GetComponent<UITweenRemove>().enabled = false;
                }
            }
            enabled = true;
        }

        private void OnDisable()
        {
            if (_running > 0)
            {
                Animate(1);
            }
            _running = 0;
        }

        public override bool IsPlaying()
        {
            return _running > 0;
        }

        protected abstract void Animate(float rate);

        public static UITweenRemove Get<T>(GameObject item) where T : UITweenRemove
        {
            return FEUtility.GetOrAddComponent<T>(item);
        }
    }
}