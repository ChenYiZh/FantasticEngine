// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSocket.h"
#include "UObject/NoExportTypes.h"
#include "UdpClientSocket.generated.h"

/**
 * Udp连接池
 */
UCLASS(DisplayName="Udp Socket")
class FANTASTICENGINE_CLIENT_API UUdpClientSocket : public UClientSocket
{
	GENERATED_BODY()

protected:
	/** 建立原生套接字 */
	virtual FSocket* MakeSocket() override;

	/** 创建连接 */
	virtual void BeginConnectImpl() override;

	/** 类型 */
	virtual EFSocketType GetType() override
	{
		return EFSocketType::Udp;
	}
};
