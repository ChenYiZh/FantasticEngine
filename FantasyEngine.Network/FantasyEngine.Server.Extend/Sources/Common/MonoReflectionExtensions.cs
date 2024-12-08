/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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
using Mono.Cecil;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// Mono反射的拓展
    /// </summary>
    internal static class MonoReflectionExtensions
    {
        /// <summary>
        /// 判断是否是子类
        /// </summary>
        public static bool IsChildOf(this TypeDefinition type, string classFullName)
        {
            if (type == null)
            {
                return false;
            }
            if (type.BaseType.Is(classFullName))
            {
                return true;
            }
            return type.BaseType.IsChildOf(classFullName);
        }

        /// <summary>
        /// 判断是否是子类
        /// </summary>
        public static bool Is(this TypeReference type, string classFullName)
        {
            if (type == null)
            {
                return false;
            }
            return type.FullName == classFullName;
        }

        /// <summary>
        /// 判断是否是子类
        /// </summary>
        public static bool IsChildOf(this TypeReference type, string classFullName)
        {
            if (type == null)
            {
                return false;
            }
            return type.Resolve().IsChildOf(classFullName);
        }

        /// <summary>
        /// 属性中是否包含Atrribute
        /// </summary>
        public static bool ContainsAttribute(this PropertyDefinition property, string attributeName)
        {
            if (property == null || property.CustomAttributes == null)
            {
                return false;
            }
            foreach (CustomAttribute attribute in property.CustomAttributes)
            {
                if (attribute.Constructor != null &&
                            attribute.Constructor.DeclaringType != null &&
                            attribute.Constructor.DeclaringType.Name == "EntityFieldAttribute")
                {
                    return true;
                }
            }
            return false;
        }
    }
}
