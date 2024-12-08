// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WorldLight.h"

#include "GameRoot.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

// Sets default values
AWorldLight::AWorldLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;
	ProjectionCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Projection Capture"));
	ProjectionCapture->SetupAttachment(RootComponent);
	ProjectionCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	ProjectionCapture->bUseFauxOrthoViewPos = true;
	ProjectionCapture->PostProcessBlendWeight = 0.f;
	ProjectionCapture->ShowFlags.Fog = 0;
}

// Called when the game starts or when spawned
void AWorldLight::BeginPlay()
{
	Super::BeginPlay();
	bInited = false;
}

// Called every frame
void AWorldLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RefreshCapture();
	if (!bInited && UGameRoot::GetRoot(this))
	{
		bInited = true;
		const UGameDefines* GameDefines = UGameRoot::GetGameDefines(this);
		const UMaterialParameterCollection* MaterialCollection = GameDefines->GetMPC_Framework();
		if (MaterialCollection && MPC_Framework.Get() != MaterialCollection)
		{
			MPC_Framework = GameDefines->GetMPC_Framework();
		}
	}
	if (MPC_Framework)
	{
		PostLightInfo();
		PostProjectionInfo();
	}
}

void AWorldLight::RefreshCapture()
{
	ProjectionCapture->SetRelativeLocation(FVector::BackwardVector * LightCaptureDistance);
	if (const APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		const ACharacter* Character = Cast<ACharacter>(Controller->GetPawn());
		const APlayerCameraManager* Camera = Controller->PlayerCameraManager;
		if (Character && Camera)
		{
			const UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
			float DeltaHeight = 0;
			if (CapsuleComponent)
			{
				DeltaHeight = -CapsuleComponent->GetScaledCapsuleHalfHeight();
			}
			FVector CameraPosition = Camera->K2_GetActorLocation();
			float Z = Character->K2_GetActorLocation().Z + DeltaHeight;
			CameraPosition.Z = Z;
			SetActorLocation(CameraPosition);
			const FVector Direction = -GetActorForwardVector();
			FVector PlaneDirection = Direction;
			PlaneDirection.Z = 0.f;
			PlaneDirection.Normalize();
			const float CosTheta = FVector::DotProduct(Direction, PlaneDirection);
			// float Length = (PlaneDirection.Length() * ShadowDistance) / CosTheta * 10;
			// ProjectionCapture->SetRelativeLocation(FVector::BackwardVector * Length);
			const float Bottom = ShadowDistance * CosTheta;
			ProjectionCapture->OrthoWidth = 210 * FMath::Sqrt(ShadowDistance * ShadowDistance - Bottom * Bottom);
		}
	}
}

void AWorldLight::PostLightInfo()
{
	if (MPC_Framework)
	{
		const FVector Direction = -GetActorForwardVector();
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightDirection"),
		                                                FLinearColor(Direction));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightColor"),
		                                                LightColor);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework, FName("WorldShadowIntensity"),
		                                                ShadowIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework, FName("WorldShadowDistance"),
		                                                ShadowDistance);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework,
		                                                FName("bUseCustomLight"),
		                                                bUseCustomLight ? 1.f : -1.f);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework, FName("WorldShadowRange"),
		                                                ShadowRange);
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework, FName("WorldShadowBias"),
		                                                ShadowBias);
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldShadowColor"),
		                                                FLinearColor(ShadowColorHSV.X, ShadowColorHSV.Y,
		                                                             ShadowColorHSV.Z, ShadowColorHSV.W));
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Framework,
		                                                FName("WorldShadowProjectionIntensity"),
		                                                ShadowProjectionIntensity);
	}
}

void AWorldLight::PostProjectionInfo()
{
	PostViewMatrix();
	PostOrthoMatrix();
}

void AWorldLight::PostViewMatrix()
{
	if (MPC_Framework)
	{
		const FLookFromMatrix ViewMatrix = FLookFromMatrix(ProjectionCapture->GetComponentLocation(),
		                                                   ProjectionCapture->GetForwardVector(),
		                                                   ProjectionCapture->GetUpVector());
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightViewMatrixX"),
		                                                FLinearColor(ViewMatrix.M[0][0], ViewMatrix.M[0][1],
		                                                             ViewMatrix.M[0][2], ViewMatrix.M[0][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightViewMatrixY"),
		                                                FLinearColor(ViewMatrix.M[1][0], ViewMatrix.M[1][1],
		                                                             ViewMatrix.M[1][2], ViewMatrix.M[1][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightViewMatrixZ"),
		                                                FLinearColor(ViewMatrix.M[2][0], ViewMatrix.M[2][1],
		                                                             ViewMatrix.M[2][2], ViewMatrix.M[2][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightViewMatrixW"),
		                                                FLinearColor(ViewMatrix.M[3][0], ViewMatrix.M[3][1],
		                                                             ViewMatrix.M[3][2], ViewMatrix.M[3][3]));
	}
}

void AWorldLight::PostOrthoMatrix()
{
	if (MPC_Framework)
	{
		const float Width = ProjectionCapture->OrthoWidth / 2.f;
		const FOrthoMatrix OrthoMatrix = FOrthoMatrix(Width, -Width, 1, 0);
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightOrthoMatrixX"),
		                                                FLinearColor(OrthoMatrix.M[0][0], OrthoMatrix.M[0][1],
		                                                             OrthoMatrix.M[0][2], OrthoMatrix.M[0][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightOrthoMatrixY"),
		                                                FLinearColor(OrthoMatrix.M[1][0], OrthoMatrix.M[1][1],
		                                                             OrthoMatrix.M[1][2], OrthoMatrix.M[1][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightOrthoMatrixZ"),
		                                                FLinearColor(OrthoMatrix.M[2][0], OrthoMatrix.M[2][1],
		                                                             OrthoMatrix.M[2][2], OrthoMatrix.M[2][3]));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Framework, FName("WorldLightOrthoMatrixW"),
		                                                FLinearColor(OrthoMatrix.M[3][0], OrthoMatrix.M[3][1],
		                                                             OrthoMatrix.M[3][2], OrthoMatrix.M[3][3]));
	}
}
