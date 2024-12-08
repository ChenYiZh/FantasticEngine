// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MobileGameVirtualJoystickWidget.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"

void UMobileGameVirtualJoystickWidget::InputGamepadFaceUpPressed()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonPressed(FGamepadKeyNames::FaceButtonTop,
	                                                   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
	                                                   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceUpReleased()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonReleased(FGamepadKeyNames::FaceButtonTop, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceDownPressed()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonPressed(FGamepadKeyNames::FaceButtonBottom, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceDownReleased()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonReleased(FGamepadKeyNames::FaceButtonBottom, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceLeftPressed()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonPressed(FGamepadKeyNames::FaceButtonLeft, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceLeftReleased()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonReleased(FGamepadKeyNames::FaceButtonLeft, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceRightPressed()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonPressed(FGamepadKeyNames::FaceButtonRight, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}

void UMobileGameVirtualJoystickWidget::InputGamepadFaceRightReleased()
{
	FInputDeviceId PrimaryInputDevice = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(
		FSlateApplicationBase::SlateAppPrimaryPlatformUser);
	FSlateApplication::Get().OnControllerButtonReleased(FGamepadKeyNames::FaceButtonRight, 
													   FSlateApplicationBase::SlateAppPrimaryPlatformUser,
													   PrimaryInputDevice, false);
}
