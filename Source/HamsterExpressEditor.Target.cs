using UnrealBuildTool;
using System.Collections.Generic;

public class HamsterExpressEditorTarget : TargetRules
{
    public HamsterExpressEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("HamsterExpress");
    }
}
