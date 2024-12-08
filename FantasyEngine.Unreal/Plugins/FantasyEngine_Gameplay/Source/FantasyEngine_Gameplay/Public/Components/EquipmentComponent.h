// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Attribute/EquipmentAttributeSet.h"
#include "GameFramework/Actor.h"
#include "EquipmentComponent.generated.h"

/** 武器组件 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="FantasyEngine", meta=(BlueprintSpawnableComponent))
class FANTASYENGINE_GAMEPLAY_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UGameplayCharacterComponent;

private:
	UPROPERTY()
	UGameplayCharacterComponent* Character = nullptr;

public:
	UFUNCTION(BlueprintPure, Exec, Category="Equipment")
	UGameplayCharacterComponent* GetGameCharacter() const;

#pragma region 属性管理

private:
	/**
	 * 属性类的基类
	 */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess, HideViewOptions, AllowAbstract="true"),
		Category="Equipment|Attributes", DisplayName="Default Attributes")
	TSubclassOf<UEquipmentAttributeSet> AttributesClass = nullptr;
	/**
	 * 属性类
	 */
	UPROPERTY(VisibleInstanceOnly, Category="Equipment|Attributes")
	UEquipmentAttributeSet* Attributes = nullptr;

public:
	/**
	 * 在设置属性时调用
	 * @param InAttributes 属性
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Equipment|Attributes", DisplayName="On Setting Attributes")
	void K2_SetAttributes(UEquipmentAttributeSet* InAttributes);
	/**
	 * 在设置属性时调用
	 * @param InAttributes 属性
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Equipment|Attributes")
	void SetAttributes(UEquipmentAttributeSet* InAttributes);

	/**
	 * 获取属性类
	 */
	UFUNCTION(BlueprintPure, Exec, Category="Equipment|Attributes", DisplayName="Attributes",
		meta=(DeterminesOutputType="Class"))
	UEquipmentAttributeSet* GetAttributes(TSubclassOf<UEquipmentAttributeSet> Class = nullptr) const;

	/**
	 * 获取属性类
	 */
	template <typename T>
	T* GetAttributes() const
	{
		return Cast<T>(Attributes);
	}
#pragma endregion

public:
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void WearTo(UGameplayCharacterComponent* Target,
	            FName SocketName,
	            EAttachmentRule LocationRule,
	            EAttachmentRule RotationRule,
	            EAttachmentRule ScaleRule,
	            bool bWeldSimulatedBodies);

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void PutOff(EDetachmentRule LocationRule = EDetachmentRule::KeepRelative,
	            EDetachmentRule RotationRule = EDetachmentRule::KeepRelative,
	            EDetachmentRule ScaleRule = EDetachmentRule::KeepRelative);

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Equipment")
	void OnPutOn(FName SocketName,
	             EAttachmentRule LocationRule,
	             EAttachmentRule RotationRule,
	             EAttachmentRule ScaleRule, bool bWeldSimulatedBodies);
	UFUNCTION(BlueprintNativeEvent, Category="Equipment")
	void OnPutOff(class UGameplayCharacterComponent* Target,
	              EDetachmentRule LocationRule = EDetachmentRule::KeepRelative,
	              EDetachmentRule RotationRule = EDetachmentRule::KeepRelative,
	              EDetachmentRule ScaleRule = EDetachmentRule::KeepRelative);

public:
	// Sets default values for this actor's properties
	UEquipmentComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
