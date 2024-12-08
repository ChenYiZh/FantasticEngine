// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenFloatComponent.generated.h"

/** Tween 动画基类，如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UTweenFloatComponent : public UTweenComponent
{
	GENERATED_BODY()

public:
	virtual void Play(float Alpha) override;

public:
	/** 起始坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	float From = 0.f;
	/** 终点坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	float To = 1.f;
};
