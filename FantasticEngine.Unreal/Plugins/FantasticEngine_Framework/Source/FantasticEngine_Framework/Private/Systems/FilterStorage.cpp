// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/FilterStorage.h"

#include "GameRoot.h"

void UFilterStorage::AddWidgetFilter(UObject* Filter)
{
	if (IsValid(Filter) && Filter->Implements<UIWidgetFilter>())
	{
		Filter->Rename(ToCStr(Filter->GetName()), UGameRoot::GetRoot(Filter));
		Singleton<UFilterStorage>(Filter)->WidgetFilters.Add(Filter);
	}
}

void UFilterStorage::RemoveWidgetFilter(UObject* Filter)
{
	Singleton<UFilterStorage>(Filter)->WidgetFilters.Remove(Filter);
}

TArray<UObject*> UFilterStorage::GetWidgetFilters(const UObject* WorldContextObject)
{
	return Singleton<UFilterStorage>(WorldContextObject)->WidgetFilters;
}

void UFilterStorage::AddLevelFilter(UObject* Filter)
{
	if (IsValid(Filter) && Filter->Implements<UILevelFilter>())
	{
		Filter->Rename(ToCStr(Filter->GetName()), UGameRoot::GetRoot(Filter));
		Singleton<UFilterStorage>(Filter)->LevelFilters.Add(Filter);
	}
}

void UFilterStorage::RemoveLevelFilter(UObject* Filter)
{
	Singleton<UFilterStorage>(Filter)->LevelFilters.Remove(Filter);
}

TArray<UObject*> UFilterStorage::GetLevelFilters(const UObject* WorldContextObject)
{
	return Singleton<UFilterStorage>(WorldContextObject)->LevelFilters;
}
