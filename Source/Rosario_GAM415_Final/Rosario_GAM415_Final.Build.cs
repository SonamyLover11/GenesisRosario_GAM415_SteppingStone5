// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rosario_GAM415_Final : ModuleRules
{
    public Rosario_GAM415_Final(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Niagara",
            "ProceduralMeshComponent"
        });
    }
}