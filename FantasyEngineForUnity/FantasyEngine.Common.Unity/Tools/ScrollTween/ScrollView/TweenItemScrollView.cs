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
    public abstract class TweenItemScrollView : TweenLineScrollView
    {
        protected override LineItem NewLineItem(float startTime)
        {
            if (_lineItems.Count > 0)
            {
                startTime = _lineItems.Last.Value.StartTime + DeltaSeconds * ScrollView.Limit;
            }
            return new Item(this, ScrollView, PlayedLine, Duration, startTime, DeltaSeconds);
        }

        private class Item : LineItem
        {
            private Dictionary<Transform, float> _itemTimes;
            private float _deltaTime;
            private float _totalDuration;

            public Item(TweenLineScrollView script, CScrollView scrollView, int line, float duration, float startTime, float deltaTime) : base(script, scrollView, line, duration, startTime)
            {
                _deltaTime = deltaTime;
                _totalDuration = deltaTime * (_items.Length - 1) + duration;
                _time = _totalDuration;
                _itemTimes = new Dictionary<Transform, float>();
                for (int i = 0; i < _items.Length; i++)
                {
                    Transform item = _items[i];
                    _itemTimes.Add(item, duration);
                }
            }

            protected override void UpdateItem(Transform item, int line, int index, float rate)
            {
                float itemRate = 0;
                if (Time.time >= StartTime + _deltaTime * index)
                {
                    _itemTimes[item] -= Time.deltaTime;
                    itemRate = Mathf.Clamp01((_duration - _itemTimes[item]) / _duration); ;
                }
                if (rate >= 1)
                {
                    itemRate = 1;
                }
                base.UpdateItem(item, line, index, itemRate);
            }
        }
    }
}