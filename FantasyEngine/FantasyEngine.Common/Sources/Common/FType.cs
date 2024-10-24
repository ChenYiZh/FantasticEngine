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
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Common
{
    /// <summary>
    /// Type缓存类，防止多次调用GetType或者typeof
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public static class FType<T>
    {
        /// <summary>
        /// 获取泛型的Type
        /// </summary>
        public static Type Type { get; private set; } = typeof(T);
    }
    /// <summary>
    /// Type的一些操作
    /// </summary>
    public static class FType
    {
        /// <summary>
        /// 根据Type，获取默认值
        /// </summary>
        public static object GetDefaultValueFromType(Type type)
        {
            return type.IsValueType ? Activator.CreateInstance(type) : null;
        }
        /// <summary>
        /// 判断是否实现某个泛型接口，少用
        /// </summary>
        public static bool IsSubInterfaceOf(this Type type, Type interfaceType)
        {
            if (type == null)
            {
                return false;
            }
            Type[] types = type.GetInterfaces();
            foreach (Type t in types)
            {
                if (t.IsGenericType ? t.GetGenericTypeDefinition() == interfaceType : t == interfaceType)
                {
                    return true;
                }
            }
            return false;
        }
    }
}
