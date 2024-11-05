// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defines/TransparentParameters.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UObject/Object.h"
#include "AnimNotify_Parameters.generated.h"

/**
 * 参数透传
 */
UCLASS(editinlinenew, const, hidecategories = Object, DisplayName="Parameters")
class FANTASTICENGINE_FRAMEWORK_API UAnimNotify_Parameters : public UAnimNotify
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif
protected:
	// Name of notify that is passed to the PlayMontage K2Node.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	FName NotifyName;
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notify")
	FTransparentParameters Parameters;
};
