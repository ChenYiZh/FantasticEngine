// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInfoPosterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostPlayerInfoDynamic);

DECLARE_MULTICAST_DELEGATE(FOnPostPlayerInfoNative)

UCLASS(BlueprintType, Blueprintable, ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UPlayerInfoPosterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInfoPosterComponent();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region 全局材质

public:
	/** 提交全局玩家信息到全局材质球 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller")
	void PostPlayerInfo();

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Post Player Info")
	FOnPostPlayerInfoDynamic K2_OnPostPlayerInfo;
	FOnPostPlayerInfoNative OnPostPlayerInfo;

private:
	/** 全局材质 */
	TWeakObjectPtr<UMaterialParameterCollection> MaterialParameterCollectionInstance = nullptr;
#pragma endregion
};
