// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XR/XRMotionControllerComponent.h"

#include "XRDeviceVisualizationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Log/FEConsole.h"

void UXRMotionControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	XRCamera = GetOwner()->GetComponentByClass<UXRCameraComponent>();
}

void UXRMotionControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!XRCamera.IsValid()) { return; }
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (!Pawn->IsLocallyControlled())
		{
			return;
		}
	}

	if (UGameplayStatics::GetPlayerControllerFromID(this, 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		//UFEConsole::Write(TEXT("MouseDown"),true,DeltaTime);
		return;
	}

	if (LastTransform.Equals(GetRelativeTransform()))
	{
		return;
	}

	const FTransform RealWorldTransform = GetRelativeTransform();
	const FTransform XRCameraRealWorldTransform = XRCamera->GetRealWorldTransform();

	const FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(
		RealWorldTransform, XRCameraRealWorldTransform);

	const FTransform WorldTransform = RelativeTransform * XRCamera->GetComponentTransform();
	SetWorldTransform(WorldTransform);
	LastTransform = GetRelativeTransform();
}

void UXRMotionControllerComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);
}
