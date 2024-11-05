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
#include "Net/FESocket.h"
#include "Proxy/ClientActionDispatcher.h"
#include "UObject/Object.h"
#include "ClientSocket.generated.h"


class UMessageWriter;

/** 连接的回调函数 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FClientSocketConnectCallback, bool, bSuccess);

/** 套接字父类 */
UCLASS(NotBlueprintable, BlueprintType, DisplayName="Client Socket")
class FANTASTICENGINE_CLIENT_API UClientSocket : public UFESocket
{
	GENERATED_BODY()
	friend class FClientSocketConnectAsyncTask;
	friend class FClientSocketMessageProcessor;
	friend class UMessageWorker;
	friend class FClientSocketHeartbeatRunnable;
	friend class FClientSocketLoopingRunnable;

private:
	/** 标识名称 */
	UPROPERTY()
	FName Name;
	/** 运行的标识 */
	int32 ReadyFlag = 0;

	/** 心跳间隔 */
	UPROPERTY()
	int32 HeartbeatInterval;

	// /** 计时器管理类 */
	// FTimerManager& TimerManager;

	/** 心跳线程逻辑实例 */
	TSharedPtr<class FClientSocketHeartbeatRunnable> HeartbeatRunnable;

	/** 循环线程逻辑实例 */
	TSharedPtr<class FClientSocketLoopingRunnable> LoopRunnable;

	/** 心跳包线程 */
	FRunnableThread* HeartbeatTimer = nullptr;

	/** 数据处理线程 */
	FRunnableThread* LoopThread = nullptr;

	/** 心跳包 */
	UPROPERTY()
	TArray<uint8> HeartbeatBuffer;
	/** Action生成类 */
	UPROPERTY()
	UClientActionDispatcher* ActionProvider = nullptr;
	/** 消息处理的中转站 */
	UPROPERTY()
	UObject* MessageContractor = nullptr;

protected:
	/** 发送的管理类 */
	UPROPERTY()
	class USocketSender* Sender = nullptr;
	/** 接收管理类 */
	UPROPERTY()
	class USocketReceiver* Receiver = nullptr;

public:
	/** 标识名称 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket", DisplayName="Name")
	FName GetName();
	/** 数据是否已经初始化了 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket")
	bool IsReady() const;
	/** 心跳间隔 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket", DisplayName="Heartbeat Interval")
	int32 GetHeartbeatInterval();
	/** 消息Id */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Client Socket")
	void SetMessageNumber(const int64 InMessageNumber);
	/** 消息Id */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket")
	int64 GetMessageNumber() const;
	/** Action生成类 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Client Socket")
	void SetActionProvider(UClientActionDispatcher* Provider);
	/** Action生成类 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket")
	UClientActionDispatcher* GetActionProvider() const;
	/** 消息处理的中转站 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Client Socket")
	void SetMessageContractor(UObject* InMessageContractor);
	/** 消息处理的中转站，IBoss */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Client Socket")
	UObject* GetMessageContractor() const;

public:
	/**
	 * @brief 初始化Socket基本信息
	 * @param InName 连接名称
	 * @param Host 地址
	 * @param InPort 端口
	 * @param InHeartbeatInterval 心跳间隔
	 * @param ActionClassFullNames Action协议类的完整名称
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void Ready(const FName& InName, const FString& Host, const int32& InPort,
	           const TArray<FString>& ActionClassFullNames, const int32& InHeartbeatInterval = 10000);
	/** 自动连接 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void AutoConnect();

	/** 连接函数[内部异步实现] */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void ConnectAsync(const FClientSocketConnectCallback& Callback);
	/** 设置消息的偏移值 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void SetMessageOffset(const int32 Offset);
	/** 设置压缩方案 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void SetCompression(UCompression* InCompression);
	/** 设置解密方案 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void SetCryptoProvider(UCryptoProvider* InCryptoProvider);

protected:
	/** 同步连接 */
	virtual bool Connect();
	/** 创建连接 */
	virtual void BeginConnectImpl()
	{
	}

	/** 初始化执行 */
	virtual void Awake();
	/** 创建原生套接字 */
	virtual FSocket* MakeSocket();
	/** 发送心跳包 */
	virtual void SendHeartbeatPackage();
	/** 创建默认心跳包数据 */
	virtual void BuildHeartbeatBuffer(TArray<uint8>& HeartbeatBuffer);
	/** 循环线程 */
	virtual void Looping();
	
public:
	/** 执行下步操作 */
	virtual void NextStep(UFESocket* InSocket) override;
	
public:
	/** 重新构建心跳包数据 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Fantastic Engine|Client Socket")
	void RebuildHeartbeatPackage();
	/** 关闭函数 */
	virtual void Close_Implementation(EOpCode OpCode) override;

public:
	/** 数据发送 异步 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Client Socket")
	void Send(UMessageWriter* Message);

	/** 立即发送消息，会打乱消息顺序。只有类似心跳包这种及时的需要用到。一般使用Send就满足使用 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Client Socket")
	void SendImmediately(UMessageWriter* Message);

protected:
	/** 消息处理 */
	UFUNCTION()
	void OnMessageReceived(UFESocket* InSocket, UMessageReader* Message);
	/** 消息处理 */
	virtual void ProcessMessage(UMessageReader* Message) override;

protected:
	/** @brief 内部函数，直接传bytes，会影响数据解析 */
	virtual void SendBytes(const TArray<uint8>& Data);
	/** 内部函数，直接传bytes，会影响数据解析，以及解析顺序 */
	virtual void SendBytesImmediately(const TArray<uint8>& Data);
};

/** 心跳线程 */
class FANTASTICENGINE_CLIENT_API FClientSocketHeartbeatRunnable : public FRunnable
{
private:
	/** Socket */
	UClientSocket* Socket;
	/** 是否执行 */
	FThreadSafeBool IsRunning;

public:
	/** 起始有效性 */
	//virtual bool Init() override { return true; }
	/** 初始化操作 */
	FClientSocketHeartbeatRunnable(UClientSocket* InSocket): Socket(InSocket), IsRunning(true)
	{
	}

	/** 线程逻辑 */
	virtual uint32 Run() override;
	/** 强制关闭时执行 */
	virtual void Stop() override;
	// /** 退出时执行 */
	// virtual void Exit() override;
};

/** 循环线程 */
class FANTASTICENGINE_CLIENT_API FClientSocketLoopingRunnable : public FRunnable
{
private:
	/** Socket */
	UClientSocket* Socket;
	/** 是否执行 */
	//FThreadSafeBool IsRunning;

public:
	/** 起始有效性 */
	//virtual bool Init() override { return true; }
	/** 初始化操作 */
	FClientSocketLoopingRunnable(UClientSocket* InSocket): Socket(InSocket)//, IsRunning(true)
	{
	}

	/** 线程逻辑 */
	virtual uint32 Run() override;
	/** 强制关闭时执行 */
	virtual void Stop() override;
	// /** 退出时执行 */
	// virtual void Exit() override;
};

/** 异步连接的任务逻辑 */
class FANTASTICENGINE_CLIENT_API FClientSocketConnectAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FClientSocketConnectAsyncTask>;

private:
	/** 连接成功的回调 */
	FClientSocketConnectCallback Callback;
	/** ClientSocket */
	TWeakObjectPtr<UClientSocket> Socket;
	/** 初始化函数 */
	FClientSocketConnectAsyncTask(UClientSocket* InSocket, const FClientSocketConnectCallback& InCallback):
		Callback(InCallback), Socket(InSocket)
	{
		//Callback = MakeShared<FClientSocketConnectCallback>(InCallback);
	}

public:
	/** 需要执行的操作 */
	void DoWork();
	/** 固定写法，本类将作为函数参数 */
	FORCEINLINE TStatId GetStatId() const;
};

/** 异步连接的任务逻辑 */
class FANTASTICENGINE_CLIENT_API FClientSocketMessageProcessor : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FClientSocketMessageProcessor>;

private:
	/** ClientSocket */
	TWeakObjectPtr<UClientSocket> Socket;
	/** 消息 */
	UMessageReader* Message;
	/** 初始化函数 */
	FClientSocketMessageProcessor(UClientSocket* InSocket, UMessageReader* InMessage):
		Socket(InSocket), Message(InMessage)
	{
	}

public:
	/** 需要执行的操作 */
	void DoWork();
	/** 固定写法，本类将作为函数参数 */
	FORCEINLINE TStatId GetStatId() const;
};
