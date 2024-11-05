// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenTransformComponent.h"

void UTweenTransformComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	const FTransform TransformFrom = From;
	const FTransform TransformTo = To;
	const FVector Location = FMath::Lerp(TransformFrom.GetTranslation(), TransformTo.GetTranslation(), Alpha);
	const FRotator Rotation = FMath::Lerp(TransformFrom.Rotator(), TransformTo.Rotator(), Alpha);
	const FVector Scale = FMath::Lerp(TransformFrom.GetScale3D(), TransformTo.GetScale3D(), Alpha);
	const FTransform Transform(Rotation, Location, Scale);
	if (bGlobal)
	{
		Component->SetWorldTransform(Transform);
	}
	else
	{
		Component->SetRelativeTransform(Transform);
	}
}
