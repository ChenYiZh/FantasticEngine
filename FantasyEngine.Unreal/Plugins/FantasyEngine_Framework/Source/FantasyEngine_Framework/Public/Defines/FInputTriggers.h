// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "UObject/Object.h"
#include "FInputTriggers.generated.h"

/**
 *当按键未使用时执行
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Untapped", NotInputConfigurable = "true"))
class UInputTriggerUntapped final : public UInputTriggerChordAction
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

protected:
	virtual ETriggerEventsSupported GetSupportedTriggerEvents() const override { return ETriggerEventsSupported::Instant; }
	// Implicit, so action cannot fire unless this is firing.
	virtual ETriggerType GetTriggerType_Implementation() const override { return ETriggerType::Explicit; }

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
// public:
//
// 	// The action that must be triggering for this trigger's action to trigger
// 	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false"))
// 	TObjectPtr<const UInputAction> ChordAction = nullptr;
};
