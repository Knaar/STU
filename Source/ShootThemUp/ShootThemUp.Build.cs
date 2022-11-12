// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","PhysicsCore","Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[]
		{
			"ShootThemUp/Dev",
			"ShootThemUp/Player",
			"ShootThemUp/Animations",
			"ShootThemUp/PIckUpGifts",
			"ShootThemUp/Weapon",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}