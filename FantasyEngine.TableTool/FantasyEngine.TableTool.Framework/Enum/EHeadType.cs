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

namespace FantasyEngine.TableTool.Framework
{
    /// <summary>
    /// 字段类型
    /// </summary>
    public enum EHeaderType
    {
        STRING = 0,
        SHORT = 1,
        INT = 2,
        LONG = 3,
        USHORT = 4,
        UINT = 5,
        ULONG = 6,
        FLOAT = 7,
        DOUBLE = 8,
        BOOL = 9,
        BYTE = 10,
        CHAR = 11,
        DATE = 12,
        TIME = 13,
        DATETIME = 14,

        ARRAY_STRING = 100,
        ARRAY_SHORT = 101,
        ARRAY_INT = 102,
        ARRAY_LONG = 103,
        ARRAY_USHORT = 104,
        ARRAY_UINT = 105,
        ARRAY_ULONG = 106,
        ARRAY_FLOAT = 107,
        ARRAY_DOUBLE = 108,
        ARRAY_BOOL = 109,
        ARRAY_BYTE = 110,
        ARRAY_CHAR = 111,
        ARRAY_DATE = 112,
        ARRAY_TIME = 113,
        ARRAY_DATETIME = 114,

        ARRAY_ARRAY_STRING = 200,
        ARRAY_ARRAY_SHORT = 201,
        ARRAY_ARRAY_INT = 202,
        ARRAY_ARRAY_LONG = 203,
        ARRAY_ARRAY_USHORT = 204,
        ARRAY_ARRAY_UINT = 205,
        ARRAY_ARRAY_ULONG = 206,
        ARRAY_ARRAY_FLOAT = 207,
        ARRAY_ARRAY_DOUBLE = 208,
        ARRAY_ARRAY_BOOL = 209,
        ARRAY_ARRAY_BYTE = 210,
        ARRAY_ARRAY_CHAR = 211,
        ARRAY_ARRAY_DATE = 212,
        ARRAY_ARRAY_TIME = 213,
        ARRAY_ARRAY_DATETIME = 214,
    }
}
