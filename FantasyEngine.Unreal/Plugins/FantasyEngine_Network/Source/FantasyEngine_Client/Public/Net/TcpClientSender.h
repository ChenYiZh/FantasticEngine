// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/SocketSender.h"
#include "TcpClientSender.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYENGINE_CLIENT_API UTcpClientSender final : public USocketSender
{
	GENERATED_BODY()

protected:
	virtual void TrySendAsync(UFESocket* InSocket, const TArray<uint8>& Buffer) override;
};
