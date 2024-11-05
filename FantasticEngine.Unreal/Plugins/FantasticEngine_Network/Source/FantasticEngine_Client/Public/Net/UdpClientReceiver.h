// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/SocketReceiver.h"
#include "UdpClientReceiver.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_CLIENT_API UUdpClientReceiver final : public USocketReceiver
{
	GENERATED_BODY()

private:
	/** 缓冲区大小 */
	const int32 BufferSize = 8192;
	/** 缓存的数据 */
	UPROPERTY()
	TArray<uint8> BufferReceived;

public:
	/** 初始化函数 */
	virtual void Initialize(UFESocket* FESocket) override
	{
		Super::Initialize(FESocket);
		BufferReceived.SetNumUninitialized(BufferSize);
	}

public:
	/** 投递接收数据请求 */
	virtual void PostReceive(UFESocket* InSocket) override;
};
