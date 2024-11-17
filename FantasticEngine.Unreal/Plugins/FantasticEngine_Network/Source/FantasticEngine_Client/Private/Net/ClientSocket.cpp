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


#include "Net/ClientSocket.h"

#include "Sockets.h"
#include "Common/FEPackageFactory.h"
#include "IO/MessageWriter.h"

#include "Log/FEConsole.h"
#include "Log/FENCategories.h"
#include "Net/SocketReceiver.h"
#include "Net/SocketSender.h"
#include "Net/TcpClientReceiver.h"
#include "Net/TcpClientSender.h"
#include "Net/UdpClientReceiver.h"
#include "Net/UdpClientSender.h"
#include "Net/UserToken.h"
#include "Proxy/ActionBoss.h"
#include "Proxy/MessageWorker.h"

FName UClientSocket::GetName()
{
	return Name;
}

bool UClientSocket::IsReady() const
{
	return FPlatformAtomics::AtomicRead(&ReadyFlag) == 1;
}

int UClientSocket::GetHeartbeatInterval()
{
	return HeartbeatInterval;
}

void UClientSocket::SetMessageNumber(const int64 InMessageNumber)
{
	UserToken->SetMessageNumber(InMessageNumber);
}

int64 UClientSocket::GetMessageNumber() const
{
	return UserToken->GetMessageNumber();
}

void UClientSocket::SetActionProvider(UClientActionDispatcher* Provider)
{
	ActionProvider = Provider;
}

UClientActionDispatcher* UClientSocket::GetActionProvider() const
{
	return ActionProvider;
}

void UClientSocket::SetMessageContractor(UObject* InMessageContractor)
{
	if (InMessageContractor == nullptr)
	{
		return;
	}
	if (!InMessageContractor->Implements<UIBoss>())
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,
		                                  TEXT("The type of InMessageContractor is not IBoss"));
		return;
	}
	MessageContractor = InMessageContractor;
}

UObject* UClientSocket::GetMessageContractor() const
{
	return MessageContractor;
}

void UClientSocket::SetMessageOffset_Implementation(const int32 Offset)
{
	MessageOffset = Offset;
}

void UClientSocket::SetCompression_Implementation(UCompression* InCompression)
{
	Compression = InCompression;
}

void UClientSocket::SetCryptoProvider_Implementation(UCryptoProvider* InCryptoProvider)
{
	CryptoProvider = InCryptoProvider;
}

void UClientSocket::Ready_Implementation(const FName& InName, const FString& Host, const int32& InPort,
                                         const TArray<FString>& ActionClassFullNames, const int32& InHeartbeatInterval)
{
	AddToRoot();
	Name = InName;
	FIPv4Endpoint Endpoint;
	FString RemoteAddress = Host + TEXT(":") + FString::FromInt(InPort);
	if (!FIPv4Endpoint::Parse(RemoteAddress, Endpoint))
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,
		                                  FString::Printf(
			                                  TEXT("The host: %s is not a correct address."), ToCStr(RemoteAddress)));
		return;
	}
	Address = Endpoint.ToInternetAddr();
	ActionProvider = NewObject<UClientActionDispatcher>(this);
	ActionProvider->ActionNameFormats = ActionClassFullNames;
	HeartbeatInterval = InHeartbeatInterval;
	UFEConsole::WriteInfoWithCategory(UFENCategories::SOCKET,TEXT("Socket is ready..."), false);
	FPlatformAtomics::InterlockedExchange(&ReadyFlag, 1);
}

void UClientSocket::AutoConnect_Implementation()
{
	if (FPlatformAtomics::AtomicRead(&bIsRunning) == 0)
	{
		FClientSocketConnectCallback Callback;
		ConnectAsync(Callback);
	}
}

void UClientSocket::ConnectAsync_Implementation(const FClientSocketConnectCallback& Callback)
{
	FPlatformAtomics::InterlockedExchange(&bIsRunning, 1);
	Awake();
	(new FAutoDeleteAsyncTask<FClientSocketConnectAsyncTask>(this, Callback))->StartBackgroundTask();
}

bool UClientSocket::Connect()
{
	if (!IsReady())
	{
		FPlatformAtomics::InterlockedExchange(&bIsRunning, 0);
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("Socket is not ready!"));
		return false;
	}
	FPlatformAtomics::InterlockedExchange(&bIsRunning, 1);
	Awake();
	UFEConsole::WriteInfoWithCategory(UFENCategories::SOCKET, TEXT("Socket is starting..."), false);

	try
	{
		BeginConnectImpl();
		// bool bSuccess = Socket->Connect(Address.ToSharedRef().Get());
		// if (!bSuccess || !GetConnected())
		// {
		// 	bIsRunning = false;
		// 	UFEConsole::WriteErrorWithCategory(UFECategories::SOCKET, TEXT("Socket connect failed!"));
		// 	Close();
		// 	return false;
		// }
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("Fail to connect the server!"));
		Close();
		return false;
	}

	if (HeartbeatTimer == nullptr)
	{
		RebuildHeartbeatPackage();
		HeartbeatRunnable = MakeShareable(new FClientSocketHeartbeatRunnable(this));
		HeartbeatTimer =
			FRunnableThread::Create(HeartbeatRunnable.Get(), ToCStr(Name.ToString() + TEXT("_HeartBeats")));
	}

	// TODO: 循环线程是否需要?
	if (LoopThread == nullptr)
	{
		LoopRunnable = MakeShareable(new FClientSocketLoopingRunnable(this));
		LoopThread = FRunnableThread::Create(LoopRunnable.Get(), ToCStr(Name.ToString() + TEXT("_Looping")));
	}

	UFEConsole::WriteInfoWithCategory(UFENCategories::SOCKET, TEXT("Socket connected."), false);
	return true;
}

void UClientSocket::Awake()
{
	if (Socket == nullptr)
	{
		Socket = MakeSocket();
		UserToken = MakeEventArgs(this);
	}
	if (Sender == nullptr)
	{
		switch (GetType())
		{
		case EFSocketType::Tcp:
			Sender = NewObject<UTcpClientSender>(this);
			break;
		case EFSocketType::Udp:
			Sender = NewObject<UUdpClientSender>(this);
			break;
		default: ;
		}
		if (Sender != nullptr)
		{
			Sender->Initialize(this);
		}
	}
	if (Receiver == nullptr)
	{
		switch (GetType())
		{
		case EFSocketType::Tcp:
			Receiver = NewObject<UTcpClientReceiver>(this);
			break;
		case EFSocketType::Udp:
			Receiver = NewObject<UUdpClientReceiver>(this);
			break;
		default: ;
		}
		if (Receiver != nullptr)
		{
			Receiver->Initialize(this);
			Receiver->OnMessageReceived.AddDynamic(this, &UClientSocket::OnMessageReceived);
		}
	}
}

void UClientSocket::Looping()
{
	while (GetIsRunning())
	{
		FPlatformProcess::Sleep(0);
		if (Operating())
		{
			continue;
		}
		if (Socket != nullptr && Socket->Wait(ESocketWaitConditions::WaitForRead, 0))
		{
			if (TryReceive(true))
			{
				Receiver->PostReceive(this);
			}
			continue;
		}
		if (Socket != nullptr && Socket->Wait(ESocketWaitConditions::WaitForWrite, 0))
		{
			if (TrySend(true))
			{
				Sender->PostSend(this);
			}
			continue;
		}
	}
}

void UClientSocket::NextStep(UFESocket* InSocket)
{
	OperationCompleted();
}

FSocket* UClientSocket::MakeSocket()
{
	return nullptr;
}

void UClientSocket::SendHeartbeatPackage()
{
	try
	{
		if (Socket != nullptr && Socket->GetConnectionState() != SCS_Connected)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("The socket is not connected."));
		}
		// 心跳包
		if (!HeartbeatBuffer.IsEmpty())
		{
			Sender->Push(this, HeartbeatBuffer, true);
		}
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("Send heartbeat package failed."));
	}
}

void UClientSocket::BuildHeartbeatBuffer(TArray<uint8>& Buffer)
{
	UMessageWriter* Message = NewObject<UMessageWriter>(this);
	Message->SetOpCode(static_cast<int8>(EOpCode::Pong));
	return UFEPackageFactory::Pack(Buffer, Message, MessageOffset, nullptr, nullptr);
}

void UClientSocket::RebuildHeartbeatPackage_Implementation()
{
	HeartbeatBuffer.Empty();
	BuildHeartbeatBuffer(HeartbeatBuffer);
}

void UClientSocket::Close_Implementation(EOpCode OpCode)
{
	if (FPlatformAtomics::InterlockedCompareExchange(&bIsRunning, 0, 1) == 0)
	{
		return;
	}
	//FPlatformAtomics::InterlockedExchange(&bIsRunning, 0);
	FScopeLock LockStep1(&Mutex);
	if (Sender != nullptr)
	{
		UMessageWriter* Message = NewObject<UMessageWriter>(this);
		Message->SetOpCode(static_cast<int8>(EOpCode::Close));
		TArray<uint8> CloseMessage;
		UFEPackageFactory::Pack(CloseMessage, Message, MessageOffset, nullptr, nullptr);
		try
		{
			Sender->Push(this, CloseMessage, true);
		}
		catch (...)
		{
		}
	}
	LockStep1.Unlock();
	Super::Close_Implementation(OpCode);
	FScopeLock LockStep2(&Mutex);
	if (HeartbeatTimer != nullptr /*&& HeartbeatTimer.IsValid()*/)
	{
		try
		{
			HeartbeatTimer->Kill(true);
			HeartbeatRunnable.Reset();
			//delete HeartbeatRunnable;
			delete HeartbeatTimer;
			HeartbeatRunnable = nullptr;
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("Fail to dispose HeartbeatTimer."));
		}
		HeartbeatTimer = nullptr;
	}

	if (LoopThread != nullptr /*&& HeartbeatTimer.IsValid()*/)
	{
		try
		{
			LoopThread->Kill(true);
			LoopRunnable.Reset();
			//delete LoopRunnable;
			delete LoopThread;
			LoopRunnable = nullptr;
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("LoopThread abort error."));
		}
		LoopThread = nullptr;
	}
	if (UserToken != nullptr)
	{
		UserToken->ClearGarbage();
		UserToken->MarkAsGarbage();
		UserToken = nullptr;
	}
	//管理类回收
	if (Sender != nullptr)
	{
		try
		{
			Sender->Release();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("Fail to release sender."));
		}
		Sender = nullptr;
	}
	if (Receiver != nullptr)
	{
		try
		{
			Receiver->Release();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,TEXT("Fail to release receiver."));
		}
		Receiver = nullptr;
	}
	UFEConsole::WriteInfoWithCategory(UFENCategories::SOCKET, TEXT("Socket Closed."), false);
}

void UClientSocket::Send(UMessageWriter* Message)
{
	AutoConnect();
	TArray<uint8> Data;
	UFEPackageFactory::Pack(Data, Message, GetMessageOffset(), GetCompression(), GetCryptoProvider());
	Sender->Push(this, Data, false);
}

void UClientSocket::SendImmediately(UMessageWriter* Message)
{
	AutoConnect();
	TArray<uint8> Data;
	UFEPackageFactory::Pack(Data, Message, GetMessageOffset(), GetCompression(), GetCryptoProvider());
	Sender->Push(this, Data, true);
}

void UClientSocket::SendBytes(const TArray<uint8>& Data)
{
	AutoConnect();
	Sender->Push(this, Data, false);
}

void UClientSocket::SendBytesImmediately(const TArray<uint8>& Data)
{
	AutoConnect();
	Sender->Push(this, Data, true);
}

uint32 FClientSocketHeartbeatRunnable::Run()
{
	while (IsRunning && Socket != nullptr && FPlatformAtomics::AtomicRead(&Socket->bIsRunning) != 0)
	{
		Socket->SendHeartbeatPackage();
		for (int32 i = 0;
		     i < Socket->GetHeartbeatInterval() && IsRunning && Socket != nullptr && FPlatformAtomics::AtomicRead(&Socket->bIsRunning) != 0;
		     i++)
		{
			FPlatformProcess::Sleep(1.f / 1000.f);
		}
	}
	return 0;
}

void FClientSocketHeartbeatRunnable::Stop()
{
	IsRunning.AtomicSet(false);
}

uint32 FClientSocketLoopingRunnable::Run()
{
	if (Socket != nullptr)
	{
		Socket->Looping();
	}
	return 0;
}

void FClientSocketLoopingRunnable::Stop()
{
	//IsRunning.AtomicSet(false);
}

//
// void FClientSocketHeartbeatRunnable::Exit()
// {
// 	delete this;
// }

void UClientSocket::OnMessageReceived(UFESocket* InSocket, UMessageReader* Message)
{
	try
	{
		if (MessageContractor != nullptr)
		{
			UMessageWorker* Worker = NewObject<UMessageWorker>(this);
			Worker->Message = Message;
			Worker->Socket = this;
			IIBoss::Execute_CheckIn(MessageContractor, Worker);
		}
		else
		{
			//Message->AddToRoot();
			(new FAutoDeleteAsyncTask<FClientSocketMessageProcessor>(this, Message))->StartBackgroundTask();
		}
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("Process message error."));
	}
}

void UClientSocket::ProcessMessage(UMessageReader* Message)
{
	if (Message == nullptr)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET,
		                                  TEXT("ClientSocket receive empty message."));
		return;
	}
	try
	{
		ActionProvider->Provide(Message);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(UFENCategories::SOCKET, TEXT("ActionProvider error."));
	}
}

void FClientSocketConnectAsyncTask::DoWork()
{
	if (Socket.IsValid())
	{
		bool bSuccess = Socket->Connect();
		Callback.ExecuteIfBound(bSuccess);
	}
}

TStatId FClientSocketConnectAsyncTask::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FClientSocketConnectAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
}

void FClientSocketMessageProcessor::DoWork()
{
	// if (Message != nullptr && Message->IsRooted())
	// {
	// 	Message->RemoveFromRoot();
	// 	Message->ClearGarbage();
	// 	Message->MarkAsGarbage();
	// }
	if (Socket.IsValid())
	{
		Socket->ProcessMessage(Message);
	}
}

TStatId FClientSocketMessageProcessor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FClientSocketMessageProcessor, STATGROUP_ThreadPoolAsyncTasks);
}
