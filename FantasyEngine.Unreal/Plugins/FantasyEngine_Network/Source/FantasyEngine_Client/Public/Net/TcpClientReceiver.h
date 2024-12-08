// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/SocketReceiver.h"
#include "TcpClientReceiver.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYENGINE_CLIENT_API UTcpClientReceiver final : public USocketReceiver
{
	GENERATED_BODY()

public:
	/** 投递接收数据请求 */
	virtual void PostReceive(UFESocket* InSocket) override;
};
