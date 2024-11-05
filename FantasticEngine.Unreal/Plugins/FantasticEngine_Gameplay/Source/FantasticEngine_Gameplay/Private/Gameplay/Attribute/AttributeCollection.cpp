// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Attribute/AttributeCollection.h"
#include "Gameplay/Attribute/ActorAttributeSet.h"
#include "Gameplay/Attribute/CharacterAttributeSet.h"
#include "Gameplay/Attribute/EquipmentAttributeSet.h"

void UAttributeCollection::SetCharacterAttributes(UCharacterAttributeSet* InCharacterAttributes)
{
	if (CharacterAttributes)
	{
		Attributeses.Remove(CharacterAttributes);
	}
	CharacterAttributes = InCharacterAttributes;
	Attributeses.Push(InCharacterAttributes);
}

void UAttributeCollection::PushEquipmentAttributes(UEquipmentAttributeSet* InEquipmentAttributes)
{
	EquipmentAttributeSets.Push(InEquipmentAttributes);
	Attributeses.Push(InEquipmentAttributes);
}

void UAttributeCollection::RemoveEquipmentAttributes(UEquipmentAttributeSet* InEquipmentAttributes)
{
	EquipmentAttributeSets.Remove(InEquipmentAttributes);
	Attributeses.Remove(InEquipmentAttributes);
}

UCharacterAttributeSet* UAttributeCollection::GetCharacterAttributes(TSubclassOf<UCharacterAttributeSet> Class) const
{
	return CharacterAttributes;
}

const TArray<UEquipmentAttributeSet*>& UAttributeCollection::GetEquipmentAttributeSets(
	TSubclassOf<UCharacterAttributeSet> Class) const
{
	return EquipmentAttributeSets;
}

const TArray<UActorAttributeSet*>& UAttributeCollection::GetAllAttributeSets() const
{
	return Attributeses;
}
