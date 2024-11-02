// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenActorComponent.h"

void UTweenActorComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (IsValid(From) && IsValid(To))
	{
		const FTransform TransformFrom = From->GetActorTransform();
		const FTransform TransformTo = To->GetActorTransform();
		const FVector Location = FMath::Lerp(TransformFrom.GetLocation(), TransformTo.GetLocation(), Alpha);
		const FRotator Rotation = FMath::Lerp(TransformFrom.Rotator(), TransformTo.Rotator(), Alpha);
		const FVector Scale = FMath::Lerp(TransformFrom.GetScale3D(), TransformTo.GetScale3D(), Alpha);
		if (bSyncLocation)
		{
			Component->SetWorldLocation(Location);
		}
		if (bSyncRotation)
		{
			Component->SetWorldRotation(Rotation);
		}
		if (bSyncScale)
		{
			Component->SetWorldScale3D(Scale);
		}
	}
}
