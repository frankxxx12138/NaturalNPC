#pragma once

#include "Components/ActorComponent.h"
#include "NaturalNPCVRSmoothLocomotionComponent.generated.h"

class APlayerController;
class UCameraComponent;

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
    float MaximumSpeed = 180.0f;

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
        meta = (ClampMin = "150.0", ClampMax = "600.0", Units = "cm"))
    float GroundTraceDistance = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Smooth Locomotion")
    bool bAllowStandardGamepadFallback = true;

    static FVector2D ApplyRadialDeadZone(
        const FVector2D& RawInput,
        float DeadZone
    );

    static FVector CalculatePlanarMoveDirection(
        const FVector2D& Input,
        const FRotator& ViewRotation
    );

private:
    FVector2D ReadLeftThumbstick(APlayerController* Controller) const;
    UCameraComponent* ResolveHeadCamera();
    bool FindGroundHeight(
        const FVector& ProbeLocation,
        float& OutGroundZ
    ) const;
    void MoveLocallyControlledPawn(
        const FVector& DesiredWorldDelta,
        float DeltaTime
    );

    TWeakObjectPtr<UCameraComponent> HeadCamera;
    FVector CurrentVelocity = FVector::ZeroVector;
};
