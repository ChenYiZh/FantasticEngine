// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenMaterialsScalarComponent.generated.h"

/** Tween 动画基类，如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UTweenMaterialsScalarComponent : public UTweenComponent
{
	GENERATED_BODY()

public:
	virtual void Play_Implementation(USceneComponent* Component, float Alpha) override;

public:
	/** 材质变量名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	FName ParameterName;
	/** 需要改变的材质 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	TArray<UMaterialInstanceDynamic*> Materials;

	/** 起始坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	float From = 0.f;
	/** 终点坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	float To = 1.f;
};
