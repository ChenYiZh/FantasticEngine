// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTriggerOnTickTracked, AActor*, OtherActor, UPrimitiveComponent*,
                                             OtherComp);

UCLASS(ClassGroup="FantasticEngine", hidecategories=(Object,LOD,Lighting,TextureStreaming), editinlinenew,
	meta=( BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UTriggerOnTick : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerOnTick();

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UPrimitiveComponent* OverlapChecker;

public:
	UPrimitiveComponent* GetOverlapChecker() const { return OverlapChecker; }

private:
	UPROPERTY()
	TArray<UPrimitiveComponent*> LastOverlapping;

public:
	/** 当有碰撞时执行 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnTriggerOnTickTracked OnBeginOverlap;

	/** 当有碰撞丢失时执行 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnTriggerOnTickTracked OnEndOverlap;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
