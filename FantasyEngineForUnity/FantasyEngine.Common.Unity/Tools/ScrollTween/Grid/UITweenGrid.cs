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
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
namespace FantasyEngine.Common
{
    //public abstract class UITweenGrid : UITweenCustom
    //{
    //    public UIGrid Grid;

    //    protected List<TweenItem> _items;

    //    protected int _running;

    //    protected virtual void Initialize() { }

    //    protected abstract void UpdateItem(Transform item, float rate);

    //    bool _waiting2replay;

    //    public override void Replay()
    //    {
    //        _waiting2replay = true;
    //    }

    //    protected virtual void OnReplay()
    //    {
    //        if (_items == null)
    //        {
    //            ResetValues();
    //        }
    //        _items.Clear();
    //        Transform grid = transform;
    //        for (int i = 0; i < grid.childCount; i++)
    //        {
    //            _items.Add(new TweenItem()
    //            {
    //                Delay = Duration + DeltaSeconds * i,
    //                Duration = Duration,
    //                Time = Duration + DeltaSeconds * i + Duration,
    //                Item = grid.GetChild(i),
    //            });
    //            UpdateItem(grid.GetChild(i), AnimationCurve.Evaluate(0));
    //        }
    //        _running = 1;
    //    }

    //    bool _waiting2play;

    //    public override void Play()
    //    {
    //        _waiting2play = true;
    //    }

    //    protected virtual void OnPlay()
    //    {
    //        if (_items == null)
    //        {
    //            ResetValues();
    //        }
    //        _running = 1;
    //    }

    //    protected virtual void Update()
    //    {
    //        if (_waiting2replay)
    //        {
    //            OnReplay();
    //            _waiting2replay = false;
    //            _waiting2play = false;
    //        }
    //        if (_waiting2play)
    //        {
    //            OnPlay();
    //            _waiting2replay = false;
    //            _waiting2play = false;
    //        }
    //        if (_running > 0)
    //        {
    //            for (int i = _items.Count - 1; i >= 0; i--)
    //            {
    //                _items[i].Time -= Time.deltaTime;
    //                float rate = Mathf.Clamp01(_items[i].Time / Duration);
    //                if (rate < 1 && rate >= 0)
    //                {
    //                    UpdateItem(_items[i].Item, AnimationCurve.Evaluate(1 - rate));
    //                }
    //                if (rate == 0)
    //                {
    //                    _items.RemoveAt(i);
    //                }
    //            }
    //            if (_items.Count == 0)
    //            {
    //                OnEnd();
    //            }
    //        }
    //    }

    //    protected virtual void OnDisable()
    //    {
    //        if (_running > 0)
    //        {
    //            OnEnd();
    //        }
    //    }

    //    protected virtual void OnEnd()
    //    {
    //        if (_items != null && _items.Count > 0)
    //        {
    //            foreach (TweenItem item in _items)
    //            {
    //                UpdateItem(item.Item, AnimationCurve.Evaluate(1));
    //            }
    //            _items.Clear();
    //        }
    //        _running = 0;
    //    }

    //    protected virtual void ResetValues()
    //    {
    //        _running = 0;
    //        Grid = gameObject.GetComponent<UIGrid>();
    //        if (_items == null)
    //        {
    //            _items = new List<TweenItem>();
    //        }
    //        Initialize();
    //    }

    //    public override bool IsPlaying()
    //    {
    //        return _running > 0;
    //    }

    //    protected class TweenItem
    //    {
    //        public float Delay { get; set; }
    //        public float Duration { get; set; }
    //        public float Time { get; set; }
    //        public Transform Item { get; set; }
    //    }
    //}
}