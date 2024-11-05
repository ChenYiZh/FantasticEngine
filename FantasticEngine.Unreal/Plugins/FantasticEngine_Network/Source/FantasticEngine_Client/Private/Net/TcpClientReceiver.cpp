// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/TcpClientReceiver.h"

#include "Sockets.h"

void UTcpClientReceiver::PostReceive(UFESocket* InSocket)
{
	if (!GetSocket()->TryReceive())
	{
		return;
	}

	const int32 BufferSize = 8192;
	TArray<uint8> Data;
	Data.SetNumUninitialized(BufferSize);
	int32 DataSize;
	if (GetSocket()->GetSocket()->Recv(Data.GetData(), BufferSize, DataSize))
	{
		ProcessReceive(InSocket, Data, DataSize);
	}
	else
	{
		ProcessReceive(InSocket, Data, 0);
	}
}
