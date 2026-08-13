#pragma once

#include "Animation/AnimSingleNodeInstance.h"
#include "NPCAdaptivePickupAnimInstance.generated.h"

struct FNPCAdaptivePickupAnimInstanceProxy;

/**
 * Plays a pickup sequence while adapting the contact pose to an object in the
 * world. The proxy keeps the feet planted, adjusts the pelvis and spine, then
 * solves the arm chains against the requested pickup point.
 */
UCLASS(Transient)
class NATURALNPCWORLDSTATE_API UNPCAdaptivePickupAnimInstance
    : public UAnimSingleNodeInstance
{
    GENERATED_BODY()

public:
    void ConfigurePickup(
        const FVector& InTargetWorldLocation,
        float InContactNormalizedTime,
        bool bInUseSupportHand,
        bool bInAdjustPelvisAndSpine,
        float InGripHalfWidth,
        float InBlendWindow,
        float InPelvisInfluence,
        float InMaxPelvisOffset,
        float InMaxArmStretch
    );

protected:
    virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
    virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;

private:
    friend struct FNPCAdaptivePickupAnimInstanceProxy;

    FVector TargetWorldLocation = FVector::ZeroVector;
    float ContactNormalizedTime = 0.55f;
    float GripHalfWidth = 8.0f;
    float BlendWindow = 0.22f;
    float PelvisInfluence = 0.30f;
    float MaxPelvisOffset = 40.0f;
    float MaxArmStretch = 1.08f;
    bool bUseSupportHand = false;
    bool bAdjustPelvisAndSpine = true;
    bool bPickupConfigured = false;
};
