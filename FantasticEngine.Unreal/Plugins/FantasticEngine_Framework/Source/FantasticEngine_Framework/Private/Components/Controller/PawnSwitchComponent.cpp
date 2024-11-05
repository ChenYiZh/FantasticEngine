// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/PawnSwitchComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Log/FEConsole.h"

// Sets default values for this component's properties
UPawnSwitchComponent::UPawnSwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

APawn* UPawnSwitchComponent::SwitchToPawnCreatedByClass_Implementation(const FVector InLocation,
                                                                       const FRotator InRotator, const bool bSmooth,
                                                                       const float Duration,
                                                                       const EViewTargetBlendFunction BlendFunc)
{
	if (!IsValid(PawnClass))
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("World"),TEXT("The default character class of controller is not valided."));
		return nullptr;
	}
	APawn* LocalPawn = GetWorld()->SpawnActor<APawn>(PawnClass, InLocation, InRotator, FActorSpawnParameters());
	if (bSmooth)
	{
		SlideToPawn(LocalPawn, Duration, BlendFunc);
	}
	else
	{
		SwitchToPawn(LocalPawn);
	}
	return LocalPawn;
}

void UPawnSwitchComponent::SlideToPawn_Implementation(APawn* Target, const float Duration,
                                                      const EViewTargetBlendFunction BlendFunc)
{
	SwitchToPawnImpl(Target, true, Duration, BlendFunc);
}

void UPawnSwitchComponent::SwitchToPawn_Implementation(APawn* Target)
{
	SwitchToPawnImpl(Target, false, -1, VTBlend_MAX);
}

void UPawnSwitchComponent::SwitchToPawnImpl(APawn* Target, const bool bSmooth, const float Duration,
                                            const EViewTargetBlendFunction BlendFunc)
{
	if (APlayerController* PlayerController = GetPlayerController())
	{
		if (!IsValid(Target))
		{
			PlayerController->UnPossess();
			K2_OnSwitchToNewPawn.Broadcast(nullptr);
			OnSwitchToNewPawn.Broadcast(nullptr);
			return;
		}
		if (bSmooth && Duration > 0)
		{
			TotalSeconds = Duration;
			SwitchTarget = Target;
			PlayerController->SetViewTargetWithBlend(Target, Duration, BlendFunc);
		}
		else
		{
			SwitchTarget = nullptr;
			TotalSeconds = -1.f;
			PlayerController->Possess(Target);
			K2_OnSwitchToNewPawn.Broadcast(Target);
			OnSwitchToNewPawn.Broadcast(Target);
		}
	}
}

void UPawnSwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalSeconds > 0)
	{
		if (!SwitchTarget.IsValid())
		{
			TotalSeconds = -1.f;
			return;
		}
		if (--TotalSeconds < 0)
		{
			if (APlayerController* PlayerController = GetPlayerController())
			{
				SwitchTarget = nullptr;
				TotalSeconds = -1.f;
				PlayerController->Possess(SwitchTarget.Get());
				K2_OnSwitchToNewPawn.Broadcast(SwitchTarget.Get());
				OnSwitchToNewPawn.Broadcast(SwitchTarget.Get());
			}
		}
	}
}

APlayerController* UPawnSwitchComponent::GetPlayerController() const
{
	return UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
}
