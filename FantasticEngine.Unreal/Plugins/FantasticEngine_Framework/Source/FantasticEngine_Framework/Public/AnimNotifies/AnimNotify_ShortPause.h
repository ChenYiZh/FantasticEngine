// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ShortPause.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName="Short Pause")
class FANTASTICENGINE_FRAMEWORK_API UAnimNotify_ShortPause : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FTimerHandle TImerHandle;
	UPROPERTY()
	float OriginRateScale;
	UPROPERTY()
	UAnimSequenceBase* PlayingAnimation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimNotify)
	float Duration;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

public:
	UFUNCTION()
	void OnDelayCallback();
};
