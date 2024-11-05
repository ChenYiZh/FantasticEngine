// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_CameraShake.h"

#include "Kismet/GameplayStatics.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (const UWorld* World = MeshComp->GetWorld())
	{
		UGameplayStatics::PlayWorldCameraShake(World, Shake,
		                                       MeshComp->GetOwner()->GetActorLocation(), InnerRadius,
		                                       OuterRadius, Falloff, bOrientShakeTowardsEpicenter);
	}
}
