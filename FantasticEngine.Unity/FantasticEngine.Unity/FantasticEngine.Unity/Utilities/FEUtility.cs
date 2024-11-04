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
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace FantasticEngine
{
    /// <summary>
    /// 工具类
    /// </summary>
    public partial class FEUtility
    {
        /// <summary>
        /// 获取Component，如果没有就创建一个Component
        /// </summary>
        public static T GetOrAddComponent<T>(Transform transform) where T : Component
        {
            return GetOrAddComponent<T>(transform.gameObject);
        }
        /// <summary>
        /// 获取Component，如果没有就创建一个Component
        /// </summary>
        public static T GetOrAddComponent<T>(GameObject gameObject) where T : Component
        {
            T component = gameObject.GetComponent<T>();
            if (component != null)
            {
                return component;
            }

            return gameObject.AddComponent<T>();
        }
        /// <summary>
        /// 实例化
        /// </summary>
        public static GameObject Instantiate(UnityEngine.Object obj, Transform parent)
        {
            GameObject res = Instantiate<GameObject>(obj, parent);
            return res;
        }
        /// <summary>
        /// 实例化
        /// </summary>
        public static T Instantiate<T>(UnityEngine.Object obj, Transform parent) where T : UnityEngine.Object
        {
            if (obj == null)
            {
                return null;
            }

            UnityEngine.Object gameObject = GameObject.Instantiate(obj, parent);
            if (gameObject != null)
            {
                gameObject.name = obj.name;
            }

            return gameObject as T;
        }
        /// <summary>
        /// 判断是否是点击状态
        /// </summary>
        public static bool IsTouching()
        {
            return Input.touchCount != 0 || Input.GetMouseButton(0);
        }
        /// <summary>
        /// 包含误差的判断逻辑，防止科学计数
        /// </summary>
        public static bool NearBy(float value, float compare, float tolerance = 0.000001f)
        {
            return Mathf.Abs(value - compare) < tolerance;
        }

        /// <summary>
        /// 0 - 360 角度转为 -180 - 180
        /// </summary>
        public static float To180(float angle)
        {
            float angle360 = To360(angle);
            if (angle360 > 180)
            {
                return angle360 - 360;
            }

            if (angle360 < -180)
            {
                return angle360 + 360;
            }

            return angle360;
        }


        /// <summary>
        /// 0 - 360 角度转为 -180 - 180
        /// </summary>
        public static Vector3 To180(Vector3 angle)
        {
            return new Vector3(To180(angle.x), To180(angle.y), To180(angle.z));
        }

        /// <summary>
        /// 将角度转为 0 - 360
        /// </summary>
        public static float To360(float angle)
        {
            return angle % 360;
        }

        /// <summary>
        /// 将角度转为 0 - 360
        /// </summary>
        public static Vector3 To360(Vector3 angle)
        {
            return new Vector3(To360(angle.x), To360(angle.y), To360(angle.z));
        }

        /// <summary>
        /// 根据给定的大小计算正好可以包裹住的安全大小，xy都必须大于0
        /// </summary>
        public static Vector2 GetSafeVector2(Vector2 inVector2, Vector2 safeRange)
        {
            float delta = 1.0f;
            if (inVector2.x / inVector2.y > safeRange.x / safeRange.y)
            {
                delta = safeRange.x / inVector2.x;
            }
            else if (inVector2.x / inVector2.y < safeRange.x / safeRange.y)
            {
                delta = safeRange.y / inVector2.y;
            }
            else
            {
                delta = safeRange.y / inVector2.y;
            }

            Vector2 result = inVector2 * delta;
            return result;
        }

        #region PlayerPrefs

        public static void DeleteLocalAll()
        {
            PlayerPrefs.DeleteAll();
        }

        public static void DeleteLocalKey(string key)
        {
            PlayerPrefs.DeleteKey(key);
        }

        public static float GetLocalFloat(string key, float defaultValue)
        {
            return PlayerPrefs.GetFloat(key, defaultValue);
        }

        public static float GetLocalFloat(string key)
        {
            return PlayerPrefs.GetFloat(key);
        }

        public static int GetLocalInt(string key, int defaultValue)
        {
            return PlayerPrefs.GetInt(key, defaultValue);
        }

        public static int GetLocalInt(string key)
        {
            return PlayerPrefs.GetInt(key);
        }

        public static string GetLocalString(string key, string defaultValue)
        {
            return PlayerPrefs.GetString(key, defaultValue);
        }

        public static string GetLocalString(string key)
        {
            return PlayerPrefs.GetString(key);
        }

        public static bool HasLocalKey(string key)
        {
            return PlayerPrefs.HasKey(key);
        }

        public static void SaveLocal()
        {
            PlayerPrefs.Save();
        }

        public static void SetLocalFloat(string key, float value)
        {
            PlayerPrefs.SetFloat(key, value);
        }

        public static void SetLocalInt(string key, int value)
        {
            PlayerPrefs.SetInt(key, value);
        }

        public static void SetLocalString(string key, string value)
        {
            PlayerPrefs.SetString(key, value);
        }


        #endregion

        /// <summary>
        /// 12.1234 -> 12.12
        /// </summary>
        public static string FromFloat(float value)
        {
            return value.ToString("F2");
        }

        /// <summary>
        /// 10′2″
        /// </summary>
        public static string FormatTimeSpanAbbr(TimeSpan time)
        {
            if (time.Ticks < 0)
            {
                time = new TimeSpan(0);
            }

            if (time.Seconds > 0)
            {
                return string.Format("{0}′{1}″", (int)time.TotalMinutes, time.Seconds);
            }

            return string.Format("{0}′", (int)time.TotalMinutes);
        }

        /// <summary>
        /// 25:59
        /// </summary>
        public static string FormatTimeSpanMMSS(TimeSpan time)
        {
            if (time.Ticks < 0)
            {
                time = new TimeSpan(0);
            }

            return string.Format("{0}:{1}", ((int)time.TotalMinutes).ToString().PadLeft(2, '0'),
                time.Seconds.ToString().PadLeft(2, '0'));
        }

        /// <summary>
        /// 12:25:59
        /// </summary>
        public static string FormatTimeSpanHHMMSS(TimeSpan time)
        {
            if (time.Ticks < 0)
            {
                time = new TimeSpan(0);
            }

            return string.Format("{0}:{1}:{2}", ((int)time.TotalHours).ToString().PadLeft(2, '0'),
                time.Minutes.ToString().PadLeft(2, '0'), time.Seconds.ToString().PadLeft(2, '0'));
        }

        /// <summary>
        /// 颜色过渡
        /// </summary>
        public static Color Lerp(Color a, Color b, float t)
        {
            return a + (b - a) * t;
        }
        /// <summary>
        /// 向量过渡
        /// </summary>
        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            return a + (b - a) * t;
        }
        /// <summary>
        /// 转换成移动端文件路径
        /// </summary>
        public static string ToMobileFilePath(string path)
        {
            if (path.StartsWith("file"))
            {
                return path;
            }
#if UNITY_ANDROID
            return "file://" + path;
#else
            return "file:///" + path;
#endif
        }

        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static float Bezier2(float time, float p0, float p1, float p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * p0 + 2 * time * oneMineT * p1 + time * time * p2;
        }

        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static float Bezier2(float time, int p0, int p1, int p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * p0 + 2 * time * oneMineT * p1 + time * time * p2;
        }

        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static Vector3 Bezier2(float time, Vector3 p0, Vector3 p1, Vector3 p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * p0 + 2 * time * oneMineT * p1 + time * time * p2;
        }

#if UNITY_2018_OR_NEWER
        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static Vector3 Bezier2(float time, Vector3Int p0, Vector3Int p1, Vector3Int p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * (Vector3)p0 + 2 * time * oneMineT * (Vector3)p1 + time * time * (Vector3)p2;
        }
#endif

        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static Vector2 Bezier2(float time, Vector2 p0, Vector2 p1, Vector2 p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * p0 + 2 * time * oneMineT * p1 + time * time * p2;
        }

#if UNITY_2018_OR_NEWER
        /// <summary>
        /// 2次贝塞尔曲线
        /// </summary>
        public static Vector2 Bezier2(float time, Vector2Int p0, Vector2Int p1, Vector2Int p2)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * (Vector2)p0 + 2 * time * oneMineT * (Vector2)p1 + time * time * (Vector2)p2;
        }
#endif
        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static float Bezier3(float time, float p0, float p1, float p2, float p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * p0 + 3 * time * oneMineT * oneMineT * p1 +
                   3 * oneMineT * time * time * p2 + time * time * time * p3;
        }

        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static float Bezier3(float time, int p0, int p1, int p2, int p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * p0 + 3 * time * oneMineT * oneMineT * p1 +
                   3 * oneMineT * time * time * p2 + time * time * time * p3;
        }

        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static Vector3 Bezier3(float time, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * p0 + 3 * time * oneMineT * oneMineT * p1 +
                   3 * oneMineT * time * time * p2 + time * time * time * p3;
        }
#if UNITY_2018_OR_NEWER
        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static Vector3 Bezier3(float time, Vector3Int p0, Vector3Int p1, Vector3Int p2, Vector3Int p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * (Vector3)p0 + 3 * time * oneMineT * oneMineT * (Vector3)p1 +
                   3 * oneMineT * time * time * (Vector3)p2 + time * time * time * (Vector3)p3;
        }
#endif
        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static Vector2 Bezier3(float time, Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * p0 + 3 * time * oneMineT * oneMineT * p1 +
                   3 * oneMineT * time * time * p2 + time * time * time * p3;
        }
#if UNITY_2018_OR_NEWER
        /// <summary>
        /// 3次贝塞尔曲线
        /// </summary>
        public static Vector2 Bezier3(float time, Vector2Int p0, Vector2Int p1, Vector2Int p2, Vector2Int p3)
        {
            float oneMineT = 1 - time;
            return oneMineT * oneMineT * oneMineT * (Vector2)p0 + 3 * time * oneMineT * oneMineT * (Vector2)p1 +
                   3 * oneMineT * time * time * (Vector2)p2 + time * time * time * (Vector2)p3;
        }
#endif
        /// <summary>
        /// 判断是否在某个包裹框内，
        /// item为需要判断的对象，
        /// area包裹框，
        /// anyPoint是否需要全部的点都包含在里面
        /// </summary>
        /// <param name="item">需要判断的对象</param>
        /// <param name="area">包裹框</param>
        /// <param name="anyPoint">是否需要全部的点都包含在里面</param>
        /// <returns></returns>
        public static bool InRect(Vector4 item, Vector4 area, bool anyPoint)
        {
            Vector2[] points = new[]
            {
            new Vector2(item.x, item.y),
            new Vector2(item.x, item.w),
            new Vector2(item.z, item.y),
            new Vector2(item.z, item.w),
        };

            if (anyPoint)
            {
                return points.Any(v => (v.x > area.x && v.x < area.z) && (v.y > area.y && v.y < area.w));
            }
            else
            {
                return points.All(v => v.x > area.x && v.x < area.z && v.y > area.y && v.y < area.w);
            }
        }

        public static void SetLayer(LayerMasks layerMasks)
        {
            foreach (KeyValuePair<GameObject, int> kv in layerMasks.layerPair)
            {
                kv.Key.layer = kv.Value;
            }
        }

        public static LayerMasks SetLayer<T>(GameObject gameObject, int layer, Func<GameObject, bool> condition = null) where T : Component
        {
            T[] components = gameObject.GetComponentsInChildren<T>(true);
            IEnumerable<GameObject> tItems = components.Select(s => s.gameObject).Distinct();
            GameObject[] items;
            if (condition == null)
            {
                items = tItems.ToArray();
            }
            else
            {
                items = tItems.Where(condition).ToArray();
            }
            return SetLayer(layer, items);
        }

        public static LayerMasks SetLayer(GameObject gameObject, int layer, bool children = true)
        {
            if (children)
            {
                GameObject[] items = gameObject.GetComponentsInChildren<Transform>(true).Select(t => t.gameObject).ToArray();
                return SetLayer(layer, items);
            }
            else
            {
                return SetLayer(layer, gameObject);
            }
        }

        public static LayerMasks SetLayer(int layer, params GameObject[] gameObjects)
        {
            LayerMasks layerMasks = GetLayers(gameObjects);
            foreach (GameObject item in gameObjects)
            {
                item.layer = layer;
            }
            return layerMasks;
        }

        public static LayerMasks GetLayers(params GameObject[] gameObjects)
        {
            LayerMasks layerMasks = new LayerMasks();
            layerMasks.layerPair = new Dictionary<GameObject, int>();

            foreach (GameObject item in gameObjects)
            {
                layerMasks.layerPair.Add(item, item.layer);
            }
            return layerMasks;
        }

        /// <summary>
        /// 隐藏过长字段
        /// </summary>
        public static string ShortText(string text, int maxLength, string pad = "...")
        {
            if (string.IsNullOrEmpty(text))
            {
                return text;
            }

            if (text.Length > maxLength)
            {
                return text.Substring(0, maxLength - 3) + pad;
            }

            return text;
        }

        /// <summary>
        /// 补全http://
        /// </summary>
        /// <param name="url"></param>
        /// <returns></returns>
        public static string CheckUrl(string url)
        {
            if (string.IsNullOrEmpty(url))
            {
                return url;
            }

            if (!url.ToLower().StartsWith("http"))
            {
                return "http://" + url;
            }

            return url;
        }
    }
}
