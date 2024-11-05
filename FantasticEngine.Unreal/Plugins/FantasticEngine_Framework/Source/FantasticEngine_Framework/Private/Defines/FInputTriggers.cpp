// Fill out your copyright notice in the Description page of Project Settings.


#include "Defines/FInputTriggers.h"

#include "EnhancedPlayerInput.h"
#include "InputAction.h"
#include "Common/StringConverter.h"
#include "Internationalization/Internationalization.h"
#include "Log/FEConsole.h"

#define LOCTEXT_NAMESPACE "EnhancedInputTriggers"
#if WITH_EDITOR
EDataValidationResult UInputTriggerUntapped::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors),
	                                                            EDataValidationResult::Valid);

	// You can't evaluate the combo if there are no combo steps!
	if (!ChordAction)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(LOCTEXT("NullUntappedAction",
		                             "A valid action is required for the Untapped Action input trigger!"));
	}

	return Result;
}
#endif
ETriggerState UInputTriggerUntapped::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                FInputActionValue ModifiedValue, float DeltaTime)
{
	// Inherit state from the chorded action
	// UFEConsole::Write(
	// 	UStringConverter::ConvertEnum(Super::UpdateState_Implementation(PlayerInput, ModifiedValue, DeltaTime)));
	return Super::UpdateState_Implementation(PlayerInput, ModifiedValue, DeltaTime) == ETriggerState::Triggered
		       ? ETriggerState::None
		       : ETriggerState::Triggered;
	//return IsActuated(ModifiedValue) ? ETriggerState::None : ETriggerState::Triggered;
	// // Ongoing on hold
	// if (IsActuated(ModifiedValue))
	// {
	// 	return ETriggerState::None;
	// }
	//
	// // Triggered on release
	// if (IsActuated(LastValue))
	// {
	// 	return ETriggerState::Triggered;
	// }
	//
	// return ETriggerState::None;
}
#undef LOCTEXT_NAMESPACE