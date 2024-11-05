// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FocusComponent.h"

#include "Common/StringConverter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log/FEConsole.h"

// Sets default values for this component's properties
UFocusComponent::UFocusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFocusComponent::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}


// Called every frame
void UFocusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TargetLookedAt && (bKeepLookAt || bUseTurn))
	{
		GetControlledTarget()->SetWorldRotation(LastWorldRotator);
		LookAtImpl(TargetLookedAt, false, bKeepLookAt, bUseTurn, DeltaTime);
	}
	else
	{
		bKeepLookAt = bUseTurn = false;
	}
}

void UFocusComponent::Reset()
{
	bUseTurn = false;
	bKeepLookAt = false;
	TargetLookedAt = nullptr;
	LastWorldRotator = FRotator::ZeroRotator;
}

void UFocusComponent::LookAt(USceneComponent* TargetComponent, const bool bFocus, const bool bKeep,
                             const bool bInUseTurn)
{
	LastWorldRotator = GetControlledTarget()->GetComponentRotation();
	LookAtImpl(TargetComponent, bFocus, bKeep, bInUseTurn, 0);
}

void UFocusComponent::LookAtImpl(USceneComponent* TargetComponent, bool bFocus, bool bKeep, const bool bInUseTurn,
                                 const float DeltaSeconds)
{
	USceneComponent* Owner = GetControlledTarget();
	bool bEndTurn = false;
	const bool bIsValid = IsValid(TargetComponent);
	if (bIsValid)
	{
		FRotator Rotator =
			UKismetMathLibrary::FindLookAtRotation(Owner->GetComponentLocation(),
			                                       TargetComponent->GetComponentLocation());
		// Rotator.Roll = 0;
		// Rotator.Pitch = 0;
		const FRotator CurrentRotator = Owner->GetComponentRotation();
		if (bInUseTurn)
		{
			const float DeltaAngle = TurnSpeed * DeltaSeconds;
			//const float DeltaAngle = (Rotator - CurrentRotator).Yaw;
			const FRotator DeltaRotator = (Rotator - CurrentRotator).Vector().GetSafeNormal().Rotation() * DeltaAngle;
			const FVector NewForward = DeltaRotator.RotateVector(FVector::ForwardVector);
			const float CosA = FVector::DotProduct(FVector::ForwardVector, NewForward);
			const float CosB = FVector::DotProduct(
				(TargetComponent->GetComponentLocation() - Owner->GetComponentLocation()).GetSafeNormal(),
				Owner->GetForwardVector());
			// if (FMath::Abs(DeltaAngle) > DeltaAngle)
			// {
			// 	Rotator.Yaw = DeltaAngle > 0 ? DeltaAngle : -DeltaAngle;
			// }
			// else
			// {
			// 	bEndTurn = true;
			// }

			if (CosA > CosB) //余弦函数中，夹角越小，越接近1
			{
				Rotator = Owner->GetComponentRotation() + DeltaRotator;
			}
		}

		if (bOnlyAxisZ)
		{
			Rotator.Roll = CurrentRotator.Roll;
			Rotator.Pitch = CurrentRotator.Pitch;
		}

		Owner->SetWorldRotation(Rotator);
		LastWorldRotator = Rotator;
	}
	else
	{
		bEndTurn = true;
	}
	OnLookAt.Broadcast(TargetComponent, bFocus, bKeep, bInUseTurn);
	K2_OnLookAt.Broadcast(TargetComponent, bFocus, bKeep, bInUseTurn);
	if (bFocus)
	{
		OnFocus.Broadcast(TargetComponent, bKeep);
		K2_OnFocus.Broadcast(TargetComponent, bKeep);
	}
	bKeepLookAt = bIsValid && bKeep;
	bUseTurn = bInUseTurn;
	if (bEndTurn)
	{
		bUseTurn = false;
		LastWorldRotator = FRotator::ZeroRotator;
	}
	if (bKeep || bUseTurn)
	{
		TargetLookedAt = TargetComponent;
	}
	else
	{
		TargetLookedAt = nullptr;
	}
}

USceneComponent* UFocusComponent::GetControlledTarget()
{
	return GetOwner()->GetRootComponent() == this ? this : GetAttachParent();
}
