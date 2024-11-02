// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RayTrace/PlayerRayTraceComponent.h"

#include "Components/RayTrace/RayTracedComponent.h"

// Sets default values for this component's properties
UPlayerRayTraceComponent::UPlayerRayTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerRayTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UPlayerRayTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnBeginRayTracing();
	if (bEnabled)
	{
		const FVector StartLocation = GetComponentLocation();
		const FVector EndLocation = StartLocation + GetForwardVector() * TraceLength;
		switch (TraceType)
		{
		case EPlayerRayTraceType::TraceChannel:
			{
				if (bMultiRayTrace)
				{
					TArray<FHitResult> Results;
					if (GetWorld()->LineTraceMultiByChannel(Results, StartLocation, EndLocation, TraceChannel))
					{
						for (FHitResult& Result : Results)
						{
							OnRayTraced(Result);
						}
					}
				}
				else
				{
					FHitResult Result;
					if (GetWorld()->LineTraceSingleByChannel(Result, StartLocation, EndLocation, TraceChannel))
					{
						OnRayTraced(Result);
					}
				}
			}
			break;
		case EPlayerRayTraceType::Profile:
			{
				if (bMultiRayTrace)
				{
					TArray<FHitResult> Results;
					if (GetWorld()->LineTraceMultiByProfile(Results, StartLocation, EndLocation, ProfileName))
					{
						for (FHitResult& Result : Results)
						{
							OnRayTraced(Result);
						}
					}
				}
				else
				{
					FHitResult Result;
					if (GetWorld()->LineTraceSingleByProfile(Result, StartLocation, EndLocation, ProfileName))
					{
						OnRayTraced(Result);
					}
				}
			}
			break;
		case EPlayerRayTraceType::ObjectTypes:
			{
				if (bMultiRayTrace)
				{
					TArray<FHitResult> Results;
					if (GetWorld()->LineTraceMultiByObjectType(Results, StartLocation, EndLocation, TraceObjectTypes))
					{
						for (FHitResult& Result : Results)
						{
							OnRayTraced(Result);
						}
					}
				}
				else
				{
					FHitResult Result;
					if (GetWorld()->LineTraceSingleByObjectType(Result, StartLocation, EndLocation, TraceObjectTypes))
					{
						OnRayTraced(Result);
					}
				}
			}
			break;
		}
	}
	OnEndRayTracing();
}

void UPlayerRayTraceComponent::OnRayTraced_Implementation(FHitResult& HitResult)
{
	AActor* Actor = HitResult.GetActor();
	UPrimitiveComponent* Component = HitResult.GetComponent();
	if (!TracedActors.Contains(Actor))
	{
		FPlayerRayTracedActor RayTracedActor;
		RayTracedActor.Actor = Actor;
		TracedActors.Add(Actor, RayTracedActor);
		if (OnBeginRayTraceActor.IsBound())
		{
			OnBeginRayTraceActor.Broadcast(Actor);
		}
		if (const URayTracedComponent* CheckComponent = Actor->GetComponentByClass<URayTracedComponent>())
		{
			if (CheckComponent->OnBeginRayTraceActor.IsBound())
			{
				CheckComponent->OnBeginRayTraceActor.Broadcast(GetOwner(), this);
			}
		}
	}
	FPlayerRayTracedActor& TracedActor = TracedActors[Actor];
	TracedActor.bChecked = true;
	TMap<UPrimitiveComponent*, FPlayerRayTracedComponent>& TracedComponents = TracedActor.Components;
	if (!TracedComponents.Contains(Component))
	{
		FPlayerRayTracedComponent RayTracedComponent;
		RayTracedComponent.Component = Component;
		TracedComponents.Add(Component, RayTracedComponent);
		if (OnBeginRayTraceComponent.IsBound())
		{
			OnBeginRayTraceComponent.Broadcast(Actor, Component);
		}
		if (const URayTracedComponent* CheckComponent = Actor->GetComponentByClass<URayTracedComponent>())
		{
			if (CheckComponent->OnBeginRayTraceComponent.IsBound())
			{
				CheckComponent->OnBeginRayTraceComponent.Broadcast(GetOwner(), this, Component);
			}
		}
	}
	FPlayerRayTracedComponent& TracedComponent = TracedComponents[Component];
	TracedComponent.bChecked = true;
}

void UPlayerRayTraceComponent::OnBeginRayTracing_Implementation()
{
	TArray<AActor*> TracedActorKeys;
	TracedActors.GetKeys(TracedActorKeys);
	for (const AActor* TracedActorKey : TracedActorKeys)
	{
		if (!IsValid(TracedActorKey) && TracedActors.Contains(TracedActorKey))
		{
			TracedActors.Remove(TracedActorKey);
		}
	}

	for (TPair<AActor*, FPlayerRayTracedActor>& TracedActor : TracedActors)
	{
		TracedActor.Value.bChecked = false;
		for (TPair<UPrimitiveComponent*, FPlayerRayTracedComponent>& TracedComponent : TracedActor.Value.Components)
		{
			TracedComponent.Value.bChecked = false;
		}
	}
}

void UPlayerRayTraceComponent::OnEndRayTracing_Implementation()
{
	TArray<AActor*> TracedActorKeys;
	TArray<UPrimitiveComponent*> TracedComponentKeys;

	TracedActors.GetKeys(TracedActorKeys);
	for (AActor* TracedActorKey : TracedActorKeys)
	{
		FPlayerRayTracedActor& TracedActor = TracedActors[TracedActorKey];

		TracedActor.Components.GetKeys(TracedComponentKeys);
		for (UPrimitiveComponent* TracedComponentKey : TracedComponentKeys)
		{
			const FPlayerRayTracedComponent& TracedComponent = TracedActor.Components[TracedComponentKey];
			if (!TracedComponent.bChecked)
			{
				TracedActor.Components.Remove(TracedComponentKey);
				if (OnEndRayTraceComponent.IsBound())
				{
					OnEndRayTraceComponent.Broadcast(TracedActorKey, TracedComponentKey);
				}
				if (const URayTracedComponent* CheckComponent = TracedActorKey->GetComponentByClass<
					URayTracedComponent>())
				{
					if (CheckComponent->OnEndRayTraceComponent.IsBound())
					{
						CheckComponent->OnEndRayTraceComponent.Broadcast(GetOwner(), this, TracedComponentKey);
					}
				}
			}
		}

		if (!TracedActor.bChecked)
		{
			TracedActors.Remove(TracedActorKey);
			if (OnEndRayTraceActor.IsBound())
			{
				OnEndRayTraceActor.Broadcast(TracedActorKey);
			}
			if (const URayTracedComponent* CheckComponent = TracedActorKey->GetComponentByClass<URayTracedComponent>())
			{
				if (CheckComponent->OnEndRayTraceActor.IsBound())
				{
					CheckComponent->OnEndRayTraceActor.Broadcast(GetOwner(), this);
				}
			}
		}
	}
}
