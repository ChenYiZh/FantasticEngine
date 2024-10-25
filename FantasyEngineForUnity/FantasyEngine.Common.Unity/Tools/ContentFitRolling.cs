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
    /// <summary>
    /// 如果内容超过限制就滚动播放
    /// </summary>
    public class ContentFitRolling : MonoBehaviour
    {
        public float deltaX = 10;
        public float deltaSeconds = 1;

        private float _waitSeconds;
        private void Awake()
        {
            _waitSeconds = deltaSeconds;
        }
        // Start is called before the first frame update
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {
            Rolling();
        }

        bool left = true;

        void Rolling()
        {
            if (_waitSeconds > 0)
            {
                _waitSeconds -= Time.deltaTime;
                return;
            }
            if (transform.childCount == 0) return;
            float maxWidth = GetComponent<RectTransform>().rect.width;
            RectTransform childTransform = transform.GetChild(0).GetComponent<RectTransform>();
            float pivotX = childTransform.pivot.x;
            float childWidth = childTransform.rect.width;
            Vector3 lp = childTransform.anchoredPosition3D;
            if (childWidth <= maxWidth)
            {
                lp.x = 0;
                childTransform.anchoredPosition3D = lp;
                return;
            }
            float range = childWidth - maxWidth;
            if (left)
            {
                lp.x -= deltaX * Time.deltaTime;
                float offset = Mathf.Lerp(-range, 0, pivotX);
                if (lp.x <= offset)
                {
                    lp.x = offset;
                    _waitSeconds = deltaSeconds;
                    left = false;
                }
            }
            else
            {
                lp.x += deltaX * Time.deltaTime;
                float offset = Mathf.Lerp(0, range, pivotX);
                if (lp.x >= offset)
                {
                    lp.x = offset;
                    _waitSeconds = deltaSeconds;
                    left = true;
                }
            }
            childTransform.anchoredPosition3D = lp;
        }
    }
}