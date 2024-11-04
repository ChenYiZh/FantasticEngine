/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using FantasticEngine.Network.Server.Entity;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// 数据表类型转换
    /// </summary>
    public static class TableFieldConverter
    {
        /// <summary>
        /// 转换成枚举
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static ETableFieldType ConvertFromType(Type type)
        {
            if (type == FEType<bool>.Type)
            {
                return ETableFieldType.Bit;
            }
            else if (type == FEType<byte>.Type)
            {
                return ETableFieldType.Byte;
            }
            else if (type == FEType<char>.Type)
            {
                return ETableFieldType.Byte;
            }
            else if (type == FEType<double>.Type)
            {
                return ETableFieldType.Double;
            }
            else if (type == FEType<float>.Type)
            {
                return ETableFieldType.Float;
            }
            else if (type == FEType<int>.Type)
            {
                return ETableFieldType.Int;
            }
            else if (type == FEType<long>.Type)
            {
                return ETableFieldType.Long;
            }
            else if (type == FEType<sbyte>.Type)
            {
                return ETableFieldType.SByte;
            }
            else if (type == FEType<short>.Type)
            {
                return ETableFieldType.Short;
            }
            else if (type == FEType<uint>.Type)
            {
                return ETableFieldType.UInt;
            }
            else if (type == FEType<ulong>.Type)
            {
                return ETableFieldType.ULong;
            }
            else if (type == FEType<ushort>.Type)
            {
                return ETableFieldType.UShort;
            }
            else if (type == FEType<string>.Type)
            {
                return ETableFieldType.String;
            }
            else if (type == FEType<DateTime>.Type)
            {
                return ETableFieldType.DateTime;
            }
            else if (type == FEType<TimeSpan>.Type)
            {
                return ETableFieldType.Long;
            }
            else if (type == FEType<byte[]>.Type)
            {
                return ETableFieldType.Blob;
            }
            else if (type.IsSubInterfaceOf(typeof(IList<>)))
            {
                return ETableFieldType.LongText;
            }
            else if (type.IsSubInterfaceOf(typeof(IDictionary<,>)))
            {
                return ETableFieldType.LongText;
            }
            return ETableFieldType.Error;
        }
        /// <summary>
        /// 打出日志
        /// </summary>
        /// <param name="field"></param>
        /// <returns></returns>
        public static string GetString(this IEntityField field)
        {
            return $"{field.Name} [{field.FieldType.ToString()}]: IsKey: {field.IsKey}, IsIndex: {field.IsIndex}, Nullable: {field.Nullable}, DefaultValue: {field.DefaultValue}";
        }
    }
}
