// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyEngineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYENGINE_API AFantasyEngineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
