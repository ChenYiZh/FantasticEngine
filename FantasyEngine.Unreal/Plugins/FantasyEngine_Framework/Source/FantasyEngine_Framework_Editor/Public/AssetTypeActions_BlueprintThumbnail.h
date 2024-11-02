#pragma once
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"
#include "AssetTypeActions/AssetTypeActions_DataAsset.h"

class FANTASYENGINE_FRAMEWORK_EDITOR_API FAssetTypeActions_BlueprintThumbnail : public FAssetTypeActions_Blueprint
{
public:
	virtual const FSlateBrush* CheckAssetData(const TSubclassOf<UObject>& ParentClass) const;

public:
	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};


// class FANTASYENGINE_FRAMEWORK_EDITOR_API FAssetTypeActions_UDataTableThumbnail : public FAssetTypeActions_Base
// {
// 	public:
// 	virtual const FSlateBrush* CheckAssetData(const TSubclassOf<UObject>& ParentClass) const;
//
// 	public:
// 	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
//
// 	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override;
// };