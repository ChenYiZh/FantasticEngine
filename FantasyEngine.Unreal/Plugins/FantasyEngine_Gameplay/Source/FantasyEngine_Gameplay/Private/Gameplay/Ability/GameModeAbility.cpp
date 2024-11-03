// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Ability/GameModeAbility.h"

#include "GameplayTask.h"
#include "GameplayTasksComponent.h"
#include "Common/StringConverter.h"
#include "Components/FocusComponent.h"
#include "Gameplay/GameEffect.h"
#include "Components/GameplayCharacterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Log/FEConsole.h"

UGameplayCharacterComponent* UGameModeAbility::GetCharacter(TSubclassOf<UGameplayCharacterComponent> Class) const
{
	return Character;
}

// UActorStrategy* UGameModeAbility::GetOriStrategy(TSubclassOf<UActorStrategy> Class) const
// {
// 	if (IsValid(Character))
// 	{
// 		return Character->GetOriStrategy();
// 	}
// 	return nullptr;
// }

bool UGameModeAbility::CanActive_Implementation()
{
	return true;
}

void UGameModeAbility::Tick_Implementation(float DeltaSeconds)
{
}

bool UGameModeAbility::IsCompleted() const
{
	return bCompleted;
}

bool UGameModeAbility::IsActived() const
{
	return bActived;
}

void UGameModeAbility::Active()
{
	if (bActived) { return; }
	bActived = true;
	PreActive();
	K2_PreActive();
	OnActive();
	K2_OnActive();
}

void UGameModeAbility::PreActive()
{
}

void UGameModeAbility::OnActive()
{
}

void UGameModeAbility::InTheEnd(const EAbilityStopCode Reason)
{
	K2_InTheEnd(Reason);
}

/* 将函数拆分出来目的是为了区分作用 */
void UGameModeAbility::Stop(const EAbilityStopCode Reason)
{
	if (bCompleted) { return; }
	InTheEnd(Reason);
	bCompleted = true;
	for (int32 i = ActiveTasks.Num() - 1; i >= 0; i--)
	{
		ActiveTasks[i]->TaskOwnerEnded();
	}
	ActiveTasks.Reset();
}

void UGameModeAbility::TickAbililty(float DeltaSeconds)
{
	if (!bActived) { return; }
	if (Duration > 0 && (Duration -= DeltaSeconds) < 0)
	{
		Finish();
	}
	if (!bCompleted)
	{
		Tick(DeltaSeconds);
	}
}

void UGameModeAbility::Finish()
{
	Stop(EAbilityStopCode::Normal);
}

void UGameModeAbility::GetOverlappingActorsOfClass(TArray<AActor*>& OverlapingActors,
                                                   TSubclassOf<AActor> ActorClass,
                                                   const AActor* Source) const
{
	if (Source == nullptr)
	{
		Source = Character->GetOwner();
	}
	if (IsValid(Source))
	{
		Source->GetOverlappingActors(OverlapingActors, ActorClass);
	}
}

void UGameModeAbility::GetOverlappingActorsOfTag(TArray<AActor*>& OverlapingActors, const FName& InTag,
                                                 const AActor* Source) const
{
	if (Source == nullptr)
	{
		Source = Character->GetOwner();
	}
	if (IsValid(Source))
	{
		Source->GetOverlappingActors(OverlapingActors);
		for (int i = OverlapingActors.Num() - 1; i >= 0; i--)
		{
			if (!OverlapingActors[i]->Tags.Contains(InTag))
			{
				OverlapingActors.RemoveAt(i);
			}
		}
	}
}

void UGameModeAbility::GetOverlappingActorsOfTags(TArray<AActor*>& OverlapingActors, const TArray<FName>& Tags,
                                                  const AActor* Source) const
{
	if (Source == nullptr)
	{
		Source = Character->GetOwner();
	}
	if (Source)
	{
		Source->GetOverlappingActors(OverlapingActors);
		for (int i = OverlapingActors.Num() - 1; i >= 0; i--)
		{
			bool bExists = false;
			TArray<FName>& ActorTags = OverlapingActors[i]->Tags;
			for (const FName& FTag : Tags)
			{
				if (ActorTags.Contains(FTag))
				{
					bExists = true;
					break;
				}
			}
			if (!bExists)
			{
				OverlapingActors.RemoveAt(i);
			}
		}
	}
}

AActor* UGameModeAbility::FindNearestActor(const TArray<AActor*>& Actors) const
{
	if (Actors.IsEmpty() || !IsValid(Character)) { return nullptr; }
	float Distance;
	return UGameplayStatics::FindNearestActor(Character->GetOwner()->GetActorLocation(), Actors, Distance);
}

AActor* UGameModeAbility::FindNearestActorAndLookAt(const TArray<AActor*>& Actors, const bool bFocus,
                                                    const bool bKeep,
                                                    const bool bInUseTurn, const bool bLookAtOnEmpty)
{
	if (!IsValid(Character)) { return nullptr; }
	UFocusComponent* FocusComponent = Character->GetOwner()->GetComponentByClass<UFocusComponent>();
	if (Actors.IsEmpty())
	{
		if (bLookAtOnEmpty)
		{
			if (IsValid(FocusComponent))
			{
				FocusComponent->LookAt(nullptr, bFocus, bKeep, bInUseTurn);
			}
		}
		return nullptr;
	}
	AActor* Target = FindNearestActor(Actors);
	if (IsValid(FocusComponent))
	{
		FocusComponent->LookAt(Target->GetRootComponent(), bFocus, bKeep, bInUseTurn);
	}
	return Target;
}

void UGameModeAbility::LookAtActor(AActor* Target, const bool bFocus, const bool bKeep, const bool bInUseTurn)
{
	if (IsValid(Character))
	{
		UFocusComponent* FocusComponent = Character->GetOwner()->GetComponentByClass<UFocusComponent>();
		if (IsValid(FocusComponent))
		{
			FocusComponent->LookAt(Target->GetRootComponent(), bFocus, bKeep, bInUseTurn);
		}
	}
}

UGameEffect* UGameModeAbility::MakeGameEffectOnly(TSubclassOf<UGameEffect> Class, AActor* Target) const
{
	return UGameEffect::MakeGameEffectOnly(this, Class, GetCharacter()->GetOwner(), Target);
}

void UGameModeAbility::MakeAndApplyGameEffect(TSubclassOf<UGameEffect> Class, AActor* Target,
                                              const int32 TargetIndex, FTransparentParameters Parameters)
{
	UGameEffect::MakeAndApplyGameEffect(this, Class, GetCharacter()->GetOwner(), Target, this, TargetIndex, Parameters);
}

void UGameModeAbility::OnAttackActor_Implementation(AActor* Target, const FAttackContext& Context)
{
}

bool UGameModeAbility::CanAttack_Implementation(TSubclassOf<UGameEffect>& Class, AActor* Target,
                                                const int32& TargetIndex, FTransparentParameters& Parameters)
{
	return true;
}

bool UGameModeAbility::AttackOverlappingActors(TSubclassOf<UGameEffect> GameEffect, FTransparentParameters Parameters,
                                               const bool bClearCache)
{
	return AttackOverlappingActors(GameEffect, Parameters, bClearCache, [&]
	{
		TArray<AActor*> InOverlappingActors;
		if (IsValid(Character))
		{
			Character->GetOwner()->GetOverlappingActors(InOverlappingActors);
		}
		return InOverlappingActors;
	});
}

bool UGameModeAbility::AttackOverlappingActorsOfClass(TSubclassOf<AActor> ActorClass,
                                                      TSubclassOf<UGameEffect> GameEffect,
                                                      FTransparentParameters Parameters,
                                                      const bool bClearCache)
{
	return AttackOverlappingActors(GameEffect, Parameters, bClearCache, [&]
	{
		TArray<AActor*> InOverlappingActors;
		GetOverlappingActorsOfClass(InOverlappingActors, ActorClass, Character->GetOwner());
		return InOverlappingActors;
	});
}

bool UGameModeAbility::AttackOverlappingActorsOfTag(const FName InTag, TSubclassOf<UGameEffect> GameEffect,
                                                    FTransparentParameters Parameters, const bool bClearCache)
{
	return AttackOverlappingActors(GameEffect, Parameters, bClearCache, [&]
	{
		TArray<AActor*> InOverlappingActors;
		GetOverlappingActorsOfTag(InOverlappingActors, InTag, Character->GetOwner());
		return InOverlappingActors;
	});
}

bool UGameModeAbility::AttackOverlappingActorsOfTags(const TArray<FName>& Tags, TSubclassOf<UGameEffect> GameEffect,
                                                     FTransparentParameters Parameters, const bool bClearCache)
{
	return AttackOverlappingActors(GameEffect, Parameters, bClearCache, [&]
	{
		TArray<AActor*> InOverlappingActors;
		GetOverlappingActorsOfTags(InOverlappingActors, Tags, Character->GetOwner());
		return InOverlappingActors;
	});
}

UGameplayTasksComponent* UGameModeAbility::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	return !bCompleted && GetCharacter() ? GetCharacter()->GetGameplayTasks() : nullptr;
}

AActor* UGameModeAbility::GetGameplayTaskOwner(const UGameplayTask* Task) const
{
	return bCompleted ? nullptr : GetCharacter()->GetOwner();
}

AActor* UGameModeAbility::GetGameplayTaskAvatar(const UGameplayTask* Task) const
{
	return bCompleted ? nullptr : GetCharacter()->GetOwner();
}

void UGameModeAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	UFEConsole::Write(UStringConverter::ConvertEnum(Task.GetState()));
}

void UGameModeAbility::OnGameplayTaskActivated(UGameplayTask& Task)
{
	ActiveTasks.Push(&Task);
}

void UGameModeAbility::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	ActiveTasks.Remove(&Task);
}
