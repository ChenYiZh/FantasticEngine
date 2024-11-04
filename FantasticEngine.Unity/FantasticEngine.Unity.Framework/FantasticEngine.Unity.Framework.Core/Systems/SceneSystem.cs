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
using System.Linq;
using FantasticEngine.Log;
using FantasticEngine.UI;
using UnityEngine;
using UnityEngine.SceneManagement;


namespace FantasticEngine
{
    /// <summary>
    /// 场景管理类
    /// </summary>
    public class SceneSystem : SystemBasis<SceneSystem>
    {
        /// <summary>
        /// 加载着的场景
        /// </summary>
        public Dictionary<string, BaseScene> Scenes { get; private set; }

        /// <summary>
        /// 正在加载场景
        /// </summary>
        private List<BaseScene> _loadingScenes;

        /// <summary>
        /// 是否正在运行
        /// </summary>
        public bool IsLoading { get; private set; }

        /// <summary>
        /// 加载标识符
        /// </summary>
        private bool _doLoad = false;

        /// <summary>
        /// 加载进度
        /// </summary>
        public float Progress { get; private set; }

        /// <summary>
        /// 准备状态
        /// </summary>
        public override bool Ready => !IsLoading;

        /// <summary>
        /// 加载
        /// </summary>
        /// <param name="force">如果已经存在的场景，仍然加载吗？</param>
        public void Load(bool clearExists, params BaseScene[] scenes)
        {
            Load(clearExists, false, scenes);
        }
        /// <summary>
        /// 加载
        /// </summary>
        /// <param name="force">如果已经存在的场景，仍然加载吗？</param>
        public void Load(bool clearExists, bool force, params BaseScene[] scenes)
        {
            if (clearExists)
            {
                if (scenes == null || IsLoading)
                {
                    return;
                }

                UI_Tips.Show(ETip.Tips_Loading);

                List<BaseScene> waitToLoad = new List<BaseScene>(scenes.Length);
                HashSet<string> exists = new HashSet<string>();
                foreach (var scene in scenes)
                {
                    if (!force && Scenes.ContainsKey(scene.SceneName))
                    {
                        exists.Add(scene.SceneName);
                        continue;
                    }

                    waitToLoad.Add(scene);
                }

                BaseScene[] waitToRemove;
                if (exists.Count > 0)
                {
                    waitToRemove = Scenes.Values.Where(v => !exists.Contains(v.SceneName)).ToArray();
                }
                else
                {
                    waitToRemove = Scenes.Values.ToArray();
                }

                Unload(waitToRemove);

                foreach (var scene in waitToLoad)
                {
                    Progress = 0;
                    IsLoading = true;
                    _doLoad = true;
                    Scenes.Add(scene.SceneName, scene);
                    _loadingScenes.Add(scene);
                    GameRoot.Root.StartCoroutine(scene.LoadAsync());
                }
            }
            else
            {
                LoadNew(force, scenes);
            }
        }

        public void Load(params BaseScene[] scenes)
        {
            LoadNew(false, scenes);
        }
        /// <summary>
        /// 加载
        /// </summary>
        /// <param name="force">如果已经存在的场景，仍然加载吗？</param>
        /// <param name="scenes"></param>
        public void LoadNew(bool force, params BaseScene[] scenes)
        {
            if (scenes == null || IsLoading)
            {
                return;
            }

            //UI_Tips.Show(ETips.Tips_Loading);

            foreach (var scene in scenes)
            {
                if (!force && Scenes.ContainsKey(scene.SceneName))
                {
                    continue;
                }

                Progress = 0;
                IsLoading = true;
                _doLoad = true;
                Scenes.Add(scene.SceneName, scene);
                _loadingScenes.Add(scene);
                GameRoot.Root.StartCoroutine(scene.LoadAsync());
            }

            if (!IsLoading)
            {
                UI_Tips.Hide(ETip.Tips_Loading);
            }
        }

        /// <summary>
        /// 卸载
        /// </summary>
        public void Unload(params BaseScene[] scenes)
        {
            if (scenes == null || IsLoading)
            {
                return;
            }

            foreach (var scene in scenes)
            {
                if (!Scenes.ContainsKey(scene.SceneName))
                {
                    continue;
                }

                Progress = 0;
                _loadingScenes.Add(scene);
                IsLoading = true;
                _doLoad = false;
                Scenes.Remove(scene.SceneName);
                GameRoot.Root.StartCoroutine(scene.UnloadAsync());
            }
        }

        private void LoadCompleted()
        {
            EventSystem.Instance.Send(EEvent.SceneLoaded);
        }

        private void UnloadCompleted()
        {
            EventSystem.Instance.Send(EEvent.SceneUnloaded);
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (IsLoading)
            {
                bool isDone = _loadingScenes.All(s => s.IsDone);
                Progress = _loadingScenes.Sum(s => s.Progress) / _loadingScenes.Count;
                if (!isDone)
                {
                    EventSystem.Instance.Send(EEvent.LoadingProgress, new TipsLoadingProgress() { Progress = Progress });
                }

                if (isDone)
                {
                    Progress = 1;
                    IsLoading = false;
                    _loadingScenes.Clear();
                    if (_doLoad)
                    {
                        LoadCompleted();
                    }
                    else
                    {
                        UnloadCompleted();
                    }
                }
            }
        }

        public override void Initialize()
        {
            base.Initialize();
            Scenes = new Dictionary<string, BaseScene>();
            _loadingScenes = new List<BaseScene>();
            IsLoading = false;
            _doLoad = false;
        }
    }
}