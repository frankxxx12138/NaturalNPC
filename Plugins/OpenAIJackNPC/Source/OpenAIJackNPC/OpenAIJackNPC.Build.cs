using UnrealBuildTool;

public class OpenAIJackNPC : ModuleRules
{
    public OpenAIJackNPC(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "EnhancedInput",
                "HTTP",
                "InputCore",
                "Json",
                "JsonUtilities",
                "AudioMixer",
                "Slate",
                "SlateCore",
                "NaturalNPCConversationGovernance",
                "NaturalNPCWorldState",
                "NavigationSystem",
                "ACECore",
                "ACERuntime"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicSystemLibraries.AddRange(
                new[]
                {
                    "Ole32.lib",
                    "Sapi.lib"
                }
            );
        }
    }
}
