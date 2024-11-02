// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlayerTriggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerTriggerTracked);

UCLASS(ClassGroup="FantasyEngine", hidecategories=(Object,LOD,Lighting,TextureStreaming), editinlinenew,
	meta=( BlueprintSpawnableComponent))
class FANTASYENGINE_FRAMEWORK_API UPlayerTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	/** 设置默认玩家数量，0代表所有在线玩家 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Trigger")
	int32 PlayerCount = 0;

public:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:
	/** 只有当前用户进入才触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnOwnerEnter;

	/** 任何用户都能触发，只在服务器上触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnAnyOneEnter;

	/** 必须所有人都进入才可以触发，只在服务器上触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnEveryOneEnter;

	/** 只有当前用户离开才触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnOwnerExit;

	/** 任何用户离开都能触发，只在服务器上触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnAnyOneExit;

	/** 必须所有人都离开才可以触发，只在服务器上触发 */
	UPROPERTY(BlueprintAssignable, Category="Player Trigger")
	FOnPlayerTriggerTracked OnEveryOneExit;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/** 进入的玩家数量 */
	UPROPERTY()
	int32 OverlappedCount = 0;

public:
	/** 是否还有玩家在包裹框里 */
	UFUNCTION(BlueprintPure, Category="Player Trigger")
	bool HasAnyOne() const { return OverlappedCount > 0; }

	/** 是否所有玩家在包裹框里 */
	UFUNCTION(BlueprintPure, Category="Player Trigger")
	bool AllIn() const;

	/** 是否没有玩家了 */
	UFUNCTION(BlueprintPure, Category="Player Trigger")
	bool NoOne() const { return OverlappedCount == 0; }
};
