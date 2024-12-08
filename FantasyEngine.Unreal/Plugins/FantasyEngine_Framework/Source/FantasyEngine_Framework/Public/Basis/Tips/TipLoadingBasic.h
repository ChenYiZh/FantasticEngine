// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TipView.h"
#include "Systems/EventSystem.h"
#include "UObject/NoExportTypes.h"
#include "TipLoadingBasic.generated.h"

UCLASS(BlueprintType)
class FANTASYENGINE_FRAMEWORK_API ULoadingParam : public UEventParam
{
	GENERATED_BODY()
public:
	float Progress;
	UPROPERTY()
	FString Message;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FANTASYENGINE_FRAMEWORK_API UTipLoadingBasic : public UTipView
{
	GENERATED_BODY()
public:
	virtual void OnInitialize_Implementation() override;
	virtual void OnQuit_Implementation() override;

public:
	UFUNCTION()
	void ShowListener();
	UFUNCTION()
	void ProgressListener(UEventParam* Param);
	UFUNCTION()
	void HideListener();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnProgress(float Progress,const FString& Message);
	virtual void OnProgress_Implementation(float Progress,const FString& Message);
	
};
