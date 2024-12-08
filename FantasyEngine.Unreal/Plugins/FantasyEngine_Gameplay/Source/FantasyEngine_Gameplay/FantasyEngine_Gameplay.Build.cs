// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FantasyEngine_Gameplay : ModuleRules
{
	public FantasyEngine_Gameplay(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "GameplayTasks", "EnhancedInput",
			"FantasyEngine_Common", "FantasyEngine_Framework", "FantasyEngine_VirtualInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		bEnableExceptions = true;
	}
}