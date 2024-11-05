// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Controller/VRPlayerControllerComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/Pawn/VRPawnOperationsComponent.h"
#include "Defines/VRInputDefines.h"
#include "Log/FEConsole.h"

void UVRPlayerControllerComponent::BindInputDefines(UEnhancedInputComponent* EnhancedInputComponent,
                                                    UInputDefines* Defines)
{
	if (const UVRInputDefines* VRDefines = Cast<UVRInputDefines>(Defines))
	{
		EnhancedInputComponent->BindAction(VRDefines->GetIA_VRTurn(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnVRTurn);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_VRMove(), ETriggerEvent::Triggered, this,
		                                   &UVRPlayerControllerComponent::OnVRMoveTriggered);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_VRMove(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnVRMoveStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_VRMove(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnVRMoveCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Grab_Left(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnGrabLeftStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Grab_Left(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnGrabLeftCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Grab_Right(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnGrabRightStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Grab_Right(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnGrabRightCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Menu_Toggle_Left(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnMenuToggleLeft);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Menu_Toggle_Right(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnMenuToggleRight);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Left(), ETriggerEvent::Triggered, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspLeftTriggered);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Left(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspLeftCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Left(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspLeftCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Right(), ETriggerEvent::Triggered, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspRightTriggered);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Right(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspRightCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Grasp_Right(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspRightCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Left(), ETriggerEvent::Triggered, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlLeftTriggered);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Left(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlLeftCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Left(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlLeftCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Right(), ETriggerEvent::Triggered, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlRightTriggered);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Right(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlRightCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_IndexCurl_Right(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandIndexCurlRightCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Left(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnHandPointLeftStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Left(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspLeftCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Left(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandGraspLeftCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Right(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnHandPointRightStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Right(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandPointRightCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_Point_Right(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandPointRightCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Left(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpLeftStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Left(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpLeftCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Left(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpLeftCompleted);

		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Right(), ETriggerEvent::Started, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpRightStarted);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Right(), ETriggerEvent::Canceled, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpRightCanceled);
		EnhancedInputComponent->BindAction(VRDefines->GetIA_Hand_ThumbUp_Right(), ETriggerEvent::Completed, this,
		                                   &UVRPlayerControllerComponent::OnHandThumbUpRightCompleted);
	}
}

void UVRPlayerControllerComponent::GetPawnVROperationsComponents(
	TArray<UVRPawnOperationsComponent*>& OperationsComponents) const
{
	if (const APawn* CurrentPawn = GetPawn())
	{
		CurrentPawn->GetComponents<UVRPawnOperationsComponent>(OperationsComponents);
	}
}

#pragma region 手柄
void UVRPlayerControllerComponent::OnVRTurn_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->SnapTurn(Instance.GetValue().Get<float>() > 0);
	}
}


void UVRPlayerControllerComponent::OnVRMoveStarted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Teleport(ETeleportState::Started);
	}
}

void UVRPlayerControllerComponent::OnVRMoveTriggered_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Teleport(ETeleportState::Triggered);
	}
}

void UVRPlayerControllerComponent::OnVRMoveCompleted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Teleport(ETeleportState::Completed);
	}
}

void UVRPlayerControllerComponent::OnGrabLeftStarted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Grab(true, true);
	}
}

void UVRPlayerControllerComponent::OnGrabLeftCompleted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Grab(true, false);
	}
}

void UVRPlayerControllerComponent::OnGrabRightStarted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Grab(false, true);
	}
}

void UVRPlayerControllerComponent::OnGrabRightCompleted_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->Grab(false, false);
	}
}

void UVRPlayerControllerComponent::OnMenuToggleLeft_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->MenuToggle(true);
	}
}

void UVRPlayerControllerComponent::OnMenuToggleRight_Implementation(const FInputActionInstance& Instance)
{
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->MenuToggle(false);
	}
}
#pragma endregion

#pragma region 手势
void UVRPlayerControllerComponent::OnHandGraspLeftTriggered_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::StartedOrTriggered, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandGraspLeftCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::Canceled, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandGraspLeftCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::Completed, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandGraspRightTriggered_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::StartedOrTriggered, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandGraspRightCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::Canceled, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandGraspRightCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandGrasp(EHandGraspState::Completed, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlLeftTriggered_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::StartedOrTriggered, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlLeftCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::Canceled, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlLeftCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::Completed, true, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlRightTriggered_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::StartedOrTriggered, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlRightCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::Canceled, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandIndexCurlRightCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandIndexCurl(EHandGraspState::Completed, false, Instance.GetValue().Get<float>());
	}
}

void UVRPlayerControllerComponent::OnHandPointLeftStarted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::StartedOrTriggered, true);
	}
}

void UVRPlayerControllerComponent::OnHandPointLeftCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::Canceled, true);
	}
}

void UVRPlayerControllerComponent::OnHandPointLeftCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::Completed, true);
	}
}

void UVRPlayerControllerComponent::OnHandPointRightStarted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::StartedOrTriggered, false);
	}
}

void UVRPlayerControllerComponent::OnHandPointRightCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::Canceled, false);
	}
}

void UVRPlayerControllerComponent::OnHandPointRightCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandPoint(EHandGraspState::Completed, false);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpLeftStarted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::StartedOrTriggered, true);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpLeftCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::Canceled, true);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpLeftCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::Completed, true);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpRightStarted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::StartedOrTriggered, false);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpRightCanceled_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::Canceled, false);
	}
}

void UVRPlayerControllerComponent::OnHandThumbUpRightCompleted_Implementation(const FInputActionInstance& Instance)
{
	if (!bUseHandsTracker) { return; }
	TArray<UVRPawnOperationsComponent*> OperationsComponents;
	GetPawnVROperationsComponents(OperationsComponents);
	for (UVRPawnOperationsComponent* Operations : OperationsComponents)
	{
		Operations->HandThumbUp(EHandGraspState::Completed, false);
	}
}
#pragma endregion
