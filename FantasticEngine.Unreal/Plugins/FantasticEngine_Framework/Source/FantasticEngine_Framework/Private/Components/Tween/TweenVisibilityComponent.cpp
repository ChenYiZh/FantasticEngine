// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenVisibilityComponent.h"

void UTweenVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();
	OnFinished.AddUObject(this, &UTweenVisibilityComponent::PlayOnFinished);
}

void UTweenVisibilityComponent::PlayOnFinished()
{
	GetComponent()->SetHiddenInGame(!bVisibleOnFinished);
}
