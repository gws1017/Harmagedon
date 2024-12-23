// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectH : ModuleRules
{
	public ProjectH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
        //모듈추가
		PublicDependencyModuleNames.AddRange(new string[] 
        { "Core", "CoreUObject", "Engine", "InputCore"
        ,"EnhancedInput","AIModule","GameplayTasks", "AnimGraphRuntime", "UMG"
        ,"FieldSystemEngine","NavigationSystem", "Niagara"
        ,"GeometryCollectionEngine","PhysicsCore","LevelSequence","MovieScene"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //상대경로로 등록가능
        PublicIncludePaths.Add(ModuleDirectory);

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
