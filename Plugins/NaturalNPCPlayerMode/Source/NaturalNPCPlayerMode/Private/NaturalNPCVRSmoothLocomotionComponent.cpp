#include "NaturalNPCVRSmoothLocomotionComponent.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"

UNaturalNPCVRSmoothLocomotionComponent::
    UNaturalNPCVRSmoothLocomotionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    bAutoActivate = true;
    SetIsReplicatedByDefault(false);
}

void UNaturalNPCVRSmoothLocomotionComponent::BeginPlay()
{
    Super::BeginPlay();
    ResolveHeadCamera();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("NATURALNPC_VR_LOCOMOTION ready pawn=%s camera=%s speed=%.1f"),
        *GetNameSafe(GetOwner()),
        *GetNameSafe(HeadCamera.Get()),
        MaximumSpeed
    );
}

FVector2D UNaturalNPCVRSmoothLocomotionComponent::ApplyRadialDeadZone(
    const FVector2D& RawInput,
    float DeadZone
)
{
    const float ClampedDeadZone = FMath::Clamp(DeadZone, 0.0f, 0.9f);
    const float Magnitude = FMath::Min(1.0f, RawInput.Size());
    if (Magnitude <= ClampedDeadZone || Magnitude <= UE_SMALL_NUMBER)
    {
        return FVector2D::ZeroVector;
    }

    const float RemappedMagnitude =
        (Magnitude - ClampedDeadZone) / (1.0f - ClampedDeadZone);
    return RawInput.GetSafeNormal() * RemappedMagnitude;
}

FVector UNaturalNPCVRSmoothLocomotionComponent::
    CalculatePlanarMoveDirection(
        const FVector2D& Input,
        const FRotator& ViewRotation
    )
{
    const FRotator YawRotation(0.0f, ViewRotation.Yaw, 0.0f);
    const FVector Forward = YawRotation.Vector();
    const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    return (Forward * Input.Y + Right * Input.X).GetClampedToMaxSize(1.0f);
}

FVector2D UNaturalNPCVRSmoothLocomotionComponent::ReadLeftThumbstick(
    APlayerController* Controller
) const
{
    if (!IsValid(Controller))
    {
        return FVector2D::ZeroVector;
    }

    TArray<FVector2D, TInlineAllocator<6>> Candidates;
    Candidates.Add(FVector2D(
        Controller->GetInputAnalogKeyState(
            EKeys::OculusTouch_Left_Thumbstick_X),
        Controller->GetInputAnalogKeyState(
            EKeys::OculusTouch_Left_Thumbstick_Y)
    ));
    Candidates.Add(FVector2D(
        Controller->GetInputAnalogKeyState(
            EKeys::ValveIndex_Left_Thumbstick_X),
        Controller->GetInputAnalogKeyState(
            EKeys::ValveIndex_Left_Thumbstick_Y)
    ));
    Candidates.Add(FVector2D(
        Controller->GetInputAnalogKeyState(
            EKeys::MixedReality_Left_Thumbstick_X),
        Controller->GetInputAnalogKeyState(
            EKeys::MixedReality_Left_Thumbstick_Y)
    ));
    Candidates.Add(FVector2D(
        Controller->GetInputAnalogKeyState(EKeys::Vive_Left_Trackpad_X),
        Controller->GetInputAnalogKeyState(EKeys::Vive_Left_Trackpad_Y)
    ));
    Candidates.Add(FVector2D(
        Controller->GetInputAnalogKeyState(
            EKeys::ValveIndex_Left_Trackpad_X),
        Controller->GetInputAnalogKeyState(
            EKeys::ValveIndex_Left_Trackpad_Y)
    ));
    if (bAllowStandardGamepadFallback)
    {
        Candidates.Add(FVector2D(
            Controller->GetInputAnalogKeyState(EKeys::Gamepad_LeftX),
            Controller->GetInputAnalogKeyState(EKeys::Gamepad_LeftY)
        ));
    }

    FVector2D StrongestInput = FVector2D::ZeroVector;
    for (const FVector2D& Candidate : Candidates)
    {
        if (Candidate.SizeSquared() > StrongestInput.SizeSquared())
        {
            StrongestInput = Candidate;
        }
    }
    return ApplyRadialDeadZone(StrongestInput, ThumbstickDeadZone);
}

UCameraComponent* UNaturalNPCVRSmoothLocomotionComponent::ResolveHeadCamera()
{
    if (HeadCamera.IsValid())
    {
        return HeadCamera.Get();
    }

    AActor* Owner = GetOwner();
    HeadCamera = IsValid(Owner)
        ? Owner->FindComponentByClass<UCameraComponent>()
        : nullptr;
    return HeadCamera.Get();
}

bool UNaturalNPCVRSmoothLocomotionComponent::FindGroundHeight(
    const FVector& ProbeLocation,
    float& OutGroundZ
) const
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !IsValid(Owner))
    {
        return false;
    }

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCVRGround),
        false,
        Owner
    );
    FHitResult Hit;
    const FVector Start = ProbeLocation + FVector(0.0f, 0.0f, 100.0f);
    const FVector End = ProbeLocation - FVector(
        0.0f,
        0.0f,
        FMath::Max(150.0f, GroundTraceDistance)
    );
    if (!World->LineTraceSingleByObjectType(
            Hit,
            Start,
            End,
            ObjectQuery,
            QueryParams))
    {
        return false;
    }

    OutGroundZ = Hit.ImpactPoint.Z;
    return true;
}

void UNaturalNPCVRSmoothLocomotionComponent::MoveLocallyControlledPawn(
    const FVector& DesiredWorldDelta,
    float DeltaTime
)
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    UCameraComponent* Camera = ResolveHeadCamera();
    UWorld* World = GetWorld();
    if (!IsValid(Pawn) || !IsValid(Camera) || !World ||
        DesiredWorldDelta.IsNearlyZero() || DeltaTime <= 0.0f)
    {
        return;
    }

    const FVector CameraLocation = Camera->GetComponentLocation();
    FVector MoveDelta = DesiredWorldDelta;
    float CurrentGroundZ = 0.0f;
    float TargetGroundZ = 0.0f;
    FVector TargetGroundProbe = CameraLocation + DesiredWorldDelta;
    TargetGroundProbe.Z = CameraLocation.Z;
    if (FindGroundHeight(CameraLocation, CurrentGroundZ) &&
        FindGroundHeight(TargetGroundProbe, TargetGroundZ))
    {
        const float GroundDelta = TargetGroundZ - CurrentGroundZ;
        if (GroundDelta > FMath::Max(10.0f, MaximumStepUp) ||
            GroundDelta < -FMath::Max(20.0f, MaximumStepDown))
        {
            CurrentVelocity = FVector::ZeroVector;
            return;
        }
        MoveDelta.Z = GroundDelta;
    }
    else
    {
        CurrentGroundZ = CameraLocation.Z - CollisionHalfHeight;
        MoveDelta.Z = 0.0f;
    }

    const float Radius = FMath::Max(10.0f, CollisionRadius);
    const float HalfHeight = FMath::Max(Radius + 1.0f, CollisionHalfHeight);
    const FVector CapsuleStart(
        CameraLocation.X,
        CameraLocation.Y,
        CurrentGroundZ + HalfHeight + 2.0f
    );
    const FVector CapsuleEnd = CapsuleStart + MoveDelta;

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCVRSmoothMove),
        false,
        Pawn
    );
    const FCollisionShape Capsule =
        FCollisionShape::MakeCapsule(Radius, HalfHeight);
    FHitResult Hit;
    if (!World->SweepSingleByObjectType(
            Hit,
            CapsuleStart,
            CapsuleEnd,
            FQuat::Identity,
            ObjectQuery,
            Capsule,
            QueryParams))
    {
        Pawn->AddActorWorldOffset(MoveDelta, false);
        return;
    }

    if (Hit.ImpactNormal.Z > 0.65f && !Hit.bStartPenetrating)
    {
        Pawn->AddActorWorldOffset(MoveDelta, false);
        return;
    }

    const float SafeFraction = Hit.bStartPenetrating
        ? 0.0f
        : FMath::Clamp(Hit.Time - 0.02f, 0.0f, 1.0f);
    const FVector SafeDelta = MoveDelta * SafeFraction;
    if (!SafeDelta.IsNearlyZero())
    {
        Pawn->AddActorWorldOffset(SafeDelta, false);
    }

    FVector RemainingDelta = MoveDelta * (1.0f - SafeFraction);
    FVector WallNormal = Hit.ImpactNormal.GetSafeNormal2D();
    if (RemainingDelta.IsNearlyZero() || WallNormal.IsNearlyZero())
    {
        CurrentVelocity = FVector::ZeroVector;
        return;
    }

    FVector SlideDelta = FVector::VectorPlaneProject(
        RemainingDelta,
        WallNormal
    );
    SlideDelta.Z = 0.0f;
    if (SlideDelta.IsNearlyZero())
    {
        CurrentVelocity = FVector::ZeroVector;
        return;
    }

    const FVector SlideStart = CapsuleStart + SafeDelta;
    FHitResult SlideHit;
    if (!World->SweepSingleByObjectType(
            SlideHit,
            SlideStart,
            SlideStart + SlideDelta,
            FQuat::Identity,
            ObjectQuery,
            Capsule,
            QueryParams))
    {
        Pawn->AddActorWorldOffset(SlideDelta, false);
    }
}

void UNaturalNPCVRSmoothLocomotionComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    APawn* Pawn = Cast<APawn>(GetOwner());
    APlayerController* Controller = IsValid(Pawn)
        ? Cast<APlayerController>(Pawn->GetController())
        : nullptr;
    UCameraComponent* Camera = ResolveHeadCamera();
    if (!bSmoothLocomotionEnabled || !IsValid(Pawn) ||
        !Pawn->IsLocallyControlled() || !IsValid(Controller) ||
        !IsValid(Camera) || DeltaTime <= 0.0f)
    {
        CurrentVelocity = FVector::ZeroVector;
        return;
    }

    const FVector2D MoveInput = ReadLeftThumbstick(Controller);
    const FVector TargetVelocity = CalculatePlanarMoveDirection(
        MoveInput,
        Camera->GetComponentRotation()
    ) * FMath::Max(20.0f, MaximumSpeed);
    const float InterpSpeed = MoveInput.IsNearlyZero()
        ? DecelerationInterpSpeed
        : AccelerationInterpSpeed;
    CurrentVelocity = FMath::VInterpTo(
        CurrentVelocity,
        TargetVelocity,
        DeltaTime,
        FMath::Max(0.0f, InterpSpeed)
    );
    CurrentVelocity.Z = 0.0f;
    if (MoveInput.IsNearlyZero() && CurrentVelocity.SizeSquared2D() < 1.0f)
    {
        CurrentVelocity = FVector::ZeroVector;
    }

    MoveLocallyControlledPawn(CurrentVelocity * DeltaTime, DeltaTime);
}
