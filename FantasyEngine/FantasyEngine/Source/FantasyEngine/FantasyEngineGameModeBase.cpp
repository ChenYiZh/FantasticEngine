// Copyright Epic Games, Inc. All Rights Reserved.


#include "FantasyEngineGameModeBase.h"

#include "test.h"

void AFantasyEngineGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	fantasy_engine::test::hello();
}
