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


#include "Net/SocketReceiver.h"

#include "Sockets.h"
#include "Common/ByteUtility.h"
#include "Common/FEPackageFactory.h"
#include "Common/SizeUtility.h"
#include "Log/FEConsole.h"
#include "Log/FENCategories.h"
#include "Net/UserToken.h"

UFESocket* USocketReceiver::GetSocket() const
{
	return Socket.Get();
}

void USocketReceiver::MessageReceived(UFESocket* InSocket, UMessageReader* InMessage)
{
	OnMessageReceived.Broadcast(InSocket, InMessage);
}

void USocketReceiver::Ping(UFESocket* InSocket, UMessageReader* InMessage)
{
	OnPing.Broadcast(InSocket, InMessage);
}

void USocketReceiver::Pong(UFESocket* InSocket, UMessageReader* InMessage)
{
	OnPong.Broadcast(InSocket, InMessage);
}

void USocketReceiver::Initialize(UFESocket* FESocket)
{
	if (FESocket == nullptr)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,
		                                  TEXT("Fail to create socket receiver, because the FESocket is null."));
		return;
	}
	Socket = FESocket;
}

void USocketReceiver::ProcessReceive(UFESocket* InSocket, TArray<uint8>& ArgsBuffer, int32 ArgsLength)
{
	if (InSocket == nullptr || InSocket->GetUserToken() == nullptr)
	{
		Socket->OperationCompleted();
		return;
	}

	UUserToken* UserToken = InSocket->GetUserToken();

	if (ArgsBuffer.Num() > 0)
	{
		int32 Offset = 0;

		TArray<UMessageReader*> Messages;
		try
		{
			//继续接收上次未接收完毕的数据
			if (UserToken->ReceivedBuffer.Num() > 0)
			{
				//上次连报头都没接收完
				if (UserToken->ReceivedStartIndex < 0)
				{
					TArray<uint8> Data;
					Data.SetNumUninitialized(ArgsLength + UserToken->ReceivedBuffer.Num());
					UByteUtility::BlockCopy(UserToken->ReceivedBuffer, 0, Data, 0, UserToken->ReceivedBuffer.Num());
					UByteUtility::BlockCopy(ArgsBuffer, 0, Data, UserToken->ReceivedBuffer.Num(), ArgsLength);
					UserToken->ReceivedBuffer.Empty();
					ArgsBuffer = Data;
					Offset = 0;
					ArgsLength = Data.Num();
				}
				//数据仍然接收不完
				else if (UserToken->ReceivedStartIndex + ArgsLength < UserToken->ReceivedBuffer.Num())
				{
					UByteUtility::BlockCopy(ArgsBuffer, 0, UserToken->ReceivedBuffer, UserToken->ReceivedStartIndex,
					                     ArgsLength);
					UserToken->ReceivedStartIndex += ArgsLength;
					Offset += ArgsLength;
				}
				//这轮数据可以接受完
				else
				{
					int32 DeltaLength = UserToken->ReceivedBuffer.Num() - UserToken->ReceivedStartIndex;
					UByteUtility::BlockCopy(ArgsBuffer, 0, UserToken->ReceivedBuffer, UserToken->ReceivedStartIndex,
					                     DeltaLength);
					UMessageReader* BigMessage = UFEPackageFactory::Unpack(
						UserToken->ReceivedBuffer, InSocket->GetMessageOffset(), InSocket->GetCompression(),
						InSocket->GetCryptoProvider());
					UserToken->ReceivedBuffer.Empty();
					Messages.Push(BigMessage);
					Offset += DeltaLength;
				}
			}

			//针对接收到的数据进行完整解析
			while (Offset < ArgsLength)
			{
				int32 TotalLength = UFEPackageFactory::GetTotalLength(
					ArgsBuffer, Offset + Socket->GetMessageOffset());
				//包头解析不全
				if (TotalLength < 0)
				{
					UserToken->ReceivedStartIndex = -1;
					UserToken->ReceivedBuffer.SetNumUninitialized(ArgsLength - Offset);
					UByteUtility::BlockCopy(ArgsBuffer, Offset, UserToken->ReceivedBuffer, 0,
					                     ArgsLength - Offset);
					break;
				}

				//包体解析不全
				if (TotalLength > ArgsLength)
				{
					UserToken->ReceivedStartIndex = ArgsLength - Offset;
					UserToken->ReceivedBuffer.SetNumUninitialized(TotalLength - Offset);
					UByteUtility::BlockCopy(ArgsBuffer, Offset, UserToken->ReceivedBuffer, 0, TotalLength - Offset);
					break;
				}

				Offset += Socket->GetMessageOffset();
				UMessageReader* Message = UFEPackageFactory::Unpack(ArgsBuffer, Offset,
				                                                   UserToken->GetSocket()->GetCompression(),
				                                                   UserToken->GetSocket()->GetCryptoProvider());
				Messages.Push(Message);
				Offset = TotalLength;
			}
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("Process Receive error."));
		}

		for (int i = 0; i < Messages.Num(); i++)
		{
			UMessageReader* Message = Messages[i];
			try
			{
				if (Message->IsError())
				{
					UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, Message->GetError());
					continue;
				}

				switch (Message->GetOpCode())
				{
				case static_cast<int8>(EOpCode::Close):
					{
						Socket->Close(EOpCode::Empty);
					}
					break;
				case static_cast<int8>(EOpCode::Ping):
					{
						Ping(GetSocket(), Message);
					}
					break;
				case static_cast<int8>(EOpCode::Pong):
					{
						Pong(GetSocket(), Message);
					}
					break;
				default:
					{
						MessageReceived(GetSocket(), Message);
					}
					break;
				}
			}
			catch (...)
			{
				UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,
				                                  TEXT("An exception occurred when resolve the message."));
			}
		}
	}
	else if (!UserToken->ReceivedBuffer.IsEmpty())
	{
		//数据错乱
		UserToken->ReceivedBuffer.Empty();
		UserToken->Reset();
	}

	if (!UserToken->ReceivedBuffer.IsEmpty())
	{
		PostReceive(InSocket);
	}
	else
	{
		Socket->NextStep(InSocket);
	}
}

void USocketReceiver::Release()
{
	//FScopeLock SetLock(&Mutex);
	ClearGarbage();
	MarkAsGarbage();
}
