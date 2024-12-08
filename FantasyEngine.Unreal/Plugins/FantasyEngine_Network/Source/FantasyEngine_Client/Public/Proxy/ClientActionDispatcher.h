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
#include "Action/ClientAction.h"
#include "UObject/Object.h"
#include "ClientActionDispatcher.generated.h"

/**
 * Action工厂
 */
UCLASS(NotBlueprintable, NotBlueprintType, DisplayName="Client Action Dispatcher")
class FANTASYENGINE_CLIENT_API UClientActionDispatcher final : public UObject
{
	GENERATED_BODY()
	friend class UClientSocket;
private:
	/**
	 * @brief Action名称的格式
	 */
	UPROPERTY()
	TArray<FString> ActionNameFormats;
	/**
	 * @brief 缓存的消息队列
	 */
	UPROPERTY()
	TArray<UMessageReader*> Messages;
	/**
	 * @brief 线程锁
	 */
	FCriticalSection Mutex;
	/**
	 * @brief 每帧执行结构
	 */
	FTSTicker::FDelegateHandle TimeHandle;
public:
	/**
	 * @brief 生成Action协议
	 */
	virtual void Provide(UMessageReader* Message);
	/**
	 * @brief Action操作
	 */
	virtual bool Tick(float DeltaSeconds);
	// /**
	//  * @brief Action名称的格式
	//  */
	// UFUNCTION(BlueprintPure)
	// FString GetActionNameFormat();

public:
	/**
	 * @brief 创建计时器
	 */
	virtual void PostInitProperties() override;
	/**
	 * @brief 移除计时器
	 */
	virtual void BeginDestroy() override;
};
