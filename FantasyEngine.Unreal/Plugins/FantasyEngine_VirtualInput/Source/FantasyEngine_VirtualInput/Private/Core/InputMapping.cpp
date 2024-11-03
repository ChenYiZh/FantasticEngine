// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/InputMapping.h"

#include "GameFramework/InputSettings.h"

FName UInputMapping::GetArrowUp()
{
	return FName(TEXT("ArrowUp"));
}

FName UInputMapping::GetArrowDown()
{
	return FName(TEXT("ArrowDown"));
}

FName UInputMapping::GetArrowLeft()
{
	return FName(TEXT("ArrowLeft"));
}

FName UInputMapping::GetArrowRight()
{
	return FName(TEXT("ArrowRight"));
}

FName UInputMapping::GetActionUp()
{
	return FName(TEXT("ActionUp"));
}

FName UInputMapping::GetActionDown()
{
	return FName(TEXT("ActionDown"));
}

FName UInputMapping::GetActionLeft()
{
	return FName(TEXT("ActionLeft"));
}

FName UInputMapping::GetActionRight()
{
	return FName(TEXT("ActionRight"));
}

FName UInputMapping::GetAxisMoveForward()
{
	return FName(TEXT("AxisMoveForward"));
}

FName UInputMapping::GetAxisMoveRight()
{
	return FName(TEXT("AxisMoveRight"));
}

FName UInputMapping::GetAxisLookUpRate()
{
	return FName(TEXT("AxisLookUpRate"));
}

FName UInputMapping::GetAxisTurnRate()
{
	return FName(TEXT("AxisTurnRate"));
}

FName UInputMapping::GetAxisLookUp()
{
	return FName(TEXT("AxisLookUp"));
}

FName UInputMapping::GetAxisTurn()
{
	return FName(TEXT("AxisTurn"));
}

FName UInputMapping::GetAxisWheel()
{
	return FName(TEXT("AxisWheel"));
}

FName UInputMapping::GetLeftShoulder()
{
	return FName(TEXT("LeftShoulder"));
}

FName UInputMapping::GetCtrl()
{
	return FName(TEXT("Ctrl"));
}

void UInputMapping::BindBaseGamePadMapping()
{
	//GetDefault<UInputSettings>()->bUseMouseForTouch = 0;

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowUp(), EKeys::Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowUp(), EKeys::Gamepad_DPad_Up));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowUp(), EKeys::Gamepad_LeftStick_Up));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowDown(), EKeys::Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowDown(), EKeys::Gamepad_DPad_Down));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowDown(), EKeys::Gamepad_LeftStick_Down));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowLeft(), EKeys::Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowLeft(), EKeys::Gamepad_DPad_Left));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowLeft(), EKeys::Gamepad_LeftStick_Left));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowRight(), EKeys::Right));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetArrowRight(), EKeys::Gamepad_DPad_Right));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetCtrl(), EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetCtrl(), EKeys::RightControl));

	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetArrowRight(), EKeys::Gamepad_LeftStick_Right));


	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionUp(), EKeys::Gamepad_FaceButton_Top));
	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionUp(), EKeys::Q));

	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionDown(), EKeys::Gamepad_FaceButton_Bottom));
	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionDown(), EKeys::SpaceBar));

	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionLeft(), EKeys::Gamepad_FaceButton_Left));
	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionLeft(), EKeys::E));

	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionRight(), EKeys::Gamepad_FaceButton_Right));
	UPlayerInput::AddEngineDefinedActionMapping(
		FInputActionKeyMapping(GetActionRight(), EKeys::LeftShift));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(GetLeftShoulder(), EKeys::Gamepad_LeftShoulder));


	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveForward(), EKeys::Gamepad_LeftY));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveForward(), EKeys::S, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveForward(), EKeys::W));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveRight(), EKeys::Gamepad_LeftX));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveRight(), EKeys::A, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisMoveRight(), EKeys::D));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisLookUpRate(), EKeys::Gamepad_RightY));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisLookUp(), EKeys::MouseY, -1));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisTurnRate(), EKeys::Gamepad_RightX));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisTurn(), EKeys::MouseX));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(GetAxisWheel(), EKeys::MouseWheelAxis, -1));
}

void UInputMapping::BindARPGMapping()
{
	BindBaseGamePadMapping();
}
