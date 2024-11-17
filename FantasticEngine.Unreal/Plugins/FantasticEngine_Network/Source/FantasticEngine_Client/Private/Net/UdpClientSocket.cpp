// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UdpClientSocket.h"

#include "Common/StringConverter.h"
#include "Common/TcpSocketBuilder.h"
#include "Common/UdpSocketBuilder.h"
#include "Net/SocketSender.h"

FSocket* UUdpClientSocket::MakeSocket()
{
	if (!ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
	{
		return nullptr;
	}
	FSocket* FSocket = FUdpSocketBuilder(
		GetName().ToString() + TEXT("_") + FString::FromInt(FDateTime::Now().GetTicks()));
	// .AsNonBlocking()
	// .AsReusable()
	//.BoundToEndpoint(Address);
	//.WithSendBufferSize(8192)
	//.WithReceiveBufferSize(8192)
	//.WithBroadcast();
	// FSocket* FSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(
	// 	NAME_DGram, GetName().ToString() + TEXT("_") + FString::FromInt(FDateTime::Now().GetTicks()), true);

	return FSocket;
}

void UUdpClientSocket::BeginConnectImpl()
{
	if (!Socket->Connect(GetAddress()))
	{
		throw TEXT("Socket connect failed!");
	}
	/** 握手标示 */
	const FString ACCEPT_FLAG = TEXT("Author ChenYiZh");
	TArray<uint8> Utf8Bytes;
	UStringConverter::ToUTF8(ACCEPT_FLAG, Utf8Bytes);
	Sender->Push(this, Utf8Bytes, true);
}
