// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UdpClientSender.h"

#include "Sockets.h"

void UUdpClientSender::TrySendAsync(UFESocket* InSocket, const TArray<uint8>& Buffer)
{
	int32 BufferSent = 0;
	while (BufferSent < Buffer.Num())
	{
		int32 BytesSent;
		if (GetSocket()->GetSocket()->SendTo(&Buffer.GetData()[BufferSent], Buffer.Num() - BufferSent, BytesSent,
		                                     InSocket->GetAddress()))
		{
			BufferSent += BytesSent;
		}
		else
		{
			break;
		}
	}
	ProcessSend(InSocket);
}
