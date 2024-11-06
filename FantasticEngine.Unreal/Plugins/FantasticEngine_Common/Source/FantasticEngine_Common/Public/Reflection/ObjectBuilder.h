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
#include "ObjectBuilder.generated.h"

/**
 * 反射创建对象
 */
UCLASS(Category="Fantastic Engine", DisplayName="Object Builder")
class FANTASTICENGINE_COMMON_API UObjectBuilder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * 缓存池
	 */
	inline static TMap<FString, FStaticConstructObjectParameters> Types;
	/**
	 * 线程锁
	 */
	inline static FCriticalSection Mutex;

public:
	/**
	 * 判断名称是不是蓝图
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Object Builder")
	static bool IsBlueprintName(const FString& ClassName);
	/**
	 * 自动搜索创建，蓝图使用时必须是同步执行
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Object Builder", DisplayName="Create",
		meta=(bIgnoreSelf = "true", WorldContext="WorldContextObject", DeterminesOutputType="SubClass"))
	static UObject* CreateObjectByName(UObject* WorldContextObject, const FString& ClassName,
	                                   TSubclassOf<UObject> SubClass = nullptr);

	// /**
	//  * 查询某个目录下的所有蓝图
	//  */
	// UFUNCTION(BlueprintCallable)
	// static void FindAllBlueprintClassFromPath(TArray<UClass*>& Result, const FString& Path, const FString& LikeName);

	/**
	 * 自动搜索创建
	 */
	template <typename T>
	static T* Create(UObject* WorldContextObject, const FString& ClassName)
	{
		return Cast<T>(CreateObjectByName(WorldContextObject, ClassName));
	}

	// /**
	//  * 自动搜索创建，如果是蓝图会转到主线程执行
	//  */
	// template <typename T>
	// static void CreateAsync(const FString& ClassName, TUniqueFunction<void(T*)> Callback)
	// {
	// 	if (IsBlueprintName(ClassName))
	// 	{
	// 		AsyncTask(ENamedThreads::GameThread, [&]()
	// 		{
	// 			Callback(Create<T>(ClassName));
	// 		});
	// 	}
	// 	Callback(Create<T>(ClassName));
	// }
};
