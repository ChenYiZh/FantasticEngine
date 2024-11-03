// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UserToken.h"

UFESocket* UUserToken::GetSocket() const
{
	return Socket.Get();
}

int32 UUserToken::GetOriginalOffset() const
{
	return OriginalOffset;
}

int32 UUserToken::GetOriginalLength() const
{
	return OriginalLength;
}


void UUserToken::SetMessageNumber(int64 NewMessageNumber)
{
	MessageNumber.Set(NewMessageNumber);
}

int64 UUserToken::GetMessageNumber() const
{
	return MessageNumber.GetValue();
}

void UUserToken::Push(const TArray<uint8>& msg, bool bImmediate)
{
	FScopeLock SetLock(&MsgMutex);
	if (bImmediate)
	{
		WaitToSendMessages.AddHead(msg);
	}
	else
	{
		WaitToSendMessages.AddTail(msg);
	}
}

bool UUserToken::TryDequeueMsg(TArray<uint8>& outMsg)
{
	FScopeLock SetLock(&MsgMutex);
	if (WaitToSendMessages.Num() > 0)
	{
		TDoubleLinkedList<TArray<uint8>>::TDoubleLinkedListNode* FirstNode = WaitToSendMessages.GetHead();
		outMsg = FirstNode->GetValue();
		WaitToSendMessages.RemoveNode(FirstNode);
		return true;
	}
	return false;
}

bool UUserToken::HasMsg()
{
	FScopeLock SetLock(&MsgMutex);
	return WaitToSendMessages.Num() > 0;
}

void UUserToken::Reset()
{
	SendingBuffer.Empty();
}
