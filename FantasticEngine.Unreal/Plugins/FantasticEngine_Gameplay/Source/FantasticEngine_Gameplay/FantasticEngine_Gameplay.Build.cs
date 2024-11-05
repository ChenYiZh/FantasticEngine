// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FantasticEngine_Gameplay : ModuleRules
{
	public FantasticEngine_Gameplay(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "GameplayTasks", "EnhancedInput",
			"FantasticEngine_Common", "FantasticEngine_Framework", "FantasticEngine_VirtualInput"
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