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
using UnityEngine;

namespace FantasyEngine.Common
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
        public static GameObject Instantiate(Object obj, Transform parent)
        {
            GameObject res = Instantiate<GameObject>(obj, parent);
            return res;
        }
        /// <summary>
        /// 实例化
        /// </summary>
        public static T Instantiate<T>(Object obj, Transform parent) where T : Object
        {
            if (obj == null)
            {
                return null;
            }

            Object gameObject = GameObject.Instantiate(obj, parent);
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
    }
}
