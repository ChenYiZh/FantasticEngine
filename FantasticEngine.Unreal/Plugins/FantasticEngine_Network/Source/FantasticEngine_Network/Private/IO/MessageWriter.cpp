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


#include "IO/MessageWriter.h"

#include "Common/ByteUtil.h"
#include "Common/FPackageFactory.h"
#include "Common/RandomUtil.h"
#include "Common/StringConverter.h"

int64 UMessageWriter::GetMsgId() const
{
	return MsgId;
}

void UMessageWriter::SetMsgId(int64 InMsgId)
{
	MsgId = InMsgId;
}

int8 UMessageWriter::GetOpCode() const
{
	return OpCode;
}

void UMessageWriter::SetOpCode(int8 InOpCode)
{
	OpCode = InOpCode;
}

int32 UMessageWriter::GetActionId() const
{
	return ActionId;
}

void UMessageWriter::SetActionId(int32 InActionId)
{
	ActionId = InActionId;
}

bool UMessageWriter::GetCompress() const
{
	return bCompress;
}

void UMessageWriter::SetCompress(bool InCompress)
{
	bCompress = InCompress;
}

bool UMessageWriter::GetSecret() const
{
	return bSecret;
}

void UMessageWriter::SetSecret(bool InSecret)
{
	bSecret = InSecret;
}

bool UMessageWriter::IsError() const
{
	return !Error.IsEmpty();
}

FString UMessageWriter::GetError()
{
	return Error;
}

int32 UMessageWriter::GetContextLength() const
{
	return ContextLength;
}

int32 UMessageWriter::GetPacketLength() const
{
	return UFPackageFactory::HeaderLength + ContextLength;
}

uint8* UMessageWriter::GetContext(int32& OutSize)
{
	OutSize = Stream.Num();
	return Stream.GetData();
}

void UMessageWriter::WriteHeader(TArray<uint8>& Buffer, int Offset)
{
	for (int i = 0; i < Offset; i++)
	{
		Buffer[i] = URandomUtil::RandomByte();
	}
	int Index = Offset;
	uint8* Data = reinterpret_cast<uint8*>(&MsgId);
	for (int i = 0; i < USizeUtil::LongSize; i++)
	{
		Buffer[Index + i] = Data[i];
	}
	Index += USizeUtil::LongSize;
	Buffer[Index] = static_cast<uint8>(OpCode);
	Index += 1;
	Data = reinterpret_cast<uint8*>(&ActionId);
	for (int i = 0; i < USizeUtil::IntSize; i++)
	{
		Buffer[Index + i] = Data[i];
	}
}

void UMessageWriter::WriteBool(const bool Value)
{
	WriteByte(Value ? UByteUtil::ONE : UByteUtil::ZERO);
}

void UMessageWriter::WriteByte(const uint8 Value)
{
	Stream.Push(Value);
	ContextLength += 1;
}

void UMessageWriter::WriteChar(const TCHAR Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::TCHARSize);
}

void UMessageWriter::WriteDateTime(const FDateTime Value)
{
	const int64 Ticks = Value.GetTicks();
	WriteBytes(reinterpret_cast<const uint8*>(&Ticks), USizeUtil::LongSize);
}

void UMessageWriter::WriteDouble(const double Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::DoubleSize);
}

void UMessageWriter::WriteFloat(const float Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::FloatSize);
}

void UMessageWriter::WriteInt(const int32 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::IntSize);
}

void UMessageWriter::WriteLong(const int64 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::LongSize);
}

void UMessageWriter::WriteSByte(const int8 Value)
{
	WriteByte(static_cast<const uint8>(Value));
}

void UMessageWriter::WriteShort(const int16 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::ShortSize);
}

void UMessageWriter::WriteString(const FString Value)
{
	TArray<uint8> Utf8Bytes;
	UStringConverter::ToUTF8(Value, Utf8Bytes);
	int32 Length = Utf8Bytes.Num();
	WriteInt(Length);
	WriteBytes(Utf8Bytes.GetData(), Length);
}

void UMessageWriter::WriteUInt(const uint32 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::UIntSize);
}

void UMessageWriter::WriteULong(const uint64 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::ULongSize);
}

void UMessageWriter::WriteUShort(const uint16 Value)
{
	WriteBytes(reinterpret_cast<const uint8*>(&Value), USizeUtil::UShortSize);
}

void UMessageWriter::WriteBytes(const uint8* Value, const int32& ValueSize)
{
	Stream.Append(Value, ValueSize);
	ContextLength += ValueSize;
}
