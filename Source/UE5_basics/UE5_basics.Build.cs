// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_basics : ModuleRules
{
	public UE5_basics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
