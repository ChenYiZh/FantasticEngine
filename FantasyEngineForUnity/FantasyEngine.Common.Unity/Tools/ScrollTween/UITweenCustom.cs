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
    public abstract class UITweenCustom : MonoBehaviour
    {
        [SerializeField]
        protected AnimationCurve _animationCurve = new AnimationCurve(new Keyframe(0, 0), new Keyframe(1, 1));
        public AnimationCurve AnimationCurve
        {
            get
            {
                if (_animationCurve == null)
                {
                    _animationCurve = new AnimationCurve(new Keyframe(0, 0), new Keyframe(1, 1));
                }
                return _animationCurve;
            }
            set
            {
                if (value == null || value.keys.Length < 2)
                {
                    _animationCurve = new AnimationCurve(new Keyframe(0, 0), new Keyframe(1, 1));
                }
                else
                {
                    _animationCurve = value;
                }
            }
        }
        public float Duration = 0.3f;
        public float DeltaSeconds = 0.1f;

        public virtual void Replay()
        {
            Play();
        }

#if UNITY_EDITOR
    [ContextMenu("Replay")]
    private void ReplayEditor()
    {
        Replay();
    }
    [ContextMenu("Play")]
    private void PlayEditor()
    {
        Play();
    }
#endif
        public abstract void Play();

        public abstract bool IsPlaying();
    }
}