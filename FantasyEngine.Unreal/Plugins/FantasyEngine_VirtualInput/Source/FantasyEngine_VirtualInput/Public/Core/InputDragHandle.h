// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputDragHandle.generated.h"

USTRUCT(BlueprintType)
struct FInputTouch
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int TouchIndex;
	UPROPERTY(BlueprintReadOnly)
	bool bIsLeft;
	UPROPERTY(BlueprintReadOnly)
	bool bIsValid;

	UPROPERTY(BlueprintReadOnly)
	FVector2D StartScreenPosition;
	UPROPERTY(BlueprintReadOnly)
	FVector2D CurrentScreenPosition;
	UPROPERTY(BlueprintReadOnly)
	FVector2D DeltaScreenPosition;

	// FVector2d StartViewportPosition;
	// FVector2d CurrentViewportPosition;
	// FVector2d DeltaViewportPosition;

	// FVector2d StartDesignPosition;
	// FVector2d CurrentDesignPosition;
	// FVector2d DeltaDesignPosition;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FANTASYENGINE_VIRTUALINPUT_API UInputDragHandle : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FTouchDelegate, const FInputTouch&)

public:
	FTouchDelegate OnTouchDown;
	FTouchDelegate OnTouchMove;
	FTouchDelegate OnTouchUp;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin=0.f, ClampMax=1.f))
	float LeftVerticalArea = 1.f;

protected:
	UPROPERTY()
	TMap<int, FInputTouch> Touchs;

	void AddInputTouch(const int& TouchIndex,const FVector2D& ScreenPosition);
	
	void OnTouchEnd(const int& TouchIndex);

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void InitializeInputComponent() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	//virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	// virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//
	// virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//
	// virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void OnTouchMoved(const ETouchIndex::Type FingerIndex, const FVector ScreenPosition);

	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector ScreenPosition);
};
