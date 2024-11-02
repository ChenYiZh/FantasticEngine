// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basis/SystemBasis.h"
#include "TextSystem.generated.h"

/** 文本生命周期管理类 */
UCLASS()
class FANTASYENGINE_FRAMEWORK_API UTextSystem : public USystemBasis
{
	GENERATED_BODY()

public:
	virtual void OnInitialize_Implementation() override;
};
