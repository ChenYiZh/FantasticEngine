// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/LevelSystem.h"

#include "GameRoot.h"
#include "Basis/Level/Map.h"
#include "Defines/EventIdsBasic.h"
#include "Kismet/GameplayStatics.h"
#include "Log/FEConsole.h"
#include "Systems/FilterStorage.h"
#include "Systems/InputSystem.h"
#include "Utilities/PathUtility.h"

FName ULevelSystem::LOG_CATEGORY = TEXT("Level System");

bool ULevelSystem::IsLoading() const
{
	return IsValid(LoadingMap);
}

void ULevelSystem::LoadMapAsync(UMap* NewMap, bool bShowLoading)
{
	if (!IsValid(NewMap))
	{
		UFEConsole::WriteErrorWithCategory(LOG_CATEGORY,TEXT("The new Map is null!"));
		return;
	}
	if (ULevelSystem* System = Singleton<ULevelSystem>(NewMap))
	{
		System->LoadMapAsyncImpl(NewMap, bShowLoading);
	}
}

void ULevelSystem::LoadMapAsyncImpl(UMap* NewMap, bool bShowLoading)
{
	if (!IsValid(NewMap))
	{
		UFEConsole::WriteErrorWithCategory(LOG_CATEGORY,TEXT("The new Map is null!"));
		return;
	}
	if (IsLoading())
	{
		UFEConsole::WriteErrorWithCategory(LOG_CATEGORY,TEXT("Another Map is loading!"));
		return;
	}
	if (bShowLoading)
	{
		UEventSystem::SendEvent(NewMap, UEventIdsBasic::SHOW_LOADING());
		ULoadingParam* LoadingParam = NewObject<ULoadingParam>(UGameRoot::GetRoot(this));
		LoadingParam->Progress = 0;
		UEventSystem::SendEvent(this, UEventIdsBasic::LOADING_PROGRESS(), LoadingParam);
	}
	// 如果 Outer 不对，会在场景销毁后，被回收。所以直接修改 Outer。
	// Rename not allowed 就将 Outer 改为 GameRoot
	if (NewMap->GetOuter() != UGameRoot::GetRoot(this))
	{
		NewMap->Rename(ToCStr(NewMap->GetName()), UGameRoot::GetRoot(this));
	}

	LoadingMap = NewMap;

	BeginLoadLevel(NewMap);
}

void ULevelSystem::BeginLoadLevel(UMap* NewMap)
{
	// 判断可不可以加载
	bool bCanLoad = true;
	TArray<UObject*> Filters = UFilterStorage::GetLevelFilters(this);
	for (UObject* Filter : Filters)
	{
		if (!IILevelFilter::Execute_OnMapBeginToLoad(Filter, NewMap))
		{
			bCanLoad = false;
		}
	}
	if (!bCanLoad)
	{
		LoadingMap = nullptr;
		return;
	}

	// 获取加载使用的Url
	FString Url;
	if (NewMap->bByObjectReference)
	{
		Url += NewMap->Map.GetLongPackageName();
	}
	else
	{
		Url += NewMap->MapName;
	}
	if (!NewMap->Options.IsEmpty())
	{
		Url += NewMap->Options;
	}

	// 通知场景注销
	for (UObject* Filter : Filters)
	{
		IILevelFilter::Execute_OnMapLeft(Filter, CurrentMap);
	}
	CurrentMap = nullptr;

	UWorld* GameWorld = NewMap->GetWorld();
	if (NewMap->bUseServerTravel)
	{
		GameWorld->ServerTravel(Url, false);
	}
	else
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GameWorld, 0);
		PlayerController->ClientTravel(Url, TRAVEL_Relative, true);
	}


	// ULoadingParam* LoadingParam = NewObject<ULoadingParam>(UGameRoot::GetRoot());
	// LoadingParam->Progress = 0.f;
	// UEventSystem::SendEvent(UEventIdsBasic::LOADING_PROGRESS(), LoadingParam);
}

void ULevelSystem::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	LoadingMap = nullptr;
	CurrentMap = nullptr;
}

void ULevelSystem::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);

	// 当前场景的帧事件
	if (IsValid(CurrentMap))
	{
		CurrentMap->Tick(DeltaTime);
		CurrentMap->K2_Tick(DeltaTime);
	}


	// 判断场景有没有加载完
	if (!IsLoading()) { return; }
	UWorld* GameWorld = UGameRoot::GetRootWorld(this);
	if (!IsValid(GameWorld)) { return; }
	const UPackage* MapPackage = GameWorld->GetPackage();
	const FString MapName = MapPackage->GetLoadedPath().GetPackageFName().ToString();
	bool bSameName;
	if (LoadingMap->bByObjectReference)
	{
		bSameName = LoadingMap->Map.GetLongPackageName() == MapName;
	}
	else
	{
		bSameName = MapName.Contains(LoadingMap->MapName);
	}
	bool bAllRoutedActorLoaded = false;
	if (bSameName)
	{
		const TArray<ULevel*>& Levels = LoadingMap->Map->GetLevels();
		bAllRoutedActorLoaded = GameWorld->HasBegunPlay() && Levels.Num() > 0;
		// if (bAllRoutedActorLoaded)
		// {
		// 	for (const ULevel* Level : Levels)
		// 	{
		// 		if (Level->bIsVisible && Level->Actors.Num() > 0 && !Level->IsFinishedRouteActorInitialization())
		// 		{
		// 			bAllRoutedActorLoaded = false;
		// 			break;
		// 		}
		// 	}
		// }
	}


	if (bAllRoutedActorLoaded)
	{
		CurrentMap = LoadingMap;
		LoadingMap = nullptr;
		CurrentMap->CurrentMap = GameWorld;
		TArray<UObject*> Filters = UFilterStorage::GetLevelFilters(this);
		for (UObject* Filter : Filters)
		{
			IILevelFilter::Execute_OnMapLoaded(Filter, CurrentMap);
		}
		CurrentMap->OnMapEntered();
		UInputSystem::SetMouseType(this, CurrentMap->MouseType, true);

		// if (bAutoShowLoading)
		// {
		// 	ULoadingParam* LoadingParam = NewObject<ULoadingParam>(UGameRoot::GetRoot());
		// 	LoadingParam->Progress = 1.f;
		// 	UEventSystem::SendEvent(UEventIdsBasic::LOADING_PROGRESS(), LoadingParam);
		// }
	}
}
