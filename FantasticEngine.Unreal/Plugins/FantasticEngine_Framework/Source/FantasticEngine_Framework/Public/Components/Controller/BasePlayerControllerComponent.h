// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePlayerControllerComponent.generated.h"


UCLASS(Blueprintable, NotBlueprintType, Abstract, ClassGroup="FantasticEngine",
	meta=(BlueprintSpawnableComponent))
class FANTASTICENGINE_FRAMEWORK_API UBasePlayerControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBasePlayerControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region 操作绑定

private:
	/** 绑定操作 */
	void BindInput(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines);

protected:
	/** 绑定操作 */
	virtual void BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines);

	/** 绑定操作 */
	UFUNCTION(BlueprintImplementableEvent, Category="Contoller|Actions", DisplayName="Bind Input Defines")
	void K2_BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines);
#pragma endregion

#pragma region Variables

private:
	/** 输入映射 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess), Category="Fantastic Engine|Configs|Input", DisplayName="Input Defines")
	TSubclassOf<UInputDefines> InputDefinesClass;

	/** 输入映射实例 */
	UPROPERTY()
	UInputDefines* InputDefinesInstance;

public:
	/** 输入映射 */
	UFUNCTION(BlueprintPure, Category="Fantastic Engine|Configs|Input", DisplayName="Input Defines",
		meta=(DeterminesOutputType="OutClass"))
	UInputDefines* GetInputDefines(TSubclassOf<UInputDefines> OutClass = nullptr) const { return InputDefinesInstance; }

	/** 输入映射 */
	template <typename T>
	T* GetInputDefines() const { return Cast<T>(GetInputDefines()); }

public:
	UFUNCTION(BlueprintPure, Category="Controller", DisplayName="Player Controller")
	APlayerController* GetPlayerController() const;
	UFUNCTION(BlueprintPure, Category="Controller", DisplayName="Player Controller")
	APawn* GetPawn() const;
#pragma endregion
};
