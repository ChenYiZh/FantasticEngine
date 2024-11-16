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
#include "ByteUtility.generated.h"

/**
 * 字节流处理
 */
UCLASS(Category="Fantastic Engine", DisplayName="Byte")
class FANTASTICENGINE_COMMON_API UByteUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 1字节 */
	inline constexpr static uint8 ONE = 1;
	/** 0字节 */
	inline constexpr static uint8 ZERO = 0;

public:
	/** 1字节 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Byte", DisplayName="ONE")
	static uint8 GetOne();
	/** 0字节 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Byte", DisplayName="ZERO")
	static uint8 GetZero();

public:
	/** 深拷贝 */
	static void BlockCopy(uint8* Src, const int32& SrcOffset, uint8* Dst, const int32& DstOffset, const int32& Count);
	/** 深拷贝 */
	static void BlockCopy(uint8* Src, const int32& SrcOffset, TArray<uint8>& Dst, const int32& DstOffset,
	                      const int32& Count);
	/** 深拷贝 */
	static void BlockCopy(const TArray<uint8>& Src, const int32& SrcOffset, TArray<uint8>& Dst, const int32& DstOffset,
	                      const int32& Count);

public:
	/**
	 * 提取数组
	 */
	//static uint8* GetBytes(uint8* Src, const int32& StartIndex, int32 Length);
	/** 数据转换 */
	static void Convert(void* Dst, const TArray<uint8>& Src, const int32& StartIndex, int32 Length);
	/** 转 Boolean */
	static bool ToBoolean(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Char */
	static TCHAR ToChar(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Float */
	static float ToFloat(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Double */
	static double ToDouble(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Int16 */
	static int16 ToInt16(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Int32 */
	static int32 ToInt32(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 Int64 */
	static int64 ToInt64(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 UInt64 */
	static uint16 ToUInt16(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 UInt32 */
	static uint32 ToUInt32(const TArray<uint8>& Src, const int32& StartIndex);
	/** 转 UInt64 */
	static uint64 ToUInt64(const TArray<uint8>& Src, const int32& StartIndex);
};
