using UnrealBuildTool;
using System.Collections.Generic;

public class HamsterExpressTarget : TargetRules
{
    public HamsterExpressTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("HamsterExpress");
    }
}
