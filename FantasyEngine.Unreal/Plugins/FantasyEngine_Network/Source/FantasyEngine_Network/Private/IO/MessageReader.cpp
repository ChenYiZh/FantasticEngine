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


#include "IO/MessageReader.h"

#include "Common/ByteUtil.h"
#include "Common/FPackageFactory.h"
#include "Common/SizeUtil.h"
#include "Common/StringConverter.h"
#include "Log/FEConsole.h"

int64 UMessageReader::GetMsgId()
{
	return MsgId;
}

int8 UMessageReader::GetOpCode()
{
	return OpCode;
}

int32 UMessageReader::GetActionId()
{
	return ActionId;
}

bool UMessageReader::GetIsCompress()
{
	return bCompress;
}

bool UMessageReader::GetIsSecret()
{
	return bSecret;
}

bool UMessageReader::IsError()
{
	return !Error.IsEmpty();
}

FString UMessageReader::GetError()
{
	return Error;
}

int32 UMessageReader::GetContentLength()
{
	return ContextLength;
}

uint8* UMessageReader::GetContext()
{
	return Context.GetData();
}

int32 UMessageReader::GetPackageLength()
{
	return PackageLength;
}

void UMessageReader::Initialize(const TArray<uint8>& Package,
                                const int32& Offset,
                                bool bInCompress,
                                bool bInSecret)
{
	bCompress = bInCompress;
	bSecret = bInSecret;
	int32 PackageSize = Package.Num();
	PackageLength = PackageSize - Offset;
	if (PackageLength < UFPackageFactory::HeaderLength)
	{
		Error = TEXT("The message's length is error.");
		return;
	}

	//读取报头
	ReadHeader(Package, Offset);

	//内容获取
	ContextLength = PackageLength - UFPackageFactory::HeaderLength;
	TArray<uint8> Array;
	Array.SetNumUninitialized(ContextLength);
	UByteUtil::BlockCopy(Package, Offset + UFPackageFactory::HeaderLength, Array, 0, ContextLength);
	Context = Array;
}

void UMessageReader::ReadHeader(const TArray<uint8>& Package, const int32& Offset)
{
	int32 Index = Offset;
	MsgId = UByteUtil::ToInt64(Package, Index);
	Index += USizeUtil::LongSize;
	OpCode = static_cast<int8>(Package[Index]);
	Index += 1;
	ActionId = UByteUtil::ToInt32(Package, Index);
}

bool UMessageReader::ReadBool()
{
	return ReadByte() == UByteUtil::ONE;
}

uint8 UMessageReader::ReadByte()
{
	uint8 Byte = Context[ReadIndex];
	ReadIndex++;
	return Byte;
}

TCHAR UMessageReader::ReadChar()
{
	TCHAR TheChar = UByteUtil::ToChar(Context, ReadIndex);
	ReadIndex += USizeUtil::TCHARSize;
	return TheChar;
}

FDateTime UMessageReader::ReadDateTime()
{
	return FDateTime(ReadLong());
}

double UMessageReader::ReadDouble()
{
	double TheDouble = UByteUtil::ToDouble(Context, ReadIndex);
	ReadIndex += USizeUtil::DoubleSize;
	return TheDouble;
}

float UMessageReader::ReadFloat()
{
	float TheFloat = UByteUtil::ToFloat(Context, ReadIndex);
	ReadIndex += USizeUtil::FloatSize;
	return TheFloat;
}

int32 UMessageReader::ReadInt()
{
	int32 TheInt = UByteUtil::ToInt32(Context, ReadIndex);
	ReadIndex += USizeUtil::IntSize;
	return TheInt;
}

int64 UMessageReader::ReadLong()
{
	int64 TheLong = UByteUtil::ToInt64(Context, ReadIndex);
	ReadIndex += USizeUtil::LongSize;
	return TheLong;
}

int8 UMessageReader::ReadSByte()
{
	return static_cast<int8>(ReadByte());
}

int16 UMessageReader::ReadShort()
{
	int16 TheShort = UByteUtil::ToInt16(Context, ReadIndex);
	ReadIndex += USizeUtil::ShortSize;
	return TheShort;
}

FString UMessageReader::ReadString()
{
	int32 Length = ReadInt();
	FString Result = UStringConverter::FromUTF8(&Context[ReadIndex], Length);
	//FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(&Context[ReadIndex]), Length);
	ReadIndex += Length;
	//return FString(TCHARData.Length(), TCHARData.Get());
	return Result;
}

uint32 UMessageReader::ReadUInt()
{
	uint32 TheUInt = UByteUtil::ToUInt32(Context, ReadIndex);
	ReadIndex += USizeUtil::UIntSize;
	return TheUInt;
}

uint64 UMessageReader::ReadULong()
{
	uint64 TheULong = UByteUtil::ToUInt64(Context, ReadIndex);
	ReadIndex += USizeUtil::ULongSize;
	return TheULong;
}

uint16 UMessageReader::ReadUShort()
{
	uint16 TheUShort = UByteUtil::ToUInt16(Context, ReadIndex);
	ReadIndex += USizeUtil::UShortSize;
	return TheUShort;
}
