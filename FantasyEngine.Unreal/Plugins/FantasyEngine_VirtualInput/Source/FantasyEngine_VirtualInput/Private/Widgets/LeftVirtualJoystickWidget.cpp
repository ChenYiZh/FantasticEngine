// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LeftVirtualJoystickWidget.h"

#include "VirtualInputSystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Utilities/GraphicUtility.h"
#include "Utilities/ObjectFactory.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"

bool ULeftVirtualJoystickWidget::GetLeftJoystickAlwaysDisplay() const
{
	return bLockLeftJoystickPosition && bAwalysDisplayLeftJoystick;
}

FVector2D ULeftVirtualJoystickWidget::GetLeftJoystickCenterPosition() const
{
	FVector2D CenterPosition = bLockLeftJoystickPosition
		                           ? LeftJoystickDefaultPosition
		                           : LeftJoystickStartPosition;
	if (bLockLeftJoystickPosition)
	{
		CenterPosition = GetLeftJoystickAnchorPosition() + LeftJoystickDefaultPosition;
	}
	return CenterPosition;
}

FVector2D ULeftVirtualJoystickWidget::GetLeftJoystickAnchorPosition() const
{
	if (LeftJoystickBackgroundSlot != nullptr)
	{
		FVector2D Maximum = LeftJoystickBackgroundSlot->GetAnchors().Maximum;
		FVector2D ScreenSize = UGraphicUtility::GetScreenSize(this);
		return FVector2D(ScreenSize.X * Maximum.X, ScreenSize.Y * Maximum.Y);
	}
	return FVector2D::Zero();
}

bool ULeftVirtualJoystickWidget::GetRightJoystickEnable() const
{
	return RightJoystickMoving && !RightScaleJoystickMoving;
}

void ULeftVirtualJoystickWidget::OnWidgetOpen_Implementation(UWidgetParam* Param)
{
	Super::OnWidgetOpen_Implementation(Param);
	// if (UseHairScreen())
	// {
	// 	float Offset = UGraphicUtility::GetHairScreenOffsetRate();
	// 	SetAnchorsInViewport(FAnchors(Offset, 0, 1.f - Offset, 1));
	// }
	Singleton<UVirtualInputSystem>(this)->bInvertGamePadRightY = bDefaultInvertGamePadRightY;
	LeftJoystickTouchEnabled = false;
	RightScaleJoystickMoving = false;
	RightJoystickMoving = false;
	LeftJoystickBackground->SetVisibility(GetLeftJoystickAlwaysDisplay()
		                                      ? ESlateVisibility::HitTestInvisible
		                                      : ESlateVisibility::Collapsed);
	LeftJoystickBackgroundSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftJoystickBackground);
	LeftJoystickThumbSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftJoystickThumb);
	if (LeftJoystickBackgroundSlot != nullptr)
	{
		LeftJoystickMoveRange = (LeftJoystickBackgroundSlot->GetSize().X / 2.0f - LeftJoystickMoveBorder);
		LeftJoystickDefaultPosition = LeftJoystickBackgroundSlot->GetPosition();
	}
}

void ULeftVirtualJoystickWidget::OnTick_Implementation(float DeltaTime)
{
	Super::OnTick_Implementation(DeltaTime);
	/**移动方向**/
	LeftControllerAnalog(LeftJoystickOffset);
	/**移动视角**/
	FVector2D DeltaScreenPosition = RightJoystickMoving
		                                ? RightJoystickEvent.DeltaScreenPosition * RightJoystickMoveScale
		                                : FVector2D::ZeroVector;
	DeltaScreenPosition *= 0.1f;
	RightControllerAnalog(DeltaScreenPosition);
	/**缩放视角**/
	float DeltaWheel = 0;
	if (RightScaleJoystickMoving)
	{
		FVector2D RightJoystickDirection = RightJoystickEvent.CurrentScreenPosition
			- RightScaleJoystickEvent.StartScreenPosition;
		RightJoystickDirection.Normalize();
		FVector2D RightJoystickDeltaDir = RightJoystickEvent.DeltaScreenPosition;
		RightJoystickDeltaDir.Normalize();
		float Dir = FVector2D::DotProduct(RightJoystickDirection, RightJoystickDeltaDir) > 0 ? 1 : -1;
		DeltaWheel = RightJoystickEvent.DeltaScreenPosition.Length() * Dir * RightJoystickWheelScale;
	}
	DeltaWheel *= 0.1f;
	MouseWheelAnalog(DeltaWheel);

	RightJoystickEvent.DeltaScreenPosition = FVector2D::ZeroVector;
	RightScaleJoystickEvent.DeltaScreenPosition = FVector2D::ZeroVector;
}

void ULeftVirtualJoystickWidget::OnTouchDown_Implementation(const FInputTouch& TouchEvent)
{
	Super::OnTouchDown_Implementation(TouchEvent);
	if (TouchEvent.bIsLeft)
	{
		OnTouchLeft(TouchEvent);
	}
	else
	{
		if (!RightJoystickMoving)
		{
			RightJoystickMoving = true;
			RightJoystickEvent = TouchEvent;
		}
		else if (!RightScaleJoystickMoving)
		{
			RightScaleJoystickMoving = true;
			RightScaleJoystickEvent = TouchEvent;
		}
	}
}

void ULeftVirtualJoystickWidget::OnTouchMove_Implementation(const FInputTouch& TouchEvent)
{
	Super::OnTouchMove_Implementation(TouchEvent);
	if (TouchEvent.bIsLeft)
	{
		OnTouchLeft(TouchEvent);
	}
	else
	{
		if (RightJoystickMoving && RightJoystickEvent.TouchIndex == TouchEvent.TouchIndex)
		{
			RightJoystickEvent = TouchEvent;
		}
		if (RightScaleJoystickMoving && RightScaleJoystickEvent.TouchIndex == TouchEvent.TouchIndex)
		{
			RightScaleJoystickEvent = TouchEvent;
		}
	}
}

void ULeftVirtualJoystickWidget::OnTouchUp_Implementation(const FInputTouch& TouchEvent)
{
	Super::OnTouchUp_Implementation(TouchEvent);
	if (LeftJoystickTouchEnabled && TouchEvent.bIsLeft && LeftJoystickBackground->IsVisible() &&
		LeftJoystickTouchIndex == TouchEvent.TouchIndex)
	{
		LeftJoystickTouchEnabled = false;
		if (!GetLeftJoystickAlwaysDisplay())
		{
			LeftJoystickBackground->SetVisibility(ESlateVisibility::Collapsed);
		}
		LeftJoystickBackground->SetRenderTransformAngle(0);
		if (LeftJoystickThumbSlot != nullptr)
		{
			LeftJoystickThumbSlot->SetPosition(FVector2D::Zero());
		}
		LeftJoystickOffset = FVector2D::Zero();
	}
	if (!TouchEvent.bIsLeft)
	{
		if (RightJoystickMoving && RightJoystickEvent.TouchIndex == TouchEvent.TouchIndex)
		{
			if (RightScaleJoystickMoving)
			{
				RightScaleJoystickMoving = false;
				RightJoystickEvent = RightScaleJoystickEvent;
			}
			else
			{
				RightJoystickMoving = false;
			}
		}
		if (RightScaleJoystickMoving && RightScaleJoystickEvent.TouchIndex == TouchEvent.TouchIndex)
		{
			RightScaleJoystickMoving = false;
		}
	}
}

void ULeftVirtualJoystickWidget::CheckLeftTouchBegun(const FInputTouch& TouchEvent)
{
	if (TouchEvent.bIsLeft && TouchEvent.bIsValid)
	{
		if (!LeftJoystickTouchEnabled)
		{
			LeftJoystickTouchEnabled = true;
			LeftJoystickTouchIndex = TouchEvent.TouchIndex;

			if (!LeftJoystickBackground->IsVisible())
			{
				LeftJoystickBackground->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (!bLockLeftJoystickPosition)
				{
					const FVector2D ScreenOffset = GetScreenOffset();
					LeftJoystickStartPosition = TouchEvent.StartScreenPosition - ScreenOffset;
					FVector2D Direction = LeftJoystickStartPosition
						- (TouchEvent.CurrentScreenPosition - ScreenOffset);
					if (Direction.SizeSquared() > LeftJoystickMoveRange * LeftJoystickMoveRange)
					{
						LeftJoystickStartPosition = TouchEvent.CurrentScreenPosition - ScreenOffset
							+ Direction.GetSafeNormal() * LeftJoystickMoveRange;
					}
				}
				if (LeftJoystickBackgroundSlot != nullptr)
				{
					LeftJoystickBackgroundSlot->SetPosition(
						GetLeftJoystickCenterPosition() - GetLeftJoystickAnchorPosition());
				}
				if (LeftJoystickThumbSlot != nullptr)
				{
					LeftJoystickThumbSlot->SetPosition(FVector2D::Zero());
				}
			}
		}
	}
}

void ULeftVirtualJoystickWidget::OnTouchLeft_Implementation(const FInputTouch& TouchEvent)
{
	CheckLeftTouchBegun(TouchEvent);
	if (LeftJoystickTouchEnabled && TouchEvent.bIsValid && LeftJoystickTouchIndex == TouchEvent.TouchIndex)
	{
		if (LeftJoystickBackgroundSlot != nullptr)
		{
			FVector2D CenterPosition = GetLeftJoystickCenterPosition();
			FVector2D Direction = TouchEvent.CurrentScreenPosition - GetScreenOffset() - CenterPosition;
			float Length = Direction.Size();
			FVector2D UnitY = -FVector2D::UnitY();
			FVector2D UnitDirection = Direction.GetSafeNormal();
			if (bRotateLeftJoystickBackgroundOnMoving && Length > 0)
			{
				float Dot = FVector2D::DotProduct(UnitDirection, UnitY);
				float Sign = FVector2D::CrossProduct(UnitDirection, UnitY) < 0
					             ? 1.f
					             : -1.f;
				float Angle = FMath::Acos(Dot) * Sign / PI * 180.f;
				LeftJoystickBackground->SetRenderTransformAngle(Angle);
			}
			if (LeftJoystickThumbSlot != nullptr)
			{
				if (Length > 0)
				{
					LeftJoystickThumbSlot->SetPosition(
						(bRotateLeftJoystickBackgroundOnMoving ? UnitY : UnitDirection)
						* (Length > LeftJoystickMoveRange ? LeftJoystickMoveRange : Length));
				}
				else
				{
					LeftJoystickThumbSlot->SetPosition(FVector2D::Zero());
				}
			}
			LeftJoystickOffset = UnitDirection * (Length > LeftJoystickMoveRange ? LeftJoystickMoveRange : Length) /
				LeftJoystickMoveRange;
		}
	}
}

void ULeftVirtualJoystickWidget::LeftControllerAnalog(const FVector2D& LeftOffset)
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	//FSlateApplication::Get().SetAllUserFocusToGameViewport();
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogX,
	                                            FSlateApplicationBase::SlateAppPrimaryPlatformUser,
	                                            PrimaryInputDevice, LeftOffset.X);
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::LeftAnalogY,
	                                            FSlateApplicationBase::SlateAppPrimaryPlatformUser,
	                                            PrimaryInputDevice, -LeftOffset.Y);
}

void ULeftVirtualJoystickWidget::RightControllerAnalog(const FVector2D& RightOffset)
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogX,
	                                            FSlateApplicationBase::SlateAppPrimaryPlatformUser,
	                                            PrimaryInputDevice, RightOffset.X);
	FSlateApplication::Get().OnControllerAnalog(FGamepadKeyNames::RightAnalogY,
	                                            FSlateApplicationBase::SlateAppPrimaryPlatformUser,
	                                            PrimaryInputDevice, -RightOffset.Y);
}

void ULeftVirtualJoystickWidget::MouseWheelAnalog(const float Delta)
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnMouseWheel(Delta);
}

FVector2D ULeftVirtualJoystickWidget::GetScreenOffset() const
{
	return GetAnchorsInViewport().Minimum * UUIFactory::GetScreenSize(this);
}
