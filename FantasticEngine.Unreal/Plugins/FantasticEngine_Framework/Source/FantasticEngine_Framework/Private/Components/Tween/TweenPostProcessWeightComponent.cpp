// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenPostProcessWeightComponent.h"

void UTweenPostProcessWeightComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (PostProcess.IsValid())
	{
		Component = PostProcess.Get();
	}
	if (UPostProcessComponent* PostProcessComponent = Cast<UPostProcessComponent>(Component))
	{
		PostProcessComponent->BlendWeight = Alpha;
	}
}
