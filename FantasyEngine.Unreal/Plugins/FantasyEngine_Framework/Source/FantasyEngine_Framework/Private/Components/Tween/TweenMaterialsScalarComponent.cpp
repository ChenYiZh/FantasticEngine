// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Tween/TweenMaterialsScalarComponent.h"


void UTweenMaterialsScalarComponent::Play_Implementation(USceneComponent* Component, float Alpha)
{
	if (Materials.IsEmpty())
	{
		Materials.Empty();
		if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component))
		{
			TArray<UMaterialInterface*> MaterialInterfaces = MeshComponent->GetMaterials();
			const int32 Count = MaterialInterfaces.Num();
			for (int32 Index = 0; Index < Count; Index++)
			{
				UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
					MaterialInterfaces[Index], MeshComponent);
				Materials.Add(MaterialInstanceDynamic);
				MeshComponent->SetMaterial(Index, MaterialInstanceDynamic);
			}
		}
	}
	for (int32 Index = Materials.Num() - 1; Index >= 0; --Index)
	{
		Materials[Index]->SetScalarParameterValue(ParameterName, FMath::Lerp(From, To, Alpha));
	}
}
