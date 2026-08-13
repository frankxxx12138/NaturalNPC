#include "NPCAdaptivePickupAnimInstance.h"

#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimSingleNodeInstanceProxy.h"
#include "TwoBoneIK.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "Components/SkeletalMeshComponent.h"

namespace
{
FCompactPoseBoneIndex FindCompactBone(
    const FBoneContainer& BoneContainer,
    const FName BoneName
)
{
    const int32 PoseIndex = BoneContainer.GetPoseBoneIndexForBoneName(
        BoneName
    );
    return PoseIndex == INDEX_NONE
        ? FCompactPoseBoneIndex(INDEX_NONE)
        : BoneContainer.MakeCompactPoseIndex(FMeshPoseBoneIndex(PoseIndex));
}

float GetContactAlpha(
    const float NormalizedTime,
    const float ContactTime,
    const float BlendWindow
)
{
    const float SafeWindow = FMath::Clamp(BlendWindow, 0.05f, 0.45f);
    const float Begin = FMath::Max(0.0f, ContactTime - SafeWindow);
    const float FullBegin = FMath::Max(Begin, ContactTime - 0.045f);
    const float FullEnd = FMath::Min(1.0f, ContactTime + 0.055f);
    const float End = FMath::Min(1.0f, ContactTime + SafeWindow);

    if (NormalizedTime < Begin || NormalizedTime > End)
    {
        return 0.0f;
    }
    if (NormalizedTime < FullBegin)
    {
        return FullBegin - Begin <= KINDA_SMALL_NUMBER
            ? 1.0f
            : FMath::SmoothStep(Begin, FullBegin, NormalizedTime);
    }
    if (NormalizedTime <= FullEnd)
    {
        return 1.0f;
    }
    return End - FullEnd <= KINDA_SMALL_NUMBER
        ? 0.0f
        : 1.0f - FMath::SmoothStep(FullEnd, End, NormalizedTime);
}

bool SolveLimb(
    FCSPose<FCompactPose>& Pose,
    const FCompactPoseBoneIndex UpperIndex,
    const FCompactPoseBoneIndex LowerIndex,
    const FCompactPoseBoneIndex EndIndex,
    const FVector& TargetLocation,
    const float Alpha,
    const bool bAllowStretching,
    const float MaxStretchScale
)
{
    if (UpperIndex == INDEX_NONE || LowerIndex == INDEX_NONE ||
        EndIndex == INDEX_NONE)
    {
        return false;
    }

    FTransform UpperTransform = Pose.GetComponentSpaceTransform(UpperIndex);
    FTransform LowerTransform = Pose.GetComponentSpaceTransform(LowerIndex);
    FTransform EndTransform = Pose.GetComponentSpaceTransform(EndIndex);
    const FVector BendDirection = (
        LowerTransform.GetLocation() -
        (UpperTransform.GetLocation() + EndTransform.GetLocation()) * 0.5f
    ).GetSafeNormal();
    const FVector JointTarget = LowerTransform.GetLocation() +
        (BendDirection.IsNearlyZero()
            ? FVector(0.0f, 20.0f, 0.0f)
            : BendDirection * 25.0f);

    AnimationCore::SolveTwoBoneIK(
        UpperTransform,
        LowerTransform,
        EndTransform,
        JointTarget,
        TargetLocation,
        bAllowStretching,
        0.95,
        FMath::Max(1.0f, MaxStretchScale)
    );

    TArray<FBoneTransform> BoneTransforms;
    BoneTransforms.Reserve(3);
    BoneTransforms.Emplace(UpperIndex, UpperTransform);
    BoneTransforms.Emplace(LowerIndex, LowerTransform);
    BoneTransforms.Emplace(EndIndex, EndTransform);
    Pose.LocalBlendCSBoneTransforms(BoneTransforms, Alpha);
    return true;
}

FQuat GetLimitedAimRotation(
    const FVector& CurrentDirection,
    const FVector& DesiredDirection,
    const float MaxDegrees
)
{
    if (CurrentDirection.IsNearlyZero() || DesiredDirection.IsNearlyZero())
    {
        return FQuat::Identity;
    }

    const FQuat FullRotation = FQuat::FindBetweenNormals(
        CurrentDirection.GetSafeNormal(),
        DesiredDirection.GetSafeNormal()
    );
    FVector Axis = FVector::ZeroVector;
    float Angle = 0.0f;
    FullRotation.ToAxisAndAngle(Axis, Angle);
    if (Axis.IsNearlyZero() || !FMath::IsFinite(Angle))
    {
        return FQuat::Identity;
    }
    const float LimitedAngle = FMath::Min(
        FMath::Abs(Angle),
        FMath::DegreesToRadians(MaxDegrees)
    );
    return FQuat(Axis, FMath::Sign(Angle) * LimitedAngle);
}
}

struct FNPCAdaptivePickupAnimInstanceProxy
    : public FAnimSingleNodeInstanceProxy
{
    explicit FNPCAdaptivePickupAnimInstanceProxy(UAnimInstance* Instance)
        : FAnimSingleNodeInstanceProxy(Instance)
    {
    }

    virtual void PreUpdate(
        UAnimInstance* InAnimInstance,
        float DeltaSeconds
    ) override
    {
        FAnimSingleNodeInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);

        const UNPCAdaptivePickupAnimInstance* PickupInstance =
            Cast<UNPCAdaptivePickupAnimInstance>(InAnimInstance);
        const USkeletalMeshComponent* Mesh = GetSkelMeshComponent();
        bEnabled = PickupInstance && PickupInstance->bPickupConfigured && Mesh;
        if (!bEnabled)
        {
            return;
        }

        TargetComponentLocation = Mesh->GetComponentTransform()
            .InverseTransformPosition(PickupInstance->TargetWorldLocation);
        ContactNormalizedTime = PickupInstance->ContactNormalizedTime;
        GripHalfWidth = PickupInstance->GripHalfWidth;
        BlendWindow = PickupInstance->BlendWindow;
        PelvisInfluence = PickupInstance->PelvisInfluence;
        MaxPelvisOffset = PickupInstance->MaxPelvisOffset;
        MaxArmStretch = PickupInstance->MaxArmStretch;
        bUseSupportHand = PickupInstance->bUseSupportHand;
        bAdjustPelvisAndSpine =
            PickupInstance->bAdjustPelvisAndSpine;
    }

    virtual bool Evaluate(FPoseContext& Output) override
    {
        if (!FAnimSingleNodeInstanceProxy::Evaluate(Output) || !bEnabled)
        {
            return true;
        }

        const UAnimSequenceBase* Sequence = Cast<UAnimSequenceBase>(
            CurrentAsset
        );
        const float Length = Sequence ? Sequence->GetPlayLength() : 0.0f;
        if (Length <= KINDA_SMALL_NUMBER)
        {
            return true;
        }

        const float NormalizedTime = FMath::Clamp(
            GetCurrentTime() / Length,
            0.0f,
            1.0f
        );
        const float ContactAlpha = GetContactAlpha(
            NormalizedTime,
            ContactNormalizedTime,
            BlendWindow
        );
        if (ContactAlpha <= ZERO_ANIMWEIGHT_THRESH)
        {
            return true;
        }
        if (!bLoggedContact)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("NPC_ADAPTIVE_PICKUP_IK contact target_cs=%s "
                    "time=%.3f alpha=%.2f support_hand=%d torso=%d"),
                *TargetComponentLocation.ToCompactString(),
                NormalizedTime,
                ContactAlpha,
                bUseSupportHand ? 1 : 0,
                bAdjustPelvisAndSpine ? 1 : 0
            );
            bLoggedContact = true;
        }

        const FBoneContainer& Bones = Output.Pose.GetBoneContainer();
        const FCompactPoseBoneIndex Pelvis = FindCompactBone(
            Bones, TEXT("pelvis")
        );
        const FCompactPoseBoneIndex Spine01 = FindCompactBone(
            Bones, TEXT("spine_01")
        );
        const FCompactPoseBoneIndex Spine03 = FindCompactBone(
            Bones, TEXT("spine_03")
        );
        const FCompactPoseBoneIndex UpperArmR = FindCompactBone(
            Bones, TEXT("upperarm_r")
        );
        const FCompactPoseBoneIndex LowerArmR = FindCompactBone(
            Bones, TEXT("lowerarm_r")
        );
        const FCompactPoseBoneIndex HandR = FindCompactBone(
            Bones, TEXT("hand_r")
        );
        const FCompactPoseBoneIndex UpperArmL = FindCompactBone(
            Bones, TEXT("upperarm_l")
        );
        const FCompactPoseBoneIndex LowerArmL = FindCompactBone(
            Bones, TEXT("lowerarm_l")
        );
        const FCompactPoseBoneIndex HandL = FindCompactBone(
            Bones, TEXT("hand_l")
        );
        const FCompactPoseBoneIndex ThighR = FindCompactBone(
            Bones, TEXT("thigh_r")
        );
        const FCompactPoseBoneIndex CalfR = FindCompactBone(
            Bones, TEXT("calf_r")
        );
        const FCompactPoseBoneIndex FootR = FindCompactBone(
            Bones, TEXT("foot_r")
        );
        const FCompactPoseBoneIndex ThighL = FindCompactBone(
            Bones, TEXT("thigh_l")
        );
        const FCompactPoseBoneIndex CalfL = FindCompactBone(
            Bones, TEXT("calf_l")
        );
        const FCompactPoseBoneIndex FootL = FindCompactBone(
            Bones, TEXT("foot_l")
        );

        if (Pelvis == INDEX_NONE || HandR == INDEX_NONE ||
            UpperArmR == INDEX_NONE || LowerArmR == INDEX_NONE)
        {
            return true;
        }

        FCSPose<FCompactPose> ComponentPose;
        ComponentPose.InitPose(Output.Pose);

        const FVector OriginalFootR = FootR != INDEX_NONE
            ? ComponentPose.GetComponentSpaceTransform(FootR).GetLocation()
            : FVector::ZeroVector;
        const FVector OriginalFootL = FootL != INDEX_NONE
            ? ComponentPose.GetComponentSpaceTransform(FootL).GetLocation()
            : FVector::ZeroVector;

        const FVector HandRLocation = ComponentPose
            .GetComponentSpaceTransform(HandR).GetLocation();
        const FVector HandLLocation = HandL != INDEX_NONE
            ? ComponentPose.GetComponentSpaceTransform(HandL).GetLocation()
            : HandRLocation;
        const FVector CurrentGripLocation = bUseSupportHand
            ? (HandRLocation + HandLLocation) * 0.5f
            : HandRLocation;

        FVector AdaptiveTarget = TargetComponentLocation;
        const float FeetHeight = FootR != INDEX_NONE && FootL != INDEX_NONE
            ? (OriginalFootR.Z + OriginalFootL.Z) * 0.5f
            : AdaptiveTarget.Z;
        if (Spine03 != INDEX_NONE)
        {
            const float ChestHeight = ComponentPose
                .GetComponentSpaceTransform(Spine03).GetLocation().Z;
            AdaptiveTarget.Z = FMath::Clamp(
                AdaptiveTarget.Z,
                FeetHeight + 2.0f,
                ChestHeight + 8.0f
            );
        }

        if (bAdjustPelvisAndSpine)
        {
            const FVector GripDelta = AdaptiveTarget - CurrentGripLocation;
            FTransform PelvisTransform = ComponentPose
                .GetComponentSpaceTransform(Pelvis);
            FVector PelvisOffset = GripDelta * PelvisInfluence;
            PelvisOffset.X = FMath::Clamp(PelvisOffset.X, -12.0f, 12.0f);
            PelvisOffset.Y = FMath::Clamp(PelvisOffset.Y, -12.0f, 12.0f);
            PelvisOffset.Z = FMath::Clamp(
                PelvisOffset.Z,
                -FMath::Min(18.0f, MaxPelvisOffset),
                MaxPelvisOffset
            );
            PelvisTransform.AddToTranslation(PelvisOffset);
            const TArray<FBoneTransform> PelvisTransforms = {
                FBoneTransform(Pelvis, PelvisTransform)
            };
            ComponentPose.LocalBlendCSBoneTransforms(
                PelvisTransforms,
                ContactAlpha
            );

            if (Spine01 != INDEX_NONE && Spine03 != INDEX_NONE)
            {
                const FVector UpdatedHandR = ComponentPose
                    .GetComponentSpaceTransform(HandR).GetLocation();
                const FVector UpdatedHandL = HandL != INDEX_NONE
                    ? ComponentPose.GetComponentSpaceTransform(HandL)
                        .GetLocation()
                    : UpdatedHandR;
                const FVector UpdatedGrip = bUseSupportHand
                    ? (UpdatedHandR + UpdatedHandL) * 0.5f
                    : UpdatedHandR;
                const FVector ChestLocation = ComponentPose
                    .GetComponentSpaceTransform(Spine03).GetLocation();
                const FQuat AimRotation = GetLimitedAimRotation(
                    UpdatedGrip - ChestLocation,
                    AdaptiveTarget - ChestLocation,
                    32.0f
                );

                FTransform Spine01Transform = ComponentPose
                    .GetComponentSpaceTransform(Spine01);
                Spine01Transform.SetRotation(
                    FQuat::Slerp(FQuat::Identity, AimRotation, 0.35f) *
                    Spine01Transform.GetRotation()
                );
                const TArray<FBoneTransform> Spine01Transforms = {
                    FBoneTransform(Spine01, Spine01Transform)
                };
                ComponentPose.LocalBlendCSBoneTransforms(
                    Spine01Transforms,
                    ContactAlpha
                );

                FTransform Spine03Transform = ComponentPose
                    .GetComponentSpaceTransform(Spine03);
                Spine03Transform.SetRotation(
                    FQuat::Slerp(FQuat::Identity, AimRotation, 0.30f) *
                    Spine03Transform.GetRotation()
                );
                const TArray<FBoneTransform> Spine03Transforms = {
                    FBoneTransform(Spine03, Spine03Transform)
                };
                ComponentPose.LocalBlendCSBoneTransforms(
                    Spine03Transforms,
                    ContactAlpha
                );
            }

            SolveLimb(
                ComponentPose,
                ThighR,
                CalfR,
                FootR,
                OriginalFootR,
                ContactAlpha,
                true,
                1.04f
            );
            SolveLimb(
                ComponentPose,
                ThighL,
                CalfL,
                FootL,
                OriginalFootL,
                ContactAlpha,
                true,
                1.04f
            );
        }

        FVector RightTarget = AdaptiveTarget;
        FVector LeftTarget = AdaptiveTarget;
        if (bUseSupportHand && HandL != INDEX_NONE)
        {
            const FVector CurrentRight = ComponentPose
                .GetComponentSpaceTransform(HandR).GetLocation();
            const FVector CurrentLeft = ComponentPose
                .GetComponentSpaceTransform(HandL).GetLocation();
            FVector HandAxis = (CurrentLeft - CurrentRight).GetSafeNormal();
            if (HandAxis.IsNearlyZero())
            {
                HandAxis = FVector::YAxisVector;
            }
            RightTarget -= HandAxis * GripHalfWidth;
            LeftTarget += HandAxis * GripHalfWidth;
        }

        SolveLimb(
            ComponentPose,
            UpperArmR,
            LowerArmR,
            HandR,
            RightTarget,
            ContactAlpha,
            true,
            MaxArmStretch
        );
        if (bUseSupportHand)
        {
            SolveLimb(
                ComponentPose,
                UpperArmL,
                LowerArmL,
                HandL,
                LeftTarget,
                ContactAlpha,
                true,
                MaxArmStretch
            );
        }

        FCSPose<FCompactPose>::ConvertComponentPosesToLocalPosesSafe(
            ComponentPose,
            Output.Pose
        );
        return true;
    }

private:
    FVector TargetComponentLocation = FVector::ZeroVector;
    float ContactNormalizedTime = 0.55f;
    float GripHalfWidth = 8.0f;
    float BlendWindow = 0.22f;
    float PelvisInfluence = 0.30f;
    float MaxPelvisOffset = 40.0f;
    float MaxArmStretch = 1.08f;
    bool bUseSupportHand = false;
    bool bAdjustPelvisAndSpine = true;
    bool bEnabled = false;
    bool bLoggedContact = false;
};

void UNPCAdaptivePickupAnimInstance::ConfigurePickup(
    const FVector& InTargetWorldLocation,
    const float InContactNormalizedTime,
    const bool bInUseSupportHand,
    const bool bInAdjustPelvisAndSpine,
    const float InGripHalfWidth,
    const float InBlendWindow,
    const float InPelvisInfluence,
    const float InMaxPelvisOffset,
    const float InMaxArmStretch
)
{
    TargetWorldLocation = InTargetWorldLocation;
    ContactNormalizedTime = FMath::Clamp(
        InContactNormalizedTime, 0.0f, 1.0f
    );
    bUseSupportHand = bInUseSupportHand;
    bAdjustPelvisAndSpine = bInAdjustPelvisAndSpine;
    GripHalfWidth = FMath::Clamp(InGripHalfWidth, 2.0f, 30.0f);
    BlendWindow = FMath::Clamp(InBlendWindow, 0.05f, 0.45f);
    PelvisInfluence = FMath::Clamp(InPelvisInfluence, 0.0f, 0.75f);
    MaxPelvisOffset = FMath::Clamp(InMaxPelvisOffset, 0.0f, 65.0f);
    MaxArmStretch = FMath::Clamp(InMaxArmStretch, 1.0f, 1.20f);
    bPickupConfigured = true;
}

FAnimInstanceProxy*
UNPCAdaptivePickupAnimInstance::CreateAnimInstanceProxy()
{
    return new FNPCAdaptivePickupAnimInstanceProxy(this);
}

void UNPCAdaptivePickupAnimInstance::DestroyAnimInstanceProxy(
    FAnimInstanceProxy* InProxy
)
{
    delete static_cast<FNPCAdaptivePickupAnimInstanceProxy*>(InProxy);
}
