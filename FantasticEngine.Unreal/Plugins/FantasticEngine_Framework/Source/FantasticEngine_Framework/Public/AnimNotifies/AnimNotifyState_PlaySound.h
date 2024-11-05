// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PlaySound.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DisplayName="Timed Play Sound")
class FANTASTICENGINE_FRAMEWORK_API UAnimNotifyState_PlaySound : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;
#endif
public:
	// Sound to Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	TObjectPtr<USoundBase> Sound;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float VolumeMultiplier = 1.f;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(ExposeOnSpawn = true))
	float PitchMultiplier = 1.f;

	// If this sound should follow its owner
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	// uint32 bFollow:1;

	// #if WITH_EDITORONLY_DATA
	// 	UPROPERTY(Config, EditAnywhere, Category = "AnimNotify")
	// 	uint32 bPreviewIgnoreAttenuation:1;
	// #endif

	// Socket or bone name to attach sound to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify",
		meta=(EditCondition="bFollow", ExposeOnSpawn = true))
	FName AttachName = FName("root");

	UPROPERTY()
	UAudioComponent* AudioComponent;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
};
