#pragma once
#include "AssetTypeActions_BlueprintThumbnail.h"

class FANTASTICENGINE_GAMEPLAY_EDITOR_API FAssetTypeActions_GameplayBlueprintThumbnail : public FAssetTypeActions_BlueprintThumbnail
{
public:
	virtual const FSlateBrush* CheckAssetData(const TSubclassOf<UObject>& ParentClass) const override;
};
