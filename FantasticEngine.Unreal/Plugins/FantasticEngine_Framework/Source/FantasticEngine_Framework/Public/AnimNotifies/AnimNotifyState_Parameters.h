// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Defines/TransparentParameters.h"
#include "UObject/Object.h"
#include "AnimNotifyState_Parameters.generated.h"

/**
 * 透传参数
 */
UCLASS(editinlinenew, const, hidecategories = Object, DisplayName="Parameters State")
class FANTASTICENGINE_FRAMEWORK_API UAnimNotifyState_Parameters : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()
public:
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif
protected:
	// Name of notify that is passed to ability.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	FName NotifyName;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notify")
	FTransparentParameters Parameters;
};
