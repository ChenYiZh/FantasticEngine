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


#include "RPC/FNetwork.h"

#include "Log/FEConsole.h"
#include "Log/FENCategories.h"
#include "Net/UdpClientSocket.h"

void UFNetwork::SetDefaultUsableSocketName(const FName Name)
{
	DefaultUsableSocketName = Name;
}

const FName& UFNetwork::GetDefaultUsableSocketName()
{
	return DefaultUsableSocketName;
}

UClientSocket* UFNetwork::MakeTcpSocket(UObject* WorldContextObject, const FName Name, const FString Host,
                                        const int Port,
                                        const TArray<FString> ActionClassFullNames, int HeartbeatInterval)
{
	UTcpClientSocket* Socket = NewObject<UTcpClientSocket>(WorldContextObject->GetWorld()->GetGameInstance());
	if (!MakeSocket(Name, Socket))
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::FNETWORK,
		                                  FString::Printf(
			                                  TEXT("The same key: %s exists in Network sockets."),
			                                  ToCStr(Name.ToString())));
		return nullptr;
	}
	Socket->Ready(Name, Host, Port, ActionClassFullNames, HeartbeatInterval);
	return Socket;
}

UClientSocket* UFNetwork::MakeUdpSocket(UObject* WorldContextObject, const FName Name, const FString Host,
                                        const int Port,
                                        const TArray<FString> ActionClassFullNames, int HeartbeatInterval)
{
	UUdpClientSocket* Socket = NewObject<UUdpClientSocket>(WorldContextObject->GetWorld()->GetGameInstance());
	if (!MakeSocket(Name, Socket))
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::FNETWORK,
		                                  FString::Printf(
			                                  TEXT("The same key: %s exists in Network sockets."),
			                                  ToCStr(Name.ToString())));
		return nullptr;
	}
	Socket->Ready(Name, Host, Port, ActionClassFullNames, HeartbeatInterval);
	return Socket;
}

UClientSocket* UFNetwork::GetSocket(const FName Name, TSubclassOf<UClientSocket> SocketClass)
{
	FScopeLock SetLock(&Mutex);
	if (Sockets.Contains(Name))
	{
		return Sockets[Name];
	}
	return nullptr;
}

void UFNetwork::Send(const int ActionId, UMessageWriter* Message)
{
	SendBySocketName(DefaultUsableSocketName, ActionId, Message);
}

void UFNetwork::SendBySocketName(const FName SocketName, int ActionId, UMessageWriter* Message)
{
	Message->SetActionId(ActionId);
	Message->SetOpCode(0);
	UClientSocket* Socket = GetSocket(SocketName);
	if (Socket != nullptr)
	{
		Socket->Send(Message);
	}
}

void UFNetwork::Shutdown()
{
	FScopeLock SetLock(&Mutex);
	for (auto KeyValue : Sockets)
	{
		try
		{
			if (KeyValue.Value != nullptr)
			{
				KeyValue.Value->Close();
				KeyValue.Value->ClearGarbage();
				KeyValue.Value->MarkAsGarbage();
			}
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::FNETWORK,
			                                  FString::Printf(
				                                  TEXT("Fail to close socket %s."), ToCStr(KeyValue.Key.ToString())));
		}
	}
	Sockets.Empty();
}

UMessageWriter* UFNetwork::MakeMessage()
{
	return NewObject<UMessageWriter>();
}

bool UFNetwork::MakeSocket(const FName Name, UClientSocket* Socket)
{
	FScopeLock SetLock(&Mutex);
	if (!Sockets.Contains(Name))
	{
		Sockets.Add(Name, Socket);
		return true;
	}
	return false;
}
