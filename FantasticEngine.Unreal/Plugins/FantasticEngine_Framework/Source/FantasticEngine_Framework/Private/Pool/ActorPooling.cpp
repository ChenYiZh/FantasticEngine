// Fill out your copyright notice in the Description page of Project Settings.


#include "Pool/ActorPooling.h"

#include "Log/FEConsole.h"

EActorPoolParentType UActorPooling::GetParentType() const
{
	if (!Parent)
	{
		return EActorPoolParentType::None;
	}
	return ParentType;
}

void UActorPooling::SetParentOfActor(AActor* Actor)
{
	if (!Actor)
	{
		ResetParent();
		return;
	}
	ParentType = EActorPoolParentType::Actor;
	Parent = Actor;
}

void UActorPooling::SetParentOfComponent(USceneComponent* Component)
{
	if (!Component)
	{
		ResetParent();
		return;
	}
	ParentType = EActorPoolParentType::Component;
	Parent = Component;
}

void UActorPooling::ResetParent()
{
	ParentType = EActorPoolParentType::None;
	Parent = nullptr;
}

void UActorPooling::GetUsingActors(TSubclassOf<AActor> InActorClass, TArray<AActor*>& OutActors) const
{
	OutActors.Empty();
	OutActors.SetNum(UsingItems.Num());
	for (int32 i = 0; i < UsingItems.Num(); i++)
	{
		OutActors[i] = UsingItems[i]->GetObject<AActor>();
	}
}

UPoolItemHandle* UActorPooling::GetActor(FVector Location, FRotator Rotator, FVector Scale, bool bRelative)
{
	InLocation = Location;
	InRotator = Rotator;
	InScale = Scale;
	bInRelative = bRelative;
	return GetItemHandle();
}

UObject* UActorPooling::CreateObject()
{
	if (!ObjectClass || !ObjectClass->IsChildOf<AActor>())
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Actor Pool"),TEXT("对象池类型不匹配"));
		return nullptr;
	}
	AActor* Actor = GetWorld()->SpawnActor<AActor>(ObjectClass, InLocation, InRotator);
	switch (GetParentType())
	{
	case EActorPoolParentType::Actor:
		{
			AActor* ParentActor = Cast<AActor>(Parent);
			Actor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
		}
		break;
	case EActorPoolParentType::Component:
		{
			USceneComponent* ParentComponent = Cast<USceneComponent>(Parent);
			Actor->AttachToComponent(ParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		break;
	}
	return Actor;
}

void UActorPooling::BeforeUse(UPoolItemHandle* Handle)
{
	Super::BeforeUse(Handle);
	if (AActor* Actor = Handle->GetObject<AActor>())
	{
		if (bInRelative)
		{
			Actor->SetActorRelativeLocation(InLocation);
			Actor->SetActorRelativeRotation(InRotator);
			Actor->SetActorRelativeScale3D(InScale);
		}
		else
		{
			Actor->SetActorLocation(InLocation);
			Actor->SetActorRotation(InRotator);
			Actor->SetActorScale3D(InScale);
		}
	}
	InLocation = FVector::ZeroVector;
	InRotator = FRotator::ZeroRotator;
	InScale = FVector::OneVector;
	bInRelative = false;
}

void UActorPooling::OnUse(UPoolItemHandle* Handle)
{
	if (AActor* Actor = Handle->GetObject<AActor>())
	{
		Actor->SetHidden(false);
		Actor->SetActorTickEnabled(true);
	}
}

void UActorPooling::OnCollect(UPoolItemHandle* Handle)
{
	if (AActor* Actor = Handle->GetObject<AActor>())
	{
		Actor->SetHidden(true);
		Actor->SetActorTickEnabled(false);
	}
}
