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


#include "Common/FEPackageFactory.h"

#include "Common/ByteUtility.h"
#include "Common/RandomUtil.h"
#include "Log/FEConsole.h"
#include "Log/FENCategories.h"

const int32 UFEPackageFactory::HeaderLength = USizeUtility::LongSize + USizeUtility::SByteSize + USizeUtility::IntSize;

int32 UFEPackageFactory::GetHeaderLength()
{
	return HeaderLength;
}

void UFEPackageFactory::Pack(TArray<uint8>& OutBuffer, UMessageWriter* Message, const int32& Offset,
                            UCompression* Compression, UCryptoProvider* CryptoProvider)
{
	if (Message == nullptr)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("The message is null on packing."));
		//uint8 NullResult[0];
		TArray<uint8> Array;
		OutBuffer = Array;
		return;
	}
	int32 ContextSize;
	uint8* Context = Message->GetContext(ContextSize);
	int32 BufferSize = HeaderLength + ContextSize;
	//uint8 Array[BufferSize];
	//uint8* Buffer = Array;
	TArray<uint8> Buffer;
	Buffer.SetNumUninitialized(BufferSize);
	Message->WriteHeader(Buffer, 0);
	UByteUtility::BlockCopy(Context, 0, Buffer, HeaderLength, Message->GetContextLength());
	bool bCompress = Message->GetCompress() && Compression != nullptr;
	bool bCrypto = Message->GetSecret() && CryptoProvider != nullptr;
	EMessageType Type = EMessageType::NoProcess;
	if (bCompress && bCrypto)
	{
		Type = EMessageType::CompressAndCrypto;
	}
	else if (bCompress)
	{
		Type = EMessageType::OnlyCompress;
	}
	else if (bCrypto)
	{
		Type = EMessageType::OnlyCrypto;
	}
	if (bCrypto)
	{
		Buffer = CryptoProvider->Encrypt(Buffer);
	}
	if (bCompress)
	{
		Buffer = Compression->Compress(Buffer);
	}
	int32 OutDataSize = Offset + USizeUtility::IntSize + 1 + BufferSize;
	TArray<uint8> Result;
	Result.SetNumUninitialized(OutDataSize);
	for (int i = 0; i < Offset; i++)
	{
		Result[i] = URandomUtil::RandomByte();
	}
	uint8* LenBs = reinterpret_cast<uint8*>(&BufferSize);
	for (int i = 0; i < USizeUtility::IntSize; i++)
	{
		Result[Offset + i] = LenBs[i];
	}
	Result[Offset + USizeUtility::IntSize] = static_cast<uint8>(Type);
	UByteUtility::BlockCopy(Buffer, 0, Result, OutDataSize - BufferSize, BufferSize);
	OutBuffer = Result;
}

UMessageReader* UFEPackageFactory::Unpack(const TArray<uint8>& Package, const int32& Offset,
                                         UCompression* Compression, UCryptoProvider* CryptoProvider)
{
	int32 Pos = 0;

	Pos += Offset;
	int32 Length = UByteUtility::ToInt32(Package, Pos);
	Pos += USizeUtility::IntSize;
	EMessageType Type = static_cast<EMessageType>(Package[Pos]);
	Pos += 1;


	int32 DataSize = Length;
	TArray<uint8> Data;
	Data.SetNumUninitialized(DataSize);
	UByteUtility::BlockCopy(Package, Pos, Data, 0, Length);
	Pos += Length;

	bool bCompress = Type == EMessageType::OnlyCompress || Type == EMessageType::CompressAndCrypto;
	bool bCrypto = Type == EMessageType::OnlyCrypto || Type == EMessageType::CompressAndCrypto;
	if (bCompress)
	{
		Data = Compression->Uncompress(Data);
	}
	if (bCrypto)
	{
		Data = CryptoProvider->Decrypt(Data);
	}
	UMessageReader* Reader = NewObject<UMessageReader>(/*GEngine->GetCurrentPlayWorld()*/);
	Reader->Initialize(Data, 0, bCompress, bCrypto);
	return Reader;
}

int32 UFEPackageFactory::GetTotalLength(const TArray<uint8>& Package, const int32& Offset)
{
	int32 LocalHeaderLength = Offset + USizeUtility::IntSize + 1;
	if (Package.Num() < LocalHeaderLength)
	{
		return -1;
	}
	int32 Length = UByteUtility::ToInt32(Package, Offset);
	//偏移值，包体大小，消息格式
	return LocalHeaderLength + Length;
}
