// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenSplineComponent.h"

void UTweenSplineComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (!IsValid(SplineActor)) { return; }
	if (const USplineComponent* Spline = SplineActor->GetComponentByClass<USplineComponent>())
	{
		const FTransform Transform = Spline->GetTransformAtTime(Alpha, bUseWorldType
			                                                               ? ESplineCoordinateSpace::World
			                                                               : ESplineCoordinateSpace::Local);

		if (bLocation && bRotation && bScale)
		{
			if (bUseWorldType)
			{
				Component->SetWorldTransform(Transform);
			}
			else
			{
				Component->SetRelativeTransform(Transform);
			}
			return;
		}

		if (bLocation)
		{
			if (bUseWorldType)
			{
				Component->SetWorldLocation(Transform.GetLocation());
			}
			else
			{
				Component->SetRelativeLocation(Transform.GetLocation());
			}
		}

		if (bRotation)
		{
			if (bUseWorldType)
			{
				Component->SetWorldRotation(Transform.GetRotation());
			}
			else
			{
				Component->SetRelativeRotation(Transform.GetRotation());
			}
		}

		if (bScale)
		{
			if (bUseWorldType)
			{
				Component->SetWorldScale3D(Transform.GetScale3D());
			}
			else
			{
				Component->SetRelativeScale3D(Transform.GetScale3D());
			}
		}
	}
}
