// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if 0
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "GameRoot.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"
#include "GameRootFactory.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UGameRootFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category=GameRootFactory, meta=(AllowAbstract = ""))
	TSubclassOf<class UGameRoot> ParentClass;

	virtual bool ConfigureProperties() override;
	virtual bool ShouldShowInNewMenu() const override;


	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

class FAssetTypeActions_GameRoot : public FAssetTypeActions_Base
{
public:
	//FAssetTypeActions_TestAsset(EAssetTypeCategories::Type InAssetCategory);

	EAssetTypeCategories::Type Category;
	virtual FColor GetTypeColor() const override { return FColor(255, 107, 129); }
	// virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
	//                              TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

	virtual FText GetName() const override { return FText::FromString(TEXT("Game Root")); }

	virtual UClass* GetSupportedClass() const override { return UGameRoot::StaticClass(); }

	virtual uint32 GetCategories() override { return Category; }

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
#endif