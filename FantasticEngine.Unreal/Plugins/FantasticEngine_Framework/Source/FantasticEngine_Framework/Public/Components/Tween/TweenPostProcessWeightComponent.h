// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PostProcessComponent.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenPostProcessWeightComponent.generated.h"

/** Tween 动画基类，如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTweenPostProcessWeightComponent : public UTweenComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Varitables")
	TWeakObjectPtr<UPostProcessComponent> PostProcess;

protected:
	virtual void Play_Implementation(USceneComponent* Component, float Alpha) override;
};
