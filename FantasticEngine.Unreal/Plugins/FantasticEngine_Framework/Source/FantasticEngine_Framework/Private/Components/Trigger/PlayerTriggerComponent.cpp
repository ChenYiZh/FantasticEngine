// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Trigger/PlayerTriggerComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Log/FEConsole.h"

void UPlayerTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UPlayerTriggerComponent::OnTriggerBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPlayerTriggerComponent::OnTriggerEndOverlap);
}

void UPlayerTriggerComponent::BeginDestroy()
{
	OnComponentBeginOverlap.RemoveDynamic(this, &UPlayerTriggerComponent::OnTriggerBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UPlayerTriggerComponent::OnTriggerEndOverlap);
	Super::BeginDestroy();
}

void UPlayerTriggerComponent::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (const APawn* Pawn = Cast<APawn>(OtherActor))
	{
		OverlappedCount++;
		if (Pawn->IsLocallyControlled())
		{
			OnOwnerEnter.Broadcast();
		}
		const int32 NumController = GetWorld()->GetNumPlayerControllers();
		if (UKismetSystemLibrary::IsServer(this))
		{
			OnAnyOneEnter.Broadcast();
			const int32 Count = PlayerCount == 0 ? NumController : PlayerCount;
			if (OverlappedCount >= Count)
			{
				OnEveryOneEnter.Broadcast();
			}
		}
	}
}

void UPlayerTriggerComponent::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const APawn* Pawn = Cast<APawn>(OtherActor))
	{
		OverlappedCount--;
		OverlappedCount = FMath::Clamp(OverlappedCount, 0, OverlappedCount);
		if (Pawn->IsLocallyControlled())
		{
			OnOwnerExit.Broadcast();
		}
		if (UKismetSystemLibrary::IsServer(this))
		{
			OnAnyOneExit.Broadcast();
			if (OverlappedCount == 0)
			{
				OnEveryOneExit.Broadcast();
			}
		}
	}
}

bool UPlayerTriggerComponent::AllIn() const
{
	const int32 NumController = GetWorld()->GetNumPlayerControllers();
	if (UKismetSystemLibrary::IsServer(this))
	{
		OnAnyOneEnter.Broadcast();
		const int32 Count = PlayerCount == 0 ? NumController : PlayerCount;
		if (OverlappedCount >= Count)
		{
			return true;
		}
	}
	return false;
}
