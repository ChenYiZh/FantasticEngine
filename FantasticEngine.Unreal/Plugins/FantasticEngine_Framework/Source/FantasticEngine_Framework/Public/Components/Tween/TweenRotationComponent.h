// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenRotationComponent.generated.h"

/** 如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTweenRotationComponent : public UTweenComponent
{
	GENERATED_BODY()

protected:
	virtual void Play_Implementation(USceneComponent* Component, float Alpha) override;

#pragma region Variables

public:
	/** 是否使用世界坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bGlobal = false;
	/** 是否使用角度运算Lerp */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bUseRotatorLerp = false;
	/** 起始坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	FVector From;
	/** 终点坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	FVector To = FVector(0.f, 0.f, 360.f);

#pragma endregion
};
