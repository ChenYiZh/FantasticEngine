// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerRayTraceComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerRayTraceType:uint8
{
	TraceChannel,
	ObjectTypes,
	Profile,
};

USTRUCT()
struct FPlayerRayTracedComponent
{
	GENERATED_BODY()

public:
	/** 记录的对象 */
	UPROPERTY()
	UPrimitiveComponent* Component;

	/** 用于判断这个对象有没有被检测过 */
	UPROPERTY()
	bool bChecked = false;
};

USTRUCT()
struct FPlayerRayTracedActor
{
	GENERATED_BODY()

public:
	/** 记录的对象 */
	UPROPERTY()
	AActor* Actor;

	/** 用于判断这个对象有没有被检测过 */
	UPROPERTY()
	bool bChecked = false;

	/** 记录的对象 */
	UPROPERTY()
	TMap<UPrimitiveComponent*, FPlayerRayTracedComponent> Components;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerRayTraceComponent, AActor*, RayTracedActor, UPrimitiveComponent*,
                                             RayTracedComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRayTraceActor, AActor*, RayTracedActor);

/** 玩家用于射线检测的工具，配合 RayTracedComponent 使用 */
UCLASS(Blueprintable, BlueprintType,
	HideCategories=(Mobility, Sockets, ComponentTick, Activation, Cooking, Physics, LOD, Collision),ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UPlayerRayTraceComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerRayTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** 开始检测前执行 */
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginRayTracing();

	/** 检测到对象时执行 */
	UFUNCTION(BlueprintNativeEvent)
	void OnRayTraced(FHitResult& HitResult);

	/** 检测完成后执行 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEndRayTracing();

public:
	/** 是否启用这个射线检测 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;

	/** 射线检测的距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float TraceLength = 10000.f;

	/** 是否检测多个物体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bMultiRayTrace = false;

	/** 射线检测类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	EPlayerRayTraceType TraceType = EPlayerRayTraceType::TraceChannel;

	/** 射线检测的 Channel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config",
		meta=(EditCondition="TraceType == EPlayerRayTraceType::TraceChannel", EditConditionHides))
	TEnumAsByte<ECollisionChannel> TraceChannel;

	/** 射线检测的 ObjectTypes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config",
		meta=(EditCondition="TraceType == EPlayerRayTraceType::ObjectTypes", EditConditionHides))
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	/** 射线检测的 Collision Preset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config",
		meta=(EditCondition="TraceType == EPlayerRayTraceType::Profile", EditConditionHides))
	FName ProfileName;

public:
	/** 当组件被检测到 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRayTraceComponent OnBeginRayTraceComponent;

	/** 当组件离开射线 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRayTraceComponent OnEndRayTraceComponent;

	/** 当Actor被检测到 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRayTraceActor OnBeginRayTraceActor;

	/** 当Actor离开射线 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerRayTraceActor OnEndRayTraceActor;

private:
	/** 记录的检测对象 */
	UPROPERTY()
	TMap<AActor*, FPlayerRayTracedActor> TracedActors;
};
