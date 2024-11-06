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
#include "ILogger.h"
#include "LogLevel.h"
#include "UObject/Object.h"
#include "FEConsole.generated.h"

/**
 * 输出类
 */
UCLASS(NotBlueprintable, NotBlueprintType, Category="Fantastic Engine", DisplayName="FEConsole")
class FANTASTICENGINE_COMMON_API UFEConsole final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * 是否输出堆栈
	 */
	inline static bool bLogStackTracker = false;
	// /**
	//  * 默认在屏幕上显示的时长
	//  */
	// inline static float DefaultDisplaySeconds = 5.f;
	/**
	 * 默认类别名称
	 */
	inline static FName CATEGORY = FName(TEXT("Log"));
	/**
	 * 需要输出堆栈的Level
	 */
	inline static TSet<FName> LogStackLevels = TSet<FName>{ULogLevel::GET_ERROR()};
	/**
	 * 已注册的Logger
	 */
	inline static TSet<UObject*> Loggers;
	/**
	 * 线程锁
	 */
	inline static FCriticalSection Mutex;

public:
	/**
	 * 是否输出堆栈
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|FEConsole", DisplayName="Log Stack Tracker?")
	static bool GetLogStackTracker();
	/**
	 * 是否输出堆栈
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void SetLogStackTracker(bool bLogTracker);
	// /**
	//  * 是否输出堆栈
	//  */
	// UFUNCTION(BlueprintPure, DisplayName="Time To Display")
	// static float GetDefaultDisplaySeconds();
	// /**
	//  * 是否输出堆栈
	//  */
	// UFUNCTION(BlueprintCallable)
	// static void SetDefaultDisplaySeconds(const float& TimeToDisplay);
	/**
	 * 默认类别名称
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|FEConsole")
	static FName GetDefaultCategory();
	/**
	 * 默认类别名称
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void SetDefaultCategory(FName Category);
	/**
	 * 添加需要输出堆栈的Level
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void PushTrackLevel(FName Level);
	/**
	 * 移除需要输出堆栈的Level
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void RemoveTrackLevel(FName Level);
	/**
	 * 注册Logger
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static bool RegistLogger(UObject* Logger);
	/**
	 * 移除Logger
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static bool RemoveLogger(UObject* Logger);
	/**
	 * 输出到Debug目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void Write(FString Message, const bool AddToScreen = true, float TimeToDisplay = 5.f);
	/**
	 * 输出到Debug目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteWithCategory(const FName Category, FString Message, const bool AddToScreen = true,
	                              float TimeToDisplay = 5.f);
	/**
	 * 输出到Info目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteInfo(FString Message, const bool AddToScreen = true, float TimeToDisplay = 5.f);
	/**
	 * 输出到Info目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteInfoWithCategory(const FName Category, FString Message, const bool AddToScreen = true,
	                                  float TimeToDisplay = 5.f);
	/**
	 * 输出到Warn目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteWarn(FString Message, const bool AddToScreen = true, float TimeToDisplay = 5.f);
	/**
	 * 输出到Warn目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteWarnWithCategory(const FName Category, FString Message, const bool AddToScreen = true,
	                                  float TimeToDisplay = 5.f);
	/**
	 * 输出到Exception目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteError(FString Message, const bool AddToScreen = true, float TimeToDisplay = 5.f);
	/**
	 * 输出到Exception目录
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteErrorWithCategory(const FName Category, FString Message, const bool AddToScreen = true,
	                                   float TimeToDisplay = 5.f);
	/**
	 * 输出到指定类别
	 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|FEConsole")
	static void WriteTo(const FName Level, const FName Category, FString Message, const bool AddToScreen = true,
	                    float TimeToDisplay = 5.f);

	/**
	 * 输出到指定类别
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|FEConsole")
	static FString FormatCustomMessage(const FName& Level, const FName& Category, FString Message);

	/**
	 * 缓存释放
	 */
	static void Release();

private:
	/**
	 * 消息发送实现
	 */
	//UFUNCTION(BlueprintCallable)
	static void SendLogMessage(const FName& Level, const FName& Category, FString& Message, const bool& bTrackconst,
	                        const bool& AddToScreen, float TimeToDisplay);
	/**
	 * 消息发送实现
	 */
	static void SendLogMessage(const FName& Level, const FString& Message, const bool& AddToScreen,
	                        const float& TimeToDisplay);
};
