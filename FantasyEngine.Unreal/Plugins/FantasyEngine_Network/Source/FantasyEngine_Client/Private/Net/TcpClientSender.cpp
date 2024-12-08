// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/TcpClientSender.h"

#include "Sockets.h"

void UTcpClientSender::TrySendAsync(UFESocket* InSocket, const TArray<uint8>& Buffer)
{
	int32 BufferSent = 0;
	while (BufferSent < Buffer.Num())
	{
		int32 BytesSent;
		if (InSocket->GetSocket()->Send(&Buffer.GetData()[BufferSent], Buffer.Num() - BufferSent, BytesSent))
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
