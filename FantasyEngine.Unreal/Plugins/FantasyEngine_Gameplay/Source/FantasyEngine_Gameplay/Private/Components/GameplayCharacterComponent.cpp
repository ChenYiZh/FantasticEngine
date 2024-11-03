// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameplayCharacterComponent.h"

#include "GameplayTasksComponent.h"
#include "Components/FocusComponent.h"
#include "Gameplay/GameEffect.h"
#include "Gameplay/Attribute/EquipmentAttributeSet.h"
#include "Tools/GameplayLibrary.h"

// Sets default values for this component's properties
UGameplayCharacterComponent::UGameplayCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameplayCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GameplayTasks))
	{
		GameplayTasks = NewObject<UGameplayTasksComponent>(GetOwner(),TEXT("TasksComp"));
		GameplayTasks->RegisterComponent();
	}
	if (!IsValid(GetOwner()->GetComponentByClass<UFocusComponent>()))
	{
		UFocusComponent* FocusComponent = NewObject<UFocusComponent>(GetOwner(),TEXT("Focus Component"));
		FocusComponent->RegisterComponent();
	}
}

void UGameplayCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	if (IsValidChecked(this) && GetWorld())
	{
		for (UGameModeAbility* Ability : Abilities)
		{
			if (IsValid(Ability) && !Ability->bCompleted)
			{
				Ability->TickAbililty(DeltaTime);
			}
		}
		for (int32 i = Abilities.Num() - 1; i >= 0; i--)
		{
			if (!IsValid(Abilities[i]) || Abilities[i]->bCompleted)
			{
				Abilities.RemoveAt(i);
			}
		}
		// for (int32 i = Tasks.Num() - 1; i >= 0; i--)
		// {
		// 	if (!IsValid(Tasks[i]))
		// 	{
		// 		Tasks.RemoveAt(i);
		// 	}
		// 	else
		// 	{
		// 		Tasks[i]->Tick(DeltaTime);
		// 	}
		// }
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGameplayCharacterComponent::OnAttackActor_Implementation(AActor* Target, const FAttackContext& Context)
{
}

void UGameplayCharacterComponent::SetAttributes_Implementation(UCharacterAttributeSet* InAttributes)
{
	Attributes = InAttributes;
	AttributeCollection->SetCharacterAttributes(Attributes);
	K2_SetAttributes(InAttributes);
}


// void AStandardCharacter::RemoveAndClearAllTasks()
// {
// 	for (int32 i = Tasks.Num() - 1; i >= 0; i--)
// 	{
// 		if (!IsValid(Tasks[i]))
// 		{
// 			Tasks.RemoveAt(i);
// 		}
// 		else
// 		{
// 			Tasks[i]->Interrupt();
// 		}
// 	}
// 	Tasks.Empty();
// }

void UGameplayCharacterComponent::GetTags(FGameplayTagContainer& Container) const
{
	Container.Reset();
	for (const UGameModeAbility* Ability : Abilities)
	{
		if (IsValid(Ability) && !Ability->bCompleted)
		{
			Container.AppendTags(Ability->AbilityTags);
		}
	}
}

bool UGameplayCharacterComponent::HasTag(const FGameplayTag Tag)
{
	FGameplayTagContainer Container;
	GetTags(Container);
	return Container.HasTag(Tag);
}

void UGameplayCharacterComponent::AddTag(const FGameplayTag Tag)
{
	GameplayTags.AddTag(Tag);
}

void UGameplayCharacterComponent::RemoveTag(const FGameplayTag Tag)
{
	GameplayTags.RemoveTag(Tag);
}

void UGameplayCharacterComponent::ClearTags()
{
	GameplayTags.Reset();
}

UCharacterAttributeSet* UGameplayCharacterComponent::GetCharacterAttributes(
	TSubclassOf<UCharacterAttributeSet> Class) const
{
	return Attributes;
}

UAttributeCollection* UGameplayCharacterComponent::GetAttributeCollection() const
{
	return AttributeCollection;
}

const TArray<UEquipmentComponent*>& UGameplayCharacterComponent::GetEquipments() const
{
	return Equipments;
}

void UGameplayCharacterComponent::PutOnEquipment(UEquipmentComponent* Equipment, FName SocketName,
                                                 EAttachmentRule LocationRule,
                                                 EAttachmentRule RotationRule,
                                                 EAttachmentRule ScaleRule, bool bWeldSimulatedBodies)
{
	if (!Equipment) { return; }
	GetOwner()->K2_AttachToActor(Equipment->GetOwner(), SocketName, LocationRule, RotationRule, ScaleRule,
	                             bWeldSimulatedBodies);
	Equipments.Push(Equipment);
	if (Equipment->AttributesClass && !Equipment->Attributes)
	{
		Equipment->Attributes = NewObject<UEquipmentAttributeSet>(Equipment, Equipment->AttributesClass);
	}
	Equipment->Character = this;
	Equipment->OnPutOn(SocketName, LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies);
	if (UEquipmentAttributeSet* EquipmentAttributes = Equipment->GetAttributes())
	{
		AttributeCollection->PushEquipmentAttributes(EquipmentAttributes);
	}
}

void UGameplayCharacterComponent::PutOffEquipment(UEquipmentComponent* Equipment,
                                                  EDetachmentRule LocationRule,
                                                  EDetachmentRule RotationRule,
                                                  EDetachmentRule ScaleRule)
{
	if (Equipments.Contains(Equipment))
	{
		Equipment->GetOwner()->K2_DetachFromActor(LocationRule, RotationRule, ScaleRule);
		Equipment->OnPutOff(this, LocationRule, RotationRule, ScaleRule);
		Equipment->Character = nullptr;
		Equipments.Remove(Equipment);
		if (UEquipmentAttributeSet* EquipmentAttributes = Equipment->GetAttributes())
		{
			AttributeCollection->RemoveEquipmentAttributes(EquipmentAttributes);
		}
	}
}

const TArray<UGameModeAbility*> UGameplayCharacterComponent::GetAbilities()
{
	return Abilities;
}

UGameModeAbility* UGameplayCharacterComponent::MakeAbility(TSubclassOf<UGameModeAbility> AbilityClass)
{
	if (!IsValid(AbilityClass)) { return nullptr; }
	UGameModeAbility* Ability = NewObject<UGameModeAbility>(this, AbilityClass);
	Ability->Character = this;
	return Ability;
}

UGameModeAbility* UGameplayCharacterComponent::MakeAndActiveAbility(TSubclassOf<UGameModeAbility> AbilityClass)
{
	if (UGameModeAbility* Ability = MakeAbility(AbilityClass))
	{
		return ActiveAbility(Ability) ? Ability : nullptr;
	}
	return nullptr;
}

bool UGameplayCharacterComponent::ActiveAbility(UGameModeAbility* Ability)
{
	if (!IsValid(Ability)) { return false; }
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (Ability->AbilityTags.HasAny(WorkingAbility->BlockIfExecuting))
		{
			return false;
		}
	}
	FGameplayTagContainer Container;
	GetTags(Container);
	if (!Ability->ActiveIfExists.IsEmpty() && !Ability->ActiveIfExists.HasAll(Container))
	{
		return false;
	}
	if (!Ability->ActiveIfExclude.IsEmpty() && Ability->ActiveIfExclude.HasAny(Container))
	{
		return false;
	}
	if (!Ability->CanActive()) { return false; }
	if (Ability->MaxCountInStack > 0)
	{
		TArray<TWeakObjectPtr<UGameModeAbility>> ExecutingAbilities;
		for (UGameModeAbility* WorkingAbility : Abilities)
		{
			if (WorkingAbility->GetClass() == Ability->GetClass())
			{
				ExecutingAbilities.Push(WorkingAbility);
			}
		}
		if (ExecutingAbilities.Num() >= Ability->MaxCountInStack)
		{
			if (!Ability->bInterruptOldAndApplyNew)
			{
				return false;
			}
			ExecutingAbilities.StableSort(
				[](const TWeakObjectPtr<UGameModeAbility> A, const TWeakObjectPtr<UGameModeAbility> B)
				{
					return A->BeginActiveTime < B->BeginActiveTime;
				});
			ExecutingAbilities[0]->Stop(EAbilityStopCode::Interrupt);
		}
	}
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (Ability->AbilityTags.HasAny(WorkingAbility->InterruptByTags))
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
		}
	}
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (WorkingAbility->AbilityTags.HasAny(Ability->BlockIfExecuting))
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
		}
	}
	Abilities.Push(Ability);
	Ability->BeginActiveTime = GetWorld()->GetTimeSeconds();
	Ability->Active();
	return true;
}

bool UGameplayCharacterComponent::ContainsAbility(UGameModeAbility* Ability)
{
	return Abilities.Contains(Ability);
}

bool UGameplayCharacterComponent::ContainsAbilityOfClass(TSubclassOf<UGameModeAbility> AbilityClass)
{
	return ContainsObjectOfClass(Abilities, AbilityClass);
}

bool UGameplayCharacterComponent::RemoveAbility(UGameModeAbility* Ability)
{
	if (ContainsAbility(Ability))
	{
		Ability->Stop(EAbilityStopCode::Interrupt);
		return true;
	}
	return false;
}

bool UGameplayCharacterComponent::RemoveAbilityOfClass(TSubclassOf<UGameModeAbility> AbilityClass)
{
	if (!IsValid(AbilityClass)) { return false; }
	bool bResult = false;
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (IsValid(WorkingAbility) && !WorkingAbility->bCompleted && WorkingAbility->IsA(AbilityClass))
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
			bResult = true;
		}
	}
	return bResult;
}

bool UGameplayCharacterComponent::RemoveAbilityOfTags(const FGameplayTagContainer RemovableTags)
{
	bool bResult = false;
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (IsValid(WorkingAbility) && !WorkingAbility->bCompleted && RemovableTags.HasAny(WorkingAbility->AbilityTags))
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
			bResult = true;
		}
	}
	return bResult;
}

bool UGameplayCharacterComponent::RemoveAbilityOfTag(const FGameplayTag RemovableTag)
{
	bool bResult = false;
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (IsValid(WorkingAbility) && !WorkingAbility->bCompleted && WorkingAbility->AbilityTags.HasTag(RemovableTag))
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
			bResult = true;
		}
	}
	return bResult;
}

void UGameplayCharacterComponent::RemoveAllAbilities()
{
	for (UGameModeAbility* WorkingAbility : Abilities)
	{
		if (IsValid(WorkingAbility) && !WorkingAbility->bCompleted)
		{
			WorkingAbility->Stop(EAbilityStopCode::Interrupt);
		}
	}
}

void UGameplayCharacterComponent::RemoveAndClearAllAbilities()
{
	RemoveAllAbilities();
	Abilities.Empty();
}

// void AGameCharacter::SetStrategy_Implementation(TSubclassOf<UActorStrategy> InStrategy)
// {
// 	if (!IsValid(InStrategy)) { return; }
// 	if (IsValid(Strategy))
// 	{
// 		Strategy->OnDetached();
// 		Strategy->DestroyComponent();
// 	}
// 	Strategy = Cast<UActorStrategy>(UObjectFactory::AddComponent(InStrategy, this));
// 	K2_SetStrategy(Strategy);
// }
//
// UActorStrategy* AGameCharacter::GetOriStrategy(TSubclassOf<UActorStrategy> Class) const
// {
// 	return Strategy;
// }

void UGameplayCharacterComponent::Attack(UGameEffect* GameEffect, AActor* Target,
                                         const int32 TargetIndex, UGameModeAbility* SourceAbility,
                                         FTransparentParameters Parameters)
{
	if (IsValid(GameEffect))
	{
		Target = GameEffect->Target;
	}
	AttackOfContext(Target, UGameplayLibrary::MakeContext(
		                GetOwner(),
		                GameEffect,
		                TargetIndex,
		                SourceAbility,
		                Parameters));
}

void UGameplayCharacterComponent::AttackOfClass(TSubclassOf<UGameEffect> GameEffect, AActor* Target, int32 TargetIndex,
                                                UGameModeAbility* SourceAbility, FTransparentParameters Parameters)
{
	UGameEffect::MakeAndApplyGameEffect(this, GameEffect, GetOwner(), Target, SourceAbility, TargetIndex, Parameters);
}

void UGameplayCharacterComponent::AttackOfContext(AActor* Target, FAttackContext Context)
{
	if (IsValid(Context.GameEffect))
	{
		if (Context.GameEffect->OnApply(Context.HitIndex, Context.Parameters))
		{
			if (IsValid(Context.SourceAbility))
			{
				Context.SourceAbility->OnAttackActor(Target, Context);
			}
			OnAttackActor(Target, Context);
		}
	}
	if (IsValid(Target))
	{
		IActorHitter::Execute_Hit(Target, Context);
	}
}

void UGameplayCharacterComponent::Hit_Implementation(const FAttackContext& Context)
{
	// if (IsValidChecked(Strategy))
	// {
	// 	IActorHitter::Execute_Hit(Strategy, Context);
	// }
	OnHit(Context);
	if (IsValid(Attributes) && Attributes->IsEmptyHealth())
	{
		GoDie(EDieCode::EmptyHealth);
	}
}

void UGameplayCharacterComponent::OnHit_Implementation(const FAttackContext& Context)
{
}

void UGameplayCharacterComponent::Born()
{
	if (!IsAlive())
	{
		AttributeCollection = NewObject<UAttributeCollection>(this);
		if (IsValid(AttributesClass))
		{
			UCharacterAttributeSet* NewAttributes = NewObject<UCharacterAttributeSet>(this, AttributesClass);
			SetAttributes(NewAttributes);
		}
		if (IsValid(Attributes))
		{
			Attributes->Reset();
		}
		// SetStrategy(StrategyClass);
		// if (IsValid(Strategy))
		// {
		// 	Strategy->Reset();
		// }
	}
	Super::Born();
}

void UGameplayCharacterComponent::GoDie(const EDieCode Code)
{
	if (!IsAlive()) { return; }
	Super::GoDie(Code);
	// if (IsValid(Strategy))
	// {
	// 	Strategy->OnGoDie(Code);
	// }
}

void UGameplayCharacterComponent::BeginDestroy()
{
	GoDie(EDieCode::Destroy);
	RemoveAndClearAllAbilities();
	//RemoveAndClearAllTasks();
	Super::BeginDestroy();
}

void UGameplayCharacterComponent::Reset()
{
	RemoveAndClearAllAbilities();
	//RemoveAndClearAllTasks();
	Super::Reset();
}

void UGameplayCharacterComponent::OnArrowRightReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowRightPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowLeftReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowLeftPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowDownReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowDownPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowUpReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnArrowUpPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnActionUpPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnActionUpReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnActionDownPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnActionDownReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnActionLeftPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnActionLeftReleased_Implementation()
{
}

void UGameplayCharacterComponent::OnActionRightPressed_Implementation()
{
}

void UGameplayCharacterComponent::OnActionRightReleased_Implementation()
{
}
