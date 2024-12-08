#include "AssetTypeActions_BlueprintThumbnail.h"

#include "GameRoot.h"
#include "Basis/Level/Map.h"
#include "FEGameInstance.h"
#include "Engine/UserDefinedStruct.h"

const FSlateBrush* FAssetTypeActions_BlueprintThumbnail::CheckAssetData(const TSubclassOf<UObject>& ParentClass) const
{
	if (ParentClass->IsChildOf<UGameRoot>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameRoot")).GetIcon();
	}
	if (ParentClass->IsChildOf<UFEGameInstance>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameInstance")).GetIcon();
	}
	if (ParentClass->IsChildOf<UGameDefines>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameDefines")).GetIcon();
	}

	if (ParentClass->IsChildOf<UEventParam>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameEvent")).GetIcon();
	}
	if (ParentClass->IsChildOf<USystemBasis>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameSystem")).GetIcon();
	}
	if (ParentClass->IsChildOf<UMap>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameScene")).GetIcon();
	}
	if (ParentClass->IsChildOf<UDataTable>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameScene")).GetIcon();
	}
	if (ParentClass->IsChildOf<UUserDefinedStruct>())
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.GameScene")).GetIcon();
	}
	return nullptr;
}

const FSlateBrush* FAssetTypeActions_BlueprintThumbnail::GetThumbnailBrush(const FAssetData& InAssetData,
                                                                           const FName InClassName) const
{
	UBlueprint* Blueprint = Cast<UBlueprint>(InAssetData.GetAsset());
	if (!Blueprint)
	{
		return FAssetTypeActions_Blueprint::GetThumbnailBrush(InAssetData, InClassName);
	}
	if (!Blueprint->ParentClass)
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.BlueprintError")).GetIcon();
	}
	if (const FSlateBrush* Brush = CheckAssetData(Blueprint->ParentClass))
	{
		return Brush;
	}
	return FAssetTypeActions_Blueprint::GetThumbnailBrush(InAssetData, InClassName);
}

const FSlateBrush* FAssetTypeActions_BlueprintThumbnail::GetIconBrush(const FAssetData& InAssetData,
                                                                      const FName InClassName) const
{
	UBlueprint* Blueprint = Cast<UBlueprint>(InAssetData.GetAsset());
	if (!Blueprint)
	{
		return FAssetTypeActions_Blueprint::GetIconBrush(InAssetData, InClassName);
	}
	if (!Blueprint->ParentClass)
	{
		return FSlateIcon(TEXT("FantasyEngineStyle"),TEXT("FantasyEngineEditor.BlueprintError")).GetIcon();
	}
	if (const FSlateBrush* Brush = CheckAssetData(Blueprint->ParentClass))
	{
		return Brush;
	}
	return FAssetTypeActions_Blueprint::GetIconBrush(InAssetData, InClassName);
}
