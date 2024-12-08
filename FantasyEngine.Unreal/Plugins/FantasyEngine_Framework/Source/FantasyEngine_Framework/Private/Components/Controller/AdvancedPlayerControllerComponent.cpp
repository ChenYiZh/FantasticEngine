// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/AdvancedPlayerControllerComponent.h"

#include "Components/Pawn/BasicOperationsComponent.h"
#include "Systems/InputSystem.h"


void UAdvancedPlayerControllerComponent::MoveForward_Implementation(const FInputActionInstance& Instance)
{
	if (ViewMode == EGameViewMode::V_2D && bLockAxisZ)
	{
		return;
	}
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->MoveForward(Instance.GetValue().Get<float>(),
		                                 QuickDirection & 1 << static_cast<int32>(EQuickDirection::ForwardAndBack));
	}
}

void UAdvancedPlayerControllerComponent::MoveRight_Implementation(const FInputActionInstance& Instance)
{
	TArray<UBasicOperationsComponent*> OperationsComponents;
	GetBasicOperationsComponents(OperationsComponents);
	for (UBasicOperationsComponent* OperationsComponent : OperationsComponents)
	{
		OperationsComponent->MoveRight(Instance.GetValue().Get<float>(),
		                               QuickDirection & 1 << static_cast<int32>(EQuickDirection::LeftAndRight));
	}
}

void UAdvancedPlayerControllerComponent::LookUp_Implementation(const FInputActionInstance& Instance)
{
	if (UInputSystem::IsCursorVisible(this)) { return; }

	const float Value = Instance.GetValue().Get<float>();

	if (ViewMode != EGameViewMode::V_3D)
	{
		TurnView(FVector2D(0.f, Value));
		return;
	}

	Super::LookUp_Implementation(Instance);
}

void UAdvancedPlayerControllerComponent::Turn_Implementation(const FInputActionInstance& Instance)
{
	if (UInputSystem::IsCursorVisible(this)) { return; }

	const float Value = Instance.GetValue().Get<float>();
	if (ViewMode != EGameViewMode::V_3D)
	{
		TurnView(FVector2D(Value, 0.f));
		return;
	}

	Super::Turn_Implementation(Instance);
}

void UAdvancedPlayerControllerComponent::TurnView_Implementation(const FVector2D Value)
{
	OnTurnView.Broadcast(Value);
	K2_OnTurnView.Broadcast(Value);
}
