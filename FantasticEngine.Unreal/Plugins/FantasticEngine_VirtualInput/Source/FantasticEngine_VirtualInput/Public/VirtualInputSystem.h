// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/InputDragHandle.h"
#include "Widgets/VirtualJoystickWidget.h"
#include "Basis/SystemBasis.h"
#include "VirtualInputSystem.generated.h"

USTRUCT(BlueprintType)
struct FANTASTICENGINE_VIRTUALINPUT_API FVirtualInputConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Input System")
	bool bMaskTouchDown = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Input System")
	float VerticalArea = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Input System")
	int HandleZOrder = -100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Input System")
	int DefaultDisplayZOrder = 50000;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASTICENGINE_VIRTUALINPUT_API UVirtualInputSystem : public USystemBasis
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FVirtualInputConfig Config;

protected:
	UPROPERTY(VisibleAnywhere)
	UInputDragHandle* Handle;
	UPROPERTY(VisibleAnywhere)
	TSet<UVirtualJoystickWidget*> Widgets;

public:
	UFUNCTION(BlueprintPure, Category="Input System")
	UInputDragHandle* GetHandle() const;
	UFUNCTION(BlueprintPure, Category="Input System")
	TSet<UVirtualJoystickWidget*> GetWidgets();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input System")
	bool bInvertGamePadRightY = false;

public:
	UFUNCTION(BlueprintCallable, Category="Input System")
	void InitInputSystem(const FVirtualInputConfig& InConfig);
	UFUNCTION(BlueprintCallable, Category="Input System")
	void CreateVirtualJoysticks();
	UFUNCTION(BlueprintCallable, Category="Input System")
	void RegistVirtualJoystickWidget(UVirtualJoystickWidget* VirtualJoystickWidget);
	// UFUNCTION(BlueprintCallable, Category="Input System")
	// void CreateVirtualJoysticksWithDisplay(TSubclassOf<UVirtualJoystickDisplay> VirtualJoystickDisplayClass);
	// UFUNCTION(BlueprintCallable, Category="Input System")
	// void AddVirtualJoysticksDisplay(TSubclassOf<UVirtualJoystickDisplay> VirtualJoystickDisplayClass);
	UFUNCTION(BlueprintCallable, Category="Input System")
	void RemoveAll();
	// UFUNCTION(BlueprintCallable, Category="Input System")
	// void SetVirtualJoysticksVisible(bool Visible);
};
