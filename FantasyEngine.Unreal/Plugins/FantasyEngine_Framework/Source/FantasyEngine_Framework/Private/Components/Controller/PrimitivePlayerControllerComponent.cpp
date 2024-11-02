// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/PrimitivePlayerControllerComponent.h"

//#include "Factories/MaterialParameterCollectionFactoryNew.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameRoot.h"
#include "Defines/DefaultInputDefines.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/Pawn//CharacterBeingComponent.h"
#include "Utilities/ObjectFactory.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Log/FEConsole.h"
#include "Materials/MaterialParameterCollection.h"
#include "Systems/InputSystem.h"

void UPrimitivePlayerControllerComponent::MoveForward_Implementation(const FInputActionInstance& Instance)
{
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->MoveForward(Instance.GetValue().Get<float>(), false);
	}
}

void UPrimitivePlayerControllerComponent::MoveRight_Implementation(const FInputActionInstance& Instance)
{
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->MoveRight(Instance.GetValue().Get<float>(), false);
	}
}

void UPrimitivePlayerControllerComponent::LookUp_Implementation(const FInputActionInstance& Instance)
{
	if (bLockTurnAndLookUp) { return; }
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->LookUp(Instance.GetValue().Get<float>());
	}
}

void UPrimitivePlayerControllerComponent::Turn_Implementation(const FInputActionInstance& Instance)
{
	if (bLockTurnAndLookUp) { return; }
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->Turn(Instance.GetValue().Get<float>());
	}
}

void UPrimitivePlayerControllerComponent::ViewClose_Implementation(const FInputActionInstance& Instance)
{
	TArray<UPawnOperationsComponent*> OperationsComponents;
	GetPawnOperationsComponents(OperationsComponents);
	for (UPawnOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->ViewClose(Instance.GetValue().Get<float>());
	}
}

void UPrimitivePlayerControllerComponent::MouseControlPressed_Implementation(const FInputActionInstance& Instance)
{
	UInputSystem::SetMouseType(this, EMouseType::Visible);
	bLockTurnAndLookUp = true;
}

void UPrimitivePlayerControllerComponent::MouseControlReleased_Implementation(const FInputActionInstance& Instance)
{
	UInputSystem::SetMouseType(this, EMouseType::Default);
	bLockTurnAndLookUp = false;
}

void UPrimitivePlayerControllerComponent::BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent,
                                                           UInputDefines* Defines)
{
	if (const UDefaultInputDefines* DefaultDefines = Cast<UDefaultInputDefines>(Defines))
	{
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_MoveForward(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::MoveForward);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_MoveForward(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::MoveForward);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_MoveRight(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::MoveRight);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_LookUp(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::LookUp);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_Turn(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::Turn);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_ViewClose(), ETriggerEvent::Triggered, this,
		                                   &UPrimitivePlayerControllerComponent::ViewClose);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_MouseControl(), ETriggerEvent::Started, this,
		                                   &UPrimitivePlayerControllerComponent::MouseControlPressed);
		EnhancedInputComponent->BindAction(DefaultDefines->GetIA_MouseControl(), ETriggerEvent::Completed, this,
		                                   &UPrimitivePlayerControllerComponent::MouseControlReleased);
	}
}

void UPrimitivePlayerControllerComponent::GetBasicOperationsComponents(
	TArray<UBasicOperationsComponent*>& OperationsComponents) const
{
	if (const APawn* CurrentPawn = GetPawn())
	{
		CurrentPawn->GetComponents<UBasicOperationsComponent>(OperationsComponents);
	}
}

void UPrimitivePlayerControllerComponent::GetPawnOperationsComponents(
	TArray<UPawnOperationsComponent*>& OperationsComponents) const
{
	if (const APawn* CurrentPawn = GetPawn())
	{
		CurrentPawn->GetComponents<UPawnOperationsComponent>(OperationsComponents);
	}
}
