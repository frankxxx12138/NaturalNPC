#if WITH_DEV_AUTOMATION_TESTS

#include "NaturalNPCVRSpeakerInteractorComponent.h"

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNaturalNPCVRSpeakerTriggerGateTest,
    "NaturalNPC.PlayerMode.VRSpeaker.TriggerGate",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FNaturalNPCVRSpeakerTriggerGateTest::RunTest(
    const FString& Parameters
)
{
    TestFalse(
        TEXT("A targeted button does not activate without the trigger"),
        UNaturalNPCVRSpeakerInteractorComponent::ShouldActivateTarget(
            false,
            false,
            true
        )
    );
    TestFalse(
        TEXT("Holding the trigger does not repeatedly activate a button"),
        UNaturalNPCVRSpeakerInteractorComponent::ShouldActivateTarget(
            true,
            true,
            true
        )
    );
    TestFalse(
        TEXT("A trigger press with no targeted button does nothing"),
        UNaturalNPCVRSpeakerInteractorComponent::ShouldActivateTarget(
            true,
            false,
            false
        )
    );
    TestTrue(
        TEXT("A fresh trigger press activates the targeted button"),
        UNaturalNPCVRSpeakerInteractorComponent::ShouldActivateTarget(
            true,
            false,
            true
        )
    );

    return true;
}

#endif
