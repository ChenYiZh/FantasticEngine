// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/InputDragHandle.h"

#include "VirtualInputSystem.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/GraphicUtility.h"

// void UInputDragHandle::AddInputTouch(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
// {
// 	int PointIndex = InGestureEvent.GetPointerIndex();
// 	if (Touchs.Contains(PointIndex))
// 	{
// 		return;
// 	}
// 	FVector2d ScreenPosition = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
// 	FVector2d ScreenSize = UGraphicUtility::GetScreenSize();
// 	FInputTouch InputTouch;
// 	InputTouch.TouchIndex = PointIndex;
// 	InputTouch.bIsLeft = ScreenPosition.X < ScreenSize.X / 2.0f
// 		&& ScreenPosition.Y >= (1 - LeftVerticalArea) * ScreenSize.Y;
// 	if (InputTouch.bIsLeft)
// 	{
// 		bool IsValid = true;
// 		for (auto KV : Touchs)
// 		{
// 			if (KV.Value.bIsLeft)
// 			{
// 				IsValid = false;
// 			}
// 		}
// 		InputTouch.bIsValid = IsValid;
// 	}
// 	else
// 	{
// 		InputTouch.bIsValid = true;
// 	}
//
// 	InputTouch.StartScreenPosition = ScreenPosition;
// 	// InputTouch.StartViewportPosition = ScreenPosition * UGraphicUtility::GetViewportScale();
// 	// InputTouch.StartDesignPosition =
// 	// 	UGraphicUtility::ViewportPositionToDesignPosition(InputTouch.StartViewportPosition);
//
// 	InputTouch.CurrentScreenPosition = ScreenPosition;
//
// 	Touchs.Add(PointIndex, InputTouch);
// 	OnTouchDown.Broadcast(InputTouch);
//}
void UInputDragHandle::AddInputTouch(const int& TouchIndex, const FVector2D& ScreenPosition)
{
	//int PointIndex = InGestureEvent.GetPointerIndex();
	if (Touchs.Contains(TouchIndex))
	{
		return;
	}
	FVector2d ScreenSize = UGraphicUtility::GetScreenSize(this);
	FInputTouch InputTouch;
	InputTouch.TouchIndex = TouchIndex;
	InputTouch.bIsLeft = ScreenPosition.X < ScreenSize.X / 2.0f
		&& ScreenPosition.Y >= (1 - LeftVerticalArea) * ScreenSize.Y;
	if (InputTouch.bIsLeft)
	{
		bool IsValid = true;
		for (auto KV : Touchs)
		{
			if (KV.Value.bIsLeft)
			{
				IsValid = false;
			}
		}
		InputTouch.bIsValid = IsValid;
	}
	else
	{
		InputTouch.bIsValid = true;
	}

	InputTouch.StartScreenPosition = ScreenPosition;
	// InputTouch.StartViewportPosition = ScreenPosition * UGraphicUtility::GetViewportScale();
	// InputTouch.StartDesignPosition =
	// 	UGraphicUtility::ViewportPositionToDesignPosition(InputTouch.StartViewportPosition);

	InputTouch.CurrentScreenPosition = ScreenPosition;

	Touchs.Add(TouchIndex, InputTouch);
	OnTouchDown.Broadcast(InputTouch);
}

void UInputDragHandle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetRootWidget() == nullptr)
	{
		UImage* Root = WidgetTree->ConstructWidget<UImage>();
		WidgetTree->RootWidget = Root;
		Root->SetBrushTintColor(FColor::White);
		Root->SetColorAndOpacity(FColor::Transparent);
		Root->SetVisibility(ESlateVisibility::Visible);
		Root->Modify();
	}
}

void UInputDragHandle::NativeConstruct()
{
	Super::NativeConstruct();
	auto Root = GetRootWidget();
	if (Root != nullptr)
	{
		auto Target = Cast<UImage>(Root);
		if (Target != nullptr)
		{
			Target->SetColorAndOpacity(FColor::Transparent);
		}
	}
	if (InputComponent == nullptr)
	{
		InitializeInputComponent();
	}
}

void UInputDragHandle::InitializeInputComponent()
{
	SetInputActionBlocking(false);
	//bStopAction = false;
	//Priority = 1000;
	Super::InitializeInputComponent();
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &UInputDragHandle::OnTouchMoved);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &UInputDragHandle::OnTouchReleased);
}

// FReply UInputDragHandle::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
// {
// 	if (InGestureEvent.IsTouchEvent())
// 	{
// 		if (!Touchs.Contains(InGestureEvent.GetPointerIndex()))
// 		{
// 			AddInputTouch(InGeometry, InGestureEvent);
// 		}
// 		auto InputTouch = &Touchs[InGestureEvent.GetPointerIndex()];
//
// 		FVector2D ScreenPosition = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
// 		//FVector2d ViewportPosition = ScreenPosition * UGraphicUtility::GetViewportScale();
// 		//FVector2d DesignPosition = UGraphicUtility::ViewportPositionToDesignPosition(ViewportPosition);
//
// 		FVector2D LastScreenPosition = InputTouch->CurrentScreenPosition;
// 		//FVector2d LastViewportPosition = InputTouch->CurrentViewportPosition;
// 		//FVector2d LastDesignPosition = InputTouch->CurrentDesignPosition;
//
// 		InputTouch->CurrentScreenPosition = ScreenPosition;
// 		//InputTouch->CurrentViewportPosition = ViewportPosition;
// 		//InputTouch->CurrentDesignPosition = DesignPosition;
//
// 		InputTouch->DeltaScreenPosition = ScreenPosition - LastScreenPosition;
// 		//InputTouch->DeltaViewportPosition = ViewportPosition - LastViewportPosition;
// 		//InputTouch->DeltaDesignPosition = DesignPosition - LastDesignPosition;
//
// 		FVector2d ViewportSize = UGraphicUtility::GetViewportSize();
// 		// Log(FString::Printf(
// 		// 	TEXT("%d, %s, (%d, %d), (%f, %f), (%f, %f)"),//, (%f, %f), (%f, %f)"),
// 		// 	InputTouch->TouchIndex,
// 		// 	InputTouch->IsLeft ? TEXT("True") : TEXT("False"),
// 		// 	GSystemResolution.ResX, GSystemResolution.ResY,
// 		// 	ViewportSize.X, ViewportSize.Y,
// 		// 	ViewportPosition.X, ViewportPosition.Y//,
// 		// 	// InputTouch->DeltaViewportPosition.X, InputTouch->DeltaViewportPosition.Y,
// 		// 	// InputTouch->StartViewportPosition.X, InputTouch->StartViewportPosition.Y
// 		// ));
// 		Log(FString::Printf(
// 			TEXT("%d, (%f, %f)"),
// 			InputTouch->TouchIndex,
// 			ScreenPosition.X, ScreenPosition.Y
// 		));
// 		OnTouchMove.Broadcast(*InputTouch);
// 	}
// 	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
// }

FReply UInputDragHandle::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InGestureEvent);
	if (InGestureEvent.IsTouchEvent())
	{
		int TouchIndex = InGestureEvent.GetPointerIndex();
		if (TouchIndex > ETouchIndex::Touch10)
		{
			return Reply;
		}
		FVector2D ScreenPosition = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
		AddInputTouch(TouchIndex, ScreenPosition);
	}
	return Reply;
}

// FReply UInputDragHandle::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
// {
// 	OnTouchEnd(InGestureEvent.GetPointerIndex());
// 	return Super::NativeOnMouseButtonUp(InGeometry, InGestureEvent);
// }

// FReply UInputDragHandle::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
// {
// 	OnTouchEnd(InGestureEvent);
// 	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
// }

// void UInputDragHandle::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
// {
// 	Super::NativeOnMouseLeave(InMouseEvent);
// 	OnTouchEnd(InMouseEvent);
// }

void UInputDragHandle::OnTouchEnd(const int& TouchIndex)
{
	//int TouchIndex = InGestureEvent.GetPointerIndex();
	//if (InGestureEvent.IsTouchEvent() && Touchs.Contains(TouchIndex))
	if (Touchs.Contains(TouchIndex))
	{
		FInputTouch InputTouch = Touchs[TouchIndex];
		Touchs.Remove(TouchIndex);
		OnTouchUp.Broadcast(InputTouch);
	}
}

void UInputDragHandle::OnTouchMoved(const ETouchIndex::Type TouchIndex, const FVector ScreenSpacePosition)
{
	FVector2D ScreenPosition = FVector2D(ScreenSpacePosition) / UGraphicUtility::GetViewportScale(this);
	//int TouchIndex = TouchType;
	if (!Touchs.Contains(TouchIndex))
	{
		if (Singleton<UVirtualInputSystem>(this)->Config.bMaskTouchDown)
		{
			return;
		}
		AddInputTouch(TouchIndex, ScreenPosition);
	}
	FInputTouch& InputTouch = Touchs[TouchIndex];

	FVector2D LastScreenPosition = InputTouch.CurrentScreenPosition;
	InputTouch.CurrentScreenPosition = ScreenPosition;
	InputTouch.DeltaScreenPosition = ScreenPosition - LastScreenPosition;

	OnTouchMove.Broadcast(InputTouch);
}

void UInputDragHandle::OnTouchReleased(const ETouchIndex::Type TouchIndex, const FVector ScreenSpacePosition)
{
	//Log(FString::Printf(TEXT("OnTouchReleased: %d"), TouchIndex));
	OnTouchEnd(TouchIndex);
}
