// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectNo6 : ModuleRules
{
	public ProjectNo6(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"GameplayTags", 
			"EnhancedInput", 
			"LDWStateMachine", 
			"LDWSaveLoad", 
			"Niagara", 
			"UMG", 
			"Slate",
			"AIModule", 
			"AudioExtensions"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"LDWGameplayTag"
		});

        PublicIncludePaths.AddRange(new string[] {
            "ProjectNo6/Public"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
