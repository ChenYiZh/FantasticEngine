// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XR/XRCameraComponent.h"

#include "IXRCamera.h"
#include "IXRTrackingSystem.h"
#include "Components/Pawn/VRPawnOperationsComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UXRCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateTransformInfo();

	OperationsComponent = GetOwner()->GetComponentByClass<UVRPawnOperationsComponent>();
	if (OperationsComponent.IsValid())
	{
		if (IsXRHeadTrackedCamera())
		{
			OperationsComponent->Settings.bLockLookUp = OperationsComponent->Settings.bLockTurn = true;
		}
	}
}

void UXRCameraComponent::HandleXRCamera()
{
	if (bLockToHmd)
	{
		Super::HandleXRCamera();
		return;
	}
	IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
	auto XRCamera = XRSystem->GetXRCamera();
	const FTransform ParentWorld = CalcNewComponentToWorld(FTransform());

	XRCamera->SetupLateUpdate(ParentWorld, this, bLockToHmd == 0);

	FQuat Orientation;
	FVector Position;
	if (XRCamera->UpdatePlayerCamera(Orientation, Position))
	{
		RealWorldTransform = FTransform(Orientation, Position);
		const FTransform FinalTransform = RealWorldTransform * ActorOrigin;

		const bool bOriginIsNotRoot = OperationsComponent.IsValid() && !OperationsComponent->VROriginIsRoot();
		USceneComponent* Origin = bOriginIsNotRoot ? OperationsComponent->GetVROrigin() : nullptr;

		if (bInitialized)
		{
			AActor* Owner = GetOwner();

			// 设置旋转角度
			if (bUseOriginTurn)
			{
				if (bUseOriginLookUp)
				{
					if (bOriginIsNotRoot)
					{
						Origin->SetRelativeRotation(RealWorldTransform.Rotator());
					}
					else
					{
						Owner->SetActorRotation(FinalTransform.Rotator(), ETeleportType::TeleportPhysics);
					}
				}
				else
				{
					if (bOriginIsNotRoot)
					{
						FRotator NewWorldRotation = RealWorldTransform.Rotator();
						const FRotator NewOriginRotation(0, NewWorldRotation.Yaw + OffsetRotation, 0);
						Origin->SetRelativeRotation(NewOriginRotation);
						//SetWorldRotation(NewWorldRotation);
						NewWorldRotation.Yaw = 0;
						SetRelativeRotation(NewWorldRotation);
					}
					else
					{
						const FRotator NewWorldRotation = FinalTransform.Rotator();
						const FRotator NewActorRotation(0, NewWorldRotation.Yaw, 0);
						Owner->SetActorRotation(NewActorRotation);
						//SetRelativeRotation(Orientation.Rotator());
						SetWorldRotation(NewWorldRotation, false, nullptr, ETeleportType::TeleportPhysics);
					}
				}
			}
			else
			{
				SetRelativeRotation(Orientation.Rotator());
			}

			// 设置坐标
			if (!bLockXYZ)
			{
				const double Height = Position.Z;
				FVector WorldLocation = FinalTransform.GetLocation();
				WorldLocation.Z -= Height;
				if (bUseOriginMove)
				{
					if (bOriginIsNotRoot)
					{
						Origin->SetRelativeLocation(
							Position + FVector::DownVector * Height + FVector::UpVector * Origin->GetRelativeLocation().
							Z + OffsetLocation);
					}
					else
					{
						Owner->SetActorLocation(WorldLocation, false, nullptr, ETeleportType::TeleportPhysics);
					}
				}
				else
				{
					Position.Z = 0;
					SetRelativeLocation(GetRelativeLocation() + Position);
				}
				if (!bLockZ)
				{
					if (bUseCameraHeight)
					{
						FVector LocalRelativeLocation = GetRelativeLocation();
						LocalRelativeLocation.Z = Height;
						SetRelativeLocation(LocalRelativeLocation);
					}
					else
					{
						if (bOriginIsNotRoot)
						{
							Origin->SetRelativeLocation(Position);
						}
						else
						{
							FVector Location = Owner->GetActorLocation();
							Location.Z += Height;
							Owner->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
						}
					}
				}
			}
		}
		bInitialized = true;
	}

	XRCamera->OverrideFOV(this->FieldOfView);
}

void UXRCameraComponent::UpdateTransformInfo()
{
	ActorOrigin = GetOwner()->GetTransform();
	RelativeOrigin = GetRelativeTransform();
}

void UXRCameraComponent::SnapTurn(float Angle)
{
	FRotator NewActorRotation = ActorOrigin.Rotator();
	NewActorRotation.Yaw += Angle;
	ActorOrigin = FTransform(NewActorRotation, ActorOrigin.GetLocation());
	FRotator NewRelativeRotation = RelativeOrigin.Rotator();
	NewRelativeRotation.Yaw += Angle;
	RelativeOrigin = FTransform(NewRelativeRotation, RelativeOrigin.GetLocation());
}

void UXRCameraComponent::MoveTo(FVector DestLocation)
{
	FVector NewLocation = DestLocation - GetOwner()->GetActorLocation() + ActorOrigin.GetLocation();
	if (const USceneComponent* Parent = GetAttachParent())
	{
		if (Parent != GetOwner()->GetRootComponent())
		{
			float Height = 0;
			Height = -Parent->GetRelativeLocation().Z;
			NewLocation.Z += Height;
		}
	}
	ActorOrigin.SetLocation(NewLocation);
}
