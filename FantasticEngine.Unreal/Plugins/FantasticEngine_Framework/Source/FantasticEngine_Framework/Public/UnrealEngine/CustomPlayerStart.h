// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CustomPlayerStart.generated.h"

/**
 * 起始位置在脚底
 */
UCLASS(Blueprintable, ClassGroup="FantasticEngine", hidecategories=Collision)
class FANTASTICENGINE_FRAMEWORK_API ACustomPlayerStart : public APlayerStart
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> DefaultSceneRoot;
	
public:
	ACustomPlayerStart(const FObjectInitializer& ObjectInitializer);

	virtual void Validate() override;

public:
	/** Returns DefaultSceneRoot subobject **/
	class USceneComponent* GetDefaultSceneRoot() const { return DefaultSceneRoot; }
};
