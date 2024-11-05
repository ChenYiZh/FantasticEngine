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
#include "FESocket.h"
#include "UObject/Object.h"
#include "SocketReceiver.generated.h"

/**
 * 收发消息处理
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessageReceiveEventHandler, UFESocket*, Socket, UMessageReader*,
                                             Message);

/**
 * 消息接收处理类
 */
UCLASS(NotBlueprintable, NotBlueprintType, DisplayName="Socket Receiver")
class FANTASTICENGINE_NETWORK_API USocketReceiver : public UObject
{
	GENERATED_BODY()
	
private:
	/** 封装的套接字 */
	UPROPERTY()
	TWeakObjectPtr<UFESocket> Socket = nullptr;

public:
	/** 封装的套接字 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket Receiver", DisplayName="Socket")
	UFESocket* GetSocket() const;

public:
	/** 接收到数据包事件 */
	UPROPERTY(BlueprintAssignable, Category="Fantastic Engine|Socket Receiver")
	FMessageReceiveEventHandler OnMessageReceived;
	/** 心跳探索事件 */
	UPROPERTY(BlueprintAssignable, Category="Fantastic Engine|Socket Receiver")
	FMessageReceiveEventHandler OnPing;
	/** 心跳回应事件 */
	UPROPERTY(BlueprintAssignable, Category="Fantastic Engine|Socket Receiver")
	FMessageReceiveEventHandler OnPong;

private:
	/** 接收到数据包事件 */
	void MessageReceived(UFESocket* InSocket, UMessageReader* InMessage);
	/** 心跳探索事件 */
	void Ping(UFESocket* InSocket, UMessageReader* InMessage);
	/** 心跳回应事件 */
	void Pong(UFESocket* InSocket, UMessageReader* InMessage);

public:
	/** 初始化 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Socket Receiver")
	virtual void Initialize(UFESocket* FSocket);

	/** 处理数据接收回调 */
	void ProcessReceive(UFESocket* InSocket, TArray<uint8>& ArgsBuffer, int32 ArgsLength);

	/** 接受类释放 */
	void Release();

	/** 投递接收数据请求 */
	virtual void PostReceive(UFESocket* InSocket)
	{
	}

protected:
	/** 关闭操作 */
	virtual void Close(UFESocket* InSocket, EOpCode OpCode)
	{
	}
};