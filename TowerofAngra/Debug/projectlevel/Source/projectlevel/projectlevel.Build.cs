// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class projectlevel : ModuleRules
{
	public projectlevel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

<<<<<<< HEAD
        bEnforceIWYU = false;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"  , "AIModule", "GameplayTasks" });
=======
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" ,"UMG", "NavigationSystem", "AIModule", "GameplayTasks" });
>>>>>>> 414f783b1704ad5a94a43c1263b089e8511a2bbb

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}