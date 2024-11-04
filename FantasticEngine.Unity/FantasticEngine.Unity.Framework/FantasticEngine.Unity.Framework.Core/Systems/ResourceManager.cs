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
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FantasticEngine
{
    /// <summary>
    /// 资源动态加载管理类
    /// </summary>
    public class ResourceManager : SystemBasis<ResourceManager>
    {
        public delegate void OnAssetLoadCompleted<T>(T obj) where T : Object;

        public Object Load(string path)
        {
            return Load<Object>(path);
        }

        public T Load<T>(string path) where T : Object
        {
            return Resources.Load(path) as T;
        }

        public IEnumerator LoadAsync(string path, OnAssetLoadCompleted<Object> callback)
        {
            yield return LoadAsync<Object>(path, callback);
        }

        public IEnumerator LoadAsync<T>(string path, OnAssetLoadCompleted<T> callback) where T : Object
        {
            var request = Resources.LoadAsync(path);
            yield return request;
            callback.Invoke(request.asset as T);
        }

        public IEnumerator LoadObjectInstanceAsync(string path, OnAssetLoadCompleted<Object> callback)
        {
            var request = Resources.LoadAsync(path);
            yield return request;
            if (request.asset != null)
            {
                callback.Invoke(GameObject.Instantiate(request.asset));
            }
            else
            {
                callback.Invoke(null);
            }
        }

        public void Release()
        {
            if (GameRoot.Root)
                GameRoot.Root.StartCoroutine(_Release());
        }

        private IEnumerator _Release()
        {
            var request = Resources.UnloadUnusedAssets();
            yield return request;
            System.GC.Collect();
        }
    }
}