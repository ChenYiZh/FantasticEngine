// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_Parameters.h"

UAnimNotify_Parameters::UAnimNotify_Parameters(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UAnimNotify_Parameters::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->OnPlayMontageNotifyBegin.Broadcast(NotifyName, BranchingPointPayload);
		}
	}
}


FString UAnimNotify_Parameters::GetNotifyName_Implementation() const
{
	return NotifyName.ToString();
}

#if WITH_EDITOR
bool UAnimNotify_Parameters::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif // WITH_EDITOR