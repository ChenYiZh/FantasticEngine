// Fill out your copyright notice in the Description page of Project Settings.


#include "GameRoot.h"

#include "FEGameInstance.h"
#include "Defines/EventIdsBasic.h"
#include "Engine/LevelScriptActor.h"
#include "Interfaces/IWidgetFilter.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Log/FEConsole.h"
#include "Materials/MaterialParameterCollection.h"
#include "Systems/EventSystem.h"
#include "Systems/FilterStorage.h"
#include "Systems/UELogger.h"
#include "Utilities/ObjectFactory.h"

void UGameRoot::Registing_Implementation()
{
	//读取系统配置
	if (UDataTable* SystemConfigs = GameDefines->GetSystemConfigs())
	{
		for (const TPair<FName, uint8*>& SystemConfig : SystemConfigs->GetRowMap())
		{
			const FSystemConfig& SystemConfigValue = *reinterpret_cast<FSystemConfig*>(SystemConfig.Value);
			if (SystemConfigValue.Enable && SystemConfigValue.SystemClass)
			{
				Systems.Add(Cast<USystemBasis>(UObjectFactory::Singleton(this, SystemConfigValue.SystemClass)));
			}
		}
	}
	//读取UI配置信息
	if (UDataTable* WidgetConfigs = GameDefines->GetWidgetConfigs())
	{
		for (const TPair<FName, uint8*>& WidgetConfig : WidgetConfigs->GetRowMap())
		{
			const FWidgetConfig& WidgetConfigValue = *reinterpret_cast<FWidgetConfig*>(WidgetConfig.Value);
			if (WidgetConfigValue.Enable)
			{
				Singleton<UUIFactory>(this)->RegistWidget(WidgetConfig.Key, WidgetConfigValue);
			}
		}
	}
	//读取UI过滤器配置信息
	if (UDataTable* WidgetFilterConfigs = GameDefines->GetWidgetFilterConfigs())
	{
		for (const TPair<FName, uint8*>& WidgetFilterConfig : WidgetFilterConfigs->GetRowMap())
		{
			const FWidgetFilterConfig& WidgetFilterConfigValue = *reinterpret_cast<FWidgetFilterConfig*>(
				WidgetFilterConfig.Value);
			if (WidgetFilterConfigValue.Enable)
			{
				UFilterStorage::AddWidgetFilter(NewObject<UObject>(this, WidgetFilterConfigValue.Filter.Get()));
			}
		}
	}
	//读取Level过滤器配置信息
	if (UDataTable* LevelFilterConfigs = GameDefines->GetLevelFilterConfigs())
	{
		for (const TPair<FName, uint8*>& LevelFilterConfig : LevelFilterConfigs->GetRowMap())
		{
			const FLevelFilterConfig& LevelFilterConfigValue = *reinterpret_cast<FLevelFilterConfig*>(
				LevelFilterConfig.Value);
			if (LevelFilterConfigValue.Enable)
			{
				UFilterStorage::AddLevelFilter(NewObject<UObject>(this, LevelFilterConfigValue.Filter));
			}
		}
	}
}

UWorld* UGameRoot::GetRootWorld(const UObject* WorldContextObject)
{
	if (const UGameRoot* Root = GetRoot(WorldContextObject))
	{
		return Root->GetWorld();
	}
	return nullptr;
}

UFEGameInstance* UGameRoot::GetGameInstance(const UObject* WorldContextObject,
                                                 TSubclassOf<UFEGameInstance> Class)
{
	return WorldContextObject->GetWorld()->GetGameInstance<UFEGameInstance>();
}

UGameRoot* UGameRoot::GetRoot(const UObject* WorldContextObject, TSubclassOf<UGameRoot> Class)
{
	return GetGameInstance(WorldContextObject)->GameRoot;
}

EGameRootState UGameRoot::GetState(const UObject* WorldContextObject)
{
	return GetRoot(WorldContextObject)->State;
}

UGameDefines* UGameRoot::GetGameDefines(const UObject* WorldContextObject, TSubclassOf<UGameDefines> Class)
{
	if (const UGameRoot* Root = GetRoot(WorldContextObject))
	{
		return Root->GameDefines;
	}
	return nullptr;
}

bool UGameRoot::IsDebug(const UObject* WorldContextObject)
{
	if (const UGameRoot* Root = GetRoot(WorldContextObject))
	{
		if (const UGameDefines* Defines = Root->GameDefines)
		{
			return Defines->IsDebug();
		}
	}
	return WITH_EDITOR;
}

void UGameRoot::Init()
{
	//Super::Init();
	State = EGameRootState::Invalid;
	// if (!Enabled)
	// {
	// 	return;
	// }
	//Instance = this;
	State = EGameRootState::Starting;
	//UWorldUtility::ClearCache();
	SingletonPool = TMap<UClass*, UObject*>();
	UFEConsole::RegistLogger(Singleton<UUELogger>(this));
	Systems = TSet<USystemBasis*>();
	FirstLevelActor = nullptr;
	if (GameDefinesClass)
	{
		GameDefines = NewObject<UGameDefines>(this, GameDefinesClass.Get());
		GameDefines->Initialize();
		//GameDefines->AddToRoot();
	}
	Registing();
	RegistEvents();
	/********* 初始化鼠标显示操作 ************/
	if (IsValid(GameDefines))
	{
		GameDefines->SetMouseType(GameDefines->GetMouseType(), true);
	}
	/*********************/
	TickHandler = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UGameRoot::Tick));
}

void UGameRoot::OnCreate_Implementation()
{
	// FString fullPath = UPathUtility::RESOURCE_PATH() + FString("Tables/ConfigsFramework.txt");
	// Log(fullPath);
	// FString content;
	// if (FPaths::FileExists(fullPath) && UPathUtility::ReadFileText(fullPath, content))
	// {
	// 	Log(content);
	// }

	//UEventSystem::SendEvent(UEventIdsBasic::GET_READY());
}

//void UGameRoot::GetReady_Implementation(UEventParam* Param)
bool UGameRoot::Ready()
{
	bool ready = true;
	for (auto system : Systems)
	{
		try
		{
			if (!system->IsReady())
			{
				ready = false;
			}
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling IsReady."), ToCStr(system->GetFName().ToString())));
		}
	}
	if (ready)
	{
		OnReady();
	}
	return ready;
}

void UGameRoot::StartUp(UEventParam* Param)
{
	FirstLevelActor = nullptr;
	State = EGameRootState::Initialized;
	ExecuteConsoleCommands(this);
	OnCreate();
	OnInitialize();
}

void UGameRoot::ExecuteConsoleCommands(const UObject* WorldContextObject)
{
	if (const UGameDefines* Defines = GetGameDefines(WorldContextObject))
	{
		TArray<FGameDefineCommand> Commands;
		Defines->GetConsoleCommands(Commands);
		for (const FGameDefineCommand& Command : Commands)
		{
			if (!Command.bOnlyDebug || Defines->IsDebug())
			{
				UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject, Command.Command);
			}
		}
	}
}

void UGameRoot::Execute_Implementation(const TArray<FString>& Cmds)
{
}

void UGameRoot::RegistEvents_Implementation()
{
	//UEventSystem::RegistEvent(UEventIdsBasic::GET_READY(), this, "GetReady");
}

void UGameRoot::UnregistEvents_Implementation()
{
	//UEventSystem::UnregistEvent(UEventIdsBasic::GET_READY(), this, "GetReady");
}

void UGameRoot::OnInitialize_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnInitialize();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnInitialize."), ToCStr(system->GetFName().ToString())));
		}
	}
}

bool UGameRoot::Tick(float DeltaTime)
{
	if (IsEngineExitRequested()) { return false; }
	if (IsValid(GameDefines))
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GameDefines->GetMPC_Framework(), FName("IsGame"),
		                                                1.f);
	}
	// if (!Enabled)
	// {
	// 	return true;
	// }
	if (State == EGameRootState::Starting)
	{
		if (!Ready())
		{
			return true;
		}
		State = EGameRootState::Ready;
		return true;
	}
	if (State == EGameRootState::Ready)
	{
		// if (FirstLevelActor == nullptr)
		// {
		const UWorld* World = GetWorld(); //UWorldUtility::FindGameWorld();
		if (World != nullptr)
		{
			const ULevel* Level = World->GetCurrentLevel();
			if (Level != nullptr && Level->IsFinishedRouteActorInitialization())
			// Level->LevelScriptActor != nullptr)
			{
				StartUp();
				//FirstLevelActor = Level->LevelScriptActor;
			}
			else
			{
				return true;
			}
		}
		// else
		// {
		// 	return true;
		// }
		//}
		// if (FirstLevelActor->HasActorBegunPlay())
		// {
		// 	StartUp();
		// }
		//return true;
	}
	OnTick(DeltaTime);
	Seconds += DeltaTime;
	if (Seconds >= 1)
	{
		Seconds = 0;
		OnEverySecond();
	}
	return true;
}

void UGameRoot::Shutdown()
{
	// if (!Enabled)
	// {
	// 	Super::Shutdown();
	// 	return;
	// }
	
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandler);
	State = EGameRootState::Shutdown;
	UnregistEvents();
	OnQuit();
	Systems.Empty();
	UFEConsole::RemoveLogger(Singleton<UUELogger>(this));
	UObjectFactory::RemoveAllSingletons(this);
	
	//Root = nullptr;
	//UWorldUtility::ClearCache();
	//Super::Shutdown();
}

bool UGameRoot::Initialized() const
{
	return State > EGameRootState::Ready;
}

void UGameRoot::PostInitProperties()
{
	Super::PostInitProperties();
	// if (!Enabled)
	// {
	// 	return;
	// }
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(
		this, &UGameRoot::OnApplicationHasEnteredForegroundDelegate_Handler);
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(
		this, &UGameRoot::OnApplicationWillEnterBackgroundDelegate_Handler);
	FCoreDelegates::ApplicationShouldUnloadResourcesDelegate.AddUObject(
		this, &UGameRoot::OnApplicationShouldUnloadResourcesDelegate_Handler);
}

void UGameRoot::BeginDestroy()
{
	State = EGameRootState::Invalid;
	// if (!Enabled)
	// {
	// 	Super::BeginDestroy();
	// 	return;
	// }
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.RemoveAll(this);
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);
	FCoreDelegates::ApplicationShouldUnloadResourcesDelegate.RemoveAll(this);
	Super::BeginDestroy();
}

void UGameRoot::OnApplicationWillEnterBackgroundDelegate_Handler()
{
	OnApplicationPause(true);
	OnApplicationFocus(false);
}

void UGameRoot::OnApplicationHasEnteredForegroundDelegate_Handler()
{
	OnApplicationFocus(true);
	OnApplicationPause(false);
}

void UGameRoot::OnApplicationShouldUnloadResourcesDelegate_Handler()
{
	try
	{
		OnLowMemory();
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("System"),TEXT("Some errors happened OnLowMemory."));
	}
}

void UGameRoot::OnLowMemory_Implementation()
{
	UFEConsole::Write("OnLowMemory_Implementation");
}

void UGameRoot::OnReady_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnReady();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnReady."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnLogin_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnLogin();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnLogin."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnLogout_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnLogout();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnLogout."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnReset_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnReset();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnReset."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnTick_Implementation(float DeltaTime)
{
	for (auto system : Systems)
	{
		if (IsEngineExitRequested()) { return; }
		if (State == EGameRootState::Initialized
			|| (State == EGameRootState::Starting && system->EnableTickBeforeInitialized()))
		{
			try
			{
				system->OnTick(DeltaTime);
			}
			catch (...)
			{
				UFEConsole::WriteErrorWithCategory(
					TEXT("System"),
					FString::Printf(TEXT("%s is error on calling OnTick."), ToCStr(system->GetFName().ToString())));
			}
		}
	}
}

void UGameRoot::OnEverySecond_Implementation()
{
	for (auto system : Systems)
	{
		if (IsEngineExitRequested()) { return; }
		if (State == EGameRootState::Initialized
			|| (State == EGameRootState::Starting && system->EnableTickBeforeInitialized()))
		{
			try
			{
				system->OnEverySecond();
			}
			catch (...)
			{
				UFEConsole::WriteErrorWithCategory(
					TEXT("System"),
					FString::Printf(
						TEXT("%s is error on calling OnEverySeconds."), ToCStr(system->GetFName().ToString())));
			}
		}
	}
}

void UGameRoot::OnApplicationFocus_Implementation(bool Focus)
{
	for (auto system : Systems)
	{
		try
		{
			system->OnApplicationFocus(Focus);
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(
					TEXT("%s is error on calling OnApplicationFocus."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnApplicationPause_Implementation(bool Pause)
{
	for (auto system : Systems)
	{
		try
		{
			system->OnApplicationPause(Pause);
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(
					TEXT("%s is error on calling OnApplicationPause."), ToCStr(system->GetFName().ToString())));
		}
	}
}

void UGameRoot::OnQuit_Implementation()
{
	for (auto system : Systems)
	{
		try
		{
			system->OnQuit();
		}
		catch (...)
		{
			UFEConsole::WriteErrorWithCategory(
				TEXT("System"),
				FString::Printf(TEXT("%s is error on calling OnQuit."), ToCStr(system->GetFName().ToString())));
		}
	}
	// if (GameDefines && GameDefines->IsRooted())
	// {
	// 	GameDefines->RemoveFromRoot();
	// }
}
