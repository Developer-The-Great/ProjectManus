// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectManus : ModuleRules
{
	public ProjectManus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Manus" });
	}
}
