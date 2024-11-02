// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/PlayerInfoPosterComponent.h"

#include "GameRoot.h"
#include "Components/Pawn/PawnOperationsComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values for this component's properties
UPlayerInfoPosterComponent::UPlayerInfoPosterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UPlayerInfoPosterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PostPlayerInfo();
}

void UPlayerInfoPosterComponent::PostPlayerInfo_Implementation()
{
	if (!MaterialParameterCollectionInstance.IsValid())
	{
		UGameDefines* GameDefines = GetGameDefines<UGameDefines>(this);
		if (GameDefines)
		{
			MaterialParameterCollectionInstance = GameDefines->GetMPC_Framework();
		}
	}
	if (MaterialParameterCollectionInstance.IsValid())
	{
		const APlayerController* Controller = Cast<APlayerController>(GetOwner());
		if (IsValid(Controller))
		{
			FVector WorldLocation = Controller->K2_GetActorLocation();
			if (const APawn* Pawn = Controller->GetPawn())
			{
				WorldLocation = Pawn->GetActorLocation();
				if (const UPawnOperationsComponent* OperationsComponent = Pawn->GetComponentByClass<
					UPawnOperationsComponent>())
				{
					if (const USpringArmComponent* ArmComponent = OperationsComponent->GetCameraBoom())
					{
						const float ArmLength = ArmComponent->TargetArmLength;
						UKismetMaterialLibrary::SetScalarParameterValue(
							GetWorld(), MaterialParameterCollectionInstance.Get(), FName("PlayerArmLength"),
							ArmLength);
					}
				}
			}
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollectionInstance.Get(),
			                                                FName("PlayerWorldPosition"),
			                                                FLinearColor(WorldLocation));
		}
	}

	OnPostPlayerInfo.Broadcast();
	K2_OnPostPlayerInfo.Broadcast();
}
