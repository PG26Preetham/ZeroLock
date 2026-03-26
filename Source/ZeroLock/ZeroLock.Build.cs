// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZeroLock : ModuleRules
{
	public ZeroLock(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "CommonUI" });
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"Paper2D",
			"NetCore",
			"AIModule",
			"NavigationSystem",
			"UMG",
			"CommonUI",
			"OnlineSubsystem",
			"OnlineSubsystemEOS",
			"OnlineSubsystemUtils",
			"SocketSubsystemEOS",
			"HTTP",
			"OpenSSL",
			"Json",
			"ModelViewViewModel",
			"SlateMVVM"

		});
		
		PrivateDefinitions.Add("P2PMODE=1");
	}
}
