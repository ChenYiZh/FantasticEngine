// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pawn/PawnOperationsComponent.h"

#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UPawnOperationsComponent::UPawnOperationsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPawnOperationsComponent::ViewClose_Implementation(float Value)
{
	USpringArmComponent* LocalCameraBoom = GetCameraBoom();
	if (IsValid(LocalCameraBoom))
	{
		float NextLength = FMath::Clamp(LocalCameraBoom->TargetArmLength + Value * 50, ViewLengthRange.X,
		                                ViewLengthRange.Y);
		LocalCameraBoom->TargetArmLength = NextLength;
		OnViewClose.Broadcast(NextLength);
		K2_OnViewClose.Broadcast(NextLength);
		return;
	}
	OnViewClose.Broadcast(Value);
	K2_OnViewClose.Broadcast(Value);
}
