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
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "IO/Compression.h"
#include "IO/CryptoProvider.h"
#include "Proxy/DirectMessageProcessor.h"
#include "UObject/Object.h"
#include "FESocket.generated.h"

/**
 * 操作码
 */
UENUM(BlueprintType)
enum EOpCode
{
	/**
	 * @brief 空数据
	 */
	Empty = -1,
	/**
	 * @brief 连续性数据
	 */
	Continuation = 0,
	/**
	 * @brief 文本数据
	 */
	Text = 1,
	/**
	 * @brief 二进制数据
	 */
	Binary = 2,
	/**
	 * @brief 关闭操作数据
	 */
	Close = 8,
	/**
	 * @brief Ping数据
	 */
	Ping = 9,
	/**
	 * @brief Pong数据
	 */
	Pong = 10,
};

/**
 * 套接字类型
 */
UENUM(BlueprintType)
enum EFSocketType
{
	/**
	 * @brief Tcp Socket
	 */
	Tcp = 0,
	/**
	 * @brief Http Socket
	 */
	Http = 1,
	/**
	 * @brief Udp Socket
	 */
	Udp = 2,
	/**
	 * @brief Web Socket
	 */
	Web = 3,
};

class UUserToken;

/**
 * 套接字管理基类
 */
UCLASS(BlueprintType, NotBlueprintable)
class FANTASTICENGINE_NETWORK_API UFESocket : public UObject
{
	GENERATED_BODY()
	friend class UMessageWorker;

public:
	/** 线程锁 */
	FCriticalSection Mutex;

protected:
	/** 是否在运行，1：True，2：False */
	int32 bIsRunning = 0;

private:
	/** 0：等待，1：发送，2：接收，3：在循环中 */
	int8 Operation = 0;

public:
	/** 尝试开始发送 */
	virtual bool TrySend(bool bOnlyWait = false)
	{
		if (bOnlyWait)
		{
			return FPlatformAtomics::InterlockedCompareExchange(&Operation, 1, 0) == 0;
		}
		return FPlatformAtomics::InterlockedCompareExchange(&Operation, 1, 0) != 2;
	}

	/**  尝试接收 */
	virtual bool TryReceive(bool bOnlyWait = false)
	{
		if (bOnlyWait)
		{
			return FPlatformAtomics::InterlockedCompareExchange(&Operation, 2, 0) == 0;
		}
		return FPlatformAtomics::InterlockedCompareExchange(&Operation, 2, 0) != 1;
	}

	/** 是否正处于操作 */
	virtual bool Operating()
	{
		return FPlatformAtomics::InterlockedCompareExchange(&Operation, 0, 0) != 0;
	}

	/** 进入循环 */
	virtual void InLooping()
	{
		FPlatformAtomics::InterlockedExchange(&Operation, 3);
	}

	/** 移出循环 */
	virtual void OutLooping()
	{
		FPlatformAtomics::InterlockedCompareExchange(&Operation, 0, 3);
	}

	/** 操作完成时执行 */
	virtual void OperationCompleted()
	{
		FPlatformAtomics::InterlockedExchange(&Operation, 0);
	}

	/** 执行下步操作 */
	virtual void NextStep(UFESocket* InSocket)
	{
	}

protected:
	/** 地址 */
	TSharedPtr<FInternetAddr> Address;

protected:
	/**
	 * 原生套接字，
	 * 在UE的SocketSubsystem中已经做了引用以及对象池
	 */
	FSocket* Socket = nullptr;

	/** 消息偏移值 */
	UPROPERTY()
	int32 MessageOffset = 2;

	/** 压缩工具 */
	UPROPERTY()
	UCompression* Compression = nullptr;

	/** 加密工具 */
	UPROPERTY()
	UCryptoProvider* CryptoProvider = nullptr;

protected:
	/** 缓存变量 */
	UPROPERTY()
	UUserToken* UserToken = nullptr;

public:
	/** 缓存变量 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket")
	UUserToken* GetUserToken() const
	{
		return UserToken;
	}

public:
	/**
	 * @brief 消息处理方案
	 */
	UPROPERTY(BlueprintReadWrite, Category="Fantastic Engine|Socket")
	UObject* MessageEventProcessor = nullptr;

public:
	/** 是否在运行 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category="Fantastic Engine|Socket", DisplayName="Is Running")
	bool GetIsRunning();
	/** 是否在运行 */
	virtual bool GetIsRunning_Implementation();
	/** 是否已经开始运行 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category="Fantastic Engine|Socket", DisplayName="Connected")
	bool GetConnected();
	/** 是否已经开始运行 */
	virtual bool GetConnected_Implementation();

	/** 地址 */
	FInternetAddr& GetAddress() const;
	/** 地址 */
	FIPv4Endpoint GetIpAddress() const;

	/** 地址 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="Host")
	FString GetHost() const;
	/** 端口 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="Address")
	int32 GetPort() const;
	/** 套接字 */
	//UFUNCTION(BlueprintPure, DisplayName="Socket")
	FSocket* GetSocket() const;

	/** 类型 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="Type")
	virtual EFSocketType GetType();

	/** 消息偏移值 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="Message Offset")
	const int& GetMessageOffset() const;

	/** 消息偏移值 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="Compression")
	UCompression* GetCompression() const;

	/** 加密工具 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket", DisplayName="CryptoProvider")
	UCryptoProvider* GetCryptoProvider() const;

	/** 关闭函数 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Socket", DisplayName="CryptoProvider")
	void Close(EOpCode OpCode = EOpCode::Close);

	/** 关闭函数 */
	virtual void Close_Implementation(EOpCode OpCode = EOpCode::Close);

public:
	/** 创建Socket的超类 */
	static UUserToken* MakeEventArgs(UFESocket* InSocket);

protected:
	/** 消息处理 */
	virtual void ProcessMessage(UMessageReader* Message)
	{
	}
};
