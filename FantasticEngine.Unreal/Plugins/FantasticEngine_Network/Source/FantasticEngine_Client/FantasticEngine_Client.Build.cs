// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FantasticEngine_Client : ModuleRules
{
	public FantasticEngine_Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PrivateIncludePaths.Add("Framework/Private");
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "UMG", "Sockets", "Networking", "HTTP",
			"FantasticEngine_Common", "FantasticEngine_Network"
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