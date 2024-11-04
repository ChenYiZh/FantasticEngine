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
using System.Threading;
using System.Threading.Tasks;
using FantasticEngine.Log;
using UnityEngine;
namespace FantasticEngine
{
    /// <summary>
    /// 贴图下载的传递对象
    /// </summary>
    public class TextureLoader
    {
        /// <summary>
        /// 需要下载的链接
        /// </summary>
        public string url = null;

        /// <summary>
        /// 贴图尺寸
        /// </summary>
#if UNITY_2018_OR_NEWER
        public Vector2Int size;
#else
        public Vector2 size;
#endif

        /// <summary>
        /// 贴图获取成功
        /// </summary>
        public bool success = false;

        /// <summary>
        /// 获取到的贴图
        /// </summary>
        public Texture2D texture = null;
#if UNITY_2018_OR_NEWER
        private TextureLoader(string url, Vector2Int size)
#else
        private TextureLoader(string url, Vector2 size)
#endif
        {
            this.url = url;
            this.size = size;
        }


#if UNITY_2018_OR_NEWER
        public static TextureLoader Create(string url, Vector2Int size)
#else
        public static TextureLoader Create(string url, Vector2 size)
#endif
        {
            // TODO: 具体情况具体分析，是否需要使用对象池
            return new TextureLoader(url, size);
        }
    }

    public class TextureSystem : SystemBasis<TextureSystem>
    {
        /// <summary>
        /// 图片尺寸缩小的比例
        /// </summary>
        private int _reduceStep = 0;

        private class TextureCache
        {
            public enum EState
            {
                None,
                Loading,
                Finish,
                Error
            }

            public EState state = EState.None;
            public Texture2D texture = null;
        }

        private Dictionary<string, TextureCache> _cache = new Dictionary<string, TextureCache>();

        /// <summary>
        /// 判断缓存池中是否存在某个Url
        /// </summary>
        public bool ContainsUrl(string url)
        {
            string key = GetKeyFromUrl(url);
            return _cache.ContainsKey(key);
        }

        private string GetKeyFromUrl(string url)
        {
            return url.Split('?')[0];
        }

        /// <summary>
        /// 加载图片并且缓存
        /// </summary>
        public IEnumerator LoadTextureOnline(TextureLoader loader,
            bool inCache = true,
#if UNITY_2018_OR_NEWER
            Vector2Int safeSize = new Vector2Int())
#else
            Vector2 safeSize = new Vector2())
#endif
        {
            string key = GetKeyFromUrl(loader.url);
            if (inCache && _cache.ContainsKey(key))
            {
                TextureCache cache = _cache[key];
                while (cache.state == TextureCache.EState.Loading)
                {
                    yield return null;
                }

                ReturnResult(cache, loader);
            }
            else
            {
                TextureCache cache = new TextureCache();
                if (inCache)
                {
                    _cache.Add(key, cache);
                }

                cache.state = TextureCache.EState.Loading;
                WebSystem.Instance.Get(loader.url, (Texture2D texture) =>
                {
                    if (texture != null)
                    {
                        if (safeSize.x > 0 || safeSize.y > 0)
                        {
                            Vector2 newSize = FEUtility.GetSafeVector2(new Vector2(texture.width, texture.height), safeSize);
                            cache.texture = ReduceTexture2D(texture, (int)newSize.x, (int)newSize.y);
                            Destroy(texture);
                        }
                        else
                        {
                            cache.texture = texture;
                        }

                        cache.state = TextureCache.EState.Finish;
                    }
                    else
                    {
                        cache.texture = null;
                        cache.state = TextureCache.EState.Error;
                    }
                }, () =>
                {
                    cache.texture = null;
                    cache.state = TextureCache.EState.Error;
                });
                while (cache.state == TextureCache.EState.Loading)
                {
                    yield return null;
                }

                ReturnResult(cache, loader);
            }
        }

        private void ReturnResult(TextureCache cache, TextureLoader loader)
        {
            if (cache.state == TextureCache.EState.Finish && cache.texture != null)
            {
                loader.success = true;
                loader.texture = cache.texture;
                return;
            }

            loader.success = false;
            loader.texture = null;
        }

        public override void Initialize()
        {
            ClearCache();
        }

        // public override void OnExit()
        // {
        //     ClearCache();
        //     base.OnExit();
        // }

        public void ClearCache()
        {
            foreach (KeyValuePair<string, TextureCache> textureCach in _cache)
            {
                if (textureCach.Value.texture != null)
                {
                    GameObject.Destroy(textureCach.Value.texture);
                }
            }

            _cache.Clear();
            ResourceManager.Instance.Release();
        }

        public void Destroy(Texture texture)
        {
            if (texture != null)
            {
                GameObject.Destroy(texture);
            }

            ResourceManager.Instance.Release();
        }

        /// <summary>
        /// 修改尺寸并且返回新的贴图
        /// </summary>
        public Texture2D ReduceTexture2D(Texture2D source, int newWidth, int newHeight)
        {
            Texture2D result = new Texture2D(newWidth, newHeight, source.format, true);
            Color[] pixels = result.GetPixels(0);
            float xScale = 1f / newWidth;
            float yScale = 1f / newHeight;
            for (int i = 0; i < pixels.Length; i++)
            {
                pixels[i] = source.GetPixelBilinear(xScale * ((float)i % newWidth), yScale * (i / newWidth));
            }
            result.SetPixels(pixels, 0);
            result.Apply();
            return result;
        }

        public void OnLowMemory()
        {
            FEConsole.WriteWarn("TextureSystem: Low Memory!");
        }
    }
}