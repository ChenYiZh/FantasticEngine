// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenRotationComponent.h"

void UTweenRotationComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	FRotator Rotation;
	if (bUseRotatorLerp)
	{
		Rotation = FMath::Lerp(FRotator(From.X, From.Z, From.Y), FRotator(To.X, To.Z, To.Y), Alpha);
	}
	else
	{
		const FVector VectorLerp = FMath::Lerp(From, To, Alpha);
		Rotation = FRotator(VectorLerp.X, VectorLerp.Z, VectorLerp.Y);
	}
	if (bGlobal)
	{
		Component->SetWorldRotation(Rotation);
	}
	else
	{
		Component->SetRelativeRotation(Rotation);
	}
}
