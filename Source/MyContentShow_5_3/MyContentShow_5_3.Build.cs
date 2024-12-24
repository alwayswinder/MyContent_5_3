// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyContentShow_5_3 : ModuleRules
{
	public MyContentShow_5_3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"UnrealEd",
			
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ProceduralMeshComponent",
			"SimplexNoise",
			"RenderCore",
			"MassEntity",
			"MassCommon",	
			"MassNavigation",
			"StructUtils",
			"MassMovement",
			"NavigationSystem",
			"AIModule",
			"MassAIBehavior",
			"StateTreeModule",
			"GameplayTags",
			"SmartObjectsModule",
			"MassSmartObjects",
			"MassSignals",
			"MassRepresentation",
			"SourceControl",
			"SourceControlWindows", "SourceControlWindows",
			
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
