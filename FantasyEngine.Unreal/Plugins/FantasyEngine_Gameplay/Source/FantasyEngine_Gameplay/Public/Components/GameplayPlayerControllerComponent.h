// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCharacterComponent.h"
#include "Components/Controller/AdvancedPlayerControllerComponent.h"
#include "Components/Controller/PrimitivePlayerControllerComponent.h"
#include "GameplayPlayerControllerComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_GAMEPLAY_API UGameplayPlayerControllerComponent : public UAdvancedPlayerControllerComponent
{
	GENERATED_BODY()

#pragma region 操作管理

public:
	/**
	 * 左上键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowUpPressed(const FInputActionInstance& Instance);
	/**
	 * 左上键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowUpReleased(const FInputActionInstance& Instance);
	/**
	 * 左下键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowDownPressed(const FInputActionInstance& Instance);
	/**
	 * 左下键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowDownReleased(const FInputActionInstance& Instance);
	/**
	 * 左左键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowLeftPressed(const FInputActionInstance& Instance);
	/**
	 * 左左键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowLeftReleased(const FInputActionInstance& Instance);
	/**
	 * 左右键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowRightPressed(const FInputActionInstance& Instance);
	/**
	 * 左右键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnArrowRightReleased(const FInputActionInstance& Instance);

	/**
	 * 右上键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionUpPressed(const FInputActionInstance& Instance);
	/**
	 * 右上键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionUpReleased(const FInputActionInstance& Instance);
	/**
	 * 右下键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionDownPressed(const FInputActionInstance& Instance);
	/**
	 * 右下键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionDownReleased(const FInputActionInstance& Instance);
	/**
	 * 右左键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionLeftPressed(const FInputActionInstance& Instance);
	/**
	 * 右左键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionLeftReleased(const FInputActionInstance& Instance);
	/**
	 * 右右键按下操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionRightPressed(const FInputActionInstance& Instance);
	/**
	 * 右右键松开操作
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Controller|Actions")
	void OnActionRightReleased(const FInputActionInstance& Instance);
#pragma endregion

protected:
	virtual void BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines) override;

	UFUNCTION(BlueprintPure, Category="Controller")
	void GetGameplayCharacterComponents(TArray<UGameplayCharacterComponent*>& GameplayCharacters) const;
};
