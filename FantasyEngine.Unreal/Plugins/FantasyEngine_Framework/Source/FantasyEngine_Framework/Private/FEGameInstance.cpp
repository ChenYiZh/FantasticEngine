// Fill out your copyright notice in the Description page of Project Settings.


#include "FEGameInstance.h"

#include "FantasyEngineSettings.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Systems/AudioSystem.h"
#include "Systems/BlueprintBridgeUtilsBase.h"
#include "Utilities/TextUtility.h"
#include "Misc/EngineVersionComparison.h"
#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#else
#include "Engine/GameEngine.h"
#endif

void UFEGameInstance::InitGameRoot(const UObject* WorldContextObject)
{
	if (UFEGameInstance* Instance = WorldContextObject->GetWorld()->GetGameInstance<UFEGameInstance>())
	{
		if (Instance)
		{
#if UE_VERSION_NEWER_THAN(5, 2, 0)
			/*** 5.2修改防止UI在加载场景时移除 ***/
			FWorldDelegates::OnWorldCleanup.RemoveAll(UGameViewportSubsystem::Get());
			FWorldDelegates::OnWorldBeginTearDown.RemoveAll(UGameViewportSubsystem::Get());
			FWorldDelegates::OnPreWorldFinishDestroy.RemoveAll(UGameViewportSubsystem::Get());
#elif UE_VERSION_NEWER_THAN(5, 1, 0)
			/*** 5.1修改防止UI在加载场景时移除 ***/
			FWorldDelegates::LevelRemovedFromWorld.RemoveAll(GetSubsystem<UGameViewportSubsystem>(Instance));
#endif

			/***** 初始化逻辑 *****/
			UClass* GameRootClass = UGameRoot::StaticClass();
			UClass* BlueprintBridgeClass = UBlueprintBridgeUtilsBase::StaticClass();
			if (const UFantasyEngineSettings* Settings = GetDefault<UFantasyEngineSettings>())
			{
				if (Settings->GameRoot.IsValid())
				{
					const FString GameRootClassPath = Settings->GameRoot.GetAssetPathString();
					GameRootClass = TSoftClassPtr<UGameRoot>(GameRootClassPath).LoadSynchronous();
					//GameRootClass = Settings->GameRoot.Get();
				}
				if (Settings->BlueprintBridgeUtils.IsValid())
				{
					const FString BlueprintBridgeClassPath = Settings->BlueprintBridgeUtils.GetAssetPathString();
					BlueprintBridgeClass = TSoftClassPtr<UBlueprintBridgeUtilsBase>(BlueprintBridgeClassPath).
						LoadSynchronous();
				}
			}
			if (Instance->GameRoot == nullptr && GameRootClass)
			{
				Instance->GameRoot = NewObject<UGameRoot>(Instance, GameRootClass);
				/****** 现在可以直接获取运行时的游戏场景 *******/
				//Instance->MainClass = NewObject<UMainClass>(GEngine->GetCurrentPlayWorld(), InMainClass.Get());
				//Instance->GameRoot->GameInstance = Instance;
				//UGameRoot::Root = Instance->GameRoot;
				if (Instance->bInited)
				{
					Instance->GameRoot->Init();
					Instance->GameRoot->GameDefines->BlueprintBridgeUtils = Cast<UBlueprintBridgeUtilsBase>(
						UObjectFactory::Singleton(WorldContextObject, BlueprintBridgeClass));
				}
			}
		}
	}
}

// UDefaultGameInstance* UDefaultGameInstance::GetInstance()
// {
// 	UGameInstance* Instance = nullptr;
// #if WITH_EDITOR
// 	if (GEngine && GEngine->GetCurrentPlayWorld()) { Instance = GEngine->GetCurrentPlayWorld()->GetGameInstance(); }
// #else
// 	UGameEngine* Engine = Cast<UGameEngine>(GEngine);
// 	if (Engine) Instance = Engine->GameInstance;
// #endif
// 	return Cast<UDefaultGameInstance>(Instance);
// 	//return UWorldUtility::FindGameWorld()->GetGameInstance<UDefaultGameInstance>();
// }

void UFEGameInstance::PostInitProperties()
{
	Super::PostInitProperties();
}

void UFEGameInstance::Init()
{
	Super::Init();
	//Instance = this;
	bInited = true;
	if (GameRoot != nullptr)
	{
		GameRoot->Init();
	}
}

void UFEGameInstance::OnStart()
{
	Super::OnStart();
	InitGameRoot(GetWorld());
}

void UFEGameInstance::Shutdown()
{
	if (GameRoot != nullptr)
	{
		GameRoot->Shutdown();
		GameRoot = nullptr;
	}
	//Instance = nullptr;
	Super::Shutdown();
}

void UFEGameInstance::BeginDestroy()
{
	if (GameRoot != nullptr)
	{
		GameRoot = nullptr;
	}
	//Instance = nullptr;
	bInited = false;
	Super::BeginDestroy();
}

void UFEGameInstance::Cmd(const FString Cmd)
{
	if (GameRoot)
	{
		TArray<FString> Cmds;
		Cmd.ParseIntoArray(Cmds,TEXT(" "));
		GameRoot->Execute(Cmds);
	}
}

FString UFEGameInstance::CustomCommand_Implementation(const FString& Cmd)
{
	return FString();
}
