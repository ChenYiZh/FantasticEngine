// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/ClientReceiver.h"

void UClientReceiver::Close(UFESocket* InSocket, EOpCode OpCode)
{
	if (InSocket != nullptr)
	{
		InSocket->Close(OpCode);
	}
}
