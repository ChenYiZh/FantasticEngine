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


#include "Common/ByteUtil.h"

#include "Common/SizeUtil.h"

uint8 UByteUtil::GetOne()
{
	return ONE;
}

uint8 UByteUtil::GetZero()
{
	return ZERO;
}

void UByteUtil::BlockCopy(uint8* Src, const int32& SrcOffset, uint8* Dst, const int32& DstOffset, const int32& Count)
{
	FMemory::Memcpy(&Dst[DstOffset], &Src[SrcOffset], Count);
	// for (int i = 0; i < Count; i++)
	// {
	// 	Dst[DstOffset + i] = Src[SrcOffset + i];
	// }
}

void UByteUtil::BlockCopy(uint8* Src, const int32& SrcOffset, TArray<uint8>& Dst, const int32& DstOffset,
                          const int32& Count)
{
	FMemory::Memcpy(&Dst.GetData()[DstOffset], &Src[SrcOffset], Count);
}

void UByteUtil::BlockCopy(const TArray<uint8>& Src, const int32& SrcOffset, TArray<uint8>& Dst, const int32& DstOffset,
                          const int32& Count)
{
	FMemory::Memcpy(&Dst.GetData()[DstOffset], &Src.GetData()[SrcOffset], Count);
}

void UByteUtil::Convert(void* Dst, const TArray<uint8>& Src, const int32& StartIndex, int32 Length)
{
	FMemory::Memcpy(Dst, &Src.GetData()[StartIndex], Length);
}

bool UByteUtil::ToBoolean(const TArray<uint8>& Src, const int32& StartIndex)
{
	bool Value;
	Convert(&Value, Src, StartIndex, USizeUtil::BoolSize);
	return Value;
	//return *reinterpret_cast<bool*>(GetBytes(Value, StartIndex, USizeUtil::BoolSize));
}

TCHAR UByteUtil::ToChar(const TArray<uint8>& Src, const int32& StartIndex)
{
	TCHAR Value;
	Convert(&Value, Src, StartIndex, USizeUtil::TCHARSize);
	return Value;
	//return *reinterpret_cast<char*>(GetBytes(Value, StartIndex, USizeUtil::CharSize));
}

float UByteUtil::ToFloat(const TArray<uint8>& Src, const int32& StartIndex)
{
	float Value;
	Convert(&Value, Src, StartIndex, USizeUtil::FloatSize);
	return Value;
	//return *reinterpret_cast<float*>(GetBytes(Value, StartIndex, USizeUtil::FloatSize));
}

double UByteUtil::ToDouble(const TArray<uint8>& Src, const int32& StartIndex)
{
	double Value;
	Convert(&Value, Src, StartIndex, USizeUtil::DoubleSize);
	return Value;
	//return *reinterpret_cast<double*>(GetBytes(Value, StartIndex, USizeUtil::DoubleSize));
}

int16 UByteUtil::ToInt16(const TArray<uint8>& Src, const int32& StartIndex)
{
	int16 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::ShortSize);
	return Value;
	//return *reinterpret_cast<int16*>(GetBytes(Value, StartIndex, USizeUtil::ShortSize));
}

int32 UByteUtil::ToInt32(const TArray<uint8>& Src, const int32& StartIndex)
{
	int32 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::IntSize);
	return Value;
	//return *reinterpret_cast<int32*>(GetBytes(Value, StartIndex, USizeUtil::IntSize));
}

int64 UByteUtil::ToInt64(const TArray<uint8>& Src, const int32& StartIndex)
{
	int64 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::LongSize);
	return Value;
	//return *reinterpret_cast<int64*>(GetBytes(Value, StartIndex, USizeUtil::LongSize));
}

uint16 UByteUtil::ToUInt16(const TArray<uint8>& Src, const int32& StartIndex)
{
	uint16 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::UShortSize);
	return Value;
	//return *reinterpret_cast<uint16*>(GetBytes(Value, StartIndex, USizeUtil::UShortSize));
}

uint32 UByteUtil::ToUInt32(const TArray<uint8>& Src, const int32& StartIndex)
{
	uint32 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::UIntSize);
	return Value;
	//return *reinterpret_cast<uint32*>(GetBytes(Value, StartIndex, USizeUtil::UIntSize));
}

uint64 UByteUtil::ToUInt64(const TArray<uint8>& Src, const int32& StartIndex)
{
	uint64 Value;
	Convert(&Value, Src, StartIndex, USizeUtil::ULongSize);
	return Value;
	//return *reinterpret_cast<uint64*>(GetBytes(Value, StartIndex, USizeUtil::ULongSize));
}
