// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenToggleVisibilityComponent.h"

void UTweenToggleVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();
	OnFinished.AddUObject(this, &UTweenToggleVisibilityComponent::PlayOnFinished);
}

void UTweenToggleVisibilityComponent::PlayOnFinished()
{
	if (!bOnEveryLoop)
	{
		GetComponent()->SetHiddenInGame(!GetComponent()->bHiddenInGame);
	}
}

void UTweenToggleVisibilityComponent::OnceFinished_Implementation()
{
	if (!bOnEveryLoop)
	{
		GetComponent()->SetHiddenInGame(!GetComponent()->bHiddenInGame);
	}
}
