// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/EventSystem.h"

#include "Log/FEConsole.h"
#include "Utilities/ObjectFactory.h"

bool UEventSystem::EnableTickBeforeInitialized_Implementation()
{
	return true;
}

void UEventSystem::RegistEventImpl(int32 EventId, FEventCallback Callback)
{
	if (!Callbacks.Contains(EventId))
	{
		FEventCallbackArray Array;
		Callbacks.Add(EventId, Array);
	}
	Callbacks[EventId].Callbacks.Add(Callback);
}

FEventCallback UEventSystem::RegistEventNative(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
                                               const FName& InFunctionName)
{
	FEventCallback Callback;
	Callback.BindUFunction(InObject, InFunctionName);
	K2_RegistEvent(WorldContextObject, EventId, Callback);
	return Callback;
}

void UEventSystem::UnregistEventImpl(int32 EventId, FEventCallback Callback)
{
	if (!Callbacks.Contains(EventId))
	{
		return;
	}
	Callbacks[EventId].Callbacks.Remove(Callback);
	//Callback.Unbind();
}

void UEventSystem::UnregistEventNative(int32 EventId, UObject* InObject, const FName& InFunctionName)
{
	if (!Callbacks.Contains(EventId))
	{
		return;
	}
	TArray<FEventCallback>& list = Callbacks[EventId].Callbacks;
	for (int i = list.Num() - 1; i >= 0; i--)
	{
		FEventCallback& Callback = list[i];
		if (Callback.GetUObject() == InObject && Callback.GetFunctionName() == InFunctionName)
		{
			list.RemoveAt(i);
			//Callback.Unbind();
		}
	}
	if (list.IsEmpty())
	{
		Callbacks.Remove(EventId);
	}
}

void UEventSystem::SendEventImpl(int32 EventId, UEventParam* Param)
{
	if (IsValid(Param))
	{
		Param->Rename(ToCStr(Param->GetName()), this);
		//Param->AddToRoot();
	}
	FEventMessagePair Pair;
	Pair.EventId = EventId;
	Pair.Param = Param;
	//Lock.lock();
	if (IsInGameThread())
	{
		//Messages.Add(Pair);
		ExecuteMessage(Pair);
		return;
	}
	FScopeLock Lock(&Mutex);
	Messages.Add(Pair);
}

void UEventSystem::OnTick_Implementation(float DeltaTime)
{
	//指针计数测试
	// for (TPair<int, TSharedPtr<TArray<TSharedPtr<FEventCallback>>>>& Pair : Callbacks.ToSharedRef().Get())
	// {
	// 	for (TSharedPtr<FEventCallback>& Call : Pair.Value.ToSharedRef().Get())
	// 	{
	// 		Log(FString::Printf(TEXT("Callback Count: %d"), Call.GetSharedReferenceCount()), DeltaTime);
	// 	}
	// }
	Super::OnTick_Implementation(DeltaTime);
	TQueue<FEventMessagePair> MsgArray;

	//Lock.lock();
	FScopeLock Lock(&Mutex);

	while (!Messages.IsEmpty())
	{
		FEventMessagePair MsgPair = Messages[0];
		Messages.RemoveAt(0);
		MsgArray.Enqueue(MsgPair);
	}

	//Lock.unlock();
	Lock.Unlock();

	if (!MsgArray.IsEmpty())
	{
		FEventMessagePair Pair;
		while (MsgArray.Dequeue(Pair))
		{
			ExecuteMessage(Pair);
		}
	}
}

void UEventSystem::ExecuteMessage(const FEventMessagePair& Message)
{
	const FEventMessagePair& Pair = Message;
	if (Callbacks.Contains(Pair.EventId))
	{
		TArray<FEventCallback>& list = Callbacks[Pair.EventId].Callbacks;
		for (int i = list.Num() - 1; i >= 0; i--)
		{
			FEventCallback& callback = list[i];
			if (callback.IsBound())
			{
				try
				{
					callback.Execute(Pair.Param);
				}
				catch (...)
				{
					UFEConsole::WriteErrorWithCategory(
						TEXT("Event System"),
						FString::Printf(TEXT("An error happened by event id: %d"), Pair.EventId));
				}
			}
			else
			{
				list.RemoveAt(i);
			}
		}
		// if (Pair.Value != nullptr)
		// {
		// 	Destroy(Pair.Value);
		// }
		if (IsValid(Pair.Param))
		{
			//Pair.Param->RemoveFromRoot();
			Pair.Param->ClearGarbage();
			Pair.Param->MarkAsGarbage();
		}
	}
}

void UEventSystem::OnQuit_Implementation()
{
	// Lock.lock();
	// Messages.ToSharedRef().Get().Empty();
	// Callbacks.ToSharedRef().Get().Empty();
	// Lock.unlock();

	FScopeLock Lock(&Mutex);
	Messages.Empty();
	Callbacks.Empty();
	Lock.Unlock();

	Super::OnQuit_Implementation();
}


void UEventSystem::K2_RegistEvent(const UObject* WorldContextObject, int32 EventId, FEventCallback Callback)
{
	if (UEventSystem* System = Singleton<UEventSystem>(WorldContextObject))
	{
		System->RegistEventImpl(EventId, Callback);
	}
}

void UEventSystem::K2_UnregistEvent(const UObject* WorldContextObject, int32 EventId, FEventCallback Callback)
{
	if (UEventSystem* System = Singleton<UEventSystem>(WorldContextObject))
	{
		System->UnregistEventImpl(EventId, Callback);
	}
}

void UEventSystem::SendEvent(const UObject* WorldContextObject, int32 EventId, UEventParam* Param)
{
	if (UEventSystem* System = Singleton<UEventSystem>(WorldContextObject)) { System->SendEventImpl(EventId, Param); }
}

FEventCallback UEventSystem::RegistEvent(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
                                         const FName& InFunctionName)
{
	if (UEventSystem* System = Singleton<UEventSystem>(WorldContextObject))
	{
		return System->RegistEventNative(WorldContextObject, EventId, InObject, InFunctionName);
	}
	return FEventCallback();
}

void UEventSystem::UnregistEvent(const UObject* WorldContextObject, int32 EventId, UObject* InObject,
                                 const FName& InFunctionName)
{
	if (UEventSystem* System = Singleton<UEventSystem>(WorldContextObject))
	{
		System->UnregistEventNative(EventId, InObject, InFunctionName);
	}
}
