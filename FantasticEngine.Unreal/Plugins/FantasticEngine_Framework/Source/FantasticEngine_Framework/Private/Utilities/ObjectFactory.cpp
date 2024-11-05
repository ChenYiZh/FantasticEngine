// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/ObjectFactory.h"

#include "GameRoot.h"
#include "Blueprint/UserWidget.h"


TMap<UClass*, UObject*>& UObjectFactory::GetSingletonPool(const UObject* WorldContextObject)
{
	return UGameRoot::GetRoot(WorldContextObject)->SingletonPool;
}

// UObject* UObjectFactory::New(UObject* Outer, UClass* Class)
// {
// 	if (Outer == nullptr || !IsValid(Outer))
// 	{
// 		// if (!UMainClass::GetRoot()->Initialized())
// 		// {
// 		Outer = UMainClass::GetRoot();
// 		//}
//
// 		if (!IsValid(Outer))
// 		{
// 			Outer = UWorldUtility::FindGameWorld();
// 			if (!IsValid(Outer))
// 			{
// 				Outer = GetTransientPackage();
// 			}
// 		}
// 	}
// 	// if (Name == NAME_None)
// 	// {
// 	// 	Name = FName(Class->GetFName().ToString());
// 	// }
// 	return NewObject<UObject>(Outer, Class, NAME_None);
// }

UActorComponent* UObjectFactory::AddComponent(TSubclassOf<UActorComponent> Class, AActor* Actor)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}
	// if (SubobjectFName == NAME_None)
	// {
	// 	FString name = Class->GetFName().ToString();
	// 	for (int i = 0; i < 100; i++)
	// 	{
	// 		SubobjectFName = FName(name + FString(" ") + FString::FromInt(i));
	// 		TSet<UActorComponent*> components = Actor->GetComponents();
	// 		bool exists = false;
	// 		for (auto component : components)
	// 		{
	// 			if (SubobjectFName == component->GetFName())
	// 			{
	// 				exists = true;
	// 				break;
	// 			}
	// 		}
	// 		if (!exists)
	// 		{
	// 			break;
	// 		}
	// 	}
	// }
	UActorComponent* Component = NewObject<UActorComponent>(Actor, Class);
	Actor->AddInstanceComponent(Component);
	Component->RegisterComponent();
	return Component;
}

USceneComponent* UObjectFactory::CopyComponent(USceneComponent* SrcComponent, TSubclassOf<USceneComponent> Class)
{
	if (IsValid(SrcComponent))
	{
		USceneComponent* NewComponent = CastChecked<USceneComponent>(
			StaticDuplicateObject(SrcComponent, SrcComponent->GetOuter()));
		USceneComponent* Parent = SrcComponent->GetAttachParent();
		if (Parent)
		{
			NewComponent->SetupAttachment(Parent);
		}
		return NewComponent;
	}
	return nullptr;
}

UObject* UObjectFactory::Singleton(const UObject* WorldContextObject, UClass* Class, const bool bIncludeChild)
{
	UGameRoot* Root = UGameRoot::GetRoot(WorldContextObject);
	if (!IsValid(Root)) { return nullptr; }
	TMap<UClass*, UObject*>& SingletonPool = Root->SingletonPool;
	bool bExists = SingletonPool.Contains(Class);
	if (!bExists && bIncludeChild)
	{
		TArray<UClass*> Keys;
		SingletonPool.GetKeys(Keys);
		for (UClass* Key : Keys)
		{
			if (Key->IsChildOf(Class))
			{
				Class = Key;
				bExists = true;
				break;
			}
		}
	}
	if (!bExists)
	{
		UObject* obj = NewObject<UObject>(Root, Class);
		//UObject* obj = NewObject<UObject>((UObject*)GetTransientPackage(), Class);
		//obj->AddToRoot();
		SingletonPool.Add(Class, obj);
	}
	return SingletonPool[Class];
}

void UObjectFactory::Destroy(UObject* Object)
{
	if (Object == nullptr)
	{
		return;
	}
	bool bRemoveFromRoot = true;
	AActor* Actor = Cast<AActor>(Object);
	if (Actor != nullptr)
	{
		Actor->Destroy();
		bRemoveFromRoot = false;
	}
	else
	{
		UActorComponent* Component = Cast<UActorComponent>(Object);
		if (Component != nullptr)
		{
			Component->DestroyComponent();
			// AActor* Actor = Cast<AActor>(Component->GetOuter());
			// if (Actor != nullptr && Actor->OwnsComponent(Component))
			// {
			// 	Actor->RemoveOwnedComponent(Component);
			// }
			bRemoveFromRoot = false;
		}
		else
		{
			UWidget* Widget = Cast<UWidget>(Object);
			if (Widget != nullptr)
			{
				Widget->RemoveFromParent();
				bRemoveFromRoot = false;
			}
		}
	}
	if (bRemoveFromRoot && Object->IsRooted())
	{
		Object->RemoveFromRoot();
	}
	Object->ClearGarbage();
	Object->MarkAsGarbage();
}

void UObjectFactory::AddToRoot(UObject* Object)
{
	if (Object == nullptr
		|| Object->IsRooted()
		|| Object->IsA<AActor>()
		|| Object->IsA<UActorComponent>()
		|| Object->IsA<UWidget>())
	{
		return;
	}
	Object->AddToRoot();
}

void UObjectFactory::RemoveFromRoot(UObject* Object)
{
	if (Object == nullptr
		|| Object->IsA<AActor>()
		|| Object->IsA<UActorComponent>()
		|| Object->IsA<UWidget>())
	{
		return;
	}
	if (!Object->IsRooted())
	{
		return;
	}
	Object->RemoveFromRoot();
}

void UObjectFactory::RemoveAllSingletons(const UObject* WorldContextObject)
{
	if (UGameRoot* Root = UGameRoot::GetRoot(WorldContextObject))
	{
		TMap<UClass*, UObject*>& SingletonPool = Root->SingletonPool;
		if (SingletonPool.Num() > 0)
		{
			for (auto singleton : SingletonPool)
			{
				// if (singleton.Value != nullptr && singleton.Value->IsRooted())
				// {
				// 	singleton.Value->RemoveFromRoot();
				// }
				Destroy(singleton.Value);
			}
			SingletonPool.Empty();
		}
	}
}

void Destroy(UObject* Object)
{
	UObjectFactory::Destroy(Object);
}

// TMap<UClass*, UObject*>& SingletonPool()
// {
// 	return UObjectFactory::GetSingletonPool();
// }
