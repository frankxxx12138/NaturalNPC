using UnrealBuildTool;

public class NaturalNPCGazeSelector : ModuleRules
{
    public NaturalNPCGazeSelector(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new[] { "Core", "CoreUObject", "Engine", "Convai" }
        );
    }
}
