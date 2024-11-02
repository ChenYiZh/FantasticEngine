// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenPlayerComponent.h"

#include "Components/Tween/TweenComponent.h"

// Sets default values for this component's properties
UTweenPlayerComponent::UTweenPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTweenPlayerComponent::PlayAllForward()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->PlayForward();
		}
	}
}

void UTweenPlayerComponent::PlayAllBack()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->PlayBack();
		}
	}
}

void UTweenPlayerComponent::ResetAllToBegin()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->ResetToBegin();
		}
	}
}

void UTweenPlayerComponent::ResetAllToEnd()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->ResetToEnd();
		}
	}
}

void UTweenPlayerComponent::PauseAll()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->Pause();
		}
	}
}

void UTweenPlayerComponent::ResumeAll()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->Resume();
		}
	}
}

void UTweenPlayerComponent::StopAll()
{
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (UTweenComponent* Tween = Cast<UTweenComponent>(Child))
		{
			Tween->Stop();
		}
	}
}
