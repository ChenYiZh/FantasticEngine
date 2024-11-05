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
#include "UObject/Object.h"
#include "MessageReader.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, DisplayName="Message Reader")
class FANTASTICENGINE_NETWORK_API UMessageReader : public UObject
{
	GENERATED_BODY()
private:
	/**
	 * @brief 消息Id
	 */
	UPROPERTY()
	int64 MsgId = 0;
	/**
	 * @brief 操作码
	 */
	int8 OpCode = 0;
	/**
	 * @brief 通讯协议Id
	 */
	UPROPERTY()
	int32 ActionId = 0;
	/**
	 * @brief 是否数据压缩
	 */
	UPROPERTY()
	bool bCompress = false;
	/**
	 * @brief 是否加密
	 */
	UPROPERTY()
	bool bSecret = false;
	/**
	 * @brief 错误信息
	 */
	UPROPERTY()
	FString Error;
	/**
	 * @brief 内容长度
	 */
	UPROPERTY()
	int32 ContextLength = 0;
	/**
	 * @brief 读取的数据指针
	 */
	UPROPERTY()
	int32 ReadIndex = 0;
	/**
	 * @brief 通信包内容
	 */
	UPROPERTY()
	TArray<uint8> Context;
	/**
	 * @brief 包体长度
	 */
	UPROPERTY()
	int32 PackageLength = 0;
public:
	/**
	 * @brief 消息Id
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="MsgId")
	int64 GetMsgId();
	/**
	 * @brief 操作码
	 */
	int8 GetOpCode();
	/**
	 * @brief 通讯协议Id
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="ActionId")
	int32 GetActionId();
	/**
	 * @brief 是否数据压缩
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Compress?")
	bool GetIsCompress();
	/**
	 * @brief 是否加密
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Secret?")
	bool GetIsSecret();
	/**
	 * @brief 是否有报错
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message")
	bool IsError();
	/**
	 * @brief 错误信息
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Error")
	FString GetError();
	/**
	 * @brief 内容长度
	 */
	UFUNCTION(BlueprintPure, DisplayName="Context Length")
	int32 GetContentLength();
	/**
	 * @brief 通信包内容
	 */
	uint8* GetContext();
	/**
	 * @brief 包体长度
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message")
	int32 GetPackageLength();
public:
	/**
	 * @brief 创建时调用
	 */
	void Initialize(const TArray<uint8>& Package, const int32& Offset, bool bInCompress, bool bInSecret);

protected:
	/**
	 * @brief 消息的基本数据读取
	 */
	virtual void ReadHeader(const TArray<uint8>& Package, const int32& Offset);

public:
	/**
	 * @brief 读取 Boolean
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	bool ReadBool();
	/**
	 * @brief 读取 Byte
	 */
	uint8 ReadByte();
	/**
	 * @brief 读取 Char
	 */
	TCHAR ReadChar();
	/**
	 * @brief 读取时间
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	FDateTime ReadDateTime();
	/**
	 * @brief 读取 Double
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	double ReadDouble();
	/**
	 * @brief 读取 Float
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	float ReadFloat();
	/**
	 * @brief 读取 Int
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	int32 ReadInt();
	/**
	 * @brief 读取 Long
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	int64 ReadLong();
	/**
	 * @brief 读取 SByte
	 */
	int8 ReadSByte();
	/**
	 * @brief 读取 Short
	 */
	//UFUNCTION(BlueprintCallable)
	int16 ReadShort();
	/**
	 * @brief 读取字符串
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	FString ReadString();
	/**
	 * @brief 读取 UInt
	 */
	//UFUNCTION(BlueprintCallable)
	uint32 ReadUInt();
	/**
	 * @brief 读取 ULong
	 */
	uint64 ReadULong();
	/**
	 * @brief 读取 UShort
	 */
	uint16 ReadUShort();
};
