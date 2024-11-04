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
using FantasticEngine.TableTool.Framework;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.TableTool.Generator.Unity
{
    /// <summary>
    /// 类型转换工具
    /// </summary>
    internal static class UnityConverter
    {
        public static string GetConvertStr(EHeaderType headerType)
        {
            switch (headerType)
            {
                case EHeaderType.STRING: return "TryConvertString(values, {0}, out {1})";
                case EHeaderType.SHORT: return "TryConvertShort(values, {0}, 0, out {1})";
                case EHeaderType.INT: return "TryConvertInt(values, {0}, 0, out {1})";
                case EHeaderType.LONG: return "TryConvertLong(values, {0}, 0, out {1})";
                case EHeaderType.USHORT: return "TryConvertUShort(values, {0}, 0, out {1})";
                case EHeaderType.UINT: return "TryConvertUInt(values, {0}, 0, out {1})";
                case EHeaderType.ULONG: return "TryConvertULong(values, {0}, 0, out {1})";
                case EHeaderType.FLOAT: return "TryConvertFloat(values, {0}, 0, out {1})";
                case EHeaderType.DOUBLE: return "TryConvertDouble(values, {0}, 0, out {1})";
                case EHeaderType.BOOL: return "TryConvertBool(values, {0}, false, out {1})";
                case EHeaderType.BYTE: return "TryConvertByte(values, {0}, 0, out {1})";
                case EHeaderType.CHAR: return "TryConvertChar(values, {0}, '', out {1})";
                case EHeaderType.DATE: return "TryConvertDateTime(values, {0}, new DateTime(), out {1})";
                case EHeaderType.TIME: return "TryConvertDateTime(values, {0}, new DateTime(), out {1})";
                case EHeaderType.DATETIME: return "TryConvertDateTime(values, {0}, new DateTime(), out {1})";

                case EHeaderType.ARRAY_STRING: return "TryConvertShortArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_SHORT: return "TryConvertShortArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_INT: return "TryConvertIntArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_LONG: return "TryConvertLongArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_USHORT: return "TryConvertUShortArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_UINT: return "TryConvertUIntArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_ULONG: return "TryConvertULongArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_FLOAT: return "TryConvertFloatArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_DOUBLE: return "TryConvertDoubleArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_BOOL: return "TryConvertBoolArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_BYTE: return "TryConvertByteArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_CHAR: return "TryConvertCharArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_DATE: return "TryConvertDateTimeArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_TIME: return "TryConvertDateTimeArray(values, {0}, out {1})";
                case EHeaderType.ARRAY_DATETIME: return "TryConvertDateTimeArray(values, {0}, out {1})";

                case EHeaderType.ARRAY_ARRAY_STRING: return "TryConvertStringArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_SHORT: return "TryConvertShortArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_INT: return "TryConvertIntArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_LONG: return "TryConvertLongArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_USHORT: return "TryConvertUShortArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_UINT: return "TryConvertUIntArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_ULONG: return "TryConvertULongArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_FLOAT: return "TryConvertFloatArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_DOUBLE: return "TryConvertDoubleArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_BOOL: return "TryConvertBoolArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_BYTE: return "TryConvertByteArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_CHAR: return "TryConvertCharArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_DATE: return "TryConvertDateTimeArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_TIME: return "TryConvertDateTimeArrayList(values, {0}, out {1})";
                case EHeaderType.ARRAY_ARRAY_DATETIME: return "TryConvertDateTimeArrayList(values, {0}, out {1})";

                default: return "TryConvertString({0}, {2}, out {1})";
            }
        }

        public static string GetTypeName(EHeaderType EHeaderType)
        {
            switch (EHeaderType)
            {
                case EHeaderType.STRING: return "string";
                case EHeaderType.SHORT: return "short";
                case EHeaderType.INT: return "int";
                case EHeaderType.LONG: return "long";
                case EHeaderType.USHORT: return "ushort";
                case EHeaderType.UINT: return "uint";
                case EHeaderType.ULONG: return "ulong";
                case EHeaderType.FLOAT: return "float";
                case EHeaderType.DOUBLE: return "double";
                case EHeaderType.BOOL: return "bool";
                case EHeaderType.BYTE: return "byte";
                case EHeaderType.CHAR: return "char";
                case EHeaderType.DATE: return "DateTime";
                case EHeaderType.TIME: return "DateTime";
                case EHeaderType.DATETIME: return "DateTime";

                case EHeaderType.ARRAY_STRING: return "string[]";
                case EHeaderType.ARRAY_SHORT: return "short[]";
                case EHeaderType.ARRAY_INT: return "int[]";
                case EHeaderType.ARRAY_LONG: return "long[]";
                case EHeaderType.ARRAY_USHORT: return "ushort[]";
                case EHeaderType.ARRAY_UINT: return "uint[]";
                case EHeaderType.ARRAY_ULONG: return "ulong[]";
                case EHeaderType.ARRAY_FLOAT: return "float[]";
                case EHeaderType.ARRAY_DOUBLE: return "double[]";
                case EHeaderType.ARRAY_BOOL: return "bool[]";
                case EHeaderType.ARRAY_BYTE: return "byte[]";
                case EHeaderType.ARRAY_CHAR: return "char[]";
                case EHeaderType.ARRAY_DATE: return "DateTime[]";
                case EHeaderType.ARRAY_TIME: return "DateTime[]";
                case EHeaderType.ARRAY_DATETIME: return "DateTime[]";

                case EHeaderType.ARRAY_ARRAY_STRING: return "List<string[]>";
                case EHeaderType.ARRAY_ARRAY_SHORT: return "List<short[]>";
                case EHeaderType.ARRAY_ARRAY_INT: return "List<int[]>";
                case EHeaderType.ARRAY_ARRAY_LONG: return "List<long[]>";
                case EHeaderType.ARRAY_ARRAY_USHORT: return "List<ushort[]>";
                case EHeaderType.ARRAY_ARRAY_UINT: return "List<uint[]>";
                case EHeaderType.ARRAY_ARRAY_ULONG: return "List<ulong[]>";
                case EHeaderType.ARRAY_ARRAY_FLOAT: return "List<float[]>";
                case EHeaderType.ARRAY_ARRAY_DOUBLE: return "List<double[]>";
                case EHeaderType.ARRAY_ARRAY_BOOL: return "List<bool[]>";
                case EHeaderType.ARRAY_ARRAY_BYTE: return "List<byte[]>";
                case EHeaderType.ARRAY_ARRAY_CHAR: return "List<char[]>";
                case EHeaderType.ARRAY_ARRAY_DATE: return "List<DateTime[]>";
                case EHeaderType.ARRAY_ARRAY_TIME: return "List<DateTime[]>";
                case EHeaderType.ARRAY_ARRAY_DATETIME: return "List<DateTime[]>";

                default: return "string";
            }
        }
    }
}
