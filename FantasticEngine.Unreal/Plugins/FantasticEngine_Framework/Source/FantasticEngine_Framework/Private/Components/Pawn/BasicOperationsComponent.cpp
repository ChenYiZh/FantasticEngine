// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pawn/BasicOperationsComponent.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values for this component's properties
UBasicOperationsComponent::UBasicOperationsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UBasicOperationsComponent::MoveForward_Implementation(float Value, bool bImmediately)
{
	AActor* Owner = GetOwner();
	APawn* Pawn = Cast<APawn>(Owner);
	const bool bIsPawnValid = IsValid(Pawn);
	const FRotator ControlRotation = bIsPawnValid ? Pawn->GetControlRotation() : FRotator::ZeroRotator;
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
	if (bImmediately)
	{
		FRotator OwnerRotation = Owner->GetActorRotation();
		OwnerRotation.Yaw = ControlRotation.Yaw + (Value > 0 ? 0 : 180.f);
		Owner->SetActorRotation(OwnerRotation);
		if (Settings.bUseMovementInput && bIsPawnValid)
		{
			Pawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
		}
		else
		{
			Owner->SetActorLocation(Owner->GetActorLocation()
				+ Owner->GetActorForwardVector() * Value * Settings.MoveSpeed
				* GetWorld()->GetDeltaSeconds());
		}
		OnMoveForward.Broadcast(Value, bImmediately);
		K2_OnMoveForward.Broadcast(Value, bImmediately);
		return;
	}
	if (Settings.bUseMovementInput && bIsPawnValid)
	{
		Pawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
	}
	else
	{
		Owner->SetActorLocation(Owner->GetActorLocation()
			+ Owner->GetActorForwardVector() * Value * Settings.MoveSpeed
			* GetWorld()->GetDeltaSeconds());
	}
	OnMoveForward.Broadcast(Value, bImmediately);
	K2_OnMoveForward.Broadcast(Value, bImmediately);
}

void UBasicOperationsComponent::MoveRight_Implementation(float Value, bool bImmediately)
{
	AActor* Owner = GetOwner();
	APawn* Pawn = Cast<APawn>(Owner);
	const bool bIsPawnValid = IsValid(Pawn);
	const FRotator ControlRotation = bIsPawnValid ? Pawn->GetControlRotation() : FRotator::ZeroRotator;
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
	if (bImmediately)
	{
		FRotator OwnerRotation = Owner->GetActorRotation();
		OwnerRotation.Yaw = ControlRotation.Yaw + (Value > 0 ? 90.f : -90.f);
		Owner->SetActorRotation(OwnerRotation);
		if (Settings.bUseMovementInput && bIsPawnValid)
		{
			Pawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
		}
		else
		{
			Pawn->SetActorLocation(Pawn->GetActorLocation()
				+ Pawn->GetActorRightVector() * Value * Settings.MoveSpeed
				* GetWorld()->GetDeltaSeconds());
		}
		OnMoveRight.Broadcast(Value, bImmediately);
		K2_OnMoveRight.Broadcast(Value, bImmediately);
		return;
	}
	if (Settings.bUseMovementInput && bIsPawnValid)
	{
		Pawn->AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
	}
	else
	{
		Pawn->SetActorLocation(Pawn->GetActorLocation()
			+ Pawn->GetActorRightVector() * Value * Settings.MoveSpeed
			* GetWorld()->GetDeltaSeconds());
	}
	OnMoveRight.Broadcast(Value, bImmediately);
	K2_OnMoveRight.Broadcast(Value, bImmediately);
}

void UBasicOperationsComponent::LookUp_Implementation(float Value)
{
	if (Settings.bLockLookUp) { return; }
	AActor* Owner = GetOwner();
	APawn* Pawn = Cast<APawn>(Owner);
	if (IsValid(Pawn) && (Pawn->bUseControllerRotationPitch || Settings.bUseControllerLookUp))
	{
		Pawn->AddControllerPitchInput(Value);
	}
	else if (UCameraComponent* LocalCamera = GetCamera())
	{
		const FVector Forward = LocalCamera->GetForwardVector();
		const FVector FinalForward = UKismetMathLibrary::RotateAngleAxis(
			Forward, Value * Settings.BaseLookUpRate * GetWorld()->GetDeltaSeconds(), LocalCamera->GetRightVector());
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FinalForward);
		LocalCamera->SetWorldRotation(Rotation);
	}
	else
	{
		const FVector Forward = Owner->GetActorForwardVector();
		const FVector FinalForward = UKismetMathLibrary::RotateAngleAxis(
			Forward, Value * Settings.BaseLookUpRate * GetWorld()->GetDeltaSeconds(), Owner->GetActorRightVector());
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FinalForward);
		Owner->SetActorRotation(Rotation);
	}
	OnLookUp.Broadcast(Value);
	K2_OnLookUp.Broadcast(Value);
}

void UBasicOperationsComponent::Turn_Implementation(float Value)
{
	if (Settings.bLockTurn) { return; }
	AActor* Owner = GetOwner();
	APawn* Pawn = Cast<APawn>(Owner);
	if (IsValid(Pawn) && (Pawn->bUseControllerRotationYaw || Settings.bUseControllerTurn))
	{
		Pawn->AddControllerYawInput(Value);
	}
	else
	{
		const FVector Forward = Owner->GetActorForwardVector();
		const FVector FinalForward = UKismetMathLibrary::RotateAngleAxis(
			Forward, Value * Settings.BaseTurnRate * GetWorld()->GetDeltaSeconds(), Owner->GetActorUpVector());
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FinalForward);
		Owner->SetActorRotation(Rotation);
	}
	OnTurn.Broadcast(Value);
	K2_OnTurn.Broadcast(Value);
}
