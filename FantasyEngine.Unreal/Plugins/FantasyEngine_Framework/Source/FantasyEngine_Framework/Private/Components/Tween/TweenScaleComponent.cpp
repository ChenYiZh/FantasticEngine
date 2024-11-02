// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenScaleComponent.h"

void UTweenScaleComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (bGlobal)
	{
		Component->SetWorldScale3D(FMath::Lerp(From, To, Alpha));
	}
	else
	{
		Component->SetRelativeScale3D(FMath::Lerp(From, To, Alpha));
	}
}
