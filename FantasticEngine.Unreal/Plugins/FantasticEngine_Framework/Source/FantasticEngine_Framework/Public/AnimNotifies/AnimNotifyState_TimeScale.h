// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_TimeScale.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, DisplayName="Time Scale")
class FANTASTICENGINE_FRAMEWORK_API UAnimNotifyState_TimeScale : public UAnimNotifyState
{
	GENERATED_BODY()
// private:
// 	float InScale;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 1.f;

public:
	//UAnimNotifyState_TimeScale();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
};
