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
#include "FantasyEngineCommonCategories.generated.h"

/**
 * 类别类
 */
UCLASS(NotBlueprintable, NotBlueprintType, DisplayName="Fantasy Engine|Categories")
class FANTASYENGINE_COMMON_API UFantasyEngineCommonCategories final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * TIME_LORD
	 */
	inline const static FName TIME_LORD = FName(TEXT("Time Lord"));
	/**
	 * REFLECTION
	 */
	inline const static FName REFLECTION = FName(TEXT("Reflection"));
	/**
	 * HTTP
	 */
	inline const static FName HTTP = FName(TEXT("Http"));

public:
	/**
	 * TIME_LORD
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Categories", DisplayName="TIME LORD")
	static FName GET_TIME_LORD();
	/**
	 * REFLECTION
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Categories", DisplayName="REFLECTION")
	static FName GET_REFLECTION();
	/**
	 * HTTP
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Categories", DisplayName="HTTP")
	static FName GET_HTTP();
};
