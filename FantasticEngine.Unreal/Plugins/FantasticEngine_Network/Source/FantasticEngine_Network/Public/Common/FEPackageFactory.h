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

#pragma once


#include "CoreMinimal.h"
#include "Common/SizeUtility.h"
#include "IO/MessageReader.h"
#include "IO/MessageWriter.h"
#include "UObject/Object.h"
#include "FEPackageFactory.generated.h"

/**
 * 消息类型
 */
UENUM(BlueprintType)
enum EMessageType
{
	/**
	 * @brief 什么都不处理
	 */
	NoProcess = 0,
	/**
	 * @brief 压缩过
	 */
	OnlyCompress = 10,
	/**
	 * @brief 加密过
	 */
	OnlyCrypto = 15,
	/**
	 * @brief 解压+加密
	 */
	CompressAndCrypto = 50,
};

/**
 * 网络处理类
 */
UCLASS(DisplayName="Fantastic Engine|Package Factory")
class FANTASTICENGINE_NETWORK_API UFEPackageFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 消息的头数据大小 */
	static const int32 HeaderLength;
	/** 消息的头数据大小 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Package Factory", DisplayName="Header Length")
	static int32 GetHeaderLength();

public:
	/**
	 * @brief 打包
	 * @param Message 消息
	 * @param Offset 偏移值
	 * @param Compression 压缩方案
	 * @param CryptoProvider 加密方案
	 * @return 字节流
	 */
	//UFUNCTION(BlueprintCallable)
	static void Pack(TArray<uint8>& OutBuffer, UMessageWriter* Message, const int32& Offset, UCompression* Compression,
	                 UCryptoProvider* CryptoProvider);
	/**
	 * @brief 解包
	 * @param Package 数据
	 * @param Offset 偏移值
	 * @param Compression 压缩方案
	 * @param CryptoProvider 加密方案
	 * @return 解析包
	 */
	//UFUNCTION(BlueprintCallable)
	static UMessageReader* Unpack(const TArray<uint8>& Package, const int32& Offset,
	                              UCompression* Compression, UCryptoProvider* CryptoProvider);
	/**
	 * @brief 预解析整体包体大小
	 */
	static int32 GetTotalLength(const TArray<uint8>& Package, const int32& Offset);
};

