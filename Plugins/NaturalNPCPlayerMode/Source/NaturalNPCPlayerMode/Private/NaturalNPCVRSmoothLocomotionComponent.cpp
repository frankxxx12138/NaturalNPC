#include "NaturalNPCVRSmoothLocomotionComponent.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputCoreTypes.h"

UNaturalNPCVRSmoothLocomotionComponent::
    UNaturalNPCVRSmoothLocomotionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    bAutoActivate = true;
    SetIsReplicatedByDefault(false);
    SmoothMoveAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_SmoothMove.IA_SmoothMove")
    ));
    SmoothTurnAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_SmoothTurn.IA_SmoothTurn")
    ));
    JumpAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_VRJump.IA_VRJump")
    ));
}

void UNaturalNPCVRSmoothLocomotionComponent::BeginPlay()
{
    Super::BeginPlay();
    ResolveHeadCamera();

    UE_LOG(
        LogTemp,
        Display,
        TEXT(
            "NATURALNPC_VR_LOCOMOTION ready pawn=%s camera=%s "
            "speed=%.1f turn_speed=%.1f gravity=%d jump=%d "
            "jump_velocity=%.1f"
        ),
        *GetNameSafe(GetOwner()),
        *GetNameSafe(HeadCamera.Get()),
        MaximumSpeed,
        MaximumTurnSpeed,
        bGravityEnabled ? 1 : 0,
        bJumpEnabled ? 1 : 0,
        JumpVelocity
    );
}

float UNaturalNPCVRSmoothLocomotionComponent::ApplyAxisDeadZone(
    float RawInput,
    float DeadZone
)
{
    const float ClampedInput = FMath::Clamp(RawInput, -1.0f, 1.0f);
    const float ClampedDeadZone = FMath::Clamp(DeadZone, 0.0f, 0.9f);
    const float Magnitude = FMath::Abs(ClampedInput);
    if (Magnitude <= ClampedDeadZone)
    {
        return 0.0f;
    }

    const float RemappedMagnitude =
        (Magnitude - ClampedDeadZone) / (1.0f - ClampedDeadZone);
    return FMath::Sign(ClampedInput) * RemappedMagnitude;
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

float UNaturalNPCVRSmoothLocomotionComponent::CalculateGravityDelta(
    float CurrentRootZ,
    bool bHasGround,
    float GroundZ,
    float DeltaTime,
    float InGravityAcceleration,
    float InMaximumFallSpeed,
    float InGroundSnapDistance,
    float& InOutVerticalVelocity,
    bool& bOutGrounded
)
{
    bOutGrounded = false;
    if (DeltaTime <= 0.0f)
    {
        return 0.0f;
    }

    const float SnapDistance = FMath::Max(0.5f, InGroundSnapDistance);
    const float GroundDelta = GroundZ - CurrentRootZ;
    if (bHasGround && InOutVerticalVelocity <= 0.0f &&
        GroundDelta >= -SnapDistance)
    {
        InOutVerticalVelocity = 0.0f;
        bOutGrounded = true;
        return GroundDelta;
    }

    InOutVerticalVelocity = FMath::Max(
        InOutVerticalVelocity - FMath::Max(1.0f, InGravityAcceleration) *
            DeltaTime,
        -FMath::Max(100.0f, InMaximumFallSpeed)
    );
    float FallDelta = InOutVerticalVelocity * DeltaTime;
    if (bHasGround && CurrentRootZ + FallDelta <= GroundZ)
    {
        FallDelta = GroundDelta;
        InOutVerticalVelocity = 0.0f;
        bOutGrounded = true;
    }
    return FallDelta;
}

bool UNaturalNPCVRSmoothLocomotionComponent::IsStepUpHeightTraversable(
    const float CurrentGroundZ,
    const float CandidateGroundZ,
    const float InMaximumStepUp
)
{
    const float StepHeight = CandidateGroundZ - CurrentGroundZ;
    return StepHeight > 0.5f &&
        StepHeight <= FMath::Max(10.0f, InMaximumStepUp);
}

FVector2D UNaturalNPCVRSmoothLocomotionComponent::ReadLeftThumbstick(
    APlayerController* Controller
) const
{
    if (!IsValid(Controller))
    {
        return FVector2D::ZeroVector;
    }

    const UInputAction* MoveAction = SmoothMoveAction.Get();
    if (!MoveAction && !SmoothMoveAction.IsNull())
    {
        MoveAction = SmoothMoveAction.LoadSynchronous();
    }
    if (const UEnhancedPlayerInput* EnhancedPlayerInput =
            Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
        EnhancedPlayerInput && MoveAction)
    {
        const FVector2D EnhancedValue = EnhancedPlayerInput
            ->GetActionValue(MoveAction)
            .Get<FVector2D>();
        if (!EnhancedValue.IsNearlyZero())
        {
            return ApplyRadialDeadZone(
                EnhancedValue,
                ThumbstickDeadZone
            );
        }
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

float UNaturalNPCVRSmoothLocomotionComponent::ReadRightThumbstickX(
    APlayerController* Controller
) const
{
    if (!IsValid(Controller))
    {
        return 0.0f;
    }

    const UInputAction* TurnAction = SmoothTurnAction.Get();
    if (!TurnAction && !SmoothTurnAction.IsNull())
    {
        TurnAction = SmoothTurnAction.LoadSynchronous();
    }
    if (const UEnhancedPlayerInput* EnhancedPlayerInput =
            Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
        EnhancedPlayerInput && TurnAction)
    {
        const float EnhancedValue = EnhancedPlayerInput
            ->GetActionValue(TurnAction)
            .Get<float>();
        if (!FMath::IsNearlyZero(EnhancedValue))
        {
            return ApplyAxisDeadZone(EnhancedValue, TurnDeadZone);
        }
    }

    TArray<float, TInlineAllocator<6>> Candidates;
    Candidates.Add(Controller->GetInputAnalogKeyState(
        EKeys::OculusTouch_Right_Thumbstick_X));
    Candidates.Add(Controller->GetInputAnalogKeyState(
        EKeys::ValveIndex_Right_Thumbstick_X));
    Candidates.Add(Controller->GetInputAnalogKeyState(
        EKeys::MixedReality_Right_Thumbstick_X));
    Candidates.Add(Controller->GetInputAnalogKeyState(
        EKeys::Vive_Right_Trackpad_X));
    Candidates.Add(Controller->GetInputAnalogKeyState(
        EKeys::ValveIndex_Right_Trackpad_X));
    if (bAllowStandardGamepadFallback)
    {
        Candidates.Add(Controller->GetInputAnalogKeyState(
            EKeys::Gamepad_RightX));
    }

    float StrongestInput = 0.0f;
    for (const float Candidate : Candidates)
    {
        if (FMath::Abs(Candidate) > FMath::Abs(StrongestInput))
        {
            StrongestInput = Candidate;
        }
    }
    return ApplyAxisDeadZone(StrongestInput, TurnDeadZone);
}

bool UNaturalNPCVRSmoothLocomotionComponent::ReadJumpButton(
    APlayerController* Controller
) const
{
    if (!IsValid(Controller))
    {
        return false;
    }

    const UInputAction* ResolvedJumpAction = JumpAction.Get();
    if (!ResolvedJumpAction && !JumpAction.IsNull())
    {
        ResolvedJumpAction = JumpAction.LoadSynchronous();
    }
    if (const UEnhancedPlayerInput* EnhancedPlayerInput =
            Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
        EnhancedPlayerInput && ResolvedJumpAction &&
        EnhancedPlayerInput->GetActionValue(ResolvedJumpAction).Get<bool>())
    {
        return true;
    }

    static const FName JumpKeys[] = {
        TEXT("OculusTouch_Right_B_Click"),
        TEXT("Gamepad_FaceButton_Right")
    };
    for (const FName KeyName : JumpKeys)
    {
        if (Controller->IsInputKeyDown(FKey(KeyName)))
        {
            return true;
        }
    }
    return false;
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
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCVRGround),
        false,
        Owner
    );
    FHitResult Hit;
    const FVector Start = ProbeLocation + FVector(
        0.0f,
        0.0f,
        FMath::Max(10.0f, MaximumStepUp) + 5.0f
    );
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
    const FVector PawnLocation = Pawn->GetActorLocation();
    FVector MoveDelta = DesiredWorldDelta;
    const float Radius = FMath::Max(10.0f, CollisionRadius);
    const float HalfHeight = FMath::Max(
        Radius + 1.0f,
        CollisionHalfHeight
    );
    float CurrentGroundZ = 0.0f;
    float TargetGroundZ = 0.0f;
    bool bHasGroundedMove = false;
    bool bUseStepUpAssist = false;
    const FVector CurrentGroundProbe(
        CameraLocation.X,
        CameraLocation.Y,
        PawnLocation.Z
    );
    FVector TargetGroundProbe = CurrentGroundProbe + DesiredWorldDelta;
    TargetGroundProbe.Z = PawnLocation.Z;
    if (bWasGrounded &&
        FindGroundHeight(CurrentGroundProbe, CurrentGroundZ) &&
        FindGroundHeight(TargetGroundProbe, TargetGroundZ))
    {
        bHasGroundedMove = true;
        if (StepUpSupportRemainingSeconds > 0.0f &&
            CurrentGroundZ >= StepUpSupportGroundZ - 1.0f)
        {
            StepUpSupportRemainingSeconds = 0.0f;
        }

        const FVector MoveDirection =
            DesiredWorldDelta.GetSafeNormal2D();
        if (!MoveDirection.IsNearlyZero())
        {
            FVector LeadingGroundProbe = TargetGroundProbe +
                MoveDirection * (Radius + 3.0f);
            LeadingGroundProbe.Z = PawnLocation.Z;
            float LeadingGroundZ = 0.0f;
            if (FindGroundHeight(LeadingGroundProbe, LeadingGroundZ) &&
                LeadingGroundZ > TargetGroundZ + 0.5f &&
                IsStepUpHeightTraversable(
                    CurrentGroundZ,
                    LeadingGroundZ,
                    MaximumStepUp))
            {
                TargetGroundZ = LeadingGroundZ;
                bUseStepUpAssist = true;
            }
        }

        if (StepUpSupportRemainingSeconds > 0.0f &&
            StepUpSupportGroundZ > TargetGroundZ)
        {
            TargetGroundZ = StepUpSupportGroundZ;
        }

        const float GroundDelta = TargetGroundZ - CurrentGroundZ;
        if (GroundDelta > FMath::Max(10.0f, MaximumStepUp))
        {
            // Treat an over-height surface as a wall. The capsule sweep below
            // can then slide along a chair or counter instead of freezing as
            // soon as the ground probe sees its top.
            TargetGroundZ = CurrentGroundZ;
            MoveDelta.Z = 0.0f;
        }
        else
        {
            MoveDelta.Z = GroundDelta < -FMath::Max(
                    20.0f,
                    MaximumStepDown)
                ? 0.0f
                : TargetGroundZ - PawnLocation.Z;
        }
    }
    else
    {
        CurrentGroundZ = PawnLocation.Z;
        MoveDelta.Z = 0.0f;
    }

    const auto CommitStepUpSupport = [this, bUseStepUpAssist, TargetGroundZ]()
    {
        if (!bUseStepUpAssist)
        {
            return;
        }
        const bool bNewStep = StepUpSupportRemainingSeconds <= 0.0f ||
            !FMath::IsNearlyEqual(
                StepUpSupportGroundZ,
                TargetGroundZ,
                1.0f
            );
        StepUpSupportGroundZ = TargetGroundZ;
        StepUpSupportRemainingSeconds = 0.35f;
        if (bNewStep)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("NATURALNPC_VR_LOCOMOTION step_up ground_z=%.1f"),
                TargetGroundZ
            );
        }
    };

    // Sweep horizontally above the higher of the two walkable surfaces.
    // A diagonal capsule sweep hits the vertical face of a stair or ramp
    // transition before its bottom can rise onto the new surface.
    const float SweepGroundZ = bHasGroundedMove
        ? FMath::Max(CurrentGroundZ, TargetGroundZ)
        : PawnLocation.Z;
    const FVector CapsuleStart(
        CameraLocation.X,
        CameraLocation.Y,
        SweepGroundZ + HalfHeight + 2.0f
    );
    const FVector CapsuleEnd = CapsuleStart + FVector(
        MoveDelta.X,
        MoveDelta.Y,
        0.0f
    );

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
        CommitStepUpSupport();
        return;
    }

    if (Hit.ImpactNormal.Z > 0.65f && !Hit.bStartPenetrating)
    {
        Pawn->AddActorWorldOffset(MoveDelta, false);
        CommitStepUpSupport();
        return;
    }

    if (Hit.bStartPenetrating)
    {
        const FVector EscapeDirection = Hit.Normal.GetSafeNormal2D();
        if (!EscapeDirection.IsNearlyZero())
        {
            Pawn->AddActorWorldOffset(
                EscapeDirection * FMath::Clamp(
                    Hit.PenetrationDepth + 1.0f,
                    1.0f,
                    Radius * 0.5f
                ),
                false
            );
        }
        CurrentVelocity = FVector::ZeroVector;
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

    const FVector SlideStart = CapsuleStart + FVector(
        SafeDelta.X,
        SafeDelta.Y,
        0.0f
    );
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

void UNaturalNPCVRSmoothLocomotionComponent::UpdateGravity(float DeltaTime)
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    UCameraComponent* Camera = ResolveHeadCamera();
    if (!bGravityEnabled || !IsValid(Pawn) || !IsValid(Camera) ||
        DeltaTime <= 0.0f)
    {
        VerticalVelocity = 0.0f;
        bWasGrounded = false;
        return;
    }

    const FVector PawnLocation = Pawn->GetActorLocation();
    const FVector CameraLocation = Camera->GetComponentLocation();
    FVector GroundProbe(
        CameraLocation.X,
        CameraLocation.Y,
        PawnLocation.Z
    );
    float GroundZ = 0.0f;
    bool bHasGround = FindGroundHeight(GroundProbe, GroundZ);
    StepUpSupportRemainingSeconds = FMath::Max(
        0.0f,
        StepUpSupportRemainingSeconds - DeltaTime
    );
    if (StepUpSupportRemainingSeconds > 0.0f &&
        (!bHasGround || GroundZ < StepUpSupportGroundZ - 1.0f))
    {
        GroundZ = StepUpSupportGroundZ;
        bHasGround = true;
    }
    else if (bHasGround && GroundZ >= StepUpSupportGroundZ - 1.0f)
    {
        StepUpSupportRemainingSeconds = 0.0f;
    }
    bool bGrounded = false;
    const float GravityDelta = CalculateGravityDelta(
        PawnLocation.Z,
        bHasGround,
        GroundZ,
        DeltaTime,
        GravityAcceleration,
        MaximumFallSpeed,
        GroundSnapDistance,
        VerticalVelocity,
        bGrounded
    );
    if (!FMath::IsNearlyZero(GravityDelta, 0.01f))
    {
        Pawn->AddActorWorldOffset(
            FVector(0.0f, 0.0f, GravityDelta),
            false
        );
    }
    if (bGrounded && !bWasGrounded)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NATURALNPC_VR_LOCOMOTION landed ground_z=%.1f"),
            GroundZ
        );
    }
    bWasGrounded = bGrounded;
}

void UNaturalNPCVRSmoothLocomotionComponent::TurnLocallyControlledPawn(
    float TurnInput,
    float DeltaTime
)
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    UCameraComponent* Camera = ResolveHeadCamera();
    if (!bSmoothTurningEnabled || !IsValid(Pawn) || !IsValid(Camera) ||
        FMath::IsNearlyZero(TurnInput) || DeltaTime <= 0.0f)
    {
        return;
    }

    const FVector PivotLocation = Camera->GetComponentLocation();
    const float DeltaYaw = TurnInput *
        FMath::Max(15.0f, MaximumTurnSpeed) * DeltaTime;
    Pawn->AddActorWorldRotation(FRotator(0.0f, DeltaYaw, 0.0f));

    // Keep the tracked head position fixed while rotating the XR origin.
    FVector PivotCorrection =
        PivotLocation - Camera->GetComponentLocation();
    PivotCorrection.Z = 0.0f;
    if (!PivotCorrection.IsNearlyZero())
    {
        Pawn->AddActorWorldOffset(PivotCorrection, false);
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
    if (!IsValid(Pawn) || !Pawn->IsLocallyControlled() ||
        !IsValid(Controller) ||
        !IsValid(Camera) || DeltaTime <= 0.0f)
    {
        CurrentVelocity = FVector::ZeroVector;
        VerticalVelocity = 0.0f;
        return;
    }

    if (!bSmoothLocomotionEnabled)
    {
        CurrentVelocity = FVector::ZeroVector;
        bJumpWasDown = false;
        UpdateGravity(DeltaTime);
        return;
    }

    TurnLocallyControlledPawn(
        ReadRightThumbstickX(Controller),
        DeltaTime
    );

    const bool bJumpDown = ReadJumpButton(Controller);
    if (bJumpEnabled && bGravityEnabled && bJumpDown &&
        !bJumpWasDown && bWasGrounded)
    {
        VerticalVelocity = FMath::Max(100.0f, JumpVelocity);
        bWasGrounded = false;
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NATURALNPC_VR_LOCOMOTION jump velocity=%.1f"),
            VerticalVelocity
        );
    }
    bJumpWasDown = bJumpDown;

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
    UpdateGravity(DeltaTime);
}
