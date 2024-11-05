// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenActorComponent.generated.h"

/** 如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTweenActorComponent : public UTweenComponent
{
	GENERATED_BODY()

protected:
	virtual void Play_Implementation(USceneComponent* Component, float Alpha) override;

#pragma region Variables

public:
	/** 起始坐标 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Tween")
	AActor* From;
	/** 终点坐标 */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Tween")
	AActor* To;

	/** 是否同步坐标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bSyncLocation = true;
	/** 是否同步角度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bSyncRotation = true;
	/** 是否同步大小 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bSyncScale = false;
#pragma endregion
};
