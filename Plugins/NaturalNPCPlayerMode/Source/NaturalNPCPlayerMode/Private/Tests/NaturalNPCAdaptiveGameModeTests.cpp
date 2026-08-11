#if WITH_DEV_AUTOMATION_TESTS

#include "NaturalNPCAdaptiveGameMode.h"

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNaturalNPCPlayerModeOptionTest,
    "NaturalNPC.PlayerMode.OptionParsing",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FNaturalNPCPlayerModeOptionTest::RunTest(const FString& Parameters)
{
    TestEqual(
        TEXT("VR option"),
        ANaturalNPCAdaptiveGameMode::ParsePlayerModeOption(TEXT("vr")),
        ENaturalNPCPlayerMode::VR
    );
    TestEqual(
        TEXT("Desktop option"),
        ANaturalNPCAdaptiveGameMode::ParsePlayerModeOption(TEXT("DESKTOP")),
        ENaturalNPCPlayerMode::Desktop
    );
    TestEqual(
        TEXT("First-person alias"),
        ANaturalNPCAdaptiveGameMode::ParsePlayerModeOption(TEXT("FirstPerson")),
        ENaturalNPCPlayerMode::Desktop
    );
    TestEqual(
        TEXT("Unknown option falls back to auto"),
        ANaturalNPCAdaptiveGameMode::ParsePlayerModeOption(TEXT("unknown")),
        ENaturalNPCPlayerMode::Auto
    );
    return true;
}

#endif
