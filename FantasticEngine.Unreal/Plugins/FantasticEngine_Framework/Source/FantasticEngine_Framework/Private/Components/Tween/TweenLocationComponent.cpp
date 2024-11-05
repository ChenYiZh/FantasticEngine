// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenLocationComponent.h"

void UTweenLocationComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (bGlobal)
	{
		Component->SetWorldLocation(FMath::Lerp(From, To, Alpha));
	}
	else
	{
		Component->SetRelativeLocation(FMath::Lerp(From, To, Alpha));
	}
}
