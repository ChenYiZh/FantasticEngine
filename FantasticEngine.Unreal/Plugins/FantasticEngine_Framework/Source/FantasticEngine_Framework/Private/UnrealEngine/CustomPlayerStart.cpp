// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngine/CustomPlayerStart.h"

#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"

ACustomPlayerStart::ACustomPlayerStart(const FObjectInitializer& ObjectInitializer): APlayerStart(ObjectInitializer)
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Static);
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetupAttachment(DefaultSceneRoot);
	Capsule->SetRelativeLocation(FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight()));
	Capsule->SetShouldUpdatePhysicsVolume(true);

	RootComponent = DefaultSceneRoot;
}

void ACustomPlayerStart::Validate()
{
	UBillboardComponent* LocalGoodSprite = GetGoodSprite();
	UBillboardComponent* LocalBadSprite = GetBadSprite();
	UCapsuleComponent* LocalCapsuleComponent = GetCapsuleComponent();

	if (ShouldBeBased() && (LocalGoodSprite || LocalBadSprite))
	{
		FVector OrigLocation = LocalCapsuleComponent->GetComponentLocation();
		FVector OrigActorLocation = GetActorLocation();
		const float Radius = LocalCapsuleComponent->GetScaledCapsuleRadius();
		FVector const Slice(Radius, Radius, 1.f);

		bool bResult = true;

		// Check for adjustment
		FHitResult Hit(ForceInit);
		const FVector TraceStart = LocalCapsuleComponent->GetComponentLocation();
		const FVector TraceEnd = TraceStart - FVector(
			0.f, 0.f, 4.f * LocalCapsuleComponent->GetScaledCapsuleHalfHeight());
		GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_Pawn,
		                                 FCollisionShape::MakeBox(Slice),
		                                 FCollisionQueryParams(SCENE_QUERY_STAT(NavObjectBase_Validate), false, this));
		if (Hit.bBlockingHit)
		{
			const FVector HitLocation = TraceStart + (TraceEnd - TraceStart) * Hit.Time;
			FVector Dest = HitLocation + FVector(0.f, 0.f, /*LocalCapsuleComponent->GetScaledCapsuleHalfHeight()*/
			                                     - 2.f);

			// Move actor (TEST ONLY) to see if navigation point moves
			TeleportTo(Dest, GetActorRotation(), false, true);

			// If only adjustment was down towards the floor, then it is a valid placement
			FVector NewLocation = LocalCapsuleComponent->GetComponentLocation();
			bResult = (NewLocation.X == OrigLocation.X &&
				NewLocation.Y == OrigLocation.Y &&
				NewLocation.Z <= OrigLocation.Z);

			// Move actor back to original position
			TeleportTo(OrigActorLocation, GetActorRotation(), false, true);
		}

		// Update sprites by result
		if (LocalGoodSprite)
		{
			LocalGoodSprite->SetVisibility(bResult);
		}
		if (LocalBadSprite)
		{
			LocalBadSprite->SetVisibility(!bResult);
		}
	}

	// Force update of icon
	MarkComponentsRenderStateDirty();
}
