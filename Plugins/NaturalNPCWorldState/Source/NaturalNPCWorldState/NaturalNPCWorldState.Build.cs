using UnrealBuildTool;

public class NaturalNPCWorldState : ModuleRules
{
    public NaturalNPCWorldState(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "AnimationCore",
                "Json",
                "NavigationSystem"
            }
        );
    }
}
