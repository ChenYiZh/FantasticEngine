// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/ObjectPooling.h"

#include "Log/FEConsole.h"
#include "Utilities/ObjectFactory.h"

// void IPoolItem::OnInitialize_Implementation(UPoolItemHandle* Handle)
// {
// }
//
// void IPoolItem::BeginToUse_Implementation()
// {
// }
//
// void IPoolItem::OnRelease_Implementation()
// {
// }

UObjectPooling* UPoolItemHandle::GetPooling(TSubclassOf<UObjectPooling> Class) const
{
	return Pooling;
}

void UPoolItemHandle::Release()
{
	UObjectPooling::Collect(Pooling, this);
}

UObject* UPoolItemHandle::GetObject(TSubclassOf<UObject> Class) const
{
	return Object;
}

int UObjectPooling::GetTotalCount() const
{
	return UsingItems.Num() + Collection.Num();
}

void UObjectPooling::BeforeUse(UPoolItemHandle* Handle)
{
}

void UObjectPooling::Initialize(TSubclassOf<UObject> InObjectClass)
{
	ObjectClass = InObjectClass;
	UsingItems.Empty();
	Collection.Empty();
}

UPoolItemHandle* UObjectPooling::GetItemHandle()
{
	UPoolItemHandle* Handle = nullptr;
	if (Collection.Num() > 0)
	{
		Handle = Collection[0];
	}
	if (Handle == nullptr)
	{
		UObject* Obj = CreateObject();
		if (!Obj)
		{
			return nullptr;
		}
		Handle = NewObject<UPoolItemHandle>(this, UPoolItemHandle::StaticClass());
		Handle->Pooling = this;
		Handle->Object = Obj;
		BeforeUse(Handle);
		if (Obj->Implements<UPoolItem>())
		{
			//IPoolItem::Execute_SetPooling(Obj, this);
			//Cast<IPoolItem>(Obj)->Pooling = this;
			IPoolItem::Execute_OnInitialize(Obj, Handle);
		}
	}
	else
	{
		BeforeUse(Handle);
	}
	Use(this, Handle);
	return Handle;
}

void UObjectPooling::Release()
{
	while (UsingItems.Num() > 0)
	{
		Collect(this, UsingItems[0]);
	}
}

void UObjectPooling::Reset()
{
	Release();
	UsingItems.Empty();
	Collection.Empty();
}

void UObjectPooling::GetUsingItems(TSubclassOf<UObject> InObjectClass, TArray<UObject*>& OutObjects) const
{
	OutObjects.Empty();
	OutObjects.SetNum(UsingItems.Num());
	for (int32 i = 0; i < UsingItems.Num(); i++)
	{
		OutObjects[i] = UsingItems[i]->GetObject();
	}
}

void UObjectPooling::Use(UObjectPooling* Pooling, UPoolItemHandle* Handle)
{
	if (!Handle) { return; }
	if (Pooling->Collection.Contains(Handle))
	{
		Pooling->Collection.Remove(Handle);
	}
	if (!Pooling->UsingItems.Contains(Handle))
	{
		Pooling->UsingItems.Add(Handle);
	}
	Pooling->OnUse(Handle);
	UObject* Obj = Handle->GetObject();
	if (Obj && Obj->Implements<UPoolItem>())
	{
		IPoolItem::Execute_BeginToUse(Obj);
	}
}

void UObjectPooling::Collect(UObjectPooling* Pooling, UPoolItemHandle* Handle)
{
	if (!Pooling) { return; }
	if (Pooling->UsingItems.Contains(Handle))
	{
		Pooling->UsingItems.Remove(Handle);
	}
	if (!Handle) { return; }
	if (!Pooling->Collection.Contains(Handle))
	{
		Pooling->Collection.Add(Handle);
	}
	Pooling->OnCollect(Handle);
	UObject* Obj = Handle->GetObject();
	if (Obj && Obj->Implements<UPoolItem>())
	{
		IPoolItem::Execute_OnRelease(Obj);
	}
}

void UObjectPooling::Collect(UPoolItemHandle* Handle)
{
	if (Handle)
	{
		Handle->Release();
	}
}

UObject* UObjectPooling::CreateObject()
{
	if (!ObjectClass)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Object Pool"),TEXT("对象池未初始化"));
		return nullptr;
	}
	return NewObject<UObject>(this, ObjectClass);
}

void UObjectPooling::OnUse(UPoolItemHandle* Handle)
{
}

void UObjectPooling::OnCollect(UPoolItemHandle* Handle)
{
}
