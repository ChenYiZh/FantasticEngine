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
    public abstract class TweenLineScrollView : UITweenScrollView
    {
        public int PlayedLine { get; private set; }

        protected LinkedList<LineItem> _lineItems;

        private int _running;

        protected virtual void OnInitialize() { }

        //protected virtual void OnDisableItem(Transform item) { }
        //protected virtual void OnDisableItem(Transform item, int line, int index) { OnDisableItem(item); }

        protected virtual void InitializeItem(Transform item) { }
        protected virtual void InitializeItem(Transform item, int line, int index) { InitializeItem(item); }


        protected virtual void UpdateItem(Transform item, float rate) { }
        protected virtual void UpdateItem(Transform item, int line, int index, float rate)
        {
            UpdateItem(item, rate);
        }

        protected virtual void ResetItem(Transform item) { }
        protected virtual void ResetItem(Transform item, int line, int index)
        {
            ResetItem(item);
        }


        protected override void Initialize()
        {
            _lineItems = new LinkedList<LineItem>();
            PlayedLine = ScrollView.TopLine - 1;
            OnInitialize();
            for (int i = ScrollView.TopLine; i <= ScrollView.BottomLine; i++)
            {
                Transform[] items = ScrollView.GetItemsByLine(i);
                for (int k = 0; k < items.Length; k++)
                {
                    InitializeItem(items[k], i, k);
                }
            }
        }
        protected override void Update()
        {
            base.Update();
            if (ScrollView == null || _running == 0) return;
            UpdateAnim();
            if (PlayedLine < ScrollView.BottomLine)
            {
                PlayedLine++;
                int startLine = PlayedLine < ScrollView.TopLine ? ScrollView.TopLine : PlayedLine;
                float startTime = Time.time + DeltaSeconds;
                for (int i = startLine; i <= ScrollView.BottomLine; i++)
                {
                    PlayedLine = i;
                    if (_lineItems.Count > 0)
                    {
                        startTime = _lineItems.Last.Value.StartTime + DeltaSeconds;
                    }
                    //Debug.LogError(_playedLine + ": " + startTime);
                    _lineItems.AddLast(NewLineItem(startTime));
                }
            }
            if (_lineItems.Count == 0)
            {
                _running = 0;
            }
        }

        protected virtual LineItem NewLineItem(float startTime)
        {
            return new LineItem(this, ScrollView, PlayedLine, Duration, startTime);
        }

        private void UpdateAnim()
        {
            if (_lineItems == null) return;
            int count = _lineItems.Count;
            if (count > 0)
            {
                LinkedListNode<LineItem> lineItem = _lineItems.First;
                for (int i = 0; i < count; i++)
                {
                    if (lineItem.Value.Update())
                    {
                        LineItem toRemove = lineItem.Value;
                        lineItem = lineItem.Next;
                        _lineItems.Remove(toRemove);
                        if (toRemove.Line < PlayedLine)
                        {
                            toRemove.Reset();
                        }
                    }
                    else
                    {
                        lineItem = lineItem.Next;
                    }
                }
            }
        }

        private void OnDisable()
        {
            if (_lineItems == null) return;
            _running = 0;
            int count = _lineItems.Count;
            if (count > 0)
            {
                LinkedListNode<LineItem> lineItem = _lineItems.First;
                for (int i = 0; i < count; i++)
                {
                    lineItem.Value.ToEnd();
                }
                _lineItems.Clear();
            }
            for (int i = ScrollView.TopLine; i <= ScrollView.BottomLine; i++)
            {
                Transform[] items = ScrollView.GetItemsByLine(i);
                for (int k = 0; k < items.Length; k++)
                {
                    try
                    {
                        UpdateItem(items[k], i, k, 1);
                    }
                    catch { }
                }
            }
        }

        protected class LineItem
        {
            public float StartTime { get; private set; }
            protected float _duration;
            protected float _time;
            public int Line { get; private set; }
            protected CScrollView _scrollView;
            protected Transform[] _items;
            protected TweenLineScrollView _script;

            public LineItem(TweenLineScrollView script, CScrollView scrollView, int line, float duration, float startTime)
            {
                _script = script;
                _scrollView = scrollView;
                Line = line;
                _duration = duration;
                _time = _duration;
                _items = _scrollView.GetItemsByLine(Line);
                StartTime = startTime;
                for (int i = 0; i < _items.Length; i++)
                {
                    _script.UpdateItem(_items[i], Line, i, 0);
                }
            }

            public bool Update()
            {
                if (Time.time < StartTime)
                {
                    return false;
                }
                _time -= Time.deltaTime;
                bool toRemove = false;
                if (_time <= 0)
                {
                    toRemove = true;
                    _time = 0;
                }
                float rate = Mathf.Clamp01((_duration - _time) / _duration);
                if (Line < _scrollView.TopLine || Line > _scrollView.BottomLine)
                {
                    rate = 1;
                    toRemove = true;
                }
                for (int i = 0; i < _items.Length; i++)
                {
                    UpdateItem(_items[i], Line, i, rate);
                }
                return toRemove;
            }

            protected virtual void UpdateItem(Transform item, int line, int index, float rate)
            {
                _script.UpdateItem(item, Line, index, _script.AnimationCurve.Evaluate(rate));
            }

            public void ToEnd()
            {
                for (int i = 0; i < _items.Length; i++)
                {
                    UpdateItem(_items[i], Line, i, 1);
                }
            }

            public void Reset()
            {
                for (int i = 0; i < _items.Length; i++)
                {
                    _script.ResetItem(_items[i], Line, i);
                }
            }
        }

        public override void Replay()
        {
            _running = 1;
            if (_lineItems != null)
            {
                _lineItems.Clear();
            }
            if (ScrollView != null)
            {
                ScrollView.Reposition();
                PlayedLine = -1;
            }
        }

        public override void Play()
        {
            _running = 1;
            if (_lineItems != null)
            {
                _lineItems.Clear();
            }
            if (ScrollView != null)
            {
                PlayedLine = ScrollView.TopLine - 1;
            }
        }

        public override bool IsPlaying()
        {
            return _running > 0;
        }
    }
}