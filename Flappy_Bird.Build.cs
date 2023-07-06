// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Flappy_Bird : ModuleRules
{
	public Flappy_Bird(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		
	}
}
