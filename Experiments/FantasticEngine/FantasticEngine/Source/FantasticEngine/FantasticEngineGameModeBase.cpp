// Copyright Epic Games, Inc. All Rights Reserved.


#include "FantasticEngineGameModeBase.h"

#include "test.h"

void AFantasticEngineGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::FromInt(fantasy_engine::Test::Add(3, 10)));
}
