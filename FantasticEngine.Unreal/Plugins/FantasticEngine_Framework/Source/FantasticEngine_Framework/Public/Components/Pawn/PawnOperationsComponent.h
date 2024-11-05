// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicOperationsComponent.h"
#include "PawnOperationsComponent.generated.h"

/** 角色控制组件 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UPawnOperationsComponent : public UBasicOperationsComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnOperationsComponent();

#pragma region 操作管理

public:
	/** 视角拉近拉远操作 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Actions")
	void ViewClose(float Value);
#pragma endregion

#pragma region Variables

public:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Variables|Camera", meta=(DisplayPriority=-1))
	FVector2D ViewLengthRange = FVector2D(300, 1500);

public:
	UPROPERTY(BlueprintAssignable, DisplayName="On View Close")
	FOnReceiveDynamic K2_OnViewClose;
	FOnReceiveNative OnViewClose;

#pragma region 默认组件

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables|Camera",
		meta = (DisplayPriority=-1, AllowPrivateAccess = "true"))
	TWeakObjectPtr<class USpringArmComponent> CameraBoom;

protected:


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom.IsValid() ? CameraBoom.Get() : nullptr;
	}
#pragma endregion
#pragma endregion
};
