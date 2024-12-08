// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Task/GameplayTask_PlayMontage.h"

#include "GameplayTasksComponent.h"
#include "AnimNotifies/AnimNotifyState_Parameters.h"
#include "AnimNotifies/AnimNotify_Parameters.h"
#include "Components/GameplayCharacterComponent.h"
#include "Gameplay/Ability/GameModeAbility_Montage.h"
#include "Gameplay/Ability/GameModeAbility_SingleMontage.h"
#include "Log/FEConsole.h"

void UGameplayTask_PlayMontage::Activate()
{
	Super::Activate();
	if (!IsValid(TaskOwner.GetObject())) { return; }
	bool bPlayedSuccessfully = false;
	if (SkeletalMeshComponent.IsValid())
	{
		if (UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, Rate,
			                                                       EMontagePlayReturnType::MontageLength,
			                                                       StartTimeSeconds);
			bPlayedSuccessfully = MontageLength > 0.f;
			if (bPlayedSuccessfully)
			{
				AnimInstancePtr = AnimInstance;
				if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
				{
					MontageInstanceID = MontageInstance->GetInstanceID();
				}

				if (StartSection != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(StartSection, MontageToPlay);
				}

				// BlendingOutDelegate.BindUObject(this, &UGameplayTask_PlayMontage::OnMontageBlendingOut);
				// AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UGameplayTask_PlayMontage::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UGameplayTask_PlayMontage::OnNotifyBeginReceived);
				AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UGameplayTask_PlayMontage::OnNotifyEndReceived);
				
				if (UGameModeAbility_Montage* Ability = Cast<UGameModeAbility_Montage>(TaskOwner.GetObject()))
				{
					Ability->ExecuteMontageBegin(MontageToPlay);
				}
			}
		}
	}
}

void UGameplayTask_PlayMontage::OnDestroy(bool bInOwnerFinished)
{
	if (AnimInstancePtr.IsValid())
	{
		FAnimMontageInstance* Instance = AnimInstancePtr->GetMontageInstanceForID(MontageInstanceID);
		if (Instance != nullptr && Instance->Montage == MontageToPlay && Instance->IsPlaying())
		{
			Instance->Stop(FAlphaBlend());
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

UGameplayTask_PlayMontage* UGameplayTask_PlayMontage::CreateGameplayTaskPlayMontageProxy(
	TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, USkeletalMeshComponent* InSkeletalMeshComponent,
	FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds,
	float StartTimeSeconds)
{
	if (!IsValid(TaskOwner.GetObject()))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("IGameplayTaskOwnerInterface is null or error."));
		return nullptr;
	}
	if (!IsValid(InSkeletalMeshComponent))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("USkeletalMeshComponent is null or error."));
		return nullptr;
	}
	if (!IsValid(InMontageToPlay))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("UAnimMontage is null or error."));
		return nullptr;
	}
	UGameplayTask_PlayMontage* MyTask = NewTask<UGameplayTask_PlayMontage>(TaskOwner, TaskInstanceName);
	if (MyTask)
	{
		MyTask->MontageToPlay = InMontageToPlay;
		MyTask->SkeletalMeshComponent = InSkeletalMeshComponent;
		MyTask->Rate = Rate;
		MyTask->StartSection = StartSection;
		MyTask->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
		MyTask->StartTimeSeconds = StartTimeSeconds;
	}
	if (UGameplayTasksComponent* TasksComponent = TaskOwner->GetGameplayTasksComponent(*MyTask))
	{
		TasksComponent->RunGameplayTask(*TaskOwner, *MyTask, FGameplayTasks::DefaultPriority, FGameplayResourceSet(),
		                                FGameplayResourceSet());
	}
	return MyTask;
}

UGameplayTask_PlayMontage* UGameplayTask_PlayMontage::CreateTaskPlayMontageProxy(
	UGameModeAbility_Montage* TaskOwner, FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate,
	FName StartSection, bool bStopWhenAbilityEnds, float StartTimeSeconds)
{
	if (!IsValid(TaskOwner))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("UGameModeAbilityMontage is null or error."));
		return nullptr;
	}
	UGameplayCharacterComponent* Character = TaskOwner->GetCharacter();
	if (!IsValid(Character))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("The AStandardCharacter is null or error."));
		return nullptr;
	}
	return CreateGameplayTaskPlayMontageProxy(TaskOwner, Character->GetMesh(), TaskInstanceName, InMontageToPlay, Rate,
	                                          StartSection, bStopWhenAbilityEnds, StartTimeSeconds);
}

UGameplayTask_PlayMontage* UGameplayTask_PlayMontage::CreateTaskPlaySingleMontageProxy(
	UGameModeAbility_SingleMontage* TaskOwner, FName TaskInstanceName, float Rate, FName StartSection,
	bool bStopWhenAbilityEnds, float StartTimeSeconds)
{
	if (!IsValid(TaskOwner))
	{
		UFEConsole::WriteErrorWithCategory(TEXT("Montage Task"),TEXT("UGameModeAbilitySingleMontage is null or error."));
		return nullptr;
	}
	return CreateTaskPlayMontageProxy(TaskOwner, TaskInstanceName, TaskOwner->GetMontageToPlay(), Rate, StartSection,
	                                  bStopWhenAbilityEnds, StartTimeSeconds);
}


// void UGameplayTask_PlayMontage::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
// {
// 	if (bInterrupted)
// 	{
// 		OnInterrupted.Broadcast(NAME_None);
// 		bInterruptedCalledBeforeBlendingOut = true;
// 	}
// 	else
// 	{
// 		OnBlendOut.Broadcast(NAME_None);
// 	}
// }

void UGameplayTask_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FTransparentParameters ThisParam;
	//ThisParam.bValid = false;
	if (!bInterrupted)
	{
		OnCompleted.Broadcast(NAME_None, ThisParam);
	}
	else
	{
		if (!NotifyCache.IsEmpty())
		{
			for (auto KeyValue : NotifyCache)
			{
				if (KeyValue.Value == nullptr)
				{
					OnNotifyEnd.Broadcast(KeyValue.Key, ThisParam);
				}
				else
				{
					OnNotifyEnd.Broadcast(KeyValue.Key, *KeyValue.Value);
				}
			}
		}
		OnInterrupted.Broadcast(NAME_None, ThisParam);
	}
	// else if (!bInterruptedCalledBeforeBlendingOut)
	// {
	// 	OnInterrupted.Broadcast(NAME_None);
	// }

	UnbindDelegates();
	if (UGameModeAbility_Montage* Ability = Cast<UGameModeAbility_Montage>(TaskOwner.GetObject()))
	{
		Ability->ExecuteMontageEnd(MontageToPlay);
	}
	EndTask();
}

void UGameplayTask_PlayMontage::UnbindDelegates()
{
	if (UAnimInstance* AnimInstance = AnimInstancePtr.Get())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UGameplayTask_PlayMontage::OnNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &UGameplayTask_PlayMontage::OnNotifyEndReceived);
	}
}

void UGameplayTask_PlayMontage::OnNotifyBeginReceived(FName NotifyName,
                                                      const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	bool IsNotifyState = true;
	FTransparentParameters* Parameters = nullptr;
	if (BranchingPointPayload.NotifyEvent != nullptr)
	{
		FAnimNotifyEvent Event = *BranchingPointPayload.NotifyEvent;
		if (Event.Notify)
		{
			IsNotifyState = false;
			if (UAnimNotify_Parameters* ThisNotify = Cast<UAnimNotify_Parameters>(Event.Notify))
			{
				Parameters = &ThisNotify->Parameters;
			}
		}
		else if (Event.NotifyStateClass)
		{
			if (UAnimNotifyState_Parameters* ThisNotifyState = Cast<
				UAnimNotifyState_Parameters>(Event.NotifyStateClass))
			{
				Parameters = &ThisNotifyState->Parameters;
			}
		}
	}
	if (IsNotifyState)
	{
		TTuple<FName, FTransparentParameters*> KeyValue(NotifyName, Parameters);
		NotifyCache.Push(KeyValue);
	}
	if (Parameters == nullptr)
	{
		FTransparentParameters ThisParam;
		//ThisParam.bValid = false;
		Parameters = &ThisParam;
	}
	if (UGameModeAbility_Montage* Ability = Cast<UGameModeAbility_Montage>(TaskOwner.GetObject()))
	{
		Ability->ExecuteNofityBegin(MontageToPlay, NotifyName, *Parameters);
	}
	OnNotifyBegin.Broadcast(NotifyName, *Parameters);
}

void UGameplayTask_PlayMontage::OnNotifyEndReceived(FName NotifyName,
                                                    const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FTransparentParameters* Parameters = nullptr;
	if (BranchingPointPayload.NotifyEvent != nullptr)
	{
		FAnimNotifyEvent Event = *BranchingPointPayload.NotifyEvent;
		if (Event.NotifyStateClass)
		{
			if (UAnimNotifyState_Parameters* ThisNotifyState = Cast<
				UAnimNotifyState_Parameters>(Event.NotifyStateClass))
			{
				Parameters = &ThisNotifyState->Parameters;
			}
		}
	}
	for (int32 i = NotifyCache.Num() - 1; i >= 0; i--)
	{
		if (NotifyCache[i].Key == NotifyName && NotifyCache[i].Value == Parameters)
		{
			NotifyCache.RemoveAt(i);
			break;
		}
	}
	if (Parameters == nullptr)
	{
		FTransparentParameters ThisParam;
		//ThisParam.bValid = false;
		Parameters = &ThisParam;
	}
	if (UGameModeAbility_Montage* Ability = Cast<UGameModeAbility_Montage>(TaskOwner.GetObject()))
	{
		Ability->ExecuteNofityEnd(MontageToPlay, NotifyName, *Parameters);
	}
	OnNotifyEnd.Broadcast(NotifyName, *Parameters);
}
