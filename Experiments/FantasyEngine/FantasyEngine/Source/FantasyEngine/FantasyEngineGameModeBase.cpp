// Copyright Epic Games, Inc. All Rights Reserved.


#include "FantasyEngineGameModeBase.h"

#include "test.h"

void AFantasyEngineGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::FromInt(fantasy_engine::Test::Add(3, 10)));
}
