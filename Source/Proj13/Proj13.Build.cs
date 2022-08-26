// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Proj13 : ModuleRules
{
	public Proj13(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
