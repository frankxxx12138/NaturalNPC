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
    TestEqual(
        TEXT("Turn input inside the dead zone is zero"),
        UNaturalNPCVRSmoothLocomotionComponent::ApplyAxisDeadZone(
            0.2f,
            0.25f
        ),
        0.0f
    );
    TestTrue(
        TEXT("Turn dead-zone remapping preserves direction"),
        UNaturalNPCVRSmoothLocomotionComponent::ApplyAxisDeadZone(
            -0.625f,
            0.25f
        ) < 0.0f
    );
    TestEqual(
        TEXT("Full right turn remains full input"),
        UNaturalNPCVRSmoothLocomotionComponent::ApplyAxisDeadZone(
            1.0f,
            0.25f
        ),
        1.0f
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

    TestTrue(
        TEXT("Floor1's 13.9 cm stair rise is a valid player step-up"),
        UNaturalNPCVRSmoothLocomotionComponent::
            IsStepUpHeightTraversable(100.0f, 113.9f, 35.0f)
    );
    TestFalse(
        TEXT("A counter-height rise is not treated as a player step-up"),
        UNaturalNPCVRSmoothLocomotionComponent::
            IsStepUpHeightTraversable(100.0f, 180.0f, 35.0f)
    );
    TestFalse(
        TEXT("Descending ground does not activate player step-up assist"),
        UNaturalNPCVRSmoothLocomotionComponent::
            IsStepUpHeightTraversable(113.9f, 100.0f, 35.0f)
    );

    float VerticalVelocity = 0.0f;
    bool bGrounded = false;
    const float FallingDelta =
        UNaturalNPCVRSmoothLocomotionComponent::CalculateGravityDelta(
            200.0f,
            true,
            0.0f,
            0.1f,
            980.0f,
            1200.0f,
            4.0f,
            VerticalVelocity,
            bGrounded
        );
    TestTrue(TEXT("Gravity moves an airborne pawn down"), FallingDelta < 0.0f);
    TestFalse(TEXT("Airborne pawn is not grounded"), bGrounded);
    TestTrue(TEXT("Gravity accumulates downward velocity"), VerticalVelocity < 0.0f);

    VerticalVelocity = -500.0f;
    const float LandingDelta =
        UNaturalNPCVRSmoothLocomotionComponent::CalculateGravityDelta(
            10.0f,
            true,
            0.0f,
            0.1f,
            980.0f,
            1200.0f,
            4.0f,
            VerticalVelocity,
            bGrounded
        );
    TestTrue(TEXT("Landing clamps exactly to the floor"),
        FMath::IsNearlyEqual(LandingDelta, -10.0f, 0.001f));
    TestTrue(TEXT("Landing reports grounded"), bGrounded);
    TestEqual(TEXT("Landing clears vertical velocity"), VerticalVelocity, 0.0f);

    VerticalVelocity = -10.0f;
    const float SnapDelta =
        UNaturalNPCVRSmoothLocomotionComponent::CalculateGravityDelta(
            3.0f,
            true,
            0.0f,
            0.016f,
            980.0f,
            1200.0f,
            4.0f,
            VerticalVelocity,
            bGrounded
        );
    TestTrue(TEXT("Near-ground pawn snaps to floor"),
        FMath::IsNearlyEqual(SnapDelta, -3.0f, 0.001f));
    TestTrue(TEXT("Snapped pawn reports grounded"), bGrounded);

    VerticalVelocity = 420.0f;
    const float JumpDelta =
        UNaturalNPCVRSmoothLocomotionComponent::CalculateGravityDelta(
            0.0f,
            true,
            0.0f,
            0.016f,
            980.0f,
            1200.0f,
            4.0f,
            VerticalVelocity,
            bGrounded
        );
    TestTrue(TEXT("Positive jump velocity leaves the floor"),
        JumpDelta > 0.0f);
    TestFalse(TEXT("Jumping pawn is not snapped back to ground"), bGrounded);
    return true;
}

#endif
