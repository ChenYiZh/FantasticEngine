// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameplayPlayerControllerComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/GameplayCharacterComponent.h"
#include "Gameplay/GameplayInputDefines.h"

void UGameplayPlayerControllerComponent::OnArrowUpPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowUpPressed();
	}
}

void UGameplayPlayerControllerComponent::OnArrowUpReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowUpReleased();
	}
}

void UGameplayPlayerControllerComponent::OnArrowDownPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowDownPressed();
	}
}

void UGameplayPlayerControllerComponent::OnArrowDownReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowDownReleased();
	}
}

void UGameplayPlayerControllerComponent::OnArrowLeftPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowLeftPressed();
	}
}

void UGameplayPlayerControllerComponent::OnArrowLeftReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowLeftReleased();
	}
}

void UGameplayPlayerControllerComponent::OnArrowRightPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowRightPressed();
	}
}

void UGameplayPlayerControllerComponent::OnArrowRightReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnArrowRightReleased();
	}
}

void UGameplayPlayerControllerComponent::OnActionUpPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionUpPressed();
	}
}

void UGameplayPlayerControllerComponent::OnActionUpReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionUpReleased();
	}
}

void UGameplayPlayerControllerComponent::OnActionDownPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionDownPressed();
	}
}

void UGameplayPlayerControllerComponent::OnActionDownReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionDownReleased();
	}
}

void UGameplayPlayerControllerComponent::OnActionLeftPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionLeftPressed();
	}
}

void UGameplayPlayerControllerComponent::OnActionLeftReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionLeftReleased();
	}
}

void UGameplayPlayerControllerComponent::OnActionRightPressed_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionRightPressed();
	}
}

void UGameplayPlayerControllerComponent::OnActionRightReleased_Implementation(const FInputActionInstance& Instance)
{
	TArray<UGameplayCharacterComponent*> GameplayCharacters;
	GetGameplayCharacterComponents(GameplayCharacters);
	for (UGameplayCharacterComponent* GameplayCharacter : GameplayCharacters)
	{
		GameplayCharacter->OnActionRightReleased();
	}
}

void UGameplayPlayerControllerComponent::BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines)
{
	Super::BindInputDefines(EnhancedInputComponent, Defines);
	if (UGameplayInputDefines* InInputDefines = Cast<UGameplayInputDefines>(Defines))
	{
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowUp(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowUpPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowUp(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowUpReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowDown(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowDownPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowDown(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowDownReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowLeft(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowLeftPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowLeft(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowLeftReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowRight(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowRightPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ArrowRight(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnArrowRightReleased);

		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionUp(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnActionUpPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionUp(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnActionUpReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionDown(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnActionDownPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionDown(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnActionDownReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionLeft(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnActionLeftPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionLeft(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnActionLeftReleased);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionRight(), ETriggerEvent::Started, this,
		                                   &UGameplayPlayerControllerComponent::OnActionRightPressed);
		EnhancedInputComponent->BindAction(InInputDefines->GetIA_ActionRight(), ETriggerEvent::Completed, this,
		                                   &UGameplayPlayerControllerComponent::OnActionRightReleased);
	}
}

void UGameplayPlayerControllerComponent::GetGameplayCharacterComponents(
	TArray<UGameplayCharacterComponent*>& GameplayCharacters) const
{
	if (const APawn* CurrentPawn = GetPawn())
	{
		CurrentPawn->GetComponents<UGameplayCharacterComponent>(GameplayCharacters);
	}
}
