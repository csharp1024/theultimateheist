// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class TheUltimateHeist : ModuleRules
{
    public TheUltimateHeist(TargetInfo Target)
    {
        
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
        
        PublicDependencyModuleNames.AddRange(
            new string[] { 
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "UMG", "Slate", "SlateCore",
                "AIModule"
            }
        );
        
        PublicDependencyModuleNames.AddRange(new string[] { 
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });
        
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
