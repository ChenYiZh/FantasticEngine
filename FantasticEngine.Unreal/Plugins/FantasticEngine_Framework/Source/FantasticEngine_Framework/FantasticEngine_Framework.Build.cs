// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class FantasticEngine_Framework : ModuleRules
{
	public FantasticEngine_Framework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// PrivateIncludePaths.AddRange(
		// 	new[]
		// 	{
		// 		"OnlineSubsystemUtils",
		// 	}
		// );
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "UMG", "PhysicsCore", "HTTP",
			"ImageWrapper", "RHI", "RenderCore", "Niagara", "NavigationSystem", "HeadMountedDisplay",
			//"OnlineSubsystem", "OnlineSubsystemUtils",
			"GameplayTags", "EnhancedInput", "FantasticEngine_Common"
		});

		//PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");


		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		//PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		if (Target.bBuildEditor)
		{
			//PRAGMA_DISABLE_OPTIMIZATION
			//PRAGMA_ENABLE_OPTIMIZATION
			OptimizeCode = CodeOptimization.Never;
			//PrivateDependencyModuleNames.Add("UnrealEd");
		}

		bEnableExceptions = true;
	}
}