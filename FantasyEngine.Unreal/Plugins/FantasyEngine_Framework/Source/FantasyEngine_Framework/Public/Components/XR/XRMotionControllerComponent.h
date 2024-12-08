// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "XRCameraComponent.h"
#include "XRMotionControllerComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = "Fantasy Engine")
class FANTASYENGINE_FRAMEWORK_API UXRMotionControllerComponent : public UMotionControllerComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;

private:
	UPROPERTY(BlueprintReadWrite, Category="Variables", meta=(AllowPrivateAccess="true"))
	TWeakObjectPtr<UXRCameraComponent> XRCamera;

public:
	UFUNCTION(BlueprintPure, DisplayName="XR Camera")
	UXRCameraComponent* GetXRCamera() const { return XRCamera.IsValid() ? XRCamera.Get() : nullptr; }

private:
	UPROPERTY()
	FTransform LastTransform;
};
