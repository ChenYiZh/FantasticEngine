// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RayTracedComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComponentRayTraced, AActor*, ActorFrom,
                                               UPlayerRayTraceComponent*,
                                               ComponentFrom, UPrimitiveComponent*, RayTracedComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorRayTraced, AActor*, ActorFrom, UPlayerRayTraceComponent*,
                                             ComponentFrom);

/** 必须在UPrimitiveComponent类型下，配合 PlayerRayTraceComponent 使用 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, ComponentTick, Activation, Cooking, Physics, LOD, Collision),
	ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API URayTracedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URayTracedComponent();

public:
	/** 当组件被检测到 */
	UPROPERTY(BlueprintAssignable)
	FOnComponentRayTraced OnBeginRayTraceComponent;

	/** 当组件离开射线 */
	UPROPERTY(BlueprintAssignable)
	FOnComponentRayTraced OnEndRayTraceComponent;

	/** 当Actor被检测到 */
	UPROPERTY(BlueprintAssignable)
	FOnActorRayTraced OnBeginRayTraceActor;

	/** 当Actor离开射线 */
	UPROPERTY(BlueprintAssignable)
	FOnActorRayTraced OnEndRayTraceActor;
};
