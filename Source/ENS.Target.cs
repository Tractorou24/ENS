// Copyright (c) 2024-2025, Equipment'N Slash contributors. All rights reserved.

using UnrealBuildTool;

public class ENSTarget : TargetRules
{
	public ENSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange(new string[] { "ENS" });
	}
}