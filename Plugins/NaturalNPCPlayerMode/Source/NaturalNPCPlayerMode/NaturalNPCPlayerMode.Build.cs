using UnrealBuildTool;

public class NaturalNPCPlayerMode : ModuleRules
{
    public NaturalNPCPlayerMode(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "XRBase"
            }
        );
    }
}
