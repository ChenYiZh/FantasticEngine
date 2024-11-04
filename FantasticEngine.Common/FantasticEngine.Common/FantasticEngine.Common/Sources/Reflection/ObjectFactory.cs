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
using FantasticEngine.Collections;
using FantasticEngine.Log;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace FantasticEngine.Reflection
{
    /// <summary>
    /// 反射创建对象
    /// </summary>
    public static class ObjectFactory
    {
        /// <summary>
        /// 缓存池
        /// </summary>
        private static IDictionary<string, Type> _types = new ThreadSafeDictionary<string, Type>();

        /// <summary>
        /// 自动通过程序集搜索创建
        /// </summary>
        public static T Create<T>(string typeName, params object[] args) where T : class
        {
            try
            {
                Type type;
                if (!_types.TryGetValue(typeName, out type))
                {
                    type = Type.GetType(typeName);
                    if (type != null)
                    {
                        _types[typeName] = type;
                    }
                    else
                    {
                        foreach (Assembly assembly in AssemblyService.Assemblies)
                        {
                            type = assembly.GetType(typeName);
                            if (type != null)
                            {
                                _types[typeName] = type;
                                break;
                            }
                        }
                        if (type == null)
                        {
                            Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
                            foreach (Assembly assembly in assemblies)
                            {
                                type = assembly.GetType(typeName);
                                if (type != null)
                                {
                                    _types[typeName] = type;
                                    break;
                                }
                            }
                        }
                    }
                }
                return Activator.CreateInstance(type, args) as T;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.REFLECTION, e);
                return null;
            }
        }

        /// <summary>
        /// 直接反射创建对象
        /// </summary>
        public static T CreateSimply<T>(string typeName, params object[] args) where T : class
        {
            try
            {
                Type type;
                if (!_types.TryGetValue(typeName, out type))
                {
                    type = Type.GetType(typeName);
                    if (type != null)
                    {
                        _types[typeName] = type;
                    }
                }
                return Activator.CreateInstance(type, args) as T;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.REFLECTION, e);
                return null;
            }
        }

        /// <summary>
        /// 通过程序集创建对象
        /// </summary>
        public static T CreateForce<T>(Assembly assembly, string typeName, params object[] args) where T : class
        {
            if (assembly == null)
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.REFLECTION, "Create object failed, because assembly is null.");
                return null;
            }
            try
            {
                Type type;
                if (!_types.TryGetValue(typeName, out type))
                {
                    type = assembly.GetType(typeName);
                    if (type != null)
                    {
                        _types[typeName] = type;
                    }
                }
                return Activator.CreateInstance(type, args) as T;
            }
            catch (Exception e)
            {
                FEConsole.WriteExceptionWithCategory(Categories.REFLECTION, e);
                return null;
            }
        }
    }
}
