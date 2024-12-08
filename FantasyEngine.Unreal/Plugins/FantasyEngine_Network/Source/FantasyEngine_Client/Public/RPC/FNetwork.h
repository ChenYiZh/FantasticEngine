/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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
#include "IO/MessageWriter.h"
#include "Net/TcpClientSocket.h"
#include "UObject/Object.h"
#include "FNetwork.generated.h"

/**
 * 网络处理类
 */
UCLASS(DisplayName="Fantasy Engine|Network")
class FANTASYENGINE_CLIENT_API UFNetwork : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * @brief 默认使用的套接字名称
	 */
	inline static FName DefaultUsableSocketName = FName(TEXT("default"));
	/**
	 * @brief 客户端套接字列表
	 */
	inline static TMap<FName, UClientSocket*> Sockets;
	/**
	 * @brief 线程锁
	 */
	inline static FCriticalSection Mutex;

public:
	/**
	 * @brief 设置默认使用的套接字名称
	 * @param Name 套接字名称
	 */
	UFUNCTION(BlueprintSetter, Category="Fantasy Engine|Network")
	static void SetDefaultUsableSocketName(const FName Name);

	/**
	 * @brief 获取默认使用的套接字名称
	 * @return 套接字名称
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Network")
	static const FName& GetDefaultUsableSocketName();

	/**
	 * @brief 创建套接字
	 * @param Name 标识名称
	 * @param Host 
	 * @param Port 
	 * @param ActionClassFullNames Action协议类的完整名称
	 * @param HeartbeatInterval 心跳间隔
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Network",
		meta=(bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static UClientSocket* MakeTcpSocket(UObject* WorldContextObject, const FName Name, const FString Host,
	                                    const int Port,
	                                    const TArray<FString> ActionClassFullNames,
	                                    int HeartbeatInterval = 10000);

	/**
	 * @brief 创建套接字
	 * @param Name 标识名称
	 * @param Host 
	 * @param Port 
	 * @param ActionClassFullNames Action协议类的完整名称
	 * @param HeartbeatInterval 心跳间隔
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Network",
		meta=(bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static UClientSocket* MakeUdpSocket(UObject* WorldContextObject, const FName Name, const FString Host,
	                                    const int Port,
	                                    const TArray<FString> ActionClassFullNames,
	                                    int HeartbeatInterval = 10000);

	/**
	 * @brief 获取指定的Socket
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Network", meta=(DeterminesOutputType="SocketClass"))
	static UClientSocket* GetSocket(const FName Name, TSubclassOf<UClientSocket> SocketClass = nullptr);

	/**
	 * @brief 使用默认套接字来发送消息，通过DefaultUsableSocketName来配置默认的套接字名称
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Network")
	static void Send(const int ActionId, UMessageWriter* Message);

	/**
	 * @brief 使用默认套接字来发送消息，通过DefaultUsableSocketName来配置默认的套接字名称
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Network", DisplayName="Send By Socket Name")
	static void SendBySocketName(const FName SocketName, int ActionId, UMessageWriter* Message);

	/**
	 * @brief 关闭所有套接字连接
	 */
	UFUNCTION(BlueprintCallable, Category="Fantasy Engine|Network")
	static void Shutdown();

	/**
	 * @brief 创建一个Message对象，用于蓝图
	 */
	UFUNCTION(BlueprintPure, Category="Fantasy Engine|Network")
	static UMessageWriter* MakeMessage();

private:
	/**
	 * @brief 创建套接字
	 */
	static bool MakeSocket(const FName Name, UClientSocket* Socket);
};
