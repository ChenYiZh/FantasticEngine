// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XR/GrabComponent.h"

#include "Components/Pawn/VRPawnOperationsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log/FEConsole.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetNetAddressable();
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->AutoReceiveInput = EAutoReceiveInput::Player0;

	bLastHeld = bIsHeld;

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		// Set Should Simulate On Drop
		if (PrimitiveComponent->IsAnySimulatingPhysics())
		{
			bSimulateOnDrop = true;
		}

		// Set the Collision Profile of the parent
		PrimitiveComponent->SetCollisionProfileName(TEXT("PhysicsActor"), true);


		PrimitiveComponent->OnClicked.AddDynamic(this, &UGrabComponent::OnMousePressed);
		//PrimitiveComponent->OnReleased.AddDynamic(this, &UGrabComponent::OnMouseReleased);
		//PrimitiveComponent->OnBeginCursorOver.AddDynamic(this, &UGrabComponent::OnBeginMouseHover);
		//PrimitiveComponent->OnEndCursorOver.AddDynamic(this, &UGrabComponent::OnEndMouseHover);
	}
}


void UGrabComponent::OnMousePressed_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	StartLocation = GetOwner()->GetActorLocation();
	if (const APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(this, 0))
	{
		FVector MouseDirection;
		FVector MousePosition;
		if (Controller->DeprojectMousePositionToWorld(MousePosition, MouseDirection))
		{
			bMouseDown = true;
			StartMousePosition = MousePosition;
			StartMouseDirection = MouseDirection;

			const APawn* Pawn = Controller->GetPawn();
			bool bFound = false;
			if (IsValid(Pawn))
			{
				UVRPawnOperationsComponent* Operations = Cast<UVRPawnOperationsComponent>(
					Pawn->GetComponentByClass(UVRPawnOperationsComponent::StaticClass()));
				if (IsValid(Operations))
				{
					const TWeakObjectPtr<UMotionControllerComponent> MotionController = bMouseSimulateRightHand
						? Operations->VRSettings.
						              MotionControllerRight
						: Operations->VRSettings.
						              MotionControllerLeft;
					if (MotionController.IsValid())
					{
						//MotionController.Get()->SetWorldLocation(MousePosition);
						bFound = true;
						Operations->Grab(!bMouseSimulateRightHand, true, this);
					}
				}
			}
			if (!bFound)
			{
				bIsHeld = true;
				GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				OnGrabbed.Broadcast();
				K2_OnGrabbed.Broadcast();
			}
		}
	}
}

void UGrabComponent::OnMouseReleased_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonReleased)
{
	bMouseDown = false;
	bool bFound = false;
	if (const APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(this, 0))
	{
		APawn* Pawn = Controller->GetPawn();
		//UFEConsole::Write(TEXT("OnMouseReleased"),true);
		if (IsValid(Pawn) && IsValid(MotionControllerRef))
		{
			bFound = true;
			UVRPawnOperationsComponent* Operations = Cast<UVRPawnOperationsComponent>(
				Pawn->GetComponentByClass(UVRPawnOperationsComponent::StaticClass()));
			bool bLeft = MotionControllerRef == Operations->VRSettings.MotionControllerLeft.Get();
			//UFEConsole::Write(TEXT("bFound"),true);
			Operations->Grab(bLeft, false);
		}
	}
	if (!bFound)
	{
		bIsHeld = false;
		OnDropped.Broadcast();
		K2_OnDropped.Broadcast();
	}
}

void UGrabComponent::OnBeginMouseHover_Implementation(UPrimitiveComponent* TouchedComponent)
{
	bHover = true;
}

void UGrabComponent::OnEndMouseHover_Implementation(UPrimitiveComponent* TouchedComponent)
{
	bHover = false;
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const bool bCurrentMouseDown = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0)->IsInputKeyDown(
		EKeys::LeftMouseButton);
	if (bLastMouseDown != bCurrentMouseDown)
	{
		bLastMouseDown = bCurrentMouseDown;
		if (bCurrentMouseDown)
		{
			// if (!bMouseDown && bHover)
			// {
			// 	OnMousePressed(Cast<UPrimitiveComponent>(GetAttachParent()), EKeys::LeftMouseButton);
			// }
		}
		else
		{
			if (bMouseDown)
			{
				OnMouseReleased(Cast<UPrimitiveComponent>(GetAttachParent()), EKeys::LeftMouseButton);
			}
		}
	}
	if (bMouseDown)
	{
		const float Distance = FVector::Distance(StartLocation, StartMousePosition);
		if (const APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(this, 0))
		{
			FVector MouseDirection;
			FVector MousePosition;
			if (Controller->DeprojectMousePositionToWorld(MousePosition, MouseDirection))
			{
				FVector NewLocation = MousePosition + MouseDirection * Distance;
				NewLocation += StartLocation - (StartMousePosition + StartMouseDirection * Distance);
				//UFEConsole::Write(TEXT("DeprojectMousePositionToWorld"),true,DeltaTime);
				if (IsValid(MotionControllerRef))
				{
					//UFEConsole::Write(TEXT("IsValid"),true,DeltaTime);
					MotionControllerRef->SetWorldLocation(NewLocation);
				}
				else
				{
					//UFEConsole::Write(TEXT("None"),true,DeltaTime);
					GetOwner()->SetActorLocation(NewLocation);
				}
			}
		}
	}

	if (bLastHeld != bIsHeld)
	{
		bLastHeld = bIsHeld;
		// if(bIsHeld)
		// {
		// 	if(UPrimitiveComponent* ParentComponent=Cast<UPrimitiveComponent>(GetAttachParent()))
		// 	{
		// 		ParentComponent.
		// 	}
		// }
	}
}

bool UGrabComponent::TryGrab_Implementation(UMotionControllerComponent* MotionController, bool bForce)
{
	if (!bForce && bIsHeld)
	{
		if (MotionController->GetOwner() != MotionControllerRef->GetOwner())
		{
			return false;
		}
	}
	if (GrabType == EGrabType::Free || GrabType == EGrabType::Snap)
	{
		SetPrimitiveCompPhysics(false);
		if (AttachParentToMotionController(MotionController))
		{
			bIsHeld = true;
		}
	}
	if (GrabType == EGrabType::Snap)
	{
		// Orient the held Actor to match GrabComponent's relative location
		GetOwner()->SetActorRelativeRotation(GetRelativeRotation().GetInverse());
		GetOwner()->SetActorLocation(
			MotionController->GetComponentLocation() -
			(GetComponentLocation() - GetOwner()->GetActorLocation()));
	}
	else if (GrabType == EGrabType::Custom)
	{
		bIsHeld = true;
	}
	if (bIsHeld)
	{
		MotionControllerRef = MotionController;
		OnGrabbed.Broadcast();
		K2_OnGrabbed.Broadcast();
		if (OnGrabHapticEffect)
		{
			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
			EControllerHand ControllerHand = MotionControllerRef->MotionSource == TEXT("Left")
				                                 ? EControllerHand::Left
				                                 : EControllerHand::Right;
			Controller->PlayHapticEffect(OnGrabHapticEffect, ControllerHand);
		}
		return true;
	}
	return false;
}

bool UGrabComponent::TryRelease_Implementation()
{
	if (GrabType == EGrabType::Free || GrabType == EGrabType::Snap)
	{
		GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (bSimulateOnDrop)
		{
			SetPrimitiveCompPhysics(true);
		}
		// else
		// {
		// 	// GetOwner()->K2_DetachFromActor(
		// 	// 	EDetachmentRule::KeepWorld,
		// 	// 	EDetachmentRule::KeepWorld,
		// 	// 	EDetachmentRule::KeepWorld);
		// 	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// }
		bIsHeld = false;
	}
	else if (GrabType == EGrabType::Custom)
	{
		bIsHeld = false;
	}
	if (bIsHeld)
	{
		return false;
	}
	OnDropped.Broadcast();
	K2_OnDropped.Broadcast();
	return true;
}

void UGrabComponent::SetPrimitiveCompPhysics_Implementation(bool bSimulate)
{
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		PrimitiveComponent->SetSimulatePhysics(bSimulate);
	}
	else
	{
		UFEConsole::WriteErrorWithCategory(
			TEXT("Framework VR"),TEXT("GrabComponent->SetSimulatingParent->Cast To PrimitiveComponent FAILED"));
	}
}

bool UGrabComponent::AttachParentToMotionController_Implementation(UMotionControllerComponent* MotionController)
{
	// if (GetOwner()->K2_AttachToComponent(
	// 	MotionController,
	// 	NAME_None,
	// 	EAttachmentRule::KeepWorld,
	// 	EAttachmentRule::KeepWorld,
	// 	EAttachmentRule::KeepWorld,
	// 	true))
	if (GetOwner()->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform))
	{
		return true;
	}
	FString Log = TEXT("Attaching ") + GetAttachParent()->GetFName().ToString() + TEXT(" to ") + MotionController->
		GetFName().ToString() + TEXT("FAILED - object not grabbed");
	UFEConsole::WriteErrorWithCategory(TEXT("Framework VR"), Log);
	return false;
}
