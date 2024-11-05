// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GameEffect.h"

#include "Components/GameplayCharacterComponent.h"
#include "Tools/GameplayLibrary.h"


AActor* UGameEffect::GetSourceAsActor() const
{
	return Source;
}

UGameplayCharacterComponent* UGameEffect::GetSourceAsCharacter(TSubclassOf<UGameplayCharacterComponent> Class) const
{
	return IsValid(Source) ? Source->GetComponentByClass<UGameplayCharacterComponent>() : nullptr;
}

UAttributeCollection* UGameEffect::GetSourceAttributes() const
{
	if (UGameplayCharacterComponent* Character = GetSourceAsCharacter())
	{
		return Character->GetAttributeCollection();
	}
	return nullptr;
}

// UAttributeCollection* UGameEffect::GetSourceStrategy(TSubclassOf<UActorStrategy> Class) const
// {
// 	if (AGameCharacter* Character = GetSourceOfOriCharacter())
// 	{
// 		return Character->GetOriStrategy();
// 	}
// 	return nullptr;
// }

AActor* UGameEffect::GetTargetAsActor() const
{
	return Target;
}

UGameplayCharacterComponent* UGameEffect::GetTargetAsCharacter(TSubclassOf<UGameplayCharacterComponent> Class) const
{
	return IsValid(Target) ? Target->GetComponentByClass<UGameplayCharacterComponent>() : nullptr;
}

UAttributeCollection* UGameEffect::GetTargetAttributes() const
{
	if (UGameplayCharacterComponent* Character = GetTargetAsCharacter())
	{
		return Character->GetAttributeCollection();
	}
	return nullptr;
}

// UActorStrategy* UGameEffect::GetTargetOriStrategy(TSubclassOf<UActorStrategy> Class) const
// {
// 	if (AGameCharacter* Character = GetTargetOfOriCharacter())
// 	{
// 		return Character->GetOriStrategy();
// 	}
// 	return nullptr;
// }

void UGameEffect::Execute_Implementation(const int32& TargetIndex, FTransparentParameters& Parameters)
{
}

bool UGameEffect::OnApply(const int32& TargetIndex, FTransparentParameters& Parameters)
{
	if (bApplied || !CanApply(TargetIndex, Parameters)) { return false; }
	bApplied = true;
	Execute(TargetIndex, Parameters);
	return true;
}

bool UGameEffect::CanApply_Implementation(const int32& TargetIndex, FTransparentParameters& Parameters)
{
	return true;
}

void UGameEffect::Apply(UGameModeAbility* Ability, const int32 TargetIndex, FTransparentParameters Parameters)
{
	if (UGameplayCharacterComponent* Character = GetSourceAsCharacter())
	{
		Character->Attack(this, Target, TargetIndex, Ability, Parameters);
	}
	else
	{
		if (OnApply(TargetIndex, Parameters))
		{
			FAttackContext Context = UGameplayLibrary::MakeContext(Source, this, TargetIndex, Ability, Parameters);
			if (IsValid(Ability))
			{
				Ability->OnAttackActor(Target, Context);
			}
			if (Target->GetClass()->ImplementsInterface(UActorHitter::StaticClass()))
			{
				IActorHitter::Execute_Hit(Target, Context);
			}
		}
	}
}

UGameEffect* UGameEffect::MakeGameEffectOnly(const UObject* WorldContextObject, TSubclassOf<UGameEffect> Class,
                                             AActor* InSource, AActor* InTarget)
{
	if (!IsValid(Class)) { return nullptr; }
	UGameEffect* Effect;
	if (IsValid(InSource))
	{
		Effect = NewObject<UGameEffect>(InSource, Class);
	}
	else if (IsValid(InTarget))
	{
		Effect = NewObject<UGameEffect>(InTarget, Class);
	}
	else
	{
		Effect = NewObject<UGameEffect>(UGameRoot::GetRootWorld(WorldContextObject), Class);
	}
	if (!IsValid(Effect)) { return nullptr; }
	Effect->Reset();
	Effect->Source = InSource;
	Effect->Target = InTarget;
	return Effect;
}

UGameEffect* UGameEffect::MakeAndApplyGameEffect(const UObject* WorldContextObject, TSubclassOf<UGameEffect> Class,
                                                 AActor* InSource, AActor* InTarget,
                                                 UGameModeAbility* SourceAbility, const int32 TargetIndex,
                                                 FTransparentParameters Parameters)
{
	UGameEffect* Effect = MakeGameEffectOnly(WorldContextObject, Class, InSource, InTarget);
	if (IsValid(Effect))
	{
		Effect->Apply(SourceAbility, TargetIndex, Parameters);
	}
	return Effect;
}

void UGameEffect::Reset()
{
	bApplied = false;
}
