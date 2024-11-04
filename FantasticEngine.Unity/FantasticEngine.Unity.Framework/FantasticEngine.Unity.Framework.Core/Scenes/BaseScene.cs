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
using FantasticEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace FantasticEngine
{
    /// <summary>
    /// 单场景生命周期管理类
    /// </summary>
    public abstract class BaseScene
    {
        public abstract string SceneName { get; }

        public Scene Scene { get; protected set; }

        public float Progress { get; protected set; }

        public bool IsLoading { get; protected set; } = false;

        public bool IsDone { get; protected set; } = true;

        public bool Wait2Unload { get; protected set; } = false;

        public AsyncOperation AsyncOperation { get; protected set; }

        public IEnumerator LoadAsync()
        {
            if (!IsDone)
            {
                yield break;
            }

            IsLoading = true;
            IsDone = false;
            Progress = 0;
            AsyncOperation = SceneManager.LoadSceneAsync(SceneName, LoadSceneMode.Additive);
            //AsyncOperation.allowSceneActivation = false;
            while (!AsyncOperation.isDone)
            {
                yield return null;
                Progress = AsyncOperation.progress;
            }

            LoadCompleted();
        }

        public void Load()
        {
            if (!IsDone)
            {
                return;
            }

            IsLoading = true;
            IsDone = false;
            SceneManager.LoadScene(SceneName, LoadSceneMode.Additive);
            LoadCompleted();
        }

        private void LoadCompleted()
        {
            AsyncOperation = null;
            if (Wait2Unload)
            {
                Wait2Unload = false;
                UnloadAsync();
                return;
            }

            Scene = SceneManager.GetSceneByName(SceneName);
            IsLoading = false;
            IsDone = true;
            GameRoot.Root.StartCoroutine(WaitToCompleted());
            //OnLoadCompleted();
        }

        private IEnumerator WaitToCompleted()
        {
            while (SceneSystem.Instance.IsLoading)
            {
                yield return null;
            }
            yield return null;
            OnLoadCompleted();
        }

        protected virtual void OnLoadCompleted()
        {
            SceneManager.SetActiveScene(Scene);
            //UI_Tips.Hide(ETips.Tips_Loading);
            //等一帧是因为可能Event消息还在队列里
            //GameRoot.Root.StartCoroutine(OnLoadCompletedTask());
        }

        protected IEnumerator OnLoadCompletedTask()
        {
            yield return null;
            yield return null;
            yield return null;
            UI_Tips.Hide(ETip.Tips_Loading);
        }

        public IEnumerator UnloadAsync()
        {
            if (AsyncOperation != null)
            {
                Wait2Unload = true;
                yield break;
            }

            AsyncOperation = SceneManager.UnloadSceneAsync(Scene);
            IsDone = false;
            yield return AsyncOperation;
            UnloadCompleted();
        }

        private void UnloadCompleted()
        {
            AsyncOperation = null;
            Wait2Unload = false;
            IsDone = true;
            OnUnloadCompleted();
        }

        protected virtual void OnUnloadCompleted()
        {
        }

        // public virtual IEnumerator Reload()
        // {
        //     yield return UnloadAsync();
        //     yield return LoadAsync();
        // }
    }
}