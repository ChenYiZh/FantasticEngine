// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FEGameMode.generated.h"

/**
 * 
 */
UCLASS(config = Game, notplaceable, BlueprintType, Blueprintable, Transient,
	hideCategories = (Info, Rendering, MovementReplication, Replication, Actor),
	meta = (ShortTooltip =
		"Game Mode Base defines the game being played, its rules, scoring, and other facets of the game type."))
class FANTASTICENGINE_FRAMEWORK_API AFEGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	/** 生成 Pawn 时，调节的高度。如果需要放在地面上，则使用 - PlayerStart.HalfHeight */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configs", meta=(AllowPrivateAccess="true"))
	float DeltaHeight = 0;

public:
	FORCEINLINE float GetDeltaHeight() const { return DeltaHeight; }


protected:
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
};
