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
#include "IO/MessageWriter.h"
#include "UObject/Object.h"
#include "SocketSender.generated.h"

/**
 * 消息发送处理类
 */
UCLASS(NotBlueprintable, NotBlueprintType, DisplayName="Socket Sender")
class FANTASTICENGINE_NETWORK_API USocketSender : public UObject
{
	GENERATED_BODY()
	friend class UFSocketWaitToSendMessage;
	friend class FSocketSenderStartSendTask;
	friend class UClientSocket;
	friend class UUdpSocket;

private:
	/** 套接字管理类 */
	UPROPERTY()
	TWeakObjectPtr<UFESocket> Socket = nullptr;

public:
	/** 封装的套接字 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Socket Sender", DisplayName="Socket")
	UFESocket* GetSocket() const;

public:
	/** 添加需要发送的数据 */
	void Push(UFESocket* InSocket, const TArray<uint8>& Msg, bool bImmediate);

	/** 消息发送处理 */
	void ProcessSend(UFESocket* InSocket);

protected:
	/** 数据发送的实现 */
	virtual void TrySendAsync(UFESocket* InSocket, const TArray<uint8>& Buffer)
	{
	}

public:
	/** 开始执行发送 */
	void PostSend(UFESocket* InSocket);

private:
	/** 消息执行完后，判断还有没有需要继续发送的消息 */
	void SendCompleted(UFESocket* InSocket);

public:
	/** 消息发送处理类 */
	UFUNCTION(BlueprintCallable, Category="Fantastic Engine|Socket Sender")
	void Initialize(UFESocket* FSocket);

public:
	/** 在释放时操作 */
	void Release();
};

