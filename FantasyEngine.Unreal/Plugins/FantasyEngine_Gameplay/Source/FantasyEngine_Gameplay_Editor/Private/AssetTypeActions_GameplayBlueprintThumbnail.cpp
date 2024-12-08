#include "AssetTypeActions_GameplayBlueprintThumbnail.h"

// #include "Gameplay/ActorStrategy.h"
#include "Gameplay/Ability/GameModeAbility.h"
#include "Gameplay/Attribute/ActorAttributeSet.h"
#include "Gameplay/GameEffect.h"

const FSlateBrush* FAssetTypeActions_GameplayBlueprintThumbnail::CheckAssetData(
	const TSubclassOf<UObject>& ParentClass) const
{
	// if (ParentClass->IsChildOf<UActorStrategy>())
	// {
	// 	return FSlateIcon(TEXT("FantasyEngineGameplayStyle"),TEXT("FantasyEngineGameplayEditor.Strategy")).GetIcon();
	// }
	if (ParentClass->IsChildOf<UGameModeAbility>())
	{
		return FSlateIcon(TEXT("FantasyEngineGameplayStyle"),TEXT("FantasyEngineGameplayEditor.Ability")).GetIcon();
	}
	if (ParentClass->IsChildOf<UActorAttributeSet>())
	{
		return FSlateIcon(TEXT("FantasyEngineGameplayStyle"),TEXT("FantasyEngineGameplayEditor.Attributes")).GetIcon();
	}
	if (ParentClass->IsChildOf<UGameEffect>())
	{
		return FSlateIcon(TEXT("FantasyEngineGameplayStyle"),TEXT("FantasyEngineGameplayEditor.GameEffect")).GetIcon();
	}
	return FAssetTypeActions_BlueprintThumbnail::CheckAssetData(ParentClass);
}
