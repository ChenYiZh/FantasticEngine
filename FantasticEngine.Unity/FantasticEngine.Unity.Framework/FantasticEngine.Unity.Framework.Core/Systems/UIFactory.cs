/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using FantasticEngine.Log;
using FantasticEngine.UI;
using UnityEngine;

namespace FantasticEngine
{
    public class TipsLoadingProgress : EventParam
    {
        public float Progress { get; set; }
    }
    /// <summary>
    /// UI控制类
    /// </summary>
    public sealed class UIFactory : SystemBasis<UIFactory>
    {
        public UI_Tips UITips { get; private set; }

        public IReadOnlyDictionary<EUIID, PanelConfig> PanelConfigs { get; private set; }

        public UIBasePanel TopPanel
        {
            get
            {
                if (PanelStack.Count > 0)
                {
                    return PanelStack[PanelStack.Count - 1].Panel;
                }

                return null;
            }
        }

        private Dictionary<EUIID, GameObject> _panelCache { get; set; }

        public List<PanelStackInfo> PanelStack { get; private set; }


        private readonly EUILevel[] LevelEnum = new EUILevel[]
            { EUILevel.BottomAll, EUILevel.Bottom, EUILevel.Middle, EUILevel.Top, EUILevel.TopAll };

        public override void Initialize()
        {
            base.Initialize();
            PanelConfigs = new Dictionary<EUIID, PanelConfig>();
            PanelStack = new List<PanelStackInfo>();
            _panelCache = new Dictionary<EUIID, GameObject>();
            GlobalConfig.RegistPanels(this);
            InitModules();
        }

        public void InitModules()
        {
            var res = ResourceManager.Instance.Load(GameDefines.Instance.ResourceUITipsPath + "UI_Tips");
            GameObject obj = null;
            if (res != null)
            {
                obj = FEUtility.Instantiate(res, GameRoot.Root.UIRoot.transform);
            }

            if (obj != null)
            {
                UITips = FEUtility.GetOrAddComponent<UI_Tips>(obj);
                //UI_Tips.Show(ETips.Tips_Loading);
            }
        }

        public void Push(EUIID uiid, PanelParam param = null)
        {
            if (!PanelConfigs.ContainsKey(uiid))
            {
                return;
            }

            GameRoot.Root.StartCoroutine(PushPanel(uiid, param));
        }

        private IEnumerator PushPanel(EUIID uiid, PanelParam param)
        {
            var config = PanelConfigs[uiid];
            GameObject prefab = null;
            if (_panelCache.ContainsKey(uiid))
            {
                prefab = _panelCache[uiid];
            }
            else
            {
                UnityEngine.Object asset = null;
                yield return ResourceManager.Instance.LoadAsync(
                    GameDefines.Instance.ResourceUIPanelPath + config.PrefabName, (res) => { asset = res; });
                if (asset != null)
                {
                    prefab = FEUtility.Instantiate(asset, GameRoot.Root.UIRoot.transform);
                }
            }

            if (prefab == null)
            {
                FEConsole.Write("can not find uiId :" + uiid.ToString());
                yield break;
            }

            int newIndex = GetValidedIndex(config.UILevel, prefab.transform);


            prefab.transform.SetSiblingIndex(newIndex);
            if (!_panelCache.ContainsKey(uiid))
            {
                _panelCache.Add(uiid, prefab);
            }

            bool inited = prefab.GetComponent<UIBasePanel>() != null;
            UIBasePanel panel = config.GetOrAddComponent(prefab);
            panel.enabled = false;
            if (panel == null)
            {
                yield break;
            }

            //执行隐藏界面
            var topPanel = TopPanel;

            panel.gameObject.SetActive(true);
            PanelStack.Add(new PanelStackInfo() { UIID = uiid, Param = param, Panel = panel });
            if (!inited)
            {
                try
                {
                    panel.OnCreate();
                    foreach (IVisual visual in panel.Visuals)
                    {
                        try
                        {
                            visual.OnCreate();
                        }
                        catch (Exception e)
                        {
                            FEConsole.WriteException(e);
                        }
                    }
                }
                catch (Exception e)
                {
                    FEConsole.WriteException(e);
                }
            }

            try
            {
                panel.OnPanelOpen(param);
                foreach (IVisual visual in panel.Visuals)
                {
                    try
                    {
                        visual.OnPanelOpen(param);
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            try
            {
                panel.BeforePlayDisplayAnimation();
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            if (config.AnimConfig.AnimIn != EUIAnim.None)
            {
                yield return FETools.PlayAnim(panel, config.AnimConfig, true);
            }
            else
            {
                var canvas = panel.GameObject.GetComponent<CanvasGroup>();
                canvas.alpha = 1;
            }

            try
            {
                panel.OnShow();
                foreach (IVisual visual in panel.Visuals)
                {
                    try
                    {
                        visual.OnShow();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            panel.enabled = true;

            if (topPanel != null)
            {
                try
                {
                    if (!config.Transparent)
                    {
                        topPanel.gameObject.SetActive(false);
                    }

                    topPanel.OnHide();
                    foreach (IVisual visual in topPanel.Visuals)
                    {
                        try
                        {
                            visual.OnHide();
                        }
                        catch (Exception e)
                        {
                            FEConsole.WriteException(e);
                        }
                    }

                    if (!config.Transparent)
                    {
                        topPanel.enabled = false;
                    }
                }
                catch (Exception e)
                {
                    FEConsole.WriteException(e);
                }
            }
        }

        private int GetValidedIndex(EUILevel level, Transform newUI)
        {
            int index = 0;
            for (int i = PanelStack.Count - 1; i >= 0; i--)
            {
                var panel = PanelStack[i].Panel;
                var config = PanelConfigs[panel.UIID];
                if ((int)config.UILevel <= (int)level)
                {
                    return panel.transform.GetSiblingIndex() + (newUI == panel.transform ? 0 : 1);
                    //return panel.transform.GetSiblingIndex() + 1;
                }
            }

            return index;
        }

        public void Pop(EUIID uiid)
        {
            if (!PanelConfigs.ContainsKey(uiid))
            {
                return;
            }

            UIBasePanel panel = null;
            var config = PanelConfigs[uiid];
            for (int i = PanelStack.Count - 1; i >= 0; i--)
            {
                var info = PanelStack[i];
                if (info.UIID == uiid)
                {
                    panel = info.Panel;
                    PanelStack.RemoveAt(i);
                    break;
                }
            }

            if (panel == null)
            {
                return;
            }

            GameRoot.Root.StartCoroutine(PopPanel(panel, config));
        }

        public void PopAll(bool destroyPanels = false)
        {
            //HashSet<EUIID> ids = new HashSet<EUIID>();
            for (int i = PanelStack.Count - 1; i >= 0; i--)
            {
                var stack = PanelStack[i];
                //if (ids.Contains(stack.UIID))
                //{
                //    continue;
                //}
                var config = PanelConfigs[stack.UIID];
                var panel = stack.Panel;
                //ids.Add(stack.UIID);
                PanelStack.RemoveAt(i);
                if (panel)
                {
                    GameRoot.Root.StartCoroutine(PopPanel(panel, config, false));
                }

                if (destroyPanels && panel)
                {
                    GameObject.Destroy(panel.gameObject);
                }
            }

            PanelStack.Clear();
            if (destroyPanels)
            {
                foreach (var kv in _panelCache)
                {
                    if (kv.Value)
                    {
                        GameObject.Destroy(kv.Value);
                    }
                }

                _panelCache.Clear();
            }
        }

        private IEnumerator PopPanel(UIBasePanel panel, PanelConfig config, bool useAnim = true)
        {
            UI_Tips.Hide(ETip.Tips_Keyboard);
            //先执行，以防背景空了
            UIBasePanel nextPanel = null;
            if (PanelStack.Count > 0)
            {
                nextPanel = PanelStack[PanelStack.Count - 1].Panel;
            }

            if (nextPanel != null)
            {
                if (!nextPanel.gameObject.activeSelf)
                {
                    nextPanel.gameObject.SetActive(true);
                    try
                    {
                        nextPanel.OnShow();
                        foreach (IVisual visual in nextPanel.Visuals)
                        {
                            try
                            {
                                visual.OnShow();
                            }
                            catch (Exception e)
                            {
                                FEConsole.WriteException(e);
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteException(e);
                    }

                    nextPanel.enabled = true;
                }
            }

            panel.enabled = false;
            if (panel.gameObject.activeSelf)
            {
                try
                {
                    panel.OnHide();
                    foreach (IVisual visual in panel.Visuals)
                    {
                        try
                        {
                            visual.OnHide();
                        }
                        catch (Exception e)
                        {
                            FEConsole.WriteException(e);
                        }
                    }
                }
                catch (Exception e)
                {
                    FEConsole.WriteException(e);
                }
            }

            try
            {
                panel.BeforePlayHideAnimation();
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            if (useAnim)
            {
                if (config.AnimConfig.AnimOut != EUIAnim.None)
                {
                    yield return FETools.PlayAnim(panel, config.AnimConfig, false);
                }
                else
                {
                    var canvas = panel.GameObject.GetComponent<CanvasGroup>();
                    canvas.alpha = 0;
                }
            }

            try
            {
                panel.OnPanelClosed();
                foreach (IVisual visual in panel.Visuals)
                {
                    try
                    {
                        visual.OnPanelClosed();
                    }
                    catch (Exception e)
                    {
                        FEConsole.WriteException(e);
                    }
                }
            }
            catch (Exception e)
            {
                FEConsole.WriteException(e);
            }

            panel.gameObject.SetActive(false);

            //if (nextPanel != null)
            //{
            //    nextPanel.transform.SetSiblingIndex(GetValidedIndex(PanelConfigs[nextPanel.UIID].UILevel));
            //}
        }

        /// <summary>
        /// 面板注册事件
        /// </summary>
        /// <typeparam name="T">UIBasePanel</typeparam>
        /// <param name="uiid">PanelID</param>
        /// <param name="prefabName">prefab名称</param>
        /// <param name="level">大层级</param>
        /// <param name="transparent">背景是否可以穿透</param>
        /// <param name="animIn">显示时播放的动画</param>
        /// <param name="animInSeconds">显示时动画播放时长</param>
        /// <param name="animOut">关闭时播放的动画</param>
        /// <param name="animOutSeconds">关闭时播放的动画时长</param>
        public void Regist<T>(
            EUIID uiid,
            string prefabName,
            EUILevel level = EUILevel.Middle,
            bool transparent = false,
            EUIAnim animIn = EUIAnim.Scale,
            float animInSeconds = 0.3f,
            EUIAnim animOut = EUIAnim.Opacity,
            float animOutSeconds = 0.3f)
            where T : UIBasePanel
        {
            if (PanelConfigs.ContainsKey(uiid))
            {
                FEConsole.WriteError("重复注册UI：" + uiid);
                return;
            }

            ((Dictionary<EUIID, PanelConfig>)PanelConfigs).Add(uiid, new PanelConfig<T>()
            {
                UIID = uiid,
                PrefabName = prefabName,
                UILevel = level,
                Transparent = transparent,
                AnimConfig = new AnimConfig()
                {
                    AnimIn = animIn,
                    AnimInSeconds = animInSeconds,
                    AnimOut = animOut,
                    AnimOutSeconds = animOutSeconds,
                },
            });
        }

        public abstract class PanelConfig
        {
            public EUIID UIID { get; set; }
            public string PrefabName { get; set; }
            public EUILevel UILevel { get; set; }
            public bool Transparent { get; set; }
            public AnimConfig AnimConfig { get; set; }

            public abstract UIBasePanel GetOrAddComponent(GameObject panel);
        }

        public sealed class PanelConfig<T> : PanelConfig where T : UIBasePanel
        {
            public override UIBasePanel GetOrAddComponent(GameObject panel)
            {
                T script = FEUtility.GetOrAddComponent<T>(panel);
                script.PanelConfig = this;
                return script;
            }
        }

        public sealed class PanelStackInfo
        {
            public EUIID UIID { get; set; }

            public PanelParam Param { get; set; }

            public UIBasePanel Panel { get; set; }
        }

        public sealed class SimplePanelStackInfo
        {
            public EUIID UIID { get; set; }

            public PanelParam Param { get; set; }
        }
    }

    public class PanelParam
    {
    }
}