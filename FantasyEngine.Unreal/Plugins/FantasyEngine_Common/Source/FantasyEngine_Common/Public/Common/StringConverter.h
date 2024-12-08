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

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StringConverter.generated.h"


/**
 * String 转换
 */
UCLASS(Category="Fantasy Engine", DisplayName="String Converter")
class FANTASYENGINE_COMMON_API UStringConverter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 从FString转到Utf8格式的字节流
	 */
	static void ToUTF8(const FString& Src, TArray<uint8>& Result);
	/**
	 * 从Utf8的字节流转FString
	 */
	static FString FromUTF8(const uint8* Bytes, const int32& Length);

	/**
	 * 获取枚举名称
	 */
	// template <typename TEnum>
	// UE_DEPRECATED(4.xx, "Use ConvertEnum<TEnum>(TEnum EnumValue) insteaded.")
	// static FORCEINLINE FString ConvertEnum(const FString& EnumTypeName, TEnum EnumValue)
	// {
	// 	return ConvertEnum<TEnum>(EnumValue);
	// }

	/**
	 * 获取枚举名称
	 */
	template <typename TEnum>
	static FORCEINLINE FString ConvertEnum(TEnum EnumValue)
	{
		return UEnum::GetValueAsString(EnumValue);
		// FString Name = FString(Typei(EnumValue).name());
		// Name.RemoveAt(0, 5);
		// const UEnum* EnumPtr = FindObject<UEnum>(((UPackage*)-1), *Name, true);
		// if (EnumPtr == nullptr)
		// {
		// 	return TEXT("");
		// }
		// return EnumPtr->GetNameStringByValue((uint8)EnumValue);
	}

	/**
	 * bool 转成 True， False 显示
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static FString ConvertBool(bool InValue);
	/**
	 * Vector保留小数显示
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static FString ConverVector(FVector InVector);
	/**
	 * Vector保留小数显示
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static FString ConvertVector2D(FVector2D InVector);
	/**
	 * 快速加密
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|String Converter")
	static FString FastEncrypt(const FString Src, const int32 Shift);

private:
	/**
	 * 单字节快速加密
	 */
	static TCHAR FastEncryptChar(const TCHAR& Char, const int32& Shift);

public:
	/** string => int16 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToInt16(const FString Src, int16& Result);
	/** string => int32 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToInt32(const FString Src, int32& Result);
	/** string => int64 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToInt64(const FString Src, int64& Result);
	/** string => uint16 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToUInt16(const FString Src, uint16& Result);
	/** string => uint32 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToUInt32(const FString Src, uint32& Result);
	/** string => uint64 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToUInt64(const FString Src, uint64& Result);
	/** string => float */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToFloat(const FString Src, float& Result);
	/** string => double */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToDouble(const FString Src, double& Result);
	/** string => bool */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToBool(const FString Src, bool& Result);
	/** string => uint8 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static bool ToUInt8(const FString Src, uint8& Result);
	/** string => char */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToChar(const FString Src, TCHAR& Result);
	/** string => datetime */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToDateTime(const FString Src, FDateTime& Result);


	/**
	 * string => string[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToStringArray(const FString Src, TArray<FString>& Result);
	/**
	 * string => int16[]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt16Array(const FString Src, TArray<int16>& Result);
	/**
	 * string => int32[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt32Array(const FString Src, TArray<int32>& Result);
	/**
	 * string => int64[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt64Array(const FString Src, TArray<int64>& Result);
	/**
	 * string => uint16[]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt16Array(const FString Src, TArray<uint16>& Result);
	/**
	 * string => uint32[]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt32Array(const FString Src, TArray<uint32>& Result);
	/**
	 * string => uint64[]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt64Array(const FString Src, TArray<uint64>& Result);
	/**
	 * string => float[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToFloatArray(const FString Src, TArray<float>& Result);
	/**
	 * string => double[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToDoubleArray(const FString Src, TArray<double>& Result);
	/**
	 * string => bool[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToBoolArray(const FString Src, TArray<bool>& Result);
	/**
	 * string => uint8[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt8Array(const FString Src, TArray<uint8>& Result);
	/**
	 * string => char[]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToCharArray(const FString Src, TArray<TCHAR>& Result);
	/**
	 * string => datetime[]
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToDateTimeArray(const FString Src, TArray<FDateTime>& Result);

	/**
	 * string => string[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToStringArrayArray(const FString Src, TArray<TArray<FString>>& Result);
	/**
	 * string => int16[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt16ArrayArray(const FString Src, TArray<TArray<int16>>& Result);
	/**
	 * string => int32[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt32ArrayArray(const FString Src, TArray<TArray<int32>>& Result);
	/**
	 * string => int64[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToInt64ArrayArray(const FString Src, TArray<TArray<int64>>& Result);
	/**
	 * string => uint16[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt16ArrayArray(const FString Src, TArray<TArray<uint16>>& Result);
	/**
	 * string => uint32[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt32ArrayArray(const FString Src, TArray<TArray<uint32>>& Result);
	/**
	 * string => uint64[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt64ArrayArray(const FString Src, TArray<TArray<uint64>>& Result);
	/**
	 * string => float[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToFloatArrayArray(const FString Src, TArray<TArray<float>>& Result);
	/**
	 * string => double[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToDoubleArrayArray(const FString Src, TArray<TArray<double>>& Result);
	/**
	 * string => bool[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToBoolArrayArray(const FString Src, TArray<TArray<bool>>& Result);
	/**
	 * string => uint8[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToUInt8ArrayArray(const FString Src, TArray<TArray<uint8>>& Result);
	/**
	 * string => char[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToCharArrayArray(const FString Src, TArray<TArray<TCHAR>>& Result);
	/**
	 * string => datetime[][]
	 */
	//UFUNCTION(BlueprintPure, Category="Fantasy Engine|String Converter")
	static void ToDateTimeArrayArray(const FString Src, TArray<TArray<FDateTime>>& Result);
};
