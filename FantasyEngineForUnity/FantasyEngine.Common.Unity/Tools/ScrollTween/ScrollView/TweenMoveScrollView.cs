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
    public class TweenMoveScrollView : TweenLineScrollView
    {
        private float _deltaPos;

        private bool _isVertical;

        private Vector2 _startPos;

        protected override void OnInitialize()
        {
            _startPos = ScrollView.StartPosition;
            _isVertical = ScrollView.IsVertical;
            _deltaPos = (ScrollView.IsVertical ? ScrollView.ItemSize.x : ScrollView.ItemSize.y) * ScrollView.Limit;
            _deltaPos /= 2.0f;
        }

        protected override void InitializeItem(Transform item)
        {
            CanvasGroup widget = FEUtility.GetOrAddComponent<CanvasGroup>(item);
            widget.alpha = 0;
        }

        protected override void UpdateItem(Transform item, int line, int index, float rate)
        {
            RectTransform rect = item.GetComponent<RectTransform>();
            float delta = _deltaPos * (1 - rate);
            float sPos = _isVertical ? _startPos.x : _startPos.y;
            float deltaPos = _isVertical ? ScrollView.ItemSize.x : ScrollView.ItemSize.y;
            Vector2 v = rect.anchoredPosition;
            float pos = sPos + deltaPos * index + delta;
            rect.anchoredPosition = new Vector2(_isVertical ? pos : v.x, _isVertical ? v.y : pos);
            CanvasGroup widget = item.gameObject.GetComponent<CanvasGroup>();
            widget.alpha = rate;
        }

        protected override void ResetItem(Transform item, int line, int index)
        {
            RectTransform rect = item.GetComponent<RectTransform>();
            float sPos = _isVertical ? _startPos.x : _startPos.y;
            float deltaPos = _isVertical ? ScrollView.ItemSize.x : ScrollView.ItemSize.y;
            Vector2 v = rect.anchoredPosition;
            float pos = sPos + _deltaPos * index;
            rect.anchoredPosition = new Vector2(_isVertical ? pos : v.x, _isVertical ? v.y : pos);
            CanvasGroup widget = item.gameObject.GetComponent<CanvasGroup>();
            widget.alpha = 1;
        }
    }
}