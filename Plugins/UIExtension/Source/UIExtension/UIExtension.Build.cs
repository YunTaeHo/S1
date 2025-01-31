// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UIExtension : ModuleRules
{
	public UIExtension(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
		new string[]
		{
				"Core",
				"CoreUObject",
				"Engine",
				"SlateCore",
				"Slate",
				"UMG",
				"CommonUI",
				"CommonGame",
				"GameplayTags"
        }
		);

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
	}
}
