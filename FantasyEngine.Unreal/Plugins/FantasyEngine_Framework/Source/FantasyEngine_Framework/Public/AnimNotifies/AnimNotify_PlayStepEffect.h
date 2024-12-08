// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayStepEffect.generated.h"

UCLASS(Blueprintable, BlueprintType, DisplayName="Play Step Effect Handle")
class UAnimNotify_PlayStepEffect_Handle : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnStep(USkeletalMeshComponent* MeshComp, bool IsLeft, EPhysicalSurface SurfaceType);
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DisplayName="Play Step Effect")
class FANTASYENGINE_FRAMEWORK_API UAnimNotify_PlayStepEffect : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	bool IsLeftFoot;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TSubclassOf<UAnimNotify_PlayStepEffect_Handle> HandleClass;
	UPROPERTY()
	UAnimNotify_PlayStepEffect_Handle* Handle;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
