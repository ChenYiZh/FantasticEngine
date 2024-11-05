// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UdpClientReceiver.h"

#include "Sockets.h"
#include "Log/FEConsole.h"

void UUdpClientReceiver::PostReceive(UFESocket* InSocket)
{
	if (!GetSocket()->TryReceive())
	{
		return;
	}
	uint32 PendingSize;
	bool bHasData = GetSocket()->GetSocket()->HasPendingData(PendingSize);
	int32 DataSize;
	if (bHasData && GetSocket()->GetSocket()->RecvFrom(BufferReceived.GetData(), BufferSize, DataSize,
	                                                   InSocket->GetAddress()))
	{
		ProcessReceive(InSocket, BufferReceived, DataSize);
	}
	else
	{
		ProcessReceive(InSocket, BufferReceived, 0);
	}
}
