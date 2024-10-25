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
using UnityEngine;
namespace FantasyEngine
{
    /// <summary>
    /// 输入防误触管理
    /// </summary>
    public class InputSystem : SystemBasis<InputSystem>
    {
        /// <summary>
        /// 点击计时
        /// </summary>
        private float _duration;

        public override void Initialize()
        {
            base.Initialize();
            _duration = 0;
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (_duration > 0)
            {
                _duration -= Time.deltaTime;
            }
        }

        public bool CanClick()
        {
            return _duration <= 0;
        }

        public void StartStopwatch()
        {
            _duration = 0.1f;
        }
    }
}