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
#include "RandomUtil.generated.h"

/**
 * 随机数管理
 */
UCLASS(Category="Fantasy Engine", DisplayName="Random")
class FANTASYENGINE_COMMON_API URandomUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 随机一个字节
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Random", DisplayName="Random Range (Byte)")
	static uint8 RandomByte();
	/**
	 * 范围随机
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Random", DisplayName="Random Range (Int)")
	static int32 RandomInt(int32 Min, int32 Max);
	/**
	 * 范围随机
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Random", DisplayName="Random Range (Float)")
	static float RandomFloat(float Min, float Max);
	/**
	 * 范围随机
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Random", DisplayName="Random Range (Double)")
	static double RandomDouble(double Min, double Max);
};
