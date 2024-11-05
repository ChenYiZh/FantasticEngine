// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "WorldLight.generated.h"

UCLASS()
class FANTASTICENGINE_FRAMEWORK_API AWorldLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bInited = false;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialParameterCollection> MPC_Framework = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* ProjectionCapture;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseCustomLight = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor LightColor = FColor::White;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LightCaptureDistance = 100000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float ShadowIntensity = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	FVector4 ShadowColorHSV = FVector4(0.f, 1.f, 0.f, 1.f);
	/**
	 * 显示的阴影距离（米）
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShadowDistance = 150.f;
	/**
	 * 阴影过度距离
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShadowRange = 10.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShadowBias = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float ShadowProjectionIntensity = 0.3f;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshCapture();

	UFUNCTION(BlueprintCallable)
	void PostLightInfo();

	UFUNCTION(BlueprintCallable)
	void PostProjectionInfo();

	UFUNCTION(BlueprintCallable)
	void PostViewMatrix();

	UFUNCTION(BlueprintCallable)
	void PostOrthoMatrix();
};
