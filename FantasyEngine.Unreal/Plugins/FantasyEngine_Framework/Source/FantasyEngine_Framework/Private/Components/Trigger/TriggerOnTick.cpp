// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Trigger/TriggerOnTick.h"


// Sets default values for this component's properties
UTriggerOnTick::UTriggerOnTick()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerOnTick::BeginPlay()
{
	Super::BeginPlay();
	OverlapChecker = Cast<UPrimitiveComponent>(GetAttachParent());
}


// Called every frame
void UTriggerOnTick::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(OverlapChecker)) { return; }
	TArray<UPrimitiveComponent*> Current;
	OverlapChecker->GetOverlappingComponents(Current);

	for (UPrimitiveComponent* Component : LastOverlapping)
	{
		if (!Current.Contains(Component))
		{
			if (OnEndOverlap.IsBound())
			{
				OnEndOverlap.Broadcast(Component->GetOwner(), Component);
			}
		}
	}

	for (UPrimitiveComponent* Component : Current)
	{
		if (!LastOverlapping.Contains(Component))
		{
			if (OnBeginOverlap.IsBound())
			{
				OnBeginOverlap.Broadcast(Component->GetOwner(), Component);
			}
		}
	}
	LastOverlapping = Current;
}
