// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FESocket.h"
#include "UObject/NoExportTypes.h"
#include "UserToken.generated.h"

/**
 * 寄宿在原生Socket的管理类
 */
UCLASS()
class FANTASYENGINE_NETWORK_API UUserToken : public UObject
{
	GENERATED_BODY()
	friend UFESocket;

private:
	/** 嵌套的Socket */
	UPROPERTY()
	TWeakObjectPtr<UFESocket> Socket;

public:
	/** 嵌套的Socket */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Socket|User Token")
	UFESocket* GetSocket() const;

public:
	/** 解析包时解析不完的数据 */
	UPROPERTY()
	TArray<uint8> ReceivedBuffer;

	/** 已经接收的数据长度 */
	UPROPERTY()
	int32 ReceivedStartIndex = 0;

private:
	/** 原本的生成时的offset */
	UPROPERTY()
	int32 OriginalOffset;
	/** 原本缓存的字节长度 */
	UPROPERTY()
	int32 OriginalLength;

public:
	/** 原本的生成时的offset */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Socket|User Token")
	int32 GetOriginalOffset() const;

	/** 原本缓存的字节长度 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Socket|User Token")
	int32 GetOriginalLength() const;

public:
	/** 设置原本的生成时的offset */
	void SetOriginalOffset(int32 Offset, int32 Length)
	{
		OriginalOffset = Offset;
		OriginalLength = Length;
	}

public:
	/** 已经发送的字节数量 */
	UPROPERTY()
	int32 SentCount;
	/** 正在发送的数据 */
	UPROPERTY()
	TArray<uint8> SendingBuffer;

private:
	/** 消息队列线程锁 */
	FCriticalSection MsgMutex;
	/** 消息Id，需要加原子锁 */
	FThreadSafeCounter64 MessageNumber = FThreadSafeCounter64(FDateTime::Now().GetTicks());
	/** 待发送的消息列表 */
	TDoubleLinkedList<TArray<uint8>> WaitToSendMessages;

public:
	/** 消息Id，get 返回时会自动 +1 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Socket")
	void SetMessageNumber(int64 NewMessageNumber);
	/** 消息Id，get 返回时会自动 +1 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Socket")
	int64 GetMessageNumber() const;

public:
	/** 缓存需要发送的数据 */
	void Push(const TArray<uint8>& msg, bool bImmediate);

	/** 判断是否有数据并且返回第一个值 */
	bool TryDequeueMsg(TArray<uint8>& outMsg);

	/** 是否有消息需要发送 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Socket")
	bool HasMsg();

	/** 重置数据 */
	void Reset();
};
