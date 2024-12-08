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
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using FantasyEngine.Common.Attribute;
using FantasyEngine.Log;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;
namespace FantasyEngine
{
    /// <summary>
    /// 无限循环的ScrollView
    /// </summary>
    [RequireComponent(typeof(ScrollRect))]
    public class CScrollView : MonoBehaviour
    {
        [SerializeField] private ScrollRect _ScrollView;

        public ScrollRect ScrollView
        {
            get
            {
                if (_ScrollView == null)
                {
                    _ScrollView = GetComponent<ScrollRect>();
                }

                return _ScrollView;
            }
        }

        [SerializeField] private GameObject _Prefab;

        public GameObject Prefab
        {
            get
            {
                if (_Prefab == null)
                {
                    _Prefab = Content.transform.GetChild(0).gameObject;
                }

                return _Prefab;
            }
        }

        /// <summary>
        /// 是否是竖向
        /// </summary>
        public bool IsVertical = true;

        /// <summary>
        /// 每行多少组数据
        ///</summary>
        public int Limit = 0;

        /// <summary>
        /// 聚焦位置
        /// </summary>
        public Aligns Align = Aligns.Default;

        /// <summary>
        /// 是否反向
        /// </summary>
        public bool Revert = false;

        /// <summary>
        /// 是否列表循环
        /// </summary>
        [SerializeField] private bool _Loop = false;

        private bool _CanLoop
        {
            get { return TotalLines > Lines; }
        }

        public bool Loop
        {
            get { return _Loop && _CanLoop; }
        }

        [SerializeField] private RectTransform _Content;

        public RectTransform Content
        {
            get
            {
                if (_Content == null)
                {
                    _Content = ScrollView.content;
                }

                return _Content;
            }
        }

        #region AutoVariable

        /// <summary>
        /// 总数
        /// </summary>
        public int TotalCount { get; private set; }

        [SerializeField, ReadOnly] private int _TotalLines;

        /// <summary>
        /// 总行数
        /// </summary>
        public int TotalLines
        {
            get { return _TotalLines; }
            private set { _TotalLines = value; }
        }

        [SerializeField, ReadOnly] private RectTransform _ScrollViewRect;

        public Vector2 ScrollViewSize
        {
            get
            {
                if (_ScrollViewRect == null)
                {
                    _ScrollViewRect = ScrollView.GetComponent<RectTransform>();
                }

                return _ScrollViewRect.rect.size;
            }
        }

        [SerializeField, ReadOnly] private Vector2 _ItemSize;

        public Vector2 ItemSize
        {
            get { return _ItemSize; }
        }

        [SerializeField, ReadOnly] private int _Lines;

        public int Lines
        {
            get { return _Lines; }
        }

        public int VisibleLines { get; private set; }

        public int VisibleCount { get; private set; }

        private Transform[,] Items;

        /// <summary>
        /// 组件匹配信息
        /// </summary>
        private Dictionary<Transform, RefreshCacheInfo> ItemsCache;

        /// <summary>
        /// 等待刷新的组件
        /// </summary>
        private Dictionary<Transform, RefreshCacheInfo> RefreshCache;

        [SerializeField, ReadOnly] private Vector2 _StartPosition;

        public Vector2 StartPosition
        {
            get { return _StartPosition; }
        }

        [SerializeField, ReadOnly] private int _TopLine;

        public int TopLine
        {
            get
            {
                if (_TopLine < 0)
                {
                    return _TopLine + TotalLines;
                }

                return _TopLine;
            }
            set
            {
                if (value < 0 && !Loop)
                {
                    _TopLine = 0;
                }
                else
                {
                    _TopLine = value;
                }

                _BottomLine = _TopLine + Lines - 1;
                _MiddleLine = (_TopLine + _BottomLine) / 2;
                if (_MiddleLine < 0)
                {
                    _MiddleLine += (-_MiddleLine / TotalLines + 1) * TotalLines;
                }
                else if (_MiddleLine >= TotalLines)
                {
                    _MiddleLine -= (_MiddleLine / TotalLines) * TotalLines;
                }

                int delta = Lines / 2;
                _TopLine = MiddleLine - delta;
                _BottomLine = MiddleLine + delta;
            }
        }

        [SerializeField, ReadOnly] private int _MiddleLine;

        public int MiddleLine
        {
            get { return _MiddleLine; }
            set
            {
                _MiddleLine = value;
                if (_MiddleLine < -TotalLines)
                {
                    _MiddleLine += (-_MiddleLine / TotalLines + 1) * TotalLines;
                }
                else if (_MiddleLine >= 2 * TotalLines)
                {
                    _MiddleLine -= (_MiddleLine / TotalLines) * TotalLines;
                }

                int delta = Lines / 2;
                _TopLine = MiddleLine - delta;
                _BottomLine = MiddleLine + delta;
                if (_TopLine < 0 && !_Loop)
                {
                    TopLine = 0;
                }
                else if (_BottomLine >= TotalLines && !Loop)
                {
                    BottomLine = TotalLines - 1;
                }
            }
        }

        [SerializeField, ReadOnly] private int _BottomLine;

        public int BottomLine
        {
            get
            {
                if (_BottomLine >= TotalLines)
                {
                    return _BottomLine - TotalLines;
                }

                return _BottomLine;
            }
            set
            {
                if (value >= TotalLines && !Loop)
                {
                    _BottomLine = TotalLines - 1;
                }
                else
                {
                    _BottomLine = value;
                }

                _TopLine = _BottomLine - Lines + 1;
                _MiddleLine = (_TopLine + _BottomLine) / 2;
                if (_MiddleLine < 0)
                {
                    _MiddleLine += (-_MiddleLine / TotalLines + 1) * TotalLines;
                }
                else if (_MiddleLine >= TotalLines)
                {
                    _MiddleLine -= (_MiddleLine / TotalLines) * TotalLines;
                }

                int delta = Lines / 2;
                _TopLine = MiddleLine - delta;
                _BottomLine = MiddleLine + delta;
            }
        }

        private bool Initialized = false;

        #endregion

        public void Initialize()
        {
            Initialized = true;
            var scrollRect = ScrollView.viewport.GetComponent<RectTransform>();
            if (IsVertical)
            {
                scrollRect.pivot = new Vector2(0.5f, 1);
            }
            else
            {
                scrollRect.pivot = new Vector2(0, 0.5f);
            }

            Prefab.transform.SetParent(Content);
            _ItemSize = Prefab.GetComponent<RectTransform>().rect.size;
            int visibleLines = 0;
            if (IsVertical)
            {
                ScrollView.vertical = true;
                ScrollView.horizontal = false;
                _Lines = Mathf.CeilToInt(ScrollViewSize.y / ItemSize.y) + 2;
                visibleLines = Mathf.FloorToInt(ScrollViewSize.y / ItemSize.y);
                if (Limit == 0)
                {
                    Limit = Mathf.FloorToInt(ScrollViewSize.x / ItemSize.x);
                }

                Content.anchorMin = new Vector2(0.5f, 1);
                Content.anchorMax = new Vector2(0.5f, 1);
                Content.pivot = new Vector2(0.5f, 1f);
            }
            else
            {
                ScrollView.vertical = false;
                ScrollView.horizontal = true;
                _Lines = Mathf.CeilToInt(ScrollViewSize.x / ItemSize.x) + 2;
                visibleLines = Mathf.FloorToInt(ScrollViewSize.x / ItemSize.x);
                if (Limit == 0)
                {
                    Limit = Mathf.FloorToInt(ScrollViewSize.y / ItemSize.y);
                }

                Content.anchorMin = new Vector2(0, 0.5f);
                Content.anchorMax = new Vector2(0, 0.5f);
                Content.pivot = new Vector2(0, 0.5f);
            }

            if (_Lines % 2 == 0)
            {
                _Lines += 1;
            }

            VisibleLines = visibleLines;
            VisibleCount = visibleLines * Limit;
            if (Items != null && Items.Length > 0)
            {
                for (int i = Items.Length - 1; i >= 0; i--)
                {
                    int line = i / Limit;
                    int row = i % Limit;
                    if (Items[line, row] == Prefab.transform)
                    {
                        continue;
                    }

                    GameObject.Destroy(Items[line, row].gameObject);
                }
            }

            if (ItemsCache == null)
            {
                ItemsCache = new Dictionary<Transform, RefreshCacheInfo>();
            }

            if (RefreshCache == null)
            {
                RefreshCache = new Dictionary<Transform, RefreshCacheInfo>();
            }

            Items = new Transform[Lines, Limit];
            for (int line = 0; line < Lines; line++)
            {
                for (int row = 0; row < Limit; row++)
                {
                    Transform transform;
                    if (line == 0 && row == 0)
                    {
                        transform = Prefab.transform;
                    }
                    else
                    {
                        transform = FEUtility.Instantiate(Prefab, Prefab.transform.parent).transform;
                    }

                    RectTransform rect = transform.GetComponent<RectTransform>();
                    if (IsVertical)
                    {
                        rect.anchorMin = new Vector2(0.5f, 1);
                        rect.anchorMax = new Vector2(0.5f, 1);
                        rect.pivot = new Vector2(0.5f, 1);
                    }
                    else
                    {
                        rect.anchorMin = new Vector2(0, 0.5f);
                        rect.anchorMax = new Vector2(0, 0.5f);
                        rect.pivot = new Vector2(0, 0.5f);
                    }

                    Items[line, row] = transform;
                    ItemsCache.Add(transform, new RefreshCacheInfo(-1, -1, -1, false));
                }
            }
        }

        public void SetTotalCount(int count, bool reposition = true)
        {
            bool initialized = Initialized;
            if (!Initialized)
            {
                Initialize();
            }

            if (TotalCount == count && !reposition)
            {
                Refresh();
            }

            TotalCount = count;
            if (count == 0)
            {
                //Debug.LogError("CScrollView.SetTotalCount must > 0.");
                gameObject.SetActive(false);
                return;
            }

            gameObject.SetActive(true);

            TotalLines = Mathf.CeilToInt(TotalCount * 1.0f / Limit);
            Vector2 contentSize = Content.rect.size;
            _StartPosition = Vector2.zero;
            if (IsVertical)
            {
                float width = Limit * ItemSize.x;
                if (Loop)
                {
                    contentSize = new Vector2(ScrollViewSize.x, ItemSize.y * TotalLines + 2 * ScrollViewSize.y);
                    if (Revert)
                    {
                        _StartPosition.Set(width / 2 - ItemSize.x / 2, -contentSize.y + ScrollViewSize.y + ItemSize.y);
                    }
                    else
                    {
                        _StartPosition.Set(-width / 2 + ItemSize.x / 2, -ScrollViewSize.y);
                    }
                }
                else
                {
                    contentSize = new Vector2(ScrollViewSize.x, Mathf.Max(ItemSize.y * TotalLines, ScrollViewSize.y));
                    if (Revert)
                    {
                        _StartPosition.Set(width / 2 - ItemSize.x / 2, -contentSize.y + ItemSize.y);
                    }
                    else
                    {
                        _StartPosition.Set(-width / 2 + ItemSize.x / 2, 0);
                    }
                }
            }
            else
            {
                float height = Limit * ItemSize.y;
                if (Loop)
                {
                    contentSize = new Vector2(ItemSize.x * TotalLines + 2 * ScrollViewSize.x, ScrollViewSize.y);
                    if (Revert)
                    {
                        _StartPosition.Set(contentSize.x - ScrollViewSize.x - ItemSize.x, -height / 2 + ItemSize.y / 2);
                    }
                    else
                    {
                        _StartPosition.Set(ScrollViewSize.x, height / 2 - ItemSize.y / 2);
                    }
                }
                else
                {
                    contentSize = new Vector2(Mathf.Max(ItemSize.x * TotalLines, ScrollViewSize.x), ScrollViewSize.y);
                    if (Revert)
                    {
                        _StartPosition.Set(contentSize.x - ItemSize.x, -height / 2 + ItemSize.y / 2);
                    }
                    else
                    {
                        _StartPosition.Set(0, height / 2 - ItemSize.y / 2);
                    }
                }
            }

            Content.sizeDelta = contentSize;
            if (TotalLines <= Lines || reposition || !initialized)
            {
                Reposition();
                //if (TotalLines <= Lines)
                //{
                //    ScrollView.movementType = ScrollRect.MovementType.Clamped;
                //}
                //else
                //{
                //    ScrollView.movementType = ScrollRect.MovementType.Elastic;
                //}
            }
            else
            {
                BottomLine = BottomLine;
            }

            for (int line = 0; line < Lines; line++)
            {
                for (int row = 0; row < Limit; row++)
                {
                    ItemsCache[Items[line, row]] = new RefreshCacheInfo(-1, -1, -1, false);
                }
            }

            ComputeIndexs();
        }

        public void JumpTo(int index)
        {
            if (index >= TotalCount)
            {
                index = TotalCount - 1;
            }

            if (index < 0)
            {
                index = 0;
            }

            int line = Mathf.FloorToInt(index * 1.0f / Limit);
            if (Align == Aligns.Center)
            {
                if (IsVertical)
                {
                    float y = ItemSize.y * line + ItemSize.y / 2;
                    if (Content.sizeDelta.y - y <= ScrollViewSize.y / 2.0f)
                    {
                        y = Content.sizeDelta.y - ScrollViewSize.y / 2.0f;
                    }
                    else if (y < ScrollViewSize.y / 2.0f)
                    {
                        y = ScrollViewSize.y / 2.0f;
                    }

                    if (Revert)
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - y - ScrollViewSize.y * 3 / 2);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - y - ScrollViewSize.y / 2);
                        }
                    }
                    else
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.up * (y + ScrollViewSize.y / 2);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.up * (y - ScrollViewSize.y / 2);
                        }
                    }
                }
                else
                {
                    float x = ItemSize.x * line + ItemSize.x / 2;
                    if (Revert)
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector3.left * (Content.sizeDelta.x - x - ScrollViewSize.x * 3 / 2);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector3.left * (Content.sizeDelta.x - x - ScrollViewSize.x / 2);
                        }
                    }
                    else
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector3.left * (x + ScrollViewSize.x / 2);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector3.left * (x - ScrollViewSize.x / 2);
                        }
                    }
                }
            }
            else
            {
                if (IsVertical)
                {
                    float y = ItemSize.y * line;
                    if (Revert)
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - y - 2 * ScrollViewSize.y);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - y - ScrollViewSize.y);
                        }
                    }
                    else
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.up * (y + ScrollViewSize.y);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.up * y;
                        }
                    }
                }
                else
                {
                    float x = ItemSize.x * line;
                    if (Revert)
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.left * (Content.sizeDelta.x - x - 2 * ScrollViewSize.x);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.left * (Content.sizeDelta.x - x - ScrollViewSize.x);
                        }
                    }
                    else
                    {
                        if (Loop)
                        {
                            Content.anchoredPosition = Vector2.left * (x + ScrollViewSize.x);
                        }
                        else
                        {
                            Content.anchoredPosition = Vector2.left * x;
                        }
                    }
                }
            }
        }

        bool _CallAfterReposition = false;

        public void Reposition()
        {
            _CallAfterReposition = true;
            TopLine = 0;
            if (IsVertical)
            {
                if (Revert)
                {
                    if (Loop)
                    {
                        Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - 2 * ScrollViewSize.y);
                    }
                    else
                    {
                        Content.anchoredPosition = Vector2.up * (Content.sizeDelta.y - ScrollViewSize.y);
                    }
                }
                else
                {
                    if (Loop)
                    {
                        Content.anchoredPosition = Vector2.up * (ScrollViewSize.y);
                    }
                    else
                    {
                        Content.anchoredPosition = Vector3.zero;
                    }
                }
            }
            else
            {
                if (Revert)
                {
                    if (Loop)
                    {
                        Content.anchoredPosition = Vector3.left * (Content.sizeDelta.x - 2 * ScrollViewSize.x);
                    }
                    else
                    {
                        Content.anchoredPosition = Vector3.left * (Content.sizeDelta.x - ScrollViewSize.x);
                    }
                }
                else
                {
                    if (Loop)
                    {
                        Content.anchoredPosition = Vector3.left * (ScrollViewSize.x);
                    }
                    else
                    {
                        Content.anchoredPosition = Vector3.zero;
                    }
                }
            }
        }

        private void BeforeLateUpdate()
        {
            int middleLine = GetMiddleLine();
            if (middleLine < 0 || middleLine > TotalLines)
            {
                MoveContent(middleLine);
                middleLine = GetMiddleLine();
            }

            if (MiddleLine != middleLine)
            {
                if (!Loop)
                {
                    if (TopLine == 0 && MiddleLine >= middleLine)
                    {
                        return;
                    }

                    if (BottomLine == TotalLines - 1 && MiddleLine <= middleLine)
                    {
                        return;
                    }
                }

                MiddleLine = middleLine;
                ComputeIndexs();
            }
        }

        private void MoveContent(int middleLine)
        {
            if (middleLine < 0)
            {
                if (TotalLines < Lines * 3)
                {
                    return;
                }

                if (IsVertical)
                {
                    if (Revert)
                    {
                        Content.anchoredPosition -= Vector2.up * (ItemSize.y * TotalLines);
                    }
                    else
                    {
                        Content.anchoredPosition += Vector2.up * (ItemSize.y * TotalLines);
                    }
                }
                else
                {
                    if (Revert)
                    {
                        Content.anchoredPosition -= Vector2.left * (ItemSize.x * TotalLines);
                    }
                    else
                    {
                        Content.anchoredPosition += Vector2.left * (ItemSize.x * TotalLines);
                    }
                }
            }
            else
            {
                if (TotalLines < Lines)
                {
                    return;
                }

                if (IsVertical)
                {
                    if (Revert)
                    {
                        Content.anchoredPosition += Vector2.up * (ItemSize.y * TotalLines);
                    }
                    else
                    {
                        Content.anchoredPosition -= Vector2.up * (ItemSize.y * TotalLines);
                    }
                }
                else
                {
                    if (Revert)
                    {
                        Content.anchoredPosition += Vector2.left * (ItemSize.x * TotalLines);
                    }
                    else
                    {
                        Content.anchoredPosition -= Vector2.left * (ItemSize.x * TotalLines);
                    }
                }
            }
        }

        private void ComputeIndexs()
        {
            if (Items == null)
            {
                return;
            }

            for (int line = _TopLine; line <= _BottomLine; line++)
            {
                for (int row = 0; row < Limit; row++)
                {
                    int tLine = line;
                    if (TotalLines > Lines)
                    {
                        if (tLine < 0)
                        {
                            tLine += TotalLines;
                        }
                        else if (tLine >= TotalLines)
                        {
                            tLine -= TotalLines;
                        }
                    }

                    int lineIndex = line % Lines;
                    if (lineIndex < 0)
                    {
                        lineIndex += Lines;
                    }

                    Transform item = Items[lineIndex, row];
                    int index = tLine * Limit + row;
                    bool active = index >= 0 && index < TotalCount;
                    if (RefreshCache.ContainsKey(item))
                    {
                        RefreshCache[item] = new RefreshCacheInfo(index, line, row, active);
                    }
                    else
                    {
                        RefreshCache.Add(item, new RefreshCacheInfo(index, line, row, active));
                    }
                }
            }
        }

        private void LateUpdate()
        {
            if (!Initialized) return;
            if (TotalCount == 0) return;
            BeforeLateUpdate();
            UpdatePositionAndIndex();
            if (_CallAfterReposition)
            {
                _CallAfterReposition = false;
                AfterReposition.Invoke();
            }
        }

        private void UpdatePositionAndIndex()
        {
            if (RefreshCache.Count > 0)
            {
                foreach (var kv in RefreshCache)
                {
                    Transform item = kv.Key;
                    var info = kv.Value;
                    if (ItemsCache[item] != info)
                    {
                        ItemsCache[item] = info;
                        UpdatePosition(item, info.Line, info.Row);
                        item.gameObject.SetActive(info.Active);
                        if (info.Active)
                        {
                            try
                            {
                                OnItemUpdated.Invoke(info.Index, item);
                            }
                            catch (Exception e)
                            {
                                FEConsole.WriteException(e);
                            }
                        }
                    }
                }

                RefreshCache.Clear();
            }
        }

        private void UpdatePosition(Transform item, int line, int row)
        {
            RectTransform rect = item.GetComponent<RectTransform>();
            if (IsVertical)
            {
                if (Revert)
                {
                    rect.anchoredPosition = _StartPosition - new Vector2(row * ItemSize.x, -line * ItemSize.y);
                }
                else
                {
                    rect.anchoredPosition = _StartPosition + new Vector2(row * ItemSize.x, -line * ItemSize.y);
                }
            }
            else
            {
                if (Revert)
                {
                    rect.anchoredPosition = _StartPosition - new Vector2(line * ItemSize.x, -row * ItemSize.y);
                }
                else
                {
                    rect.anchoredPosition = _StartPosition + new Vector2(line * ItemSize.x, -row * ItemSize.y);
                }
            }
        }

        private int GetMiddleLine()
        {
            int result;
            if (Loop)
            {
                if (IsVertical)
                {
                    float y = Content.anchoredPosition.y + ScrollViewSize.y / 2 - ScrollViewSize.y;
                    int index = (int)(y / ItemSize.y);
                    result = Revert ? TotalLines - index : index;
                }
                else
                {
                    float x = -Content.anchoredPosition.x + ScrollViewSize.x / 2 - ScrollViewSize.x;
                    int index = (int)(x / ItemSize.x);
                    result = Revert ? TotalLines - index : index;
                }
            }
            else
            {
                if (IsVertical)
                {
                    float y = Content.anchoredPosition.y + ScrollViewSize.y / 2;
                    int index = (int)(y / ItemSize.y);
                    result = Revert ? TotalLines - index : index;
                }
                else
                {
                    float x = -Content.anchoredPosition.x + ScrollViewSize.x / 2;
                    int index = (int)(x / ItemSize.x);
                    result = Revert ? TotalLines - index : index;
                }
            }

            if (result < -TotalLines)
            {
                result += TotalLines;
            }
            else if (result >= 2 * TotalLines)
            {
                result -= TotalLines;
            }

            return result;
        }

        public void Refresh()
        {
            if (!gameObject.activeSelf)
            {
                return;
            }

            foreach (var kv in ItemsCache)
            {
                var info = kv.Value;
                if (info.Active)
                {
                    try
                    {
                        OnItemUpdated.Invoke(info.Index, kv.Key);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteException(e);
                    }
                }
            }
        }

        [SerializeField] private UnityEvent _AfterReposition;

        public UnityEvent AfterReposition
        {
            get
            {
                if (_AfterReposition == null)
                {
                    _AfterReposition = new UnityEvent();
                }

                return _AfterReposition;
            }
            set
            {
                if (_AfterReposition == null)
                {
                    _AfterReposition = new UnityEvent();
                }

                if (value == null)
                {
                    _AfterReposition.RemoveAllListeners();
                }
                else
                {
                    _AfterReposition = value;
                }
            }
        }

        [SerializeField] private LoopScrollViewEvent _OnItemUpdated;

        public LoopScrollViewEvent OnItemUpdated
        {
            get
            {
                if (_OnItemUpdated == null)
                {
                    _OnItemUpdated = new LoopScrollViewEvent();
                }

                return _OnItemUpdated;
            }
            set
            {
                if (_OnItemUpdated == null)
                {
                    _OnItemUpdated = new LoopScrollViewEvent();
                }

                if (value == null)
                {
                    _OnItemUpdated.RemoveAllListeners();
                }
                else
                {
                    _OnItemUpdated = value;
                }
            }
        }


        public Transform[] GetItemsByLine(int lineNum)
        {
            int line = lineNum % Lines;
            if (line < 0)
            {
                line += Lines;
            }

            Transform[] items = new Transform[Limit];
            for (int i = 0; i < Limit; i++)
            {
                items[i] = Items[line, i];
            }

            return items;
        }

        private struct RefreshCacheInfo
        {
            public int Index { get; set; }
            public int Line { get; set; }
            public int Row { get; set; }
            public bool Active { get; set; }

            public RefreshCacheInfo(int index, int line, int row, bool active)
            {
                Index = index;
                Line = line;
                Row = row;
                Active = active;
            }

            public static bool operator ==(RefreshCacheInfo info1, RefreshCacheInfo info2)
            {
                return info1.Index == info2.Index && info1.Line == info2.Line && info1.Row == info2.Row &&
                       info1.Active == info2.Active;
            }

            public static bool operator !=(RefreshCacheInfo info1, RefreshCacheInfo info2)
            {
                return !(info1 == info2);
            }

            public override bool Equals(object obj)
            {
                if (!(obj is RefreshCacheInfo))
                {
                    return false;
                }

                var info = (RefreshCacheInfo)obj;
                return Index == info.Index &&
                       Line == info.Line &&
                       Row == info.Row &&
                       Active == info.Active;
            }

            public override int GetHashCode()
            {
                var hashCode = 1644092300;
                hashCode = hashCode * -1521134295 + Index.GetHashCode();
                hashCode = hashCode * -1521134295 + Line.GetHashCode();
                hashCode = hashCode * -1521134295 + Row.GetHashCode();
                hashCode = hashCode * -1521134295 + Active.GetHashCode();
                return hashCode;
            }
        }

        public enum Aligns
        {
            Default = 0,
            Center = 1,
        }

        [Serializable]
        public class LoopScrollViewEvent : UnityEvent<int, Transform>
        {
        }
    }
}