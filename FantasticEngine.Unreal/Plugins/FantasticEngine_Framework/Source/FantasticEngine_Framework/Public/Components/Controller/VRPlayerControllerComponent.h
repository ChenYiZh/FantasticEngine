// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Controller/PrimitivePlayerControllerComponent.h"
#include "Components/Pawn/VRPawnOperationsComponent.h"
#include "VRPlayerControllerComponent.generated.h"

/** VR Player Controller */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasticEngine", meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UVRPlayerControllerComponent : public UBasePlayerControllerComponent
{
	GENERATED_BODY()
#pragma region 输入管理

public:
	/** 是否识别手势 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fantastic Engine|Configs|Input", DisplayName="Use Hands Tracker")
	bool bUseHandsTracker = true;
#pragma endregion

protected:
	virtual void BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines) override;

protected:
	UFUNCTION(BlueprintPure, Category="Controller")
	void GetPawnVROperationsComponents(TArray<UVRPawnOperationsComponent*>& OperationsComponents) const;

#pragma region 操作管理

public:
#pragma region 手柄
	/** VRTurn按键 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnVRTurn(const FInputActionInstance& Instance);

	/** VRMove */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnVRMoveStarted(const FInputActionInstance& Instance);
	/** VRMove */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnVRMoveTriggered(const FInputActionInstance& Instance);
	/** VRMove */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnVRMoveCompleted(const FInputActionInstance& Instance);

	/** Left Grab */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnGrabLeftStarted(const FInputActionInstance& Instance);
	/** Left Grab */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnGrabLeftCompleted(const FInputActionInstance& Instance);

	/** Right Grab */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnGrabRightStarted(const FInputActionInstance& Instance);
	/** Right Grab */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnGrabRightCompleted(const FInputActionInstance& Instance);

	/** Left Menu */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnMenuToggleLeft(const FInputActionInstance& Instance);
	/** Right Menu */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnMenuToggleRight(const FInputActionInstance& Instance);
#pragma endregion

#pragma region 手势
	/** Left Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspLeftTriggered(const FInputActionInstance& Instance);
	/** Left Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspLeftCanceled(const FInputActionInstance& Instance);
	/** Left Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspLeftCompleted(const FInputActionInstance& Instance);

	/** Right Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspRightTriggered(const FInputActionInstance& Instance);
	/** Right Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspRightCanceled(const FInputActionInstance& Instance);
	/** Right Hand Grasp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandGraspRightCompleted(const FInputActionInstance& Instance);


	/** Left Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlLeftTriggered(const FInputActionInstance& Instance);
	/** Left Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlLeftCanceled(const FInputActionInstance& Instance);
	/** Left Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlLeftCompleted(const FInputActionInstance& Instance);

	/** Right Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlRightTriggered(const FInputActionInstance& Instance);
	/** Right Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlRightCanceled(const FInputActionInstance& Instance);
	/** Right Hand IndexCurl */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandIndexCurlRightCompleted(const FInputActionInstance& Instance);


	/** Left Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointLeftStarted(const FInputActionInstance& Instance);
	/** Left Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointLeftCanceled(const FInputActionInstance& Instance);
	/** Left Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointLeftCompleted(const FInputActionInstance& Instance);

	/** Right Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointRightStarted(const FInputActionInstance& Instance);
	/** Right Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointRightCanceled(const FInputActionInstance& Instance);
	/** Right Hand Point */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandPointRightCompleted(const FInputActionInstance& Instance);


	/** Left Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpLeftStarted(const FInputActionInstance& Instance);
	/** Left Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpLeftCanceled(const FInputActionInstance& Instance);
	/** Left Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpLeftCompleted(const FInputActionInstance& Instance);

	/** Right Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpRightStarted(const FInputActionInstance& Instance);
	/** Right Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpRightCanceled(const FInputActionInstance& Instance);
	/** Right Hand ThumbUp */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnHandThumbUpRightCompleted(const FInputActionInstance& Instance);
#pragma endregion
};
