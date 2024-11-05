// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenFloatComponent.h"

void UTweenFloatComponent::Play(float Alpha)
{
	Alpha = From + (To - From) * Alpha;
	Super::Play(GetComponent(), Alpha);
	OnPlay.Broadcast(GetComponent(), Alpha);
	K2_OnPlay.Broadcast(GetComponent(), Alpha);
}
