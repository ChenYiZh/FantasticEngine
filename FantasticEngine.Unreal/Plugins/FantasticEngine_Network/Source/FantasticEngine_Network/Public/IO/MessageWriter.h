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
#include "Net/FESocket.h"
#include "UObject/Object.h"
#include "MessageWriter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, DisplayName="Message Writer")
class FANTASTICENGINE_NETWORK_API UMessageWriter final : public UObject
{
	GENERATED_BODY()
private:
	/**
	 * @brief 消息Id
	 */
	UPROPERTY()
	int64 MsgId;
	/**
	 * @brief 操作码
	 */
	UPROPERTY()
	int8 OpCode = static_cast<int8>(Binary);
	/**
	 * @brief 通讯协议Id
	 */
	UPROPERTY()
	int32 ActionId;
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
	FString Error = FString();
	/**
	 * @brief 内容长度
	 */
	UPROPERTY()
	int32 ContextLength = 0;

public:
	/**
	 * @brief 消息Id
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="MsgId")
	int64 GetMsgId() const;
	/**
	 * @brief 消息Id
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void SetMsgId(int64 InMsgId);
	/**
	 * @brief 操作码
	 */
	//UFUNCTION(BlueprintPure, DisplayName="OpCode")
	int8 GetOpCode() const;
	/**
	 * @brief 操作码
	 */
	//UFUNCTION(BlueprintCallable)
	void SetOpCode(int8 InOpCode);
	/**
	 * @brief 通讯协议Id
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="ActionId")
	int32 GetActionId() const;
	/**
	 * @brief 通讯协议Id
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void SetActionId(int32 InActionId);
	/**
	 * @brief 是否数据压缩
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Compress?")
	bool GetCompress() const;
	/**
	 * @brief 是否数据压缩
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void SetCompress(bool InCompress);
	/**
	 * @brief 是否加密
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Secret?")
	bool GetSecret() const;
	/**
	 * @brief 是否加密
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void SetSecret(bool InSecret);
	/**
	 * @brief 是否有报错
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="IsError?")
	bool IsError() const;
	/**
	 * @brief 错误信息
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Error")
	FString GetError();
	/**
	 * @brief 内容长度
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Context Length")
	int32 GetContextLength() const;
	/**
	 * @brief 包体长度
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Message", DisplayName="Packet Length")
	int32 GetPacketLength() const;

private:
	/**
	 * @brief 缓存池
	 */
	UPROPERTY()
	TArray<uint8> Stream;

public:
	/**
	 * @brief 内容信息
	 */
	uint8* GetContext(int32& OutSize);
	/**
	 * @brief 写入消息头数据
	 */
	void WriteHeader(TArray<uint8>& Buffer, int Offset);

public:
	/**
	 * @brief 写入 Boolean
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteBool(const bool Value);
	/**
	 * @brief 写入 Byte
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteByte(const uint8 Value);
	/**
	 * @brief 写入 Char
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteChar(const TCHAR Value);
	/**
	 * @brief 写入 DateTime
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteDateTime(const FDateTime Value);
	/**
	 * @brief 写入 Double
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteDouble(const double Value);
	/**
	 * @brief 写入 Float
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteFloat(const float Value);
	/**
	 * @brief 写入 Int
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteInt(const int32 Value);
	/**
	 * @brief 写入 Long
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteLong(const int64 Value);
	/**
	 * @brief 写入 SByte
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteSByte(const int8 Value);
	/**
	 * @brief 写入 Short
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteShort(const int16 Value);
	/**
	 * @brief 写入 String
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Message")
	void WriteString(const FString Value);
	/**
	 * @brief 写入 UInt
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteUInt(const uint32 Value);
	/**
	 * @brief 写入 ULong
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteULong(const uint64 Value);
	/**
	 * @brief 写入 UShort
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteUShort(const uint16 Value);
	/**
	 * @brief 写入 Bytes
	 */
	//UFUNCTION(BlueprintCallable)
	void WriteBytes(const uint8* Value, const int32& ValueSize);
};
