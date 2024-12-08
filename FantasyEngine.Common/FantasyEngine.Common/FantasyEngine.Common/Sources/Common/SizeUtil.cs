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

namespace FantasyEngine
{
    public static class SizeUtil
    {
        public static int BoolSize { get; private set; }

        public static int CharSize { get; private set; }


        public static int FloatSize { get; private set; }

        public static int DoubleSize { get; private set; }

        public static int DecimalSize { get; private set; }


        public static int SByteSize { get; private set; }

        public static int ShortSize { get; private set; }

        public static int IntSize { get; private set; }

        public static int LongSize { get; private set; }


        public static int ByteSize { get; private set; }

        public static int UShortSize { get; private set; }

        public static int UIntSize { get; private set; }

        public static int ULongSize { get; private set; }

        static SizeUtil()
        {
            BoolSize = sizeof(bool);
            CharSize = sizeof(char);

            FloatSize = sizeof(float);
            DoubleSize = sizeof(double);
            DecimalSize = sizeof(decimal);

            SByteSize = sizeof(sbyte);
            ShortSize = sizeof(short);
            IntSize = sizeof(int);
            LongSize = sizeof(long);

            ByteSize = sizeof(byte);
            UShortSize = sizeof(ushort);
            UIntSize = sizeof(uint);
            ULongSize = sizeof(ulong);
        }
    }
}
