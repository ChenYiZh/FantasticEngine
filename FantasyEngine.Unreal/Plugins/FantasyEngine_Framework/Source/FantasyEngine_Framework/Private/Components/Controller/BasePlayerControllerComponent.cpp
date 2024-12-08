// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/BasePlayerControllerComponent.h"

#include "EnhancedInputComponent.h"
#include "Defines/InputDefines.h"

// Sets default values for this component's properties
UBasePlayerControllerComponent::UBasePlayerControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UBasePlayerControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetOwner()->InputComponent);
	if (IsValid(EnhancedInputComponent))
	{
		if (!IsValid(InputDefinesInstance))
		{
			InputDefinesInstance = NewObject<UInputDefines>(this, InputDefinesClass);
			if (IsValid(InputDefinesInstance))
			{
				InputDefinesInstance->Initialize();
			}
		}
		if (IsValid(InputDefinesInstance))
		{
			BindInput(EnhancedInputComponent, InputDefinesInstance);
		}
	}
}

void UBasePlayerControllerComponent::BindInput(UEnhancedInputComponent* EnhancedInputComponent, UInputDefines* Defines)
{
	BindInputDefines(EnhancedInputComponent, Defines);
	K2_BindInputDefines(EnhancedInputComponent, Defines);
}

void UBasePlayerControllerComponent::BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent,
                                                      UInputDefines* Defines)
{
}

APlayerController* UBasePlayerControllerComponent::GetPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

APawn* UBasePlayerControllerComponent::GetPawn() const
{
	if (APlayerController* Controller = GetPlayerController())
	{
		return Controller->GetPawn();
	}
	return nullptr;
}
