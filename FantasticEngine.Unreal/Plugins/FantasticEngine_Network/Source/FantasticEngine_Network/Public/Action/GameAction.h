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
#include "WorldObject.h"
#include "IO/MessageReader.h"
#include "UObject/Object.h"
#include "GameAction.generated.h"


/**
 * @brief 消息基类
 */
UCLASS(Blueprintable, BlueprintType, DisplayName="Game Action")
class FANTASTICENGINE_NETWORK_API UGameAction : public UWorldObject
{
	GENERATED_BODY()

private:
	/**
	 * @brief 消息Id
	 */
	long mMsgId;
	/**
	 * @brief ActionId
	 */
	int mActionId;
	/**
	 * @brief Action执行的时机
	 */
	FDateTime EnterTime;
	/**
	 * @brief 接收到的消息
	 */
	UPROPERTY()
	UMessageReader* mReader = nullptr;

protected:
	/**
	 * @brief 是否统计时间
	 */
	UPROPERTY(BlueprintReadWrite, Category="Fantastic Engine|Game Action")
	bool bAnalysisTime;
	/**
	 * @brief 警告超时时间毫秒，0为全部统计
	 */
	UPROPERTY(BlueprintReadWrite, Category="Fantastic Engine|Game Action")
	int AlertTimeout = 1000;

public:
	/**
	 * @brief 消息Id
	 * @return 消息Id
	 */
	//UFUNCTION(BlueprintPure)
	long GetMsgId() const;
	/**
	 * @brief ActionId
	 * @return ActionId
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Game Action")
	int GetActionId() const;
	/**
	 * @brief 是否统计时间
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Game Action")
	bool GetAnalysisTime() const;
	/**
	 * @brief 警告超时时间毫秒，0为全部统计
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Game Action")
	int GetAlertTimeout() const;

protected:
	/**
	 * @brief 类名
	 * @return 类名
	 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Game Action")
	FName GetTypeName() const;

public:
	/**
	 * @brief 刚创建时处理，所有参数都还没有赋值
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Fantastic Engine|Game Action")
	void Awake();
	/**
	 * @brief 工作函数
	 * @param InActionId 消息Id
	 * @param InReader 消息
	 */
	void Work(int InActionId, UMessageReader* InReader);
	/**
	 * @brief 判断有效性
	 * @return 有效性
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Fantastic Engine|Game Action")
	bool Check();
	/**
	 * @brief 消息预处理
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Fantastic Engine|Game Action")
	void SetReader(UMessageReader* Reader);
	/**
	 * @brief 消息处理
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Fantastic Engine|Game Action")
	void TakeAction(UMessageReader* Reader);
};
