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

namespace FantasticEngine.TableTool.Generator.UnrealEngine
{
    internal class UEConverter
    {
        public static string GetTypeString(EHeaderType type)
        {
            switch (type)
            {
                case EHeaderType.STRING: return "FString";
                case EHeaderType.SHORT: return "int16";
                case EHeaderType.INT: return "int32";
                case EHeaderType.LONG: return "int64";
                case EHeaderType.USHORT: return "uint16";
                case EHeaderType.UINT: return "uint32";
                case EHeaderType.ULONG: return "uint64";
                case EHeaderType.FLOAT: return "float";
                case EHeaderType.DOUBLE: return "double";
                case EHeaderType.BOOL: return "bool";
                case EHeaderType.BYTE: return "uint8";
                case EHeaderType.CHAR: return "CHAR";
                case EHeaderType.DATE: return "FDateTime";
                case EHeaderType.TIME: return "FDateTime";
                case EHeaderType.DATETIME: return "FDateTime";

                case EHeaderType.ARRAY_STRING: return "TArray<FString>";
                case EHeaderType.ARRAY_SHORT: return "TArray<int16>";
                case EHeaderType.ARRAY_INT: return "TArray<int32>";
                case EHeaderType.ARRAY_LONG: return "TArray<int64>";
                case EHeaderType.ARRAY_USHORT: return "TArray<uint16>";
                case EHeaderType.ARRAY_UINT: return "TArray<uint32>";
                case EHeaderType.ARRAY_ULONG: return "TArray<uint64>";
                case EHeaderType.ARRAY_FLOAT: return "TArray<float>";
                case EHeaderType.ARRAY_DOUBLE: return "TArray<double>";
                case EHeaderType.ARRAY_BOOL: return "TArray<bool>";
                case EHeaderType.ARRAY_BYTE: return "TArray<uint8>";
                case EHeaderType.ARRAY_CHAR: return "TArray<char>";
                case EHeaderType.ARRAY_DATE: return "TArray<FDateTime>";
                case EHeaderType.ARRAY_TIME: return "TArray<FDateTime>";
                case EHeaderType.ARRAY_DATETIME: return "TArray<FDateTime>";

                case EHeaderType.ARRAY_ARRAY_STRING: return "TArray<TArray<FString>>";
                case EHeaderType.ARRAY_ARRAY_SHORT: return "TArray<TArray<int16>>";
                case EHeaderType.ARRAY_ARRAY_INT: return "TArray<TArray<int32>>";
                case EHeaderType.ARRAY_ARRAY_LONG: return "TArray<TArray<int64>>";
                case EHeaderType.ARRAY_ARRAY_USHORT: return "TArray<TArray<uint16>>";
                case EHeaderType.ARRAY_ARRAY_UINT: return "TArray<TArray<uint32>>";
                case EHeaderType.ARRAY_ARRAY_ULONG: return "TArray<TArray<uint64>>";
                case EHeaderType.ARRAY_ARRAY_FLOAT: return "TArray<TArray<float>>";
                case EHeaderType.ARRAY_ARRAY_DOUBLE: return "TArray<TArray<double>>";
                case EHeaderType.ARRAY_ARRAY_BOOL: return "TArray<TArray<bool>>";
                case EHeaderType.ARRAY_ARRAY_BYTE: return "TArray<TArray<uint8>>";
                case EHeaderType.ARRAY_ARRAY_CHAR: return "TArray<TArray<char>>";
                case EHeaderType.ARRAY_ARRAY_DATE: return "TArray<TArray<FDateTime>>";
                case EHeaderType.ARRAY_ARRAY_TIME: return "TArray<TArray<FDateTime>>";
                case EHeaderType.ARRAY_ARRAY_DATETIME: return "TArray<TArray<FDateTime>>";
            }
            return "FString";
        }

        public static bool IgnoreProperty(EHeaderType type)
        {
            switch (type)
            {
                case EHeaderType.STRING:
                case EHeaderType.INT:
                case EHeaderType.LONG:
                case EHeaderType.FLOAT:
                case EHeaderType.DOUBLE:
                case EHeaderType.BOOL:
                case EHeaderType.BYTE:
                case EHeaderType.DATE:
                case EHeaderType.TIME:
                case EHeaderType.DATETIME: return false;
                case EHeaderType.ARRAY_STRING:
                case EHeaderType.ARRAY_INT:
                case EHeaderType.ARRAY_LONG:
                case EHeaderType.ARRAY_FLOAT:
                case EHeaderType.ARRAY_DOUBLE:
                case EHeaderType.ARRAY_BOOL:
                case EHeaderType.ARRAY_BYTE:
                case EHeaderType.ARRAY_DATE:
                case EHeaderType.ARRAY_TIME:
                case EHeaderType.ARRAY_DATETIME: return false;
                case EHeaderType.ARRAY_ARRAY_STRING:
                case EHeaderType.ARRAY_ARRAY_INT:
                case EHeaderType.ARRAY_ARRAY_LONG:
                case EHeaderType.ARRAY_ARRAY_FLOAT:
                case EHeaderType.ARRAY_ARRAY_DOUBLE:
                case EHeaderType.ARRAY_ARRAY_BOOL:
                case EHeaderType.ARRAY_ARRAY_BYTE:
                case EHeaderType.ARRAY_ARRAY_DATE:
                case EHeaderType.ARRAY_ARRAY_TIME:
                case EHeaderType.ARRAY_ARRAY_DATETIME: return true;
            }
            return true;
        }

        public static string GetMethodString(EHeaderType type)
        {
            switch (type)
            {
                case EHeaderType.STRING: return "{0} = {1};";
                case EHeaderType.SHORT: return "UStringConverter::ToInt16({1}, {0});";
                case EHeaderType.INT: return "UStringConverter::ToInt32({1}, {0});";
                case EHeaderType.LONG: return "UStringConverter::ToInt64({1}, {0});";
                case EHeaderType.USHORT: return "UStringConverter::ToUInt16({1}, {0});";
                case EHeaderType.UINT: return "UStringConverter::ToUInt32({1}, {0});";
                case EHeaderType.ULONG: return "UStringConverter::ToUInt64({1}, {0});";
                case EHeaderType.FLOAT: return "UStringConverter::ToFloat({1}, {0});";
                case EHeaderType.DOUBLE: return "UStringConverter::ToDouble({1}, {0});";
                case EHeaderType.BOOL: return "UStringConverter::ToBool({1}, {0});";
                case EHeaderType.BYTE: return "UStringConverter::ToUInt8({1}, {0});";
                case EHeaderType.CHAR: return "UStringConverter::ToChar({1}, {0});";
                case EHeaderType.DATE: return "UStringConverter::ToDateTime({1}, {0});";
                case EHeaderType.TIME: return "UStringConverter::ToDateTime({1}, {0});";
                case EHeaderType.DATETIME: return "UStringConverter::ToDateTime({1}, {0});";

                case EHeaderType.ARRAY_STRING: return "UStringConverter::ToStringArray({1}, {0});";
                case EHeaderType.ARRAY_SHORT: return "UStringConverter::ToInt16Array({1}, {0});";
                case EHeaderType.ARRAY_INT: return "UStringConverter::ToInt32Array({1}, {0});";
                case EHeaderType.ARRAY_LONG: return "UStringConverter::ToInt64Array({1}, {0});";
                case EHeaderType.ARRAY_USHORT: return "UStringConverter::ToUInt16Array({1}, {0});";
                case EHeaderType.ARRAY_UINT: return "UStringConverter::ToUInt32Array({1}, {0});";
                case EHeaderType.ARRAY_ULONG: return "UStringConverter::ToUInt64Array({1}, {0});";
                case EHeaderType.ARRAY_FLOAT: return "UStringConverter::ToFloatArray({1}, {0});";
                case EHeaderType.ARRAY_DOUBLE: return "UStringConverter::ToDoubleArray({1}, {0});";
                case EHeaderType.ARRAY_BOOL: return "UStringConverter::ToBoolArray({1}, {0});";
                case EHeaderType.ARRAY_BYTE: return "UStringConverter::ToUInt8Array({1}, {0});";
                case EHeaderType.ARRAY_CHAR: return "UStringConverter::ToCharArray({1}, {0});";
                case EHeaderType.ARRAY_DATE: return "UStringConverter::ToDateTimeArray({1}, {0});";
                case EHeaderType.ARRAY_TIME: return "UStringConverter::ToDateTimeArray({1}, {0});";
                case EHeaderType.ARRAY_DATETIME: return "UStringConverter::ToDateTimeArray({1}, {0});";

                case EHeaderType.ARRAY_ARRAY_STRING: return "UStringConverter::ToStringArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_SHORT: return "UStringConverter::ToInt16ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_INT: return "UStringConverter::ToInt32ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_LONG: return "UStringConverter::ToInt64ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_USHORT: return "UStringConverter::ToUInt16ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_UINT: return "UStringConverter::ToUInt32ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_ULONG: return "UStringConverter::ToUInt64ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_FLOAT: return "UStringConverter::ToFloatArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_DOUBLE: return "UStringConverter::ToDoubleArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_BOOL: return "UStringConverter::ToBoolArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_BYTE: return "UStringConverter::ToUInt8ArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_CHAR: return "UStringConverter::ToCharArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_DATE: return "UStringConverter::ToDateTimeArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_TIME: return "UStringConverter::ToDateTimeArrayArray({1}, {0});";
                case EHeaderType.ARRAY_ARRAY_DATETIME: return "UStringConverter::ToDateTimeArrayArray({1}, {0});";
            }
            //switch (type)
            //{
            //    case EHeaderType.BOOL:
            //        return "{0} = {1}.ToBool();";
            //    case EHeaderType.BYTE:
            //        return "{0} = {1}[0];";
            //    case EHeaderType.CHAR:
            //        return "{0} = {1}[0];";
            //    case EHeaderType.DATE:
            //        return "FDateTime::Parse(ToCStr({1}),{0});";
            //    case EHeaderType.DATETIME:
            //        return "FDateTime::Parse(ToCStr({1}),{0});";
            //    case EHeaderType.DOUBLE:
            //        return "{0} = FCString::Atod(ToCStr({1}));";
            //    case EHeaderType.FLOAT:
            //        return "{0} = FCString::Atof(ToCStr({1}));";
            //    case EHeaderType.INT:
            //        return "{0} = FCString::Atoi(ToCStr({1}));";
            //    case EHeaderType.LONG:
            //        return "{0} = FCString::Atoi64(ToCStr({1}));";
            //    case EHeaderType.SHORT:
            //        return "{0} = FCString::Atoi(ToCStr({1}));";
            //    case EHeaderType.STRING:
            //        return "{0} = {1};";
            //    case EHeaderType.TIME:
            //        return "FDateTime::Parse(ToCStr({1}),{0});";
            //    case EHeaderType.UINT:
            //        //return "{0} = FCString::Strtoui64(ToCStr({1}),nullptr,10);";
            //        return "{0} = FCString::Atoi(ToCStr({1}));";
            //    case EHeaderType.ULONG:
            //        //return "{0} = FCString::Strtoui64(ToCStr({1}),nullptr,10);";
            //        return "{0} = FCString::Atoi64(ToCStr({1}));";
            //    case EHeaderType.USHORT:
            //        //return "{0} = FCString::Strtoui64(ToCStr({1}),nullptr,10);";
            //        return "{0} = FCString::Atoi(ToCStr({1}));";
            //}
            return null;
        }
    }
}
