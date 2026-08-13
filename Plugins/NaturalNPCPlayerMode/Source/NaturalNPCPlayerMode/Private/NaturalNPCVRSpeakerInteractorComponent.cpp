#include "NaturalNPCVRSpeakerInteractorComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "EngineUtils.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputCoreTypes.h"

namespace
{
const FName SpeakerClassFragment(TEXT("BP_InteractiveSpeaker"));

float SquaredDistanceToComponentBounds(
    const UPrimitiveComponent* Component,
    const FVector& Point
)
{
    if (!Component)
    {
        return TNumericLimits<float>::Max();
    }
    const FVector BoundsDelta = (
        Point - Component->Bounds.Origin
    ).GetAbs() - Component->Bounds.BoxExtent;
    const FVector OutsideDelta(
        FMath::Max(0.0f, BoundsDelta.X),
        FMath::Max(0.0f, BoundsDelta.Y),
        FMath::Max(0.0f, BoundsDelta.Z)
    );
    return OutsideDelta.SizeSquared();
}
}

UNaturalNPCVRSpeakerInteractorComponent::
    UNaturalNPCVRSpeakerInteractorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    bAutoActivate = true;
    SetIsReplicatedByDefault(false);
    SpeakerInteractAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_SpeakerInteract.IA_SpeakerInteract")
    ));
    LeftTriggerFallbackAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_Grab_Left_Pressed.IA_Grab_Left_Pressed")
    ));
    RightTriggerFallbackAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_Grab_Right_Pressed.IA_Grab_Right_Pressed")
    ));
}

void UNaturalNPCVRSpeakerInteractorComponent::BeginPlay()
{
    Super::BeginPlay();
    ResolveControllerOrigins();
    UE_LOG(
        LogTemp,
        Display,
        TEXT(
            "NATURALNPC_SPEAKER_INTERACT ready pawn=%s "
            "left_aim=%s left_grip=%s right_aim=%s right_grip=%s "
            "touch_targeting=%d trigger_required=1 radius=%.1f"
        ),
        *GetNameSafe(GetOwner()),
        *GetNameSafe(LeftAim.Get()),
        *GetNameSafe(LeftGrip.Get()),
        *GetNameSafe(RightAim.Get()),
        *GetNameSafe(RightGrip.Get()),
        bActivateSpeakerButtonsOnTouch ? 1 : 0,
        TouchRadius
    );
}

void UNaturalNPCVRSpeakerInteractorComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!bSpeakerInteractionEnabled)
    {
        bLeftTriggerWasDown = false;
        bRightTriggerWasDown = false;
        return;
    }

    APawn* Pawn = Cast<APawn>(GetOwner());
    APlayerController* Controller = Pawn
        ? Cast<APlayerController>(Pawn->GetController())
        : nullptr;
    if (!Controller || !Pawn->IsLocallyControlled())
    {
        return;
    }

    if ((!RightAim.IsValid() && !RightGrip.IsValid()) ||
        (!LeftAim.IsValid() && !LeftGrip.IsValid()))
    {
        ResolveControllerOrigins();
    }

    const bool bRightTriggerDown =
        ReadTrigger(Controller, true) >= TriggerThreshold;
    const bool bLeftTriggerDown =
        ReadTrigger(Controller, false) >= TriggerThreshold;

    const auto TryActivateForHand = [this](
        const bool bTriggerDown,
        const bool bWasTriggerDown,
        USceneComponent* Aim,
        USceneComponent* Grip,
        const TCHAR* HandName)
    {
        if (!bTriggerDown || bWasTriggerDown)
        {
            return false;
        }

        UPrimitiveComponent* TargetButton = FindTargetButton(Aim, Grip);
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NATURALNPC_SPEAKER_INTERACT trigger hand=%s target=%s"),
            HandName,
            *GetNameSafe(TargetButton)
        );
        if (!ShouldActivateTarget(
                bTriggerDown,
                bWasTriggerDown,
                IsValid(TargetButton)))
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("NATURALNPC_SPEAKER_INTERACT no_target hand=%s"),
                HandName
            );
            return false;
        }

        return ActivateButton(TargetButton);
    };

    const bool bActivatedByRight = TryActivateForHand(
        bRightTriggerDown,
        bRightTriggerWasDown,
        RightAim.Get(),
        RightGrip.Get(),
        TEXT("right")
    );
    if (!bActivatedByRight)
    {
        TryActivateForHand(
            bLeftTriggerDown,
            bLeftTriggerWasDown,
            LeftAim.Get(),
            LeftGrip.Get(),
            TEXT("left")
        );
    }

    bRightTriggerWasDown = bRightTriggerDown;
    bLeftTriggerWasDown = bLeftTriggerDown;
}

bool UNaturalNPCVRSpeakerInteractorComponent::ShouldActivateTarget(
    const bool bTriggerDown,
    const bool bWasTriggerDown,
    const bool bHasTarget
)
{
    return bTriggerDown && !bWasTriggerDown && bHasTarget;
}

float UNaturalNPCVRSpeakerInteractorComponent::ReadTrigger(
    APlayerController* Controller,
    const bool bRightHand
) const
{
    if (!Controller)
    {
        return 0.0f;
    }

    float StrongestValue = bRightHand
        ? ReadActionValue(Controller, SpeakerInteractAction)
        : 0.0f;
    StrongestValue = FMath::Max(
        StrongestValue,
        ReadActionValue(
            Controller,
            bRightHand
                ? RightTriggerFallbackAction
                : LeftTriggerFallbackAction
        )
    );

    static const FName RightTriggerKeys[] = {
        TEXT("OculusTouch_Right_Trigger_Axis"),
        TEXT("OculusTouch_Right_Trigger_Click"),
        TEXT("ValveIndex_Right_Trigger_Axis"),
        TEXT("ValveIndex_Right_Trigger_Click"),
        TEXT("Vive_Right_Trigger_Axis"),
        TEXT("Vive_Right_Trigger_Click"),
        TEXT("MixedReality_Right_Trigger_Axis"),
        TEXT("MixedReality_Right_Trigger_Click"),
        TEXT("Gamepad_RightTriggerAxis")
    };
    static const FName LeftTriggerKeys[] = {
        TEXT("OculusTouch_Left_Trigger_Axis"),
        TEXT("OculusTouch_Left_Trigger_Click"),
        TEXT("ValveIndex_Left_Trigger_Axis"),
        TEXT("ValveIndex_Left_Trigger_Click"),
        TEXT("Vive_Left_Trigger_Axis"),
        TEXT("Vive_Left_Trigger_Click"),
        TEXT("MixedReality_Left_Trigger_Axis"),
        TEXT("MixedReality_Left_Trigger_Click"),
        TEXT("Gamepad_LeftTriggerAxis")
    };
    const FName* TriggerKeys = bRightHand
        ? RightTriggerKeys
        : LeftTriggerKeys;
    const int32 TriggerKeyCount = bRightHand
        ? UE_ARRAY_COUNT(RightTriggerKeys)
        : UE_ARRAY_COUNT(LeftTriggerKeys);
    for (int32 Index = 0; Index < TriggerKeyCount; ++Index)
    {
        StrongestValue = FMath::Max(
            StrongestValue,
            Controller->GetInputAnalogKeyState(FKey(TriggerKeys[Index]))
        );
        if (Controller->IsInputKeyDown(FKey(TriggerKeys[Index])))
        {
            StrongestValue = 1.0f;
        }
    }
    return StrongestValue;
}

float UNaturalNPCVRSpeakerInteractorComponent::ReadActionValue(
    APlayerController* Controller,
    const TSoftObjectPtr<UInputAction>& ActionReference
) const
{
    if (!Controller || ActionReference.IsNull())
    {
        return 0.0f;
    }

    const UInputAction* Action = ActionReference.Get();
    if (!Action)
    {
        Action = ActionReference.LoadSynchronous();
    }
    const UEnhancedPlayerInput* EnhancedInput =
        Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
    return EnhancedInput && Action
        ? EnhancedInput->GetActionValue(Action).GetMagnitude()
        : 0.0f;
}

void UNaturalNPCVRSpeakerInteractorComponent::ResolveControllerOrigins()
{
    LeftAim.Reset();
    LeftGrip.Reset();
    RightAim.Reset();
    RightGrip.Reset();
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    TArray<USceneComponent*> Components;
    Owner->GetComponents<USceneComponent>(Components);
    for (USceneComponent* Component : Components)
    {
        if (!Component)
        {
            continue;
        }
        const FName Name = Component->GetFName();
        if (Name == TEXT("MotionControllerLeftAim"))
        {
            LeftAim = Component;
        }
        else if (Name == TEXT("MotionControllerLeftGrip"))
        {
            LeftGrip = Component;
        }
        else if (Name == TEXT("MotionControllerRightAim"))
        {
            RightAim = Component;
        }
        else if (Name == TEXT("MotionControllerRightGrip"))
        {
            RightGrip = Component;
        }
    }
    if (!RightAim.IsValid())
    {
        RightAim = RightGrip;
    }
    if (!RightGrip.IsValid())
    {
        RightGrip = RightAim;
    }
    if (!LeftAim.IsValid())
    {
        LeftAim = LeftGrip;
    }
    if (!LeftGrip.IsValid())
    {
        LeftGrip = LeftAim;
    }
}

UPrimitiveComponent*
UNaturalNPCVRSpeakerInteractorComponent::FindTouchButton(
    USceneComponent* Aim,
    USceneComponent* Grip
) const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    TArray<FVector, TInlineAllocator<3>> TouchPoints;
    if (Grip)
    {
        TouchPoints.Add(Grip->GetComponentLocation());
    }
    if (Aim)
    {
        const FVector AimLocation = Aim->GetComponentLocation();
        TouchPoints.Add(AimLocation);
        TouchPoints.Add(
            AimLocation + Aim->GetForwardVector().GetSafeNormal() *
                FMath::Max(0.0f, TouchForwardOffset)
        );
    }
    if (TouchPoints.IsEmpty())
    {
        return nullptr;
    }

    UPrimitiveComponent* BestButton = nullptr;
    float BestDistanceSquared = FMath::Square(
        FMath::Max(2.0f, TouchRadius)
    );
    for (TActorIterator<AActor> ActorIt(World); ActorIt; ++ActorIt)
    {
        AActor* Actor = *ActorIt;
        if (!Actor || !Actor->GetClass()->GetName().Contains(
                SpeakerClassFragment.ToString()))
        {
            continue;
        }

        TArray<UPrimitiveComponent*> Components;
        Actor->GetComponents<UPrimitiveComponent>(Components);
        for (UPrimitiveComponent* Component : Components)
        {
            if (FunctionForButton(Component).IsNone() ||
                !Component->IsVisible())
            {
                continue;
            }
            for (const FVector& TouchPoint : TouchPoints)
            {
                const float DistanceSquared =
                    SquaredDistanceToComponentBounds(
                        Component,
                        TouchPoint
                    );
                if (DistanceSquared <= BestDistanceSquared)
                {
                    BestDistanceSquared = DistanceSquared;
                    BestButton = Component;
                }
            }
        }
    }
    return BestButton;
}

UPrimitiveComponent*
UNaturalNPCVRSpeakerInteractorComponent::FindTargetButton(
    USceneComponent* Aim,
    USceneComponent* Grip
) const
{
    UWorld* World = GetWorld();
    if (!World || (!Aim && !Grip))
    {
        return nullptr;
    }

    if (bActivateSpeakerButtonsOnTouch)
    {
        if (UPrimitiveComponent* TouchButton = FindTouchButton(Aim, Grip))
        {
            return TouchButton;
        }
    }

    const FVector TouchOrigin = Grip
        ? Grip->GetComponentLocation()
        : Aim->GetComponentLocation();
    const FVector RayOrigin = Aim
        ? Aim->GetComponentLocation()
        : TouchOrigin;
    const FVector RayDirection = Aim
        ? Aim->GetForwardVector().GetSafeNormal()
        : FVector::ForwardVector;

    UPrimitiveComponent* BestRayButton = nullptr;
    float BestRayScore = TNumericLimits<float>::Max();

    for (TActorIterator<AActor> ActorIt(World); ActorIt; ++ActorIt)
    {
        AActor* Actor = *ActorIt;
        if (!Actor || !Actor->GetClass()->GetName().Contains(
                SpeakerClassFragment.ToString()))
        {
            continue;
        }

        TArray<UPrimitiveComponent*> Components;
        Actor->GetComponents<UPrimitiveComponent>(Components);
        for (UPrimitiveComponent* Component : Components)
        {
            if (FunctionForButton(Component).IsNone() ||
                !Component->IsVisible())
            {
                continue;
            }

            const FVector ButtonLocation = Component->Bounds.Origin;
            const FVector ToButton = ButtonLocation - RayOrigin;
            const float AlongRay = FVector::DotProduct(
                ToButton,
                RayDirection
            );
            if (AlongRay < 0.0f || AlongRay > PointerDistance)
            {
                continue;
            }
            const float LateralDistance = (
                ToButton - RayDirection * AlongRay
            ).Size();
            if (LateralDistance > PointerRadius)
            {
                continue;
            }
            const float Score = LateralDistance + AlongRay * 0.001f;
            if (Score < BestRayScore)
            {
                BestRayScore = Score;
                BestRayButton = Component;
            }
        }
    }
    return BestRayButton;
}

bool UNaturalNPCVRSpeakerInteractorComponent::ActivateButton(
    UPrimitiveComponent* Button
) const
{
    if (!Button)
    {
        return false;
    }
    AActor* Speaker = Button->GetOwner();
    const FName FunctionName = FunctionForButton(Button);
    UFunction* Function = Speaker
        ? Speaker->FindFunction(FunctionName)
        : nullptr;
    if (!Function)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("NATURALNPC_SPEAKER_INTERACT missing_function actor=%s function=%s"),
            *GetNameSafe(Speaker),
            *FunctionName.ToString()
        );
        return false;
    }

    Speaker->ProcessEvent(Function, nullptr);
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NATURALNPC_SPEAKER_INTERACT activated actor=%s button=%s function=%s"),
        *GetNameSafe(Speaker),
        *GetNameSafe(Button),
        *FunctionName.ToString()
    );
    return true;
}

FName UNaturalNPCVRSpeakerInteractorComponent::FunctionForButton(
    const UPrimitiveComponent* Button
)
{
    if (!Button)
    {
        return NAME_None;
    }
    const FString Name = Button->GetName();
    if (Name.Contains(TEXT("Button_Jazz")))
    {
        return TEXT("PressJazzButton");
    }
    if (Name.Contains(TEXT("Button_Blues")))
    {
        return TEXT("PressBluesButton");
    }
    if (Name.Contains(TEXT("Button_Dance")))
    {
        return TEXT("PressDanceButton");
    }
    if (Name.Contains(TEXT("Button_Pause")))
    {
        return TEXT("PressPauseButton");
    }
    return NAME_None;
}
