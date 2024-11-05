// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Pawn/CharacterBeingComponent.h"

#include "GameRoot.h"
#include "Common/StringConverter.h"
#include "Components/FocusComponent.h"
#include "Defines/EventIdsBasic.h"
#include "EventParams/EventParam_Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/EventSystem.h"

// Sets default values for this component's properties
UCharacterBeingComponent::UCharacterBeingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCharacterBeingComponent::BeginPlay()
{
	Super::BeginPlay();
	Born();
}

bool UCharacterBeingComponent::IsAlive() const
{
	return bAlive;
}

void UCharacterBeingComponent::K2_Born()
{
	Born();
}

void UCharacterBeingComponent::Born()
{
	if (bAlive) { return; }
	bAlive = true;

	AActor* Owner = GetOwner();
	UFocusComponent* FocusComponent = Owner->GetComponentByClass<UFocusComponent>();
	if (IsValid(FocusComponent))
	{
		FocusComponent->Reset();
	}

	OnBorn();
}

void UCharacterBeingComponent::K2_GoDie(const EDieCode Code)
{
	GoDie(Code);
}

void UCharacterBeingComponent::GoDie(const EDieCode Code)
{
	if (!bAlive) { return; }
	bAlive = false;
	OnGoDie(Code);
	if (!IsValid(UGameRoot::GetRoot(this)))
	{
		return;
	}
	UEventParam_Actor* Param = NewObject<UEventParam_Actor>(UGameRoot::GetRoot(this));
	Param->Actor = GetOwner();
	UEventSystem::SendEvent(this, UEventIdsBasic::EID_ACTOR_GO_DIE(), Param);
}

void UCharacterBeingComponent::K2_Reset()
{
	Reset();
}

void UCharacterBeingComponent::Reset()
{
	if (IsValid(PoolItemHandle))
	{
		PoolItemHandle->Release();
	}
	OnReset();
}

UPoolItemHandle* UCharacterBeingComponent::GetPoolItemHandle() const
{
	return PoolItemHandle;
}

void UCharacterBeingComponent::OnInitialize_Implementation(UPoolItemHandle* Handle)
{
	PoolItemHandle = Handle;
}

void UCharacterBeingComponent::BeginToUse_Implementation()
{
	Born();
}

void UCharacterBeingComponent::OnReset_Implementation()
{
	OnResetNative.Broadcast();
	K2_OnReset.Broadcast();
}

void UCharacterBeingComponent::OnGoDie_Implementation(const EDieCode Code)
{
	OnDead.Broadcast(Code);
	K2_OnDead.Broadcast(Code);
}

void UCharacterBeingComponent::OnBorn_Implementation()
{
	OnBornNative.Broadcast();
	K2_OnBorn.Broadcast();
}
