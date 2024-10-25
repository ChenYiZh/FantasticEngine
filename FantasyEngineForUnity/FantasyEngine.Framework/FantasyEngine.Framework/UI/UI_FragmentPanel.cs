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
using FantasyEngine.Log;
using UnityEngine;
using UnityEngine.Events;
namespace FantasyEngine
{
    /// <summary>
    /// 一个UI下多分页切换结构
    /// </summary>
    #region 父类

    public class FragmentParam
    {
    }

    public class FragmentPanelParam : PanelParam
    {
        public int FragmentID { get; set; }
        public FragmentParam Param { get; set; }
    }

    public interface IFragment
    {
        int FragmentID { get; }
        bool Shown { get; }
        void Hide();
        void OnUpdate();
        void Show(FragmentParam param);
    }

    public interface IFragment<M, F> : IFragment where F : IFragment<M, F> where M : FragmentManagement<M, F>, new()
    {
        void Create(M mainView);
    }

    public abstract class UI_Fragment<M, F> : IFragment<M, F>
        where F : UI_Fragment<M, F> where M : FragmentManagement<M, F>, new()
    {
        public abstract int FragmentID { get; }

        private bool _initialized { get; set; }

        public bool Initialized
        {
            get { return _initialized; }
        }

        public bool Shown { get; private set; }

        protected M Management { get; private set; }

        protected UIBasePanel MainPanel
        {
            get { return Management.MainPanel; }
        }

        protected Transform transform { get; private set; }

        protected GameObject gameObject { get; private set; }

        public virtual void Create(M management)
        {
            Shown = false;
            _initialized = false;
            Management = management;
            transform = management.transform;
            gameObject = management.gameObject;
        }

        public virtual void Hide()
        {
            Shown = false;
            OnHide();
        }

        public virtual void Show(FragmentParam param)
        {
            if (!_initialized)
            {
                _initialized = true;
#if !UNITY_EDITOR
                try
                {
#endif
                    Initialize();
#if !UNITY_EDITOR
                }
                catch { }
#endif
            }
            if (Shown) return;
            Shown = true;
            Management.Select(FragmentID, param);
#if !UNITY_EDITOR
            try
            {
#endif
                OnShow(param);
#if !UNITY_EDITOR
            }
            catch { }
#endif
        }

        protected virtual void Initialize()
        {
        }

        public virtual void OnUpdate()
        {
        }

        protected virtual void OnHide()
        {
        }

        protected virtual void OnShow(FragmentParam param)
        {
        }

        public virtual Coroutine StartCoroutine(IEnumerator routine)
        {
            return Management.StartCoroutine(routine);
        }
    }

    public interface IFragmentManagement
    {
    }

    public abstract class FragmentManagement<M, F> : FEComponent, IFragmentManagement
        where F : IFragment<M, F> where M : FragmentManagement<M, F>, new()
    {
        public virtual int DefaultUIID { get; set; }
        Dictionary<int, F> _fragments;

        public IReadOnlyDictionary<int, F> Fragments
        {
            get { return _fragments; }
        }

        public F FocusFragment { get; private set; }

        public UIBasePanel MainPanel { get; private set; }

        public GameObject gameObject
        {
            get { return MainPanel.gameObject; }
        }

        public Transform transform
        {
            get { return MainPanel.transform; }
        }

        public event Action<int, FragmentParam> onSelect;

        public virtual void SetPanel(UIBasePanel mainPanel)
        {
            DefaultUIID = 0;
            MainPanel = mainPanel;
        }

        public virtual bool RegistFragment(F fragment)
        {
            if (_fragments == null)
            {
                _fragments = new Dictionary<int, F>();
            }

            if (_fragments.ContainsKey(fragment.FragmentID))
            {
                FConsole.WriteError("已经注册过FragmentID: " + fragment.FragmentID);
                return false;
            }

            fragment.Create((M)this);
            _fragments.Add(fragment.FragmentID, fragment);
            return true;
        }

        public virtual void Select(int fragmentid, FragmentParam param = null)
        {
            foreach (KeyValuePair<int, F> fragment in _fragments)
            {
                if (fragment.Key != fragmentid && fragment.Value.Shown)
                {
                    fragment.Value.Hide();
                }
            }

            if (!_fragments[fragmentid].Shown)
            {
                _fragments[fragmentid].Show(param);
            }

            FocusFragment = _fragments[fragmentid];
            if (onSelect != null)
            {
                onSelect(fragmentid, param);
            }
        }

        public virtual void OnOpen(FragmentPanelParam param = null)
        {
            if (param != null)
            {
                int id = param.FragmentID;
                Select(id, param.Param);
            }
            else
            {
                Select(DefaultUIID, null);
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            foreach (KeyValuePair<int, F> fragment in _fragments)
            {
                if (fragment.Value.Shown)
                {
#if !UNITY_EDITOR
                    try
                    {
#endif
                        fragment.Value.OnUpdate();
#if !UNITY_EDITOR
                    }
                    catch { }
#endif
                }
            }
        }

        public void OnClose()
        {
            foreach (KeyValuePair<int, F> fragment in _fragments)
            {
                if (fragment.Value.Shown)
                {
                    fragment.Value.Hide();
                }
            }
        }

        public override void OnDestroy()
        {
            base.OnDestroy();
            foreach (KeyValuePair<int, F> fragment in _fragments)
            {
                if (fragment.Value.Shown)
                {
                    fragment.Value.Hide();
                }
            }
        }

        public virtual Coroutine StartCoroutine(IEnumerator routine)
        {
            return MainPanel.StartCoroutine(routine);
        }
    }

    public abstract class UI_FragmentPanel<M, F> : UIVisual
        where F : IFragment<M, F> where M : FragmentManagement<M, F>, new()
    {
        public M Management { get; private set; }

        public virtual int DefaultUIID
        {
            get { return 0; }
        }

        public IReadOnlyDictionary<int, F> Fragments
        {
            get { return Management.Fragments; }
        }

        public F FocusFragment
        {
            get { return Management.FocusFragment; }
        }

        protected abstract void OnInitialize();

        public override void OnCreate()
        {
            Management = new M();
            Management.SetPanel(MainPanel);
            Management.onSelect += OnSelect;
            Management.DefaultUIID = DefaultUIID;
            OnInitialize();
            Management.Initialize();
        }

        protected virtual bool RegistFragment(F fragment)
        {
            if (Management != null)
            {
                return Management.RegistFragment(fragment);
            }
            else
            {
                return false;
            }
        }

        public virtual void Select(int fragmentid, FragmentParam param = null)
        {
            if (Management != null)
            {
                Management.Select(fragmentid, param);
            }
        }

        protected virtual void OnSelect(int fragmentid, FragmentParam param = null)
        {
        }

        public override void OnPanelOpen(PanelParam param = null)
        {
            base.OnPanelOpen(param);
            if (Management != null)
            {
                Management.OnOpen(param as FragmentPanelParam);
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (Management != null)
            {
                Management.OnUpdate();
            }
        }

        public override void OnPanelClosed()
        {
            base.OnPanelClosed();
            if (Management != null)
            {
                Management.OnClose();
            }
        }

        private void OnDestroy()
        {
            if (Management != null)
            {
                Management.OnDestroy();
            }
        }

        public void Awake()
        {
            if (Management != null)
            {
                Management.Awake();
            }
        }

        public void Start()
        {
            if (Management != null)
            {
                Management.Start();
            }
        }

        public override void OnLateUpdate()
        {
            base.OnLateUpdate();
            if (Management != null)
            {
                Management.OnLateUpdate();
            }
        }

        public override void OnFixedUpdate()
        {
            base.OnFixedUpdate();
            if (Management != null)
            {
                Management.OnFixedUpdate();
            }
        }

        private void OnEnable()
        {
            if (Management != null)
            {
                Management.OnEnable();
            }
        }

        private void OnDisable()
        {
            if (Management != null)
            {
                Management.OnDisable();
            }
        }
    }

    #endregion

    #region 纯净类

    public abstract class UI_Fragment : UI_Fragment<FragmentManagement, UI_Fragment>
    {
    }

    public class FragmentManagement : FragmentManagement<FragmentManagement, UI_Fragment>
    {
    }

    public abstract class UI_FragmentPanel : UI_FragmentPanel<FragmentManagement, UI_Fragment>
    {
    }

    #endregion

    #region 页签类型

    public abstract class UI_TabFragment : UI_Fragment<TabFragmentManagement, UI_TabFragment>
    {
        public abstract string ButtonRoute { get; }
        public GameObject Selection { get; private set; }
        public GameObject Button { get; private set; }

        public abstract string ViewRoute { get; }
        public GameObject View { get; private set; }

        public override void Create(TabFragmentManagement management)
        {
            base.Create(management);

            View = management.transform.Find(ViewRoute).gameObject;
            Button = management.transform.Find(ButtonRoute).gameObject;

            Selection = Button;
            if (!string.IsNullOrEmpty(management.SelectionRoute))
            {
                Transform selectionGrid = management.transform.Find(management.SelectionRoute);
                Transform selection = Selection.transform;
                for (int i = 0; i < 10 && selection.parent != null; i++)
                {
                    if (selection.parent == selectionGrid)
                    {
                        Selection = selection.gameObject;
                        break;
                    }

                    selection = selection.parent;
                }
            }
        }

        public override void Show(FragmentParam param = null)
        {
            base.Show(param);
            View.SetActive(true);
        }

        public override void Hide()
        {
            base.Hide();
            View.SetActive(false);
        }
    }

    public class TabFragmentManagement : FragmentManagement<TabFragmentManagement, UI_TabFragment>
    {
        public virtual string SelectionRoute { get; set; }

        Dictionary<GameObject, List<int>> _selections;

        public IReadOnlyDictionary<GameObject, List<int>> Selections
        {
            get { return _selections; }
        }

        public event Action<GameObject> onSelectSelections;

        //protected virtual bool Locked { get; set; }
        public bool CustomSelectAction { get; set; }

        public TabFragmentManagement()
        {
            CustomSelectAction = true;
        }

        public override void Initialize()
        {
            base.Initialize();
            SelectionRoute = null;
        }

        public override bool RegistFragment(UI_TabFragment fragment)
        {
            if (base.RegistFragment(fragment))
            {
                if (_selections == null)
                {
                    _selections = new Dictionary<GameObject, List<int>>();
                }

                if (CustomSelectAction)
                {
                    FETools.BindClick(fragment.Button, () => { fragment.Show(); });
                }

                if (CustomSelectAction && fragment.Button != fragment.Selection)
                {
                    FETools.BindClick(fragment.Selection, () => { Select(fragment.Selection); });
                }

                if (!_selections.ContainsKey(fragment.Selection))
                {
                    _selections.Add(fragment.Selection, new List<int>());
                }

                _selections[fragment.Selection].Add(fragment.FragmentID);

                return true;
            }
            else
            {
                return false;
            }
        }

        public void Select(GameObject selection)
        {
            if (onSelectSelections != null)
            {
                onSelectSelections(selection);
            }
        }

        public override void Select(int fragmentid, FragmentParam param = null)
        {
            base.Select(fragmentid, param);
            foreach (KeyValuePair<int, UI_TabFragment> fragment in Fragments)
            {
                ((UI_TabFragment)fragment.Value).View.SetActive(fragment.Key == fragmentid);
            }

            Select(((UI_TabFragment)Fragments[fragmentid]).Selection);
        }

        public override void OnOpen(FragmentPanelParam param)
        {
            //activityBtGrid.GetComponent<UIGrid>().repositionNow = true;

            UI_TabFragment selectionFragment = Fragments.Values.Where(f => f.Selection != f.Button).FirstOrDefault();
            if (selectionFragment != null)
            {
                Transform selectionGrid = selectionFragment.Selection.transform.parent;
                foreach (Transform sel in selectionGrid)
                {
                    if (!_selections.ContainsKey(sel.gameObject))
                    {
                        sel.gameObject.SetActive(false);
                    }
                }
            }

            Transform[] buttonParents = Fragments.Values.Select(f => ((UI_TabFragment)f).Button.transform.parent).Distinct()
                .ToArray();
            foreach (Transform buttonParent in buttonParents)
            {
                foreach (Transform button in buttonParent)
                {
                    if (Fragments.Values.Select(f => ((UI_TabFragment)f).Button == button.gameObject).Count() == 0)
                    {
                        button.gameObject.SetActive(false);
                    }
                }
            }

            base.OnOpen(param);

            RefreshSelections();
        }

        //IEnumerator ToClose()
        //{
        //    yield return null;
        //    Close();
        //}
        public virtual void RefreshSelections()
        {
            //if (Fragments.Values.All(f => !((UI_TabFragment)f).Button.activeSelf))
            //{
            //    StartCoroutine(ToClose());
            //    Locked = true;
            //    return;
            //}
            foreach (KeyValuePair<GameObject, List<int>> selection in _selections)
            {
                int[] fragment_ids = selection.Value.Where(k => ((UI_TabFragment)Fragments[k]).Button.gameObject.activeSelf)
                    .ToArray();
                selection.Key.SetActive(fragment_ids.Length > 0);
                if (CustomSelectAction)
                {
                    FETools.BindClick(selection.Key, () =>
                    {
                        int[] fragment_ids2 = selection.Value
                            .Where(k => ((UI_TabFragment)Fragments[k]).Button.gameObject.activeSelf).ToArray();
                        if (fragment_ids2.Length > 1)
                        {
                            int fragmentid = fragment_ids2.Select(k => ((UI_TabFragment)Fragments[k]))
                                .OrderBy(f => f.Button.transform.GetSiblingIndex()).First().FragmentID;
                            //Select(selection.Key);
                            Fragments[fragmentid].Show();
                        }
                        else if (fragment_ids2.Length == 1)
                        {
                            Fragments[fragment_ids2[0]].Show();
                        }
                    });
                }

                if (selection.Value.Count > 0)
                {
                    UI_TabFragment fragment = (UI_TabFragment)Fragments[selection.Value[0]];
                    if (fragment.Button != fragment.Selection)
                    {
                        Transform buttonGrid = fragment.Button.transform.parent;

                        for (int i = 0; i < buttonGrid.childCount; i++)
                        {
                            GameObject item = buttonGrid.transform.GetChild(i).gameObject;
                            if (item.activeSelf)
                            {
                                UI_TabFragment tf = Fragments.Values.FirstOrDefault(f => f.Button == item);
                                item.SetActive(tf != null);
                            }
                        }
                    }
                }
            }

            Transform selectionGrid = null;
            if (!string.IsNullOrEmpty(SelectionRoute))
            {
                selectionGrid = transform.Find(SelectionRoute);
            }
            else
            {
                selectionGrid = ((UI_TabFragment)Fragments.Values.First()).Selection.transform.parent;
            }

            for (int i = 0; i < selectionGrid.childCount; i++)
            {
                GameObject item = selectionGrid.transform.GetChild(i).gameObject;
                if (item.activeSelf)
                {
                    bool visible = _selections.ContainsKey(item) &&
                                   _selections[item].Any(id => ((UI_TabFragment)Fragments[id]).Button.activeSelf);
                    item.SetActive(visible);
                }
            }
        }
    }

    public abstract class UI_TabFragmentPanel : UI_FragmentPanel<TabFragmentManagement, UI_TabFragment>
    {
        Dictionary<GameObject, List<int>> _selections;

        public IReadOnlyDictionary<GameObject, List<int>> Selections
        {
            get { return _selections; }
        }

        protected virtual bool Locked { get; set; }

        public void Select(GameObject selection)
        {
            Management.Select(selection);
            OnSelect(selection);
        }

        protected virtual void OnSelect(GameObject selection)
        {
        }

        public virtual void RefreshSelections()
        {
            Management.RefreshSelections();
        }
    }

    #endregion
}