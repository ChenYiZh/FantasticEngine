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
using System.Net;
using System.Text;
using FantasyEngine.Log;
using FantasyEngine.UI;
using Newtonsoft.Json;
using UnityEngine;
using UnityEngine.Networking;
namespace FantasyEngine
{
    /// <summary>
    /// Http消息的基类
    /// </summary>
    [Serializable]
    public class RequestResult
    {

    }

    /// <summary>
    /// Json类型的，Http请求的消息
    /// </summary>
    public class WebSystem : SystemBasis<WebSystem>
    {
        private const string CATEGORY = "WebSystem";

        private const int SUCCESS_CODE = 200;

        private const string HTTP_HEADER = "http://";

        private const string HTTP_METHOD_POST = "POST";
        private const string HTTP_METHOD_GET = "GET";
        private const string HTTP_METHOD_DELETE = "DELETE";
        private const string HTTP_METHOD_PUT = "PUT";

        public void Get(string url, Action<Texture2D> callback, Action fallCallback = null, bool useRequestPanel = false)
        {
            Action<DownloadHandler> requestHandler = (DownloadHandler downloadHandler) =>
            {
                DownloadHandlerTexture textureHandler = downloadHandler as DownloadHandlerTexture;
                if (textureHandler == null)
                {
                    fallCallback?.Invoke();
                    return;
                }

                if (callback != null)
                {
                    callback.Invoke(textureHandler.texture);
                }
            };
            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_GET, url, null, useRequestPanel, new DownloadHandlerTexture(),
                requestHandler,
                fallCallback, false));
        }

        public void Get(string url, AudioType audioType, Action<AudioClip> callback, Action fallCallback = null,
            bool useRequestPanel = false)
        {
            Action<DownloadHandler> requestHandler = (DownloadHandler downloadHandler) =>
            {
                DownloadHandlerAudioClip audioHandler = downloadHandler as DownloadHandlerAudioClip;
                if (audioHandler == null)
                {
                    fallCallback?.Invoke();
                    return;
                }

                if (callback != null)
                {
                    callback.Invoke(audioHandler.audioClip);
                }
            };
            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_GET, url, null, useRequestPanel,
                new DownloadHandlerAudioClip(url, audioType),
                requestHandler,
                fallCallback, false));
        }

        public void Get(string url, Action<byte[]> callback, Action fallCallback = null, bool useRequestPanel = true)
        {
            Action<DownloadHandler> requestHandler = (downloadHandler) =>
            {
                if (callback != null)
                {
                    callback.Invoke(downloadHandler.data);
                }
            };
            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_GET, url, null, useRequestPanel, new DownloadHandlerBuffer(),
                requestHandler,
                fallCallback));
        }

        public void Get(string url, Action<string> callback, Action fallCallback = null, bool useRequestPanel = true)
        {
            Action<DownloadHandler> requestHandler = (downloadHandler) =>
            {
                if (callback != null)
                {
                    callback.Invoke(downloadHandler.text);
                }
            };
            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_GET, url, null, useRequestPanel, new DownloadHandlerBuffer(),
                requestHandler,
                fallCallback));
        }

        private void Get(string url, Dictionary<string, string> form, Action<DownloadHandler> callback = null,
            Action fallCallback = null, bool useRequestPanel = true)
        {
            string extra = "";
            if (form != null)
            {
                foreach (var kv in form)
                {
                    extra += $"{kv.Key}={kv.Value}&";
                }
            }

            if (extra.Length > 0)
            {
                url += "?" + extra.Substring(0, extra.Length - 1);
            }

            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_GET, url, null, useRequestPanel, new DownloadHandlerBuffer(),
                result =>
                {
                    if (callback != null)
                    {
                        callback.Invoke(result);
                    }
                }, fallCallback));
        }

        public void Get<TResult>(string url, Action<TResult> callback,
            Action fallCallback = null, bool useRequestPanel = true) where TResult : RequestResult
        {
            Dictionary<string, object> data = null;
            Get(url, data, callback, fallCallback);
        }

        public void Get<TResult>(string url, Dictionary<string, string> form, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true) where TResult : RequestResult
        {
            Dictionary<string, object> data = null;
            if (form != null && form.Count > 0)
            {
                data = new Dictionary<string, object>();
                foreach (KeyValuePair<string, string> kv in form)
                {
                    data.Add(kv.Key, kv.Value);
                }
            }

            Get(url, data, callback, fallCallback, useRequestPanel);
        }

        public void Get<TResult>(string url, Dictionary<string, object> form, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true) where TResult : RequestResult
        {
            string data = form == null ? null : JsonConvert.SerializeObject(form);
            Get(url, data, callback, fallCallback, useRequestPanel);
        }

        public void Get<TResult>(string url, string data, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true) where TResult : RequestResult
        {
            FEConsole.WriteWithCategory(CATEGORY, $"[GET] {url}\r\n" + (data == null ? "" : data));
            GameRoot.Root.StartCoroutine(Request<TResult>(HTTP_METHOD_GET, url,
                data != null ? Encoding.UTF8.GetBytes(data) : null, result =>
                {
                    if (result != null && callback != null)
                    {
                        callback.Invoke(result);
                    }
                    else if (fallCallback != null)
                    {
                        fallCallback.Invoke();
                    }
                }, null, useRequestPanel));
        }

        public void Post<TForm>(string url, TForm form, Action callback = null, Action fallCallback = null,
            bool useRequestPanel = true)
            where TForm : class
        {
            Post(url, form != null ? JsonConvert.SerializeObject(form) : null, callback, fallCallback, useRequestPanel);
        }

        public void Post(string url, Dictionary<string, object> form, Action callback = null, Action fallCallback = null,
            bool useRequestPanel = true)
        {
            Post(url, form != null ? JsonConvert.SerializeObject(form) : null, callback, fallCallback, useRequestPanel);
        }

        public void Post(string url, string data, Action callback = null, Action fallCallback = null,
            bool useRequestPanel = true)
        {
            FEConsole.Write($"[POST] {url}:\n{data}");
            Post(url, string.IsNullOrEmpty(data) ? null : Encoding.UTF8.GetBytes(data), callback, fallCallback,
                useRequestPanel);
        }

        public void Post(string url, byte[] data, Action callback = null, Action fallCallback = null,
            bool useRequestPanel = true)
        {
            Post<RequestResult>(url, data, (RequestResult result) =>
            {
                if (callback != null) callback.Invoke();
            }, fallCallback, useRequestPanel);
        }

        public void Post<TForm, TResult>(string url, TForm form, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true)
            where TForm : class where TResult : RequestResult
        {
            Post(url, form != null ? JsonConvert.SerializeObject(form) : null, callback, fallCallback, useRequestPanel);
        }

        public void Post<TResult>(string url, Dictionary<string, object> form, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true) where TResult : RequestResult
        {
            Post(url, form != null ? JsonConvert.SerializeObject(form) : null, callback, fallCallback, useRequestPanel);
        }

        public void Post<TResult>(string url, string data, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true)
            where TResult : RequestResult
        {
            FEConsole.WriteWithCategory(CATEGORY, $"[POST] {url}\r\n" + data);
            Post(url, string.IsNullOrEmpty(data) ? null : Encoding.UTF8.GetBytes(data), callback,
                fallCallback, useRequestPanel);
        }

        public void Post<TResult>(string url, byte[] data, Action<TResult> callback = null,
            Action fallCallback = null, bool useRequestPanel = true)
            where TResult : RequestResult
        {
            GameRoot.Root.StartCoroutine(Request<TResult>(HTTP_METHOD_POST, url, data, result =>
            {
                if (result != null /*&& result.Code == SUCCESS_CODE */ && callback != null)
                {
                    callback.Invoke(result);
                }
                else if (fallCallback != null)
                {
                    fallCallback.Invoke();
                }
            }, null, useRequestPanel));
        }

        public void Post(string url, Dictionary<string, object> form, Action<string> callback, Action fallCallback = null, bool useRequestPanel = true)
        {
            Action<DownloadHandler> requestHandler = (downloadHandler) =>
            {
                if (callback != null)
                {
                    callback.Invoke(downloadHandler.text);
                }
            };
            byte[] data = null;
            if (form != null)
            {
                data = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(form));
            }
            GameRoot.Root.StartCoroutine(Request(HTTP_METHOD_POST, url, data, useRequestPanel, new DownloadHandlerBuffer(),
                requestHandler,
                fallCallback));
        }

        private IEnumerator Request(
            string method, string url, byte[] data,
            bool useRequestPanel,
            DownloadHandler downloadHandler,
            Action<DownloadHandler> callback,
            Action fallCallback = null,
            bool authorized = true)
        {
            if (!url.ToLower().StartsWith("http"))
            {
                url = HTTP_HEADER + url;
            }

            method = method.ToUpper();
            StringBuilder log = new StringBuilder();
            log.AppendLine($"[{method}]{url}");
            bool isNullData = data == null || data.Length == 0;

            UnityWebRequest request =
                new UnityWebRequest(url, method, downloadHandler, isNullData ? null : new UploadHandlerRaw(data));
            try
            {
                log.AppendLine("HEADER: ");
                log.AppendLine("    Content-Type: " + "application/json;charset=utf-8");
                request.SetRequestHeader("Content-Type", "application/json;charset=utf-8");
            }
            catch (Exception e)
            {
                log.AppendLine("ERROR:");
                log.AppendLine(e.Message);
                log.AppendLine(e.StackTrace);
                FEConsole.WriteErrorWithCategory(CATEGORY, log.ToString());
                callback.Invoke(null);
                yield break;
            }
#if UNITY_2018_OR_NEWER
            request.timeout = 300;
#endif
            if (useRequestPanel)
            {
                UI_Tips.Show(ETip.Tips_Request);
            }
#if UNITY_2018_OR_NEWER
            yield return request.SendWebRequest();
#else
            yield return request.Send();
#endif
            if (useRequestPanel)
            {
                UI_Tips.Hide(ETip.Tips_Request);
            }

            try
            {
#if UNITY_2018_OR_NEWER
                bool requestSuccess = request.result == UnityWebRequest.Result.Success;
#else
                bool requestSuccess = !request.isError;
#endif
                if (!requestSuccess)
                {
                    FEConsole.WriteErrorWithCategory(CATEGORY, $"Response Code: {request.responseCode}");
#if UNITY_2018_OR_NEWER
                    log.AppendLine("ERROR: " + request.result.ToString());
#endif
                    log.AppendLine(request.error);
                    log.AppendLine(downloadHandler.text);
                    FEConsole.WriteErrorWithCategory(CATEGORY, log);
                    if (fallCallback != null)
                    {
                        fallCallback.Invoke();
                    }

                    yield break;
                }

                try
                {
                    if (callback != null)
                    {
                        callback.Invoke(request.downloadHandler);
                    }
                }
                catch (Exception e)
                {
                    log.AppendLine("ERROR:");
                    log.AppendLine(e.Message);
                    log.AppendLine(e.StackTrace);
                    FEConsole.WriteErrorWithCategory(CATEGORY, log.ToString());
                    if (fallCallback != null) fallCallback.Invoke();
                }
            }
            catch (Exception e)
            {
                log.AppendLine("ERROR:");
                log.AppendLine(e.Message);
                log.AppendLine(e.StackTrace);
                FEConsole.WriteErrorWithCategory(CATEGORY, log.ToString());
                if (fallCallback != null) fallCallback.Invoke();
            }

            request.Dispose();
        }

        public IEnumerator Request<TResult>(string method, string url, byte[] data, Action<TResult> callback,
            Action fallCallback = null, bool useRequestPanel = true)
            where TResult : RequestResult
        {
            Action<DownloadHandler> requestHandler = (downloadHandler) =>
            {
                string result = downloadHandler.text;
                FEConsole.WriteWithCategory(CATEGORY, "Http Response: " + result);
                TResult resultObj = JsonConvert.DeserializeObject<TResult>(result);
                if (callback != null)
                {
                    callback.Invoke(resultObj);
                }
            };
            yield return Request(method, url, data, useRequestPanel, new DownloadHandlerBuffer(), requestHandler,
                fallCallback);
        }

        /// <summary>
        /// 请求获取字节流
        /// </summary>
        public IEnumerator Request(string method, string url, byte[] data,
            Action<byte[]> callback,
            Action fallCallback = null,
            bool useRequestPanel = true)
        {
            Action<DownloadHandler> requestHandler = (downloadHandler) =>
            {
                byte[] result = downloadHandler.data;
                if (callback != null)
                {
                    callback.Invoke(result);
                }
            };
            yield return Request(method, url, data, useRequestPanel, new DownloadHandlerBuffer(), requestHandler,
                fallCallback);
        }
    }
}