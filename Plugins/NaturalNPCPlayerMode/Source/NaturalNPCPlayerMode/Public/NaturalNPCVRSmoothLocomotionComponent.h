#pragma once

#include "Components/ActorComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "NaturalNPCVRSmoothLocomotionComponent.generated.h"

class APlayerController;
class UCameraComponent;
class UInputAction;

UCLASS(ClassGroup = (NaturalNPC), meta = (BlueprintSpawnableComponent))
class NATURALNPCPLAYERMODE_API UNaturalNPCVRSmoothLocomotionComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UNaturalNPCVRSmoothLocomotionComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion")
    bool bSmoothLocomotionEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion",
        meta = (ClampMin = "20.0", ClampMax = "600.0", Units = "cm/s"))
    float MaximumSpeed = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Turning")
    bool bSmoothTurningEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Turning",
        meta = (ClampMin = "15.0", ClampMax = "240.0", Units = "deg/s"))
    float MaximumTurnSpeed = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Turning",
        meta = (ClampMin = "0.0", ClampMax = "0.9"))
    float TurnDeadZone = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion",
        meta = (ClampMin = "0.0", ClampMax = "30.0"))
    float AccelerationInterpSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion",
        meta = (ClampMin = "0.0", ClampMax = "30.0"))
    float DecelerationInterpSpeed = 12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion",
        meta = (ClampMin = "0.0", ClampMax = "0.9"))
    float ThumbstickDeadZone = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Collision",
        meta = (ClampMin = "10.0", ClampMax = "60.0", Units = "cm"))
    float CollisionRadius = 24.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Collision",
        meta = (ClampMin = "50.0", ClampMax = "120.0", Units = "cm"))
    float CollisionHalfHeight = 88.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Grounding",
        meta = (ClampMin = "10.0", ClampMax = "80.0", Units = "cm"))
    float MaximumStepUp = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Grounding",
        meta = (ClampMin = "20.0", ClampMax = "160.0", Units = "cm"))
    float MaximumStepDown = 70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Grounding",
        meta = (ClampMin = "150.0", ClampMax = "3000.0", Units = "cm"))
    float GroundTraceDistance = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Gravity")
    bool bGravityEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Gravity",
        meta = (ClampMin = "100.0", ClampMax = "3000.0", Units = "cm/s^2"))
    float GravityAcceleration = 980.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Gravity",
        meta = (ClampMin = "100.0", ClampMax = "3000.0", Units = "cm/s"))
    float MaximumFallSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Gravity",
        meta = (ClampMin = "0.5", ClampMax = "20.0", Units = "cm"))
    float GroundSnapDistance = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Jump")
    bool bJumpEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Jump",
        meta = (ClampMin = "100.0", ClampMax = "1000.0", Units = "cm/s"))
    float JumpVelocity = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion")
    bool bAllowStandardGamepadFallback = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Input")
    TSoftObjectPtr<UInputAction> SmoothMoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Input")
    TSoftObjectPtr<UInputAction> SmoothTurnAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion|Input")
    TSoftObjectPtr<UInputAction> JumpAction;

    static FVector2D ApplyRadialDeadZone(
        const FVector2D& RawInput,
        float DeadZone
    );

    static float ApplyAxisDeadZone(float RawInput, float DeadZone);

    static FVector CalculatePlanarMoveDirection(
        const FVector2D& Input,
        const FRotator& ViewRotation
    );

    static float CalculateGravityDelta(
        float CurrentRootZ,
        bool bHasGround,
        float GroundZ,
        float DeltaTime,
        float GravityAcceleration,
        float MaximumFallSpeed,
        float GroundSnapDistance,
        float& InOutVerticalVelocity,
        bool& bOutGrounded
    );

    static bool IsStepUpHeightTraversable(
        float CurrentGroundZ,
        float CandidateGroundZ,
        float MaximumStepUp
    );

private:
    FVector2D ReadLeftThumbstick(APlayerController* Controller) const;
    float ReadRightThumbstickX(APlayerController* Controller) const;
    bool ReadJumpButton(APlayerController* Controller) const;
    UCameraComponent* ResolveHeadCamera();
    bool FindGroundHeight(
        const FVector& ProbeLocation,
        float& OutGroundZ
    ) const;
    void MoveLocallyControlledPawn(
        const FVector& DesiredWorldDelta,
        float DeltaTime
    );
    void UpdateGravity(float DeltaTime);
    void TurnLocallyControlledPawn(float TurnInput, float DeltaTime);

    TWeakObjectPtr<UCameraComponent> HeadCamera;
    FVector CurrentVelocity = FVector::ZeroVector;
    float VerticalVelocity = 0.0f;
    float StepUpSupportGroundZ = 0.0f;
    float StepUpSupportRemainingSeconds = 0.0f;
    bool bWasGrounded = false;
    bool bJumpWasDown = false;
};
