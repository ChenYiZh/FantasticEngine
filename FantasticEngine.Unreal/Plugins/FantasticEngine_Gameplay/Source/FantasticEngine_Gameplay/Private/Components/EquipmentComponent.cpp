// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipmentComponent.h"
#include "Components/GameplayCharacterComponent.h"

void UEquipmentComponent::SetAttributes_Implementation(UEquipmentAttributeSet* InAttributes)
{
	Attributes = InAttributes;
	K2_SetAttributes(InAttributes);
}

UGameplayCharacterComponent* UEquipmentComponent::GetGameCharacter() const
{
	return Character;
}

UEquipmentAttributeSet* UEquipmentComponent::GetAttributes(TSubclassOf<UEquipmentAttributeSet> Class) const
{
	return Attributes;
}

void UEquipmentComponent::WearTo(UGameplayCharacterComponent* Target, FName SocketName, EAttachmentRule LocationRule,
                     EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies)
{
	if (Character)
	{
		return;
	}
	Character->PutOnEquipment(this, SocketName, LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies);
}

void UEquipmentComponent::PutOff(EDetachmentRule LocationRule, EDetachmentRule RotationRule, EDetachmentRule ScaleRule)
{
	if (Character) { Character->PutOffEquipment(this, LocationRule, RotationRule, ScaleRule); }
}

void UEquipmentComponent::OnPutOn_Implementation(FName SocketName, EAttachmentRule LocationRule,
                                     EAttachmentRule RotationRule,
                                     EAttachmentRule ScaleRule, bool bWeldSimulatedBodies)
{
}

void UEquipmentComponent::OnPutOff_Implementation(UGameplayCharacterComponent* Target,
                                      EDetachmentRule LocationRule,
                                      EDetachmentRule RotationRule,
                                      EDetachmentRule ScaleRule)
{
}

// Sets default values
UEquipmentComponent::UEquipmentComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AttributesClass && !Attributes)
	{
		Attributes = NewObject<UEquipmentAttributeSet>(this, AttributesClass);
	}
}