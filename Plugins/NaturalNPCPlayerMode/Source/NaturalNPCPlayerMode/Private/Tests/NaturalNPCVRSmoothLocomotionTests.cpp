#if WITH_DEV_AUTOMATION_TESTS

#include "NaturalNPCVRSmoothLocomotionComponent.h"

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNaturalNPCVRSmoothLocomotionMathTest,
    "NaturalNPC.PlayerMode.VRSmoothLocomotionMath",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FNaturalNPCVRSmoothLocomotionMathTest::RunTest(
    const FString& Parameters
)
{
    TestTrue(
        TEXT("Input inside the radial dead zone is zero"),
        UNaturalNPCVRSmoothLocomotionComponent::ApplyRadialDeadZone(
            FVector2D(0.1f, 0.0f),
            0.2f
        ).IsNearlyZero()
    );
    TestTrue(
        TEXT("Full input remains full input"),
        UNaturalNPCVRSmoothLocomotionComponent::ApplyRadialDeadZone(
            FVector2D(0.0f, 1.0f),
            0.2f
        ).Equals(FVector2D(0.0f, 1.0f), 0.001f)
    );

    const FVector ForwardAtNinety =
        UNaturalNPCVRSmoothLocomotionComponent::
            CalculatePlanarMoveDirection(
                FVector2D(0.0f, 1.0f),
                FRotator(35.0f, 90.0f, 15.0f)
            );
    TestTrue(
        TEXT("Forward movement follows headset yaw only"),
        ForwardAtNinety.Equals(FVector(0.0f, 1.0f, 0.0f), 0.001f)
    );

    const FVector RightAtNinety =
        UNaturalNPCVRSmoothLocomotionComponent::
            CalculatePlanarMoveDirection(
                FVector2D(1.0f, 0.0f),
                FRotator(0.0f, 90.0f, 0.0f)
            );
    TestTrue(
        TEXT("Strafe movement is perpendicular to headset yaw"),
        RightAtNinety.Equals(FVector(-1.0f, 0.0f, 0.0f), 0.001f)
    );
    return true;
}

#endif
