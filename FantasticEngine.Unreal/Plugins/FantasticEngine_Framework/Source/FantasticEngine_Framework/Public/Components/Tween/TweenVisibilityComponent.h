// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Tween/TweenComponent.h"
#include "TweenVisibilityComponent.generated.h"

/** 如果有父节点则调节父节点，不然调节当前节点 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, Cooking, Physics, LOD, Collision),
	ClassGroup=(Tween), meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTweenVisibilityComponent : public UTweenComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PlayOnFinished();

#pragma region Variables

public:
	/** 是否在流程结束后设置为可见 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bVisibleOnFinished = false;

#pragma endregion
};
