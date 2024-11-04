// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasticEngineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASTICENGINE_API AFantasticEngineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
