using UnrealBuildTool;

public class NaturalNPCConversationGovernance : ModuleRules
{
    public NaturalNPCConversationGovernance(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );
    }
}
