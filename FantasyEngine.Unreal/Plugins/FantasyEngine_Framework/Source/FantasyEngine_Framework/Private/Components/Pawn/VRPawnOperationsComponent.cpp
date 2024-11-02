// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pawn/VRPawnOperationsComponent.h"

#include "IXRCamera.h"
#include "IXRTrackingSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Camera/CameraComponent.h"
#include "Components/XR/GrabComponent.h"
#include "Components/XR/XRCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log/FEConsole.h"

// Sets default values
UVRPawnOperationsComponent::UVRPawnOperationsComponent()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
}

void UVRPawnOperationsComponent::MoveTo_Implementation(FVector NewLocation, bool bOnlyRoot)
{
	UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera());
	if (IsValid(XRCamera) && VROriginIsRoot())
	//if (IsValid(XRCamera))
	{
		if (VROriginIsRoot())
		{
			if (bOnlyRoot)
			{
				XRCamera->ActorOrigin.SetLocation(NewLocation);
			}
			else
			{
				XRCamera->MoveTo(NewLocation);
			}
		}
		else
		{
			// USceneComponent* Origin = GetVROrigin();
			// FVector RelativeLocation = Origin->GetRelativeLocation() - XRCamera->OffsetLocation;
			//
			// FVector TargetLocation = Origin->GetAttachParent()->GetComponentTransform().
			//                                  InverseTransformPositionNoScale(
			// 	                                 ProjectedTeleportLocation);
			// XRCamera->OffsetLocation = TargetLocation - RelativeLocation;
			//
			// XRCamera->OffsetLocation.Z = 0;
			//
			//
			// UFEConsole::WriteInfo(XRCamera->OffsetLocation.ToString());
		}
	}
	else if (VROriginIsRoot())
	{
		if (bOnlyRoot)
		{
			GetOwner()->SetActorLocation(NewLocation);
		}
		else
		{
			UCameraComponent* LocalCamera = GetCamera();
			const FVector CameraOriginLocation = LocalCamera->GetAttachParent()->GetComponentLocation();
			const FVector OwnerLocation = GetOwner()->GetActorLocation();
			const double DeltaZ = OwnerLocation.Z - CameraOriginLocation.Z;
			//FVector NewLocation = ProjectedTeleportLocation;
			NewLocation.Z += DeltaZ;
			GetOwner()->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	else
	{
		if (bOnlyRoot)
		{
			GetOwner()->SetActorLocation(NewLocation);
		}
		else
		{
			UCameraComponent* LocalCamera = GetCamera();
			const FVector CameraOriginLocation = LocalCamera->GetAttachParent()->GetComponentLocation();
			const FVector OwnerLocation = GetOwner()->GetActorLocation();
			NewLocation = NewLocation - CameraOriginLocation + OwnerLocation;
			GetOwner()->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
		}
	}
	// else
	// {
	// 	UCameraComponent* LocalCamera = GetCamera();
	// 	const FVector CameraOriginLocation = LocalCamera->GetAttachParent()->GetComponentLocation();
	// 	const FVector OwnerLocation = GetVROrigin()->GetComponentLocation();
	// 	const double DeltaZ = OwnerLocation.Z - CameraOriginLocation.Z;
	// 	FVector NewLocation = ProjectedTeleportLocation;
	// 	NewLocation.Z += DeltaZ;
	// 	GetVROrigin()->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	// }

	// if (UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	// {
	// 	XRCamera->MoveTo(ProjectedTeleportLocation);
	// }
	// else
	// {
	// 	UCameraComponent* LocalCamera = GetCamera();
	// 	FVector CameraRelativeLocation;
	// 	if (IsValid(LocalCamera))
	// 	{
	// 		CameraRelativeLocation = LocalCamera->GetRelativeLocation();
	// 		CameraRelativeLocation.Z = 0;
	// 	}
	// 	FRotator ActorRotation = GetOwner()->GetActorRotation();
	// 	ActorRotation.Roll = ActorRotation.Pitch = 0;
	// 	FVector DestLocation = ProjectedTeleportLocation - ActorRotation.RotateVector(CameraRelativeLocation);
	//
	// 	GetOwner()->TeleportTo(DestLocation, ActorRotation);
	// }
}

void UVRPawnOperationsComponent::MoveForward_Implementation(float Value, bool bImmediately)
{
	if (UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	{
		if (XRCamera->bLockXYZ)
		{
			OnMoveForward.Broadcast(Value, bImmediately);
			K2_OnMoveForward.Broadcast(Value, bImmediately);
			return;
		}
		if (XRCamera->bLockToHmd)
		{
			XRCamera->AddRelativeLocation(FVector::ForwardVector * Value * Settings.MoveSpeed
				* GetWorld()->GetDeltaSeconds());

			OnMoveForward.Broadcast(Value, bImmediately);
			K2_OnMoveForward.Broadcast(Value, bImmediately);
			return;
		}
	}
	if (VROriginIsRoot())
	{
		Super::MoveForward_Implementation(Value, bImmediately);
		return;
	}
	USceneComponent* LocalVROrigin = GetVROrigin();
	FVector Forward = GetCamera()->GetForwardVector();
	Forward.Z = 0;
	LocalVROrigin->AddWorldOffset(Forward.GetSafeNormal() * Value * Settings.MoveSpeed
		* GetWorld()->GetDeltaSeconds());

	OnMoveForward.Broadcast(Value, bImmediately);
	K2_OnMoveForward.Broadcast(Value, bImmediately);
}

void UVRPawnOperationsComponent::MoveRight_Implementation(float Value, bool bImmediately)
{
	if (UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	{
		if (XRCamera->bLockXYZ)
		{
			OnMoveRight.Broadcast(Value, bImmediately);
			K2_OnMoveRight.Broadcast(Value, bImmediately);
			return;
		}
		if (XRCamera->bLockToHmd)
		{
			XRCamera->AddRelativeLocation(FVector::RightVector * Value * Settings.MoveSpeed
				* GetWorld()->GetDeltaSeconds());
			OnMoveRight.Broadcast(Value, bImmediately);
			K2_OnMoveRight.Broadcast(Value, bImmediately);
			return;
		}
	}
	if (VROriginIsRoot())
	{
		Super::MoveRight_Implementation(Value, bImmediately);
		return;
	}
	USceneComponent* LocalVROrigin = GetVROrigin();
	FVector Right = GetCamera()->GetRightVector();
	Right.Z = 0;
	LocalVROrigin->AddWorldOffset(Right.GetSafeNormal() * Value * Settings.MoveSpeed
		* GetWorld()->GetDeltaSeconds());

	OnMoveRight.Broadcast(Value, bImmediately);
	K2_OnMoveRight.Broadcast(Value, bImmediately);
}

void UVRPawnOperationsComponent::Turn_Implementation(float Value)
{
	USceneComponent* Origin = GetCamera();
	if (const UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	{
		if (XRCamera->bUseOriginTurn)
		{
			Origin = GetVROrigin();
		}
	}
	const FVector Forward = Origin->GetForwardVector();
	const FVector FinalForward = UKismetMathLibrary::RotateAngleAxis(
		Forward, Value * Settings.BaseTurnRate * GetWorld()->GetDeltaSeconds(), Origin->GetUpVector());
	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FinalForward);
	Origin->SetWorldRotation(Rotation);
	OnTurn.Broadcast(Value);
	K2_OnTurn.Broadcast(Value);
}

void UVRPawnOperationsComponent::LookUp_Implementation(float Value)
{
	USceneComponent* Origin = GetCamera();
	if (UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	{
		if (XRCamera->bUseOriginLookUp)
		{
			Origin = GetVROrigin();
		}
	}
	const FVector Forward = Origin->GetForwardVector();
	const FVector FinalForward = UKismetMathLibrary::RotateAngleAxis(
		Forward, Value * Settings.BaseLookUpRate * GetWorld()->GetDeltaSeconds(), Origin->GetRightVector());
	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, FinalForward);
	Origin->SetWorldRotation(Rotation);
	OnLookUp.Broadcast(Value);
	K2_OnLookUp.Broadcast(Value);
}

void UVRPawnOperationsComponent::SnapTurn_Implementation(bool bRightTurn)
{
	const float LocalYawDelta = bRightTurn ? VRSettings.SnapTurnDegrees : -VRSettings.SnapTurnDegrees;
	UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera());
	if (IsValid(XRCamera) && VROriginIsRoot())
	//if (IsValid(XRCamera))
	{
		if (VROriginIsRoot())
		{
			XRCamera->SnapTurn(LocalYawDelta);
		}
		else
		{
			//XRCamera->OffsetRotation += LocalYawDelta;
		}
	}
	else // if (VROriginIsRoot())
	{
		const UCameraComponent* LocalCamera = GetCamera();
		const FVector CameraLocation = LocalCamera->GetComponentLocation();
		GetOwner()->AddActorWorldRotation(FRotator(0, LocalYawDelta, 0));
		const FVector NewCameraLocation = LocalCamera->GetComponentLocation();
		const FVector DeltaLocation = CameraLocation - NewCameraLocation;
		GetOwner()->AddActorWorldOffset(DeltaLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	// else
	// {
	// 	const UCameraComponent* LocalCamera = GetCamera();
	// 	const FVector CameraLocation = LocalCamera->GetComponentLocation();
	// 	GetVROrigin()->AddWorldRotation(FRotator(0, LocalYawDelta, 0));
	// 	const FVector NewCameraLocation = LocalCamera->GetComponentLocation();
	// 	const FVector DeltaLocation = CameraLocation - NewCameraLocation;
	// 	GetVROrigin()->AddWorldOffset(DeltaLocation, false, nullptr, ETeleportType::TeleportPhysics);
	// }


	// if (UXRCameraComponent* XRCamera = Cast<UXRCameraComponent>(GetCamera()))
	// {
	// 	XRCamera->SnapTurn(LocalYawDelta);
	// }
	// else
	// {
	// 	const UCameraComponent* LocalCamera = GetCamera();
	// 	const FVector LocalCameraLocation = LocalCamera->GetComponentLocation();
	// 	const FTransform LocalCameraRelativeTransform = LocalCamera->GetRelativeTransform();
	// 	const FRotator CombinedRotation = UKismetMathLibrary::ComposeRotators(
	// 		GetOwner()->GetActorRotation(), FRotator(0, 0, LocalYawDelta));
	// 	const FTransform LocalToWorldTransform = FTransform(CombinedRotation, GetOwner()->GetActorLocation());
	// 	GetOwner()->AddActorWorldRotation(FRotator(0, 0, LocalYawDelta));
	// 	const FTransform NewTransform = LocalCameraRelativeTransform * LocalToWorldTransform;
	// 	GetOwner()->SetActorLocation(LocalCameraLocation - NewTransform.GetLocation() + GetOwner()->GetActorLocation());
	// }
}

void UVRPawnOperationsComponent::Teleport_Implementation(ETeleportState TeleportState)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!IsValid(Pawn))
	{
		return;
	}
	if (TeleportState == ETeleportState::Started)
	{
		bTeleportTraceActive = true;
		bValidTeleportLocation = true;
		if (VRSettings.TeleportTraceNiagaraSystem.IsValid())
		{
			VRSettings.TeleportTraceNiagaraSystem->SetVisibility(true);
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			TeleportVisualizerReference = GetWorld()->SpawnActor<AActor>(
				VRSettings.TeleportVisualizerClass, SpawnParameters);
		}
	}
	else if (TeleportState == ETeleportState::Triggered)
	{
		if (!IsValid(VRSettings.TeleportMotionAim))
		{
			return;
		}
		FVector StartPos = VRSettings.TeleportMotionAim->GetComponentLocation();
		FVector ForwardVector = VRSettings.TeleportMotionAim->GetForwardVector();
		constexpr float LocalTeleportLaunchSpeed = 650.f;
		constexpr float LocalTeleportProjectileRadius = 3.6f;
		constexpr float LocalNavMeshCeilHeight = 8.f;
		TArray<AActor*> ActorsToIgnore{GetOwner()};

		FHitResult OutHit;
		TArray<FVector> TeleportTracePathPositions;
		FVector OutLastTraceDestination;
		UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
			this,
			OutHit, TeleportTracePathPositions, OutLastTraceDestination,
			StartPos,
			LocalTeleportLaunchSpeed * ForwardVector,
			true,
			LocalTeleportProjectileRadius,
			TeleportObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			0
		);
		TeleportTracePathPositions.Insert(StartPos, 0);

		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(this);
		FVector OutLocation;
		// bool bSuccess = NavigationSystem->K2_ProjectPointToNavigation(this, OutHit.Location, OutLocation, nullptr,
		//                                                               nullptr);
		//FVector ProjectedLocation = OutLocation;
		//ProjectedLocation.Z -= LocalNavMeshCeilHeight;

		bool bSuccess = true;
		FVector ProjectedLocation = OutHit.Location;

		ProjectedTeleportLocation = ProjectedLocation;

		bool bValidReference = IsValid(TeleportVisualizerReference);

		if (bValidTeleportLocation != bSuccess)
		{
			bValidTeleportLocation = bSuccess;
			if (bValidReference)
			{
				TeleportVisualizerReference->GetRootComponent()->SetVisibility(bValidTeleportLocation, true);
			}
		}

		if (bValidReference)
		{
			TeleportVisualizerReference->SetActorLocation(ProjectedTeleportLocation);
		}

		if (VRSettings.TeleportTraceNiagaraSystem.IsValid())
		{
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
				VRSettings.TeleportTraceNiagaraSystem.Get(),TEXT("User.PointArray"), TeleportTracePathPositions);
		}
	}
	else if (TeleportState == ETeleportState::Completed)
	{
		bTeleportTraceActive = false;
		if (IsValid(TeleportVisualizerReference))
		{
			TeleportVisualizerReference->Destroy();
		}
		VRSettings.TeleportTraceNiagaraSystem->SetVisibility(false);

		//Try Teleport
		if (bValidTeleportLocation)
		{
			bValidTeleportLocation = false;

			MoveTo(ProjectedTeleportLocation);

			OnTeleported.Broadcast(ProjectedTeleportLocation);
			K2_OnTeleported.Broadcast(ProjectedTeleportLocation);
		}
	}
}

void UVRPawnOperationsComponent::Grab_Implementation(bool bLeft, bool bStarted, UGrabComponent* MouseGrabComponent)
{
	GrabOnServer(bLeft, bStarted, MouseGrabComponent);
}

void UVRPawnOperationsComponent::GrabOnServer_Implementation(bool bLeft, bool bStarted,
                                                             UGrabComponent* MouseGrabComponent)
{
	if (bStarted)
	{
		UMotionControllerComponent* MotionController = bLeft
			                                               ? (VRSettings.MotionControllerLeft.IsValid()
				                                                  ? VRSettings.MotionControllerLeft.Get()
				                                                  : nullptr)
			                                               : (VRSettings.MotionControllerRight.IsValid()
				                                                  ? VRSettings.MotionControllerRight.Get()
				                                                  : nullptr);
		if (!IsValid(MotionController))
		{
			return;
		}

		UGrabComponent* NearestGrabComponent = MouseGrabComponent;
		if (IsValid(NearestGrabComponent))
		{
			MotionController->SetWorldLocation(NearestGrabComponent->GetComponentLocation());
		}
		else
		{
			//Get Grab Component Near Motion Controller
			FVector LocalGripPosition = MotionController->GetComponentLocation();
			constexpr float GrabRadiusFromGripPosition = 6.f;
			const TArray<AActor*> ActorsToIgnore;

			FHitResult OutHit;
			if (UKismetSystemLibrary::SphereTraceSingleForObjects(
				this,
				LocalGripPosition,
				LocalGripPosition,
				GrabRadiusFromGripPosition,
				GrabObjectTypes,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				OutHit,
				true
			))
			{
				TArray<UGrabComponent*> LocalGrabComponents;
				OutHit.GetActor()->GetComponents<UGrabComponent>(LocalGrabComponents);
				if (LocalGrabComponents.Num() > 0)
				{
					float LocalNearestComponentDistance = 9999999.f;
					for (UGrabComponent* LocalGrabComponent : LocalGrabComponents)
					{
						float Distance = (LocalGrabComponent->GetComponentLocation() - LocalGripPosition).
							SquaredLength();
						if (Distance <= LocalNearestComponentDistance)
						{
							LocalNearestComponentDistance = Distance;
							NearestGrabComponent = LocalGrabComponent;
						}
					}
				}
			}
		}
		//

		if (IsValid(NearestGrabComponent))
		{
			//Try Grab
			if (NearestGrabComponent->TryGrab(MotionController))
			{
				MulticastOnGrab(bLeft, MotionController->GetComponentLocation(), NearestGrabComponent);
			}
		}
	}
	else
	{
		UGrabComponent* GrabComponent = bLeft ? HeldComponentLeft : HeldComponentRight;
		//UFEConsole::Write(TEXT("End Started"));
		if (!IsValid(GrabComponent))
		{
			//UFEConsole::Write(TEXT("End Started None"));
			return;
		}
		MulticastRelease(bLeft, GrabComponent);
	}
}

void UVRPawnOperationsComponent::MulticastOnGrab_Implementation(bool bLeft, FVector WorldLocation,
                                                                UGrabComponent* GrabComponent)
{
	OnGrab(bLeft, WorldLocation, GrabComponent);
}

void UVRPawnOperationsComponent::OnGrab_Implementation(bool bLeft, FVector WorldLocation,
                                                       UGrabComponent* GrabComponent)
{
	if (!UKismetSystemLibrary::IsServer(this))
	{
		UMotionControllerComponent* MotionController = bLeft
			                                               ? (VRSettings.MotionControllerLeft.IsValid()
				                                                  ? VRSettings.MotionControllerLeft.Get()
				                                                  : nullptr)
			                                               : (VRSettings.MotionControllerRight.IsValid()
				                                                  ? VRSettings.MotionControllerRight.Get()
				                                                  : nullptr);
		MotionController->SetWorldLocation(WorldLocation);
		GrabComponent->TryGrab(MotionController, true);
	}
	if (bLeft)
	{
		HeldComponentLeft = GrabComponent;
		if (HeldComponentLeft == HeldComponentRight)
		{
			OnDropped.Broadcast(false, HeldComponentRight);
			K2_OnDropped.Broadcast(false, HeldComponentRight);
			HeldComponentRight = nullptr;
		}
		OnGrabbed.Broadcast(true, HeldComponentLeft);
		K2_OnGrabbed.Broadcast(true, HeldComponentLeft);
	}
	else
	{
		HeldComponentRight = GrabComponent;
		if (HeldComponentLeft == HeldComponentRight)
		{
			OnDropped.Broadcast(true, HeldComponentLeft);
			K2_OnDropped.Broadcast(true, HeldComponentLeft);
			HeldComponentLeft = nullptr;
		}
		OnGrabbed.Broadcast(false, HeldComponentRight);
		K2_OnGrabbed.Broadcast(false, HeldComponentRight);
	}
}

void UVRPawnOperationsComponent::MulticastRelease_Implementation(bool bLeft, UGrabComponent* GrabComponent)
{
	Release(bLeft, GrabComponent);
}

void UVRPawnOperationsComponent::Release_Implementation(bool bLeft, UGrabComponent* GrabComponent)
{
	GrabComponent->TryRelease();
	// if (GrabComponent->TryRelease())
	// {
	if (bLeft)
	{
		HeldComponentLeft = nullptr;
	}
	else
	{
		HeldComponentRight = nullptr;
	}
	//}
	OnDropped.Broadcast(bLeft, GrabComponent);
	K2_OnDropped.Broadcast(bLeft, GrabComponent);
}

void UVRPawnOperationsComponent::MenuToggle_Implementation(bool bLeft)
{
	OnMenuToggle.Broadcast(bLeft);
	K2_OnMenuToggle.Broadcast(bLeft);
}

void UVRPawnOperationsComponent::HandGrasp_Implementation(EHandGraspState GraspState, bool bLeft, float Value)
{
	OnHandGrasp.Broadcast(bLeft, GraspState, Value);
	K2_OnHandGrasp.Broadcast(bLeft, GraspState, Value);
}

void UVRPawnOperationsComponent::HandIndexCurl_Implementation(EHandGraspState GraspState, bool bLeft, float Value)
{
	OnHandIndexCurl.Broadcast(bLeft, GraspState, Value);
	K2_OnHandIndexCurl.Broadcast(bLeft, GraspState, Value);
}

void UVRPawnOperationsComponent::HandPoint_Implementation(EHandGraspState GraspState, bool bLeft)
{
	OnHandPoint.Broadcast(bLeft, GraspState);
	K2_OnHandPoint.Broadcast(bLeft, GraspState);
}

void UVRPawnOperationsComponent::HandThumbUp_Implementation(EHandGraspState GraspState, bool bLeft)
{
	OnHandThumbUp.Broadcast(bLeft, GraspState);
	K2_OnHandThumbUp.Broadcast(bLeft, GraspState);
}
