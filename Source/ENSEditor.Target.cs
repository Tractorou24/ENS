// Copyright (c) 2024-2025, BloodTear contributors. All rights reserved.

using UnrealBuildTool;

public class ENSEditorTarget : TargetRules
{
	public ENSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange(new string[] { "ENS" });
	}
}