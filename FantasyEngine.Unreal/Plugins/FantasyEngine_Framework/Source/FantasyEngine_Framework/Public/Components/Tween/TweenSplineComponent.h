// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenSplineComponent.generated.h"

/** 如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UTweenSplineComponent : public UTweenComponent
{
	GENERATED_BODY()

protected:
	virtual void Play_Implementation(USceneComponent* Component, float Alpha) override;

#pragma region Variables

public:
	/** 是否指同步坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bLocation = true;
	/** 是否指同步角度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bRotation = true;
	/** 是否指同步大小 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bScale = false;
	/** 路径坐标类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bUseWorldType = true;
	/** 场景中的Actor并且这个Actor必须包含 Spline Component */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Tween")
	AActor* SplineActor;
#pragma endregion
};
