// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DisplayName="Camera Shake")
class FANTASYENGINE_FRAMEWORK_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> Shake;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InnerRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OuterRadius = 10000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Falloff = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bOrientShakeTowardsEpicenter = true;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
