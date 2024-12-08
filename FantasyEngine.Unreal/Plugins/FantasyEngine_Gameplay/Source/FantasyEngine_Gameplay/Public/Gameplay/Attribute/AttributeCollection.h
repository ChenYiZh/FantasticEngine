// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttributeCollection.generated.h"

class UCharacterAttributeSet;
class UEquipmentAttributeSet;
class UActorAttributeSet;
/**
 * 
 */
UCLASS()
class FANTASYENGINE_GAMEPLAY_API UAttributeCollection : public UObject
{
	GENERATED_BODY()

	friend class UGameplayCharacterComponent;

private:
	/**
	 * 角色属性
	 */
	UPROPERTY()
	UCharacterAttributeSet* CharacterAttributes = nullptr;
	/**
	 * 装备属性
	 */
	UPROPERTY()
	TArray<UEquipmentAttributeSet*> EquipmentAttributeSets;
	// /**
	//  * 补充的一些属性
	//  */
	// UPROPERTY()
	// TArray<UActorAttributeSet*> AdditionalAttributes;
	/**
	 * 属性缓存，空间换时间
	 */
	UPROPERTY()
	TArray<UActorAttributeSet*> Attributeses;

private:
	void SetCharacterAttributes(UCharacterAttributeSet* InCharacterAttributes);
	void PushEquipmentAttributes(UEquipmentAttributeSet* InEquipmentAttributes);
	void RemoveEquipmentAttributes(UEquipmentAttributeSet* InEquipmentAttributes);

public:
	UFUNCTION(BlueprintPure, Exec, Category="Attributes", DisplayName="Character Attributes",
		meta=(DeterminesOutputType="Class"))
	UCharacterAttributeSet* GetCharacterAttributes(TSubclassOf<UCharacterAttributeSet> Class = nullptr) const;

	UFUNCTION(BlueprintPure, Exec, Category="Attributes", DisplayName="Equipment AttributeSets",
		meta=(DeterminesOutputType="Class"))
	const TArray<UEquipmentAttributeSet*>& GetEquipmentAttributeSets(TSubclassOf<UCharacterAttributeSet> Class = nullptr) const;

	UFUNCTION(BlueprintPure, Exec, Category="Attributes", DisplayName="All AttributeSets")
	const TArray<UActorAttributeSet*>& GetAllAttributeSets() const;
};
