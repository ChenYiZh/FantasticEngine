// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AssetSystem.h"

#include "GameRoot.h"
//#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/AssetManager.h"
#include "Utilities/PathUtility.h"

// UUserWidget* UAssetSystem::LoadWidget(const FString& ClassName)
// {
// 	FStringFormatOrderedArguments Args;
// 	Args.Add(UMainClass::GetGameDefines()->WidgetPath);
// 	Args.Add(ClassName);
// 	FString Asset = FString::Format(TEXT("/Game{0}/{1}"), Args);
// 	ConstructorHelpers::StripObjectClass(Asset, true);
// 	TSubclassOf<UUserWidget> Class = ConstructorHelpersInternal::FindOrLoadClass(Asset, UUserWidget::StaticClass());
// 	if (Class)
// 	{
// 		return CreateWidget<UUserWidget>(UWorldUtility::FindGameWorld(), Class);
// 	}
// 	return nullptr;
// }

void UAssetSystem::LoadWidgetAsync(const FString& ClassPath, FAssetClassCallback Callback)
{
	//LoadUClassAsyncImpl(UPathUtility::CombineBlueprintPath(UPathUtility::WIDGETS_PATH(), ClassName), Callback);
	FSoftObjectPath SoftPath(ClassPath);
	LoadUClassAsyncImpl(UPathUtility::CheckBlueprintPath(SoftPath.GetAssetPath().ToString()), Callback);
}

void UAssetSystem::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	AssetHandle.bIsLoading = false;
}

void UAssetSystem::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);
	if (!AssetHandle.bIsLoading && !TaskQueue.ToSharedRef().Get().IsEmpty())
	{
		TSharedPtr<FAssetHandle> TAssetHandle;
		if (TaskQueue.ToSharedRef().Get().Dequeue(TAssetHandle))
		{
			FAssetHandle& Handle = TAssetHandle.ToSharedRef().Get();
			Handle.bIsLoading = true;
			if (Handle.bIsClass)
			{
				BeginLoadUClassAsync(Handle);
			}
			else
			{
				BeginLoadUObjectAsync(Handle);
			}
		}
	}
}

void UAssetSystem::LoadUClassAsyncImpl(FString FullPath, FAssetClassCallback Callback)
{
	FAssetHandle AssetClassHandle;
	AssetClassHandle.bIsClass = true;
	AssetClassHandle.bIsLoading = false;
	AssetClassHandle.FullPath = FullPath;
	AssetClassHandle.ClassPath = FullPath;
	AssetClassHandle.ClassCallback = Callback;
	if (!AssetHandle.bIsLoading && TaskQueue.ToSharedRef().Get().IsEmpty())
	{
		BeginLoadUClassAsync(AssetClassHandle);
	}
	else
	{
		TaskQueue.ToSharedRef().Get().Enqueue(MakeShared<FAssetHandle>(AssetClassHandle));
	}
}

void UAssetSystem::BeginLoadUClassAsync(const FAssetHandle& AssetClassHandle)
{
	AssetHandle = AssetClassHandle;
	AssetHandle.bIsLoading = true;
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	TSharedPtr<FStreamableHandle> Handle = Manager.RequestAsyncLoad(
		AssetClassHandle.ClassPath, FStreamableDelegate::CreateUObject(this, &UAssetSystem::OnUClassLoaded));
}

void UAssetSystem::OnUClassLoaded()
{
	AssetHandle.bIsLoading = false;
	FAssetHandle AssetClassHandle = AssetHandle;
	UClass* Class = AssetClassHandle.ClassPath.ResolveClass();
	AssetClassHandle.ClassCallback.ExecuteIfBound(Class);
}

void UAssetSystem::LoadUObjectAsyncImpl(FString FullPath, FAssetObjectCallback Callback)
{
	FAssetHandle AssetClassHandle;
	AssetClassHandle.bIsClass = false;
	AssetClassHandle.bIsLoading = false;
	AssetClassHandle.FullPath = FullPath;
	AssetClassHandle.ObjectPath = FullPath;
	AssetClassHandle.ObjectCallback = Callback;
	if (!AssetHandle.bIsLoading && TaskQueue.ToSharedRef().Get().IsEmpty())
	{
		BeginLoadUObjectAsync(AssetClassHandle);
	}
	else
	{
		TaskQueue.ToSharedRef().Get().Enqueue(MakeShared<FAssetHandle>(AssetClassHandle));
	}
}

void UAssetSystem::BeginLoadUObjectAsync(const FAssetHandle& AssetClassHandle)
{
	AssetHandle = AssetClassHandle;
	AssetHandle.bIsLoading = true;
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	TSharedPtr<FStreamableHandle> Handle = Manager.RequestAsyncLoad(
		AssetClassHandle.ObjectPath, FStreamableDelegate::CreateUObject(this, &UAssetSystem::OnUObjectLoaded));
}

void UAssetSystem::OnUObjectLoaded()
{
	AssetHandle.bIsLoading = false;
	FAssetHandle AssetClassHandle = AssetHandle;
	UObject* Class = AssetClassHandle.ObjectPath.ResolveObject();
	AssetClassHandle.ObjectCallback.ExecuteIfBound(Class);
}

void UAssetSystem::Release(bool IncludeTaskQueue)
{
}

void UAssetSystem::LoadUClassAsync(const UObject* WorldContextObject, const FString FullPath,
                                   FAssetClassCallback Callback)
{
	Singleton<UAssetSystem>(WorldContextObject)->LoadUClassAsyncImpl(FullPath, Callback);
}

UClass* UAssetSystem::LoadUClass(FString FullPath)
{
	ConstructorHelpers::StripObjectClass(FullPath, true);
	UClass* Class = ConstructorHelpersInternal::FindOrLoadClass(FullPath, UUserWidget::StaticClass());
	return Class;
}

void UAssetSystem::LoadUObjectAsync(const UObject* WorldContextObject, const FString FullPath,
                                    FAssetObjectCallback Callback)
{
	Singleton<UAssetSystem>(WorldContextObject)->LoadUObjectAsyncImpl(FullPath, Callback);
}
