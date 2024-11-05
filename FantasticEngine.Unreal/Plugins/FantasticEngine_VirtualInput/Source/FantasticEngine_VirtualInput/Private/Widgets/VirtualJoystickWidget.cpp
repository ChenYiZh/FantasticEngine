// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/VirtualJoystickWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Core/InputMapping.h"
#include "Core/VirtualInputSystemFunc.h"
#include "Log/FEConsole.h"
#include "Systems/InputSystem.h"
#include "Utilities/GraphicUtility.h"

void UVirtualJoystickWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UVirtualInputSystemFunc::RegistVirtualJoystickWidget(this);
}

void UVirtualJoystickWidget::OnWidgetOpen_Implementation(UWidgetParam* Param)
{
	Super::OnWidgetOpen_Implementation(Param);
	if (!IsValid(InputComponent))
	{
		InitializeInputComponent();
	}
	InputHandles.Push(InputComponent->BindAction(UInputMapping::GetCtrl(), EInputEvent::IE_Pressed, this,
	                                             &UVirtualJoystickWidget::OnCtrlPressed).GetHandle());
	InputHandles.Push(InputComponent->BindAction(UInputMapping::GetCtrl(), EInputEvent::IE_Released, this,
	                                             &UVirtualJoystickWidget::OnCtrlReleased).GetHandle());
	bShowOnClose = UInputSystem::IsCursorVisible(this);
	UInputSystem::SetCursorVisible(this, false);
}

void UVirtualJoystickWidget::OnWidgetClosed_Implementation()
{
	if (IsValid(InputComponent))
	{
		for (int32 Handle : InputHandles)
		{
			InputComponent->RemoveActionBindingForHandle(Handle);
		}
	}
	UInputSystem::SetCursorVisible(this, bShowOnClose);
	Super::OnWidgetClosed_Implementation();
}

void UVirtualJoystickWidget::OnCtrlPressed()
{
	if (!UInputSystem::IsCursorVisible(this))
	{
		UInputSystem::SetCursorVisible(this, true);
	}
}

void UVirtualJoystickWidget::OnCtrlReleased()
{
	if (UInputSystem::IsCursorVisible(this))
	{
		UInputSystem::SetCursorVisible(this, false);
	}
}

void UVirtualJoystickWidget::OnTouchDownDelegate(const FInputTouch& TouchEvent)
{
	try
	{
		OnTouchDown(TouchEvent);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Virtual Joystick"),TEXT("There are some errors happened OnTouchDown."));
	}
}

void UVirtualJoystickWidget::OnTouchMoveDelegate(const FInputTouch& TouchEvent)
{
	try
	{
		OnTouchMove(TouchEvent);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Virtual Joystick"),TEXT("There are some errors happened OnTouchMove."));
	}
}

void UVirtualJoystickWidget::OnTouchUpDelegate(const FInputTouch& TouchEvent)
{
	try
	{
		OnTouchUp(TouchEvent);
	}
	catch (...)
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Virtual Joystick"),TEXT("There are some errors happened OnTouchUp."));
	}
}

void UVirtualJoystickWidget::OnTouchDown_Implementation(const FInputTouch& TouchEvent)
{
}

void UVirtualJoystickWidget::OnTouchMove_Implementation(const FInputTouch& TouchEvent)
{
	// 	Log(FString::Printf(
	// 	TEXT("%d, %s, (%f, %f), (%f, %f), (%f, %f), (%f, %f)"),
	// 	TouchEvent->TouchIndex,
	// 	TouchEvent->IsLeft ? TEXT("True") : TEXT("False"),
	// 	ViewportSize.X, ViewportSize.Y,
	// 	ViewportPosition.X, ViewportPosition.Y,
	// 	InputTouch->DeltaViewportPosition.X, InputTouch->DeltaViewportPosition.Y,
	// 	InputTouch->StartViewportPosition.X, InputTouch->StartViewportPosition.Y
	// ));
	// Log(FString::Printf(TEXT("OnTouchMove_Implementation: %f, %f"), TouchEvent.CurrentViewportPosition.X,
	//                     TouchEvent.CurrentViewportPosition.Y));
}

void UVirtualJoystickWidget::OnTouchUp_Implementation(const FInputTouch& TouchEvent)
{
}
