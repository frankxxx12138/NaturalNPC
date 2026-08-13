#include "NPCWorldStateAgentComponent.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/Skeleton.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "NPCWorldActionTarget.h"
#include "NPCWorldStateObjectComponent.h"
#include "NPCWorldStateSubsystem.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NPCAdaptivePickupAnimInstance.h"
#include "TimerManager.h"

UNPCWorldStateAgentComponent::UNPCWorldStateAgentComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    const TSoftObjectPtr<UAnimSequence> PistolPickup(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_PickingUp_Small.Human2_PickingUp_Small")
        )
    );
    const TSoftObjectPtr<UAnimSequence> PistolHeldIdle(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_MF_Pistol_Idle_ADS.Human2_MF_Pistol_Idle_ADS")
        )
    );
    const TSoftObjectPtr<UAnimSequence> TwoHandPickup(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_Pickup_TwoHand.Human2_Pickup_TwoHand")
        )
    );
    const TSoftObjectPtr<UAnimSequence> GroundPickup(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_Pickup_Floor.Human2_Pickup_Floor")
        )
    );
    const TSoftObjectPtr<UAnimSequence> TablePickup(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_Pickup_Table.Human2_Pickup_Table")
        )
    );
    const TSoftObjectPtr<UAnimSequence> TwoHandHeldIdle(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_Held_Idle_TwoHand.Human2_Held_Idle_TwoHand")
        )
    );
    const TSoftObjectPtr<UAnimSequence> HeldWalk(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_BoxWalk_Carry.Human2_BoxWalk_Carry")
        )
    );

    DefaultPickupAnimation = TwoHandPickup;
    GroundPickupAnimation = GroundPickup;
    TablePickupAnimation = TablePickup;
    DefaultDropAnimation = TwoHandPickup;
    DefaultHeldIdleAnimation = TwoHandHeldIdle;
    DefaultHeldWalkAnimation = HeldWalk;

    FNPCWorldItemAnimationProfile PistolProfile;
    PistolProfile.Category = TEXT("pistol");
    PistolProfile.PickupAnimation = PistolPickup;
    PistolProfile.HeldIdleAnimation = PistolHeldIdle;
    PistolProfile.HeldWalkAnimation = HeldWalk;
    PistolProfile.DropAnimation = PistolPickup;
    PistolProfile.PickupEffectTriggerNormalizedTime = 0.25f;
    PistolProfile.PickupAnimationPlayRate = 2.5f;
    ItemAnimationProfiles.Add(PistolProfile);

    for (const FName Category : {
        FName(TEXT("ball")),
        FName(TEXT("cube")),
        FName(TEXT("log")),
        FName(TEXT("object"))})
    {
        FNPCWorldItemAnimationProfile& Profile =
            ItemAnimationProfiles.AddDefaulted_GetRef();
        Profile.Category = Category;
        Profile.PickupAnimation = TwoHandPickup;
        Profile.HeldIdleAnimation = TwoHandHeldIdle;
        Profile.DropAnimation = TwoHandPickup;
        Profile.PickupEffectTriggerNormalizedTime = 0.82f;
        Profile.bCenterObjectBetweenHands = true;
    }
    PickupApproachWalkAnimation = TSoftObjectPtr<UAnimSequence>(
        FSoftObjectPath(TEXT(
            "/Game/MetaHumans/Human2/Animations/Locomotion/"
            "Human2_MF_Unarmed_Walk_Fwd.Human2_MF_Unarmed_Walk_Fwd"
        ))
    );
    PickupApproachRunAnimation = TSoftObjectPtr<UAnimSequence>(
        FSoftObjectPath(TEXT(
            "/Game/MetaHumans/Human2/Animations/Locomotion/"
            "Human2_MF_Unarmed_Jog_Fwd.Human2_MF_Unarmed_Jog_Fwd"
        ))
    );
}

void UNPCWorldStateAgentComponent::BeginPlay()
{
    Super::BeginPlay();
    if (UWorld* World = GetWorld())
    {
        if (UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            Subsystem->RefreshAutoDiscovery();
        }
        RefreshWorldState();
        if (bAutoRefresh)
        {
            World->GetTimerManager().SetTimer(
                RefreshTimerHandle,
                this,
                &UNPCWorldStateAgentComponent::RefreshWorldState,
                FMath::Max(0.1f, RefreshIntervalSeconds),
                true
            );
        }
    }
}

void UNPCWorldStateAgentComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    QueuedNaturalLanguageActions.Reset();
    bNaturalLanguageSequenceActive = false;
    CancelPendingAction();
    if (HeldActor.IsValid())
    {
        FNPCWorldActionResult IgnoredResult;
        DropHeldActor(false, IgnoredResult);
    }
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RefreshTimerHandle);
    }
    Super::EndPlay(EndPlayReason);
}

void UNPCWorldStateAgentComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bApproachingActionTarget)
    {
        UpdateApproach(DeltaTime);
    }
}

void UNPCWorldStateAgentComponent::RestoreAnimationState(
    TArray<FActionAnimationMeshState>& SavedStates
)
{
    for (const FActionAnimationMeshState& Saved : SavedStates)
    {
        USkeletalMeshComponent* Mesh = Saved.Mesh.Get();
        if (!Mesh)
        {
            continue;
        }
        Mesh->SetAnimationMode(Saved.AnimationMode, true);
        if (Saved.AnimationMode == EAnimationMode::AnimationBlueprint)
        {
            Mesh->SetAnimInstanceClass(Saved.AnimClass);
        }
    }
    SavedStates.Reset();
}

void UNPCWorldStateAgentComponent::RestoreHeldAnimationState()
{
    RestoreAnimationState(HeldAnimationMeshStates);
}

void UNPCWorldStateAgentComponent::CancelPendingAction()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ActionEffectTimerHandle);
        World->GetTimerManager().ClearTimer(ActionFinishTimerHandle);
    }
    RestoreAnimationState(ActionAnimationMeshStates);
    bActionInProgress = false;
    bPendingEffectApplied = false;
    bPendingEffectSucceeded = false;
    bApproachingActionTarget = false;
    PendingAction = FNPCWorldActionDefinition();
    PendingActionTarget.Reset();
    PendingObjectId = NAME_None;
    PendingParameters.Reset();
    ApproachPathPoints.Reset();
    ApproachPathPointIndex = 0;
    ApproachElapsedSeconds = 0.0f;
    bApproachGroundOffsetInitialized = false;
    ApproachActorGroundOffsetZ = 0.0f;
    ApproachRepathElapsedSeconds = 0.0f;
    ApproachStallElapsedSeconds = 0.0f;
    ApproachRepathFailureCount = 0;
    ApproachLastTargetLocation = FVector::ZeroVector;
    ApproachGoalLocation = FVector::ZeroVector;
    ActiveApproachAnimation.Reset();
}

void UNPCWorldStateAgentComponent::RefreshWorldState()
{
    if (UWorld* World = GetWorld())
    {
        if (UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            CachedWorldStateJson = Subsystem->BuildWorldStateJson(
                GetOwner(),
                PerceptionRadius
            );
            CachedWorldStateText = Subsystem->BuildWorldStateText(
                GetOwner(),
                PerceptionRadius
            );
            OnWorldStateUpdated.Broadcast(CachedWorldStateJson);
        }
    }
}

TArray<FNPCWorldObjectState>
UNPCWorldStateAgentComponent::GetVisibleWorldObjects() const
{
    if (const UWorld* World = GetWorld())
    {
        if (const UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            return Subsystem->GetWorldState(GetOwner(), PerceptionRadius);
        }
    }
    return {};
}

FNPCWorldActionResult UNPCWorldStateAgentComponent::MakeResult(
    bool bSuccess,
    FName ObjectId,
    FName ActionId,
    const FString& Message
) const
{
    FNPCWorldActionResult Result;
    Result.bSuccess = bSuccess;
    Result.ObjectId = ObjectId;
    Result.ActionId = ActionId;
    Result.Message = Message;
    return Result;
}

USceneComponent* UNPCWorldStateAgentComponent::FindHandAnchor(
    FName SocketName,
    FName& OutResolvedSocket
) const
{
    OutResolvedSocket = NAME_None;
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    TArray<USkeletalMeshComponent*> Meshes;
    Owner->GetComponents(Meshes);
    USkeletalMeshComponent* SocketFallback = nullptr;
    for (USkeletalMeshComponent* Mesh : Meshes)
    {
        if (!Mesh || !Mesh->DoesSocketExist(SocketName))
        {
            continue;
        }
        if (!SocketFallback)
        {
            SocketFallback = Mesh;
        }
        if (Mesh->GetName().Contains(HandMeshNameHint))
        {
            OutResolvedSocket = SocketName;
            return Mesh;
        }
    }
    if (SocketFallback)
    {
        OutResolvedSocket = SocketName;
        return SocketFallback;
    }

    TArray<USceneComponent*> SceneComponents;
    Owner->GetComponents(SceneComponents);
    for (USceneComponent* Component : SceneComponents)
    {
        if (Component && !HandMeshNameHint.IsEmpty() &&
            Component->GetName().Contains(HandMeshNameHint))
        {
            return Component;
        }
    }
    return SocketName.IsNone() ? Owner->GetRootComponent() : nullptr;
}

FVector UNPCWorldStateAgentComponent::GetTargetInteractionLocation(
    AActor* Target
) const
{
    if (!IsValid(Target))
    {
        return FVector::ZeroVector;
    }
    const FVector ActorLocation = Target->GetActorLocation();
    FVector Origin = ActorLocation;
    FVector Extent = FVector::ZeroVector;
    Target->GetActorBounds(false, Origin, Extent);
    return Extent.IsNearlyZero() ? ActorLocation : Origin;
}

float UNPCWorldStateAgentComponent::GetPickupReachDistance(
    const FNPCWorldActionDefinition& Action
) const
{
    return FMath::Max(
        20.0f,
        Action.MaxDistance > 0.0f
            ? Action.MaxDistance
            : DefaultPickupDistance
    );
}

bool UNPCWorldStateAgentComponent::PickupActor(
    const FNPCWorldActionDefinition& Action,
    AActor* Target,
    FNPCWorldActionResult& OutResult
)
{
    if (!IsValid(Target) || !IsValid(GetOwner()))
    {
        OutResult = MakeResult(
            false,
            NAME_None,
            TEXT("pickup"),
            TEXT("The target object is no longer valid.")
        );
        return false;
    }
    if (HeldActor.IsValid())
    {
        OutResult = MakeResult(
            false,
            Target->GetFName(),
            TEXT("pickup"),
            TEXT("The NPC is already holding another object.")
        );
        return false;
    }

    const FVector TargetLocation = GetTargetInteractionLocation(Target);
    const float Distance = FVector::Dist2D(
        GetOwner()->GetActorLocation(),
        TargetLocation
    );
    const float MaxDistance = GetPickupReachDistance(Action);
    if (Distance > MaxDistance)
    {
        OutResult = MakeResult(
            false,
            Target->GetFName(),
            TEXT("pickup"),
            FString::Printf(
                TEXT("The object is too far away (%.0f cm, reach %.0f cm)."),
                Distance,
                MaxDistance
            )
        );
        return false;
    }

    const FName SocketName = Action.AttachSocket.IsNone()
        ? HandSocket
        : Action.AttachSocket;
    FName ResolvedSocket = NAME_None;
    USceneComponent* HandAnchor = FindHandAnchor(
        SocketName,
        ResolvedSocket
    );
    if (!HandAnchor || !Target->GetRootComponent())
    {
        OutResult = MakeResult(
            false,
            Target->GetFName(),
            TEXT("pickup"),
            FString::Printf(
                TEXT("Could not find hand socket or anchor '%s' on the NPC."),
                *SocketName.ToString()
            )
        );
        return false;
    }

    HeldPrimitiveStates.Reset();
    TArray<UPrimitiveComponent*> PrimitiveComponents;
    Target->GetComponents(PrimitiveComponents);
    for (UPrimitiveComponent* Primitive : PrimitiveComponents)
    {
        if (!Primitive)
        {
            continue;
        }
        FHeldPrimitiveState& Saved = HeldPrimitiveStates.AddDefaulted_GetRef();
        Saved.Component = Primitive;
        Saved.bWasSimulatingPhysics = Primitive->IsSimulatingPhysics();
        Saved.bWasGravityEnabled = Primitive->IsGravityEnabled();
        Saved.PawnResponse = Primitive->GetCollisionResponseToChannel(ECC_Pawn);
        Primitive->SetSimulatePhysics(false);
        Primitive->SetEnableGravity(false);
        Primitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    }

    const bool bAttached = Target->AttachToComponent(
        HandAnchor,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        ResolvedSocket
    );
    if (!bAttached)
    {
        for (const FHeldPrimitiveState& Saved : HeldPrimitiveStates)
        {
            if (UPrimitiveComponent* Primitive = Saved.Component.Get())
            {
                Primitive->SetCollisionResponseToChannel(
                    ECC_Pawn,
                    Saved.PawnResponse
                );
                Primitive->SetEnableGravity(Saved.bWasGravityEnabled);
                Primitive->SetSimulatePhysics(Saved.bWasSimulatingPhysics);
            }
        }
        HeldPrimitiveStates.Reset();
        OutResult = MakeResult(
            false,
            Target->GetFName(),
            TEXT("pickup"),
            TEXT("The object could not be attached to the NPC hand.")
        );
        return false;
    }

    FVector AttachLocationOffset = Action.AttachLocationOffset;
    const FNPCWorldItemAnimationProfile* AnimationProfile =
        FindItemAnimationProfile(Target);
    const USkeletalMeshComponent* HandMesh =
        Cast<USkeletalMeshComponent>(HandAnchor);
    if (AnimationProfile && AnimationProfile->bCenterObjectBetweenHands &&
        HandMesh && !ResolvedSocket.IsNone() &&
        HandMesh->DoesSocketExist(SupportHandSocket))
    {
        const FTransform PrimaryHandTransform = HandMesh->GetSocketTransform(
            ResolvedSocket,
            RTS_World
        );
        const FVector HandsMidpoint = (
            PrimaryHandTransform.GetLocation() +
            HandMesh->GetSocketLocation(SupportHandSocket)
        ) * 0.5f;
        AttachLocationOffset +=
            PrimaryHandTransform.InverseTransformPosition(HandsMidpoint);
    }
    Target->GetRootComponent()->SetRelativeLocationAndRotation(
        AttachLocationOffset,
        Action.AttachRotationOffset
    );
    HeldActor = Target;

    if (UNPCWorldStateObjectComponent* ObjectComponent =
        Target->FindComponentByClass<UNPCWorldStateObjectComponent>())
    {
        ObjectComponent->State.Add(TEXT("is_held"), TEXT("true"));
        ObjectComponent->State.Add(TEXT("held_by"), GetOwner()->GetName());
        ObjectComponent->NotifyStateChanged();
        OutResult.ObjectId = ObjectComponent->GetResolvedObjectId();
    }
    else
    {
        OutResult.ObjectId = Target->GetFName();
    }
    OutResult.bSuccess = true;
    OutResult.ActionId = TEXT("pickup");
    OutResult.Message = FString::Printf(
        TEXT("Picked up %s."),
        *Target->GetName()
    );
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION effect=pickup success=1 npc=%s object=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *OutResult.ObjectId.ToString()
    );
    return true;
}

bool UNPCWorldStateAgentComponent::DropHeldActor(
    bool bThrow,
    FNPCWorldActionResult& OutResult
)
{
    AActor* Target = HeldActor.Get();
    if (!IsValid(Target))
    {
        OutResult = MakeResult(
            false,
            NAME_None,
            bThrow ? FName(TEXT("throw")) : FName(TEXT("drop")),
            TEXT("The NPC is not holding an object.")
        );
        return false;
    }

    FName ObjectId = Target->GetFName();
    if (UNPCWorldStateObjectComponent* ObjectComponent =
        Target->FindComponentByClass<UNPCWorldStateObjectComponent>())
    {
        ObjectId = ObjectComponent->GetResolvedObjectId();
    }

    RestoreHeldAnimationState();
    Target->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    UPrimitiveComponent* ImpulseTarget = nullptr;
    for (const FHeldPrimitiveState& Saved : HeldPrimitiveStates)
    {
        if (UPrimitiveComponent* Primitive = Saved.Component.Get())
        {
            Primitive->SetCollisionResponseToChannel(
                ECC_Pawn,
                Saved.PawnResponse
            );
            Primitive->SetEnableGravity(Saved.bWasGravityEnabled);
            Primitive->SetSimulatePhysics(Saved.bWasSimulatingPhysics);
            if (!ImpulseTarget && Primitive->IsSimulatingPhysics())
            {
                ImpulseTarget = Primitive;
            }
        }
    }
    if (bThrow && ImpulseTarget && GetOwner())
    {
        ImpulseTarget->AddImpulse(
            GetOwner()->GetActorForwardVector() * ThrowImpulse,
            NAME_None,
            true
        );
    }

    if (UNPCWorldStateObjectComponent* ObjectComponent =
        Target->FindComponentByClass<UNPCWorldStateObjectComponent>())
    {
        ObjectComponent->State.Add(TEXT("is_held"), TEXT("false"));
        ObjectComponent->State.Remove(TEXT("held_by"));
        ObjectComponent->NotifyStateChanged();
    }
    HeldActor.Reset();
    HeldPrimitiveStates.Reset();
    ActiveHeldIdleAnimation.Reset();
    HeldIdleSourceAction = FNPCWorldActionDefinition();
    OutResult = MakeResult(
        true,
        ObjectId,
        bThrow ? FName(TEXT("throw")) : FName(TEXT("drop")),
        bThrow
            ? FString::Printf(TEXT("Threw %s."), *Target->GetName())
            : FString::Printf(TEXT("Dropped %s."), *Target->GetName())
    );
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION effect=%s success=1 npc=%s object=%s"),
        bThrow ? TEXT("throw") : TEXT("drop"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *ObjectId.ToString()
    );
    return true;
}

bool UNPCWorldStateAgentComponent::ExecuteCustomAction(
    AActor* Target,
    FName ActionId,
    const FString& Parameters,
    FNPCWorldActionResult& OutResult
)
{
    FString Message;
    if (Target->GetClass()->ImplementsInterface(
        UNPCWorldActionTarget::StaticClass()))
    {
        const bool bSuccess =
            INPCWorldActionTarget::Execute_ExecuteNPCWorldAction(
                Target,
                GetOwner(),
                ActionId,
                Parameters,
                Message
            );
        OutResult = MakeResult(
            bSuccess,
            Target->GetFName(),
            ActionId,
            Message
        );
        return bSuccess;
    }

    TArray<FName> FunctionCandidates = {ActionId};
    if (ActionId == TEXT("fire"))
    {
        FunctionCandidates.Append({
            FName(TEXT("Fire")),
            FName(TEXT("Shoot")),
            FName(TEXT("Use"))
        });
    }
    for (const FName FunctionName : FunctionCandidates)
    {
        if (UFunction* Function = Target->FindFunction(FunctionName))
        {
            if (Function->ParmsSize == 0)
            {
                Target->ProcessEvent(Function, nullptr);
                OutResult = MakeResult(
                    true,
                    Target->GetFName(),
                    ActionId,
                    FString::Printf(
                        TEXT("Executed %s on %s."),
                        *ActionId.ToString(),
                        *Target->GetName()
                    )
                );
                return true;
            }
        }
    }

    OutResult = MakeResult(
        false,
        Target->GetFName(),
        ActionId,
        TEXT("This custom action needs the NPCWorldActionTarget interface.")
    );
    return false;
}

bool UNPCWorldStateAgentComponent::ExecuteActionEffect(
    const FNPCWorldActionDefinition& Action,
    AActor* Target,
    FName ObjectId,
    const FString& Parameters,
    FNPCWorldActionResult& OutResult
)
{
    if (!IsValid(Target) && Action.ActionId != TEXT("drop") &&
        Action.ActionId != TEXT("throw"))
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            Action.ActionId,
            TEXT("The target object is no longer valid.")
        );
        return false;
    }

    bool bSuccess = false;
    if (Action.ActionId == TEXT("pickup"))
    {
        bSuccess = PickupActor(Action, Target, OutResult);
    }
    else if (Action.ActionId == TEXT("drop") ||
        Action.ActionId == TEXT("throw"))
    {
        bSuccess = DropHeldActor(Action.ActionId == TEXT("throw"), OutResult);
    }
    else if (Action.ActionId == TEXT("inspect"))
    {
        const UNPCWorldStateObjectComponent* ObjectComponent = Target
            ? Target->FindComponentByClass<UNPCWorldStateObjectComponent>()
            : nullptr;
        const FString Name = ObjectComponent
            ? ObjectComponent->GetResolvedDisplayName().ToString()
            : (Target ? Target->GetName() : ObjectId.ToString());
        OutResult = MakeResult(
            true,
            ObjectId,
            Action.ActionId,
            ObjectComponent && !ObjectComponent->Description.IsEmpty()
                ? ObjectComponent->Description
                : FString::Printf(TEXT("It is %s."), *Name)
        );
        bSuccess = true;
    }
    else
    {
        bSuccess = ExecuteCustomAction(
            Target,
            Action.ActionId,
            Parameters,
            OutResult
        );
        OutResult.ObjectId = ObjectId;
    }
    return bSuccess;
}

const FNPCWorldItemAnimationProfile*
UNPCWorldStateAgentComponent::FindItemAnimationProfile(AActor* Target) const
{
    FName Category = TEXT("object");
    if (const UNPCWorldStateObjectComponent* ObjectComponent = Target
        ? Target->FindComponentByClass<UNPCWorldStateObjectComponent>()
        : nullptr)
    {
        Category = ObjectComponent->Category.IsNone()
            ? FName(TEXT("object"))
            : ObjectComponent->Category;
    }

    return ItemAnimationProfiles.FindByPredicate(
        [Category](const FNPCWorldItemAnimationProfile& Profile)
        {
            return Profile.Category == Category;
        }
    );
}

UAnimSequence* UNPCWorldStateAgentComponent::ResolveActionAnimation(
    const FNPCWorldActionDefinition& Action,
    AActor* Target
) const
{
    TSoftObjectPtr<UAnimSequence> Animation = Action.ActionAnimation;
    if (Animation.IsNull() && Action.ActionId == TEXT("pickup"))
    {
        if (UAnimSequence* HeightSpecificAnimation =
                GetPickupAnimationForTarget(Target))
        {
            return HeightSpecificAnimation;
        }
    }
    const FNPCWorldItemAnimationProfile* Profile =
        FindItemAnimationProfile(Target);
    if (Animation.IsNull() && Profile)
    {
        if (Action.ActionId == TEXT("pickup"))
        {
            Animation = Profile->PickupAnimation;
        }
        else if (Action.ActionId == TEXT("drop"))
        {
            Animation = Profile->DropAnimation;
        }
        else if (Action.ActionId == TEXT("throw"))
        {
            Animation = Profile->ThrowAnimation;
        }
    }
    if (Animation.IsNull())
    {
        if (Action.ActionId == TEXT("pickup"))
        {
            Animation = DefaultPickupAnimation;
        }
        else if (Action.ActionId == TEXT("drop"))
        {
            Animation = DefaultDropAnimation;
        }
        else if (Action.ActionId == TEXT("throw"))
        {
            Animation = DefaultThrowAnimation;
        }
    }
    return Animation.IsNull() ? nullptr : Animation.LoadSynchronous();
}

bool UNPCWorldStateAgentComponent::ShouldUseTablePickupAnimation(
    AActor* Target,
    float* OutHeightAboveGround
) const
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || !IsValid(Target))
    {
        if (OutHeightAboveGround)
        {
            *OutHeightAboveGround = 0.0f;
        }
        return false;
    }

    float GroundZ = 0.0f;
    if (!FindApproachGroundHeight(Owner->GetActorLocation(), GroundZ))
    {
        FVector OwnerBoundsOrigin = Owner->GetActorLocation();
        FVector OwnerBoundsExtent = FVector::ZeroVector;
        Owner->GetActorBounds(false, OwnerBoundsOrigin, OwnerBoundsExtent);
        GroundZ = OwnerBoundsOrigin.Z - OwnerBoundsExtent.Z;
    }

    const float HeightAboveGround =
        GetTargetInteractionLocation(Target).Z - GroundZ;
    if (OutHeightAboveGround)
    {
        *OutHeightAboveGround = HeightAboveGround;
    }
    return HeightAboveGround >= TablePickupMinimumHeight;
}

UAnimSequence* UNPCWorldStateAgentComponent::GetPickupAnimationForTarget(
    AActor* Target
) const
{
    const TSoftObjectPtr<UAnimSequence>& Animation =
        ShouldUseTablePickupAnimation(Target)
            ? TablePickupAnimation
            : GroundPickupAnimation;
    return Animation.IsNull() ? nullptr : Animation.LoadSynchronous();
}

UAnimSequence* UNPCWorldStateAgentComponent::ResolveHeldIdleAnimation(
    const FNPCWorldActionDefinition& Action,
    AActor* Target
) const
{
    TSoftObjectPtr<UAnimSequence> Animation = Action.HeldIdleAnimation;
    if (Animation.IsNull())
    {
        if (const FNPCWorldItemAnimationProfile* Profile =
            FindItemAnimationProfile(Target))
        {
            Animation = Profile->HeldIdleAnimation;
        }
    }
    if (Animation.IsNull())
    {
        Animation = DefaultHeldIdleAnimation;
    }
    return Animation.IsNull() ? nullptr : Animation.LoadSynchronous();
}

UAnimSequence* UNPCWorldStateAgentComponent::GetHeldWalkAnimation() const
{
    AActor* Target = HeldActor.Get();
    if (!IsValid(Target))
    {
        return nullptr;
    }

    TSoftObjectPtr<UAnimSequence> Animation;
    if (const FNPCWorldItemAnimationProfile* Profile =
        FindItemAnimationProfile(Target))
    {
        Animation = Profile->HeldWalkAnimation;
    }
    if (Animation.IsNull())
    {
        Animation = DefaultHeldWalkAnimation;
    }
    return Animation.IsNull() ? nullptr : Animation.LoadSynchronous();
}

UAnimSequence* UNPCWorldStateAgentComponent::GetCurrentHeldIdleAnimation() const
{
    AActor* Target = HeldActor.Get();
    return IsValid(Target)
        ? ResolveHeldIdleAnimation(HeldIdleSourceAction, Target)
        : nullptr;
}

void UNPCWorldStateAgentComponent::SuspendHeldIdleAnimation()
{
    RestoreHeldAnimationState();
}

USkeletalMeshComponent*
UNPCWorldStateAgentComponent::FindActionAnimationMesh(
    const FNPCWorldActionDefinition& Action
) const
{
    const FName SocketName = Action.AttachSocket.IsNone()
        ? HandSocket
        : Action.AttachSocket;
    FName ResolvedSocket = NAME_None;
    return Cast<USkeletalMeshComponent>(
        FindHandAnchor(SocketName, ResolvedSocket)
    );
}

bool UNPCWorldStateAgentComponent::PlaySynchronizedAnimation(
    UAnimSequence* Animation,
    USkeletalMeshComponent* PrimaryMesh,
    bool bLooping,
    float PlayRate,
    bool bReverse,
    TArray<FActionAnimationMeshState>& SavedStates,
    AActor* AdaptivePickupTarget,
    float PickupContactNormalizedTime,
    bool bUseSupportHand,
    bool bAdjustPickupPelvisAndSpine
)
{
    if (!Animation || !PrimaryMesh ||
        !PrimaryMesh->GetSkeletalMeshAsset() || !GetOwner())
    {
        return false;
    }

    const USkeleton* MeshSkeleton =
        PrimaryMesh->GetSkeletalMeshAsset()->GetSkeleton();
    if (!MeshSkeleton || Animation->GetSkeleton() != MeshSkeleton)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("NPC world action animation '%s' is not compatible with "
                "mesh '%s'."),
            *Animation->GetPathName(),
            *PrimaryMesh->GetPathName()
        );
        return false;
    }

    const float SafePlayRate = FMath::Max(0.01f, FMath::Abs(PlayRate));
    FVector PickupTargetLocation = FVector::ZeroVector;
    FVector PickupTargetExtent = FVector::ZeroVector;
    const bool bUseAdaptivePickup =
        bEnableAdaptivePickupIK && IsValid(AdaptivePickupTarget) &&
        !bLooping && !bReverse;
    if (bUseAdaptivePickup)
    {
        AdaptivePickupTarget->GetActorBounds(
            false,
            PickupTargetLocation,
            PickupTargetExtent
        );
    }
    const float GripHalfWidth = FMath::Clamp(
        FMath::Min(PickupTargetExtent.GetAbsMax(), 30.0f) * 0.55f,
        4.0f,
        22.0f
    );
    TArray<USkeletalMeshComponent*> SkeletalMeshes;
    GetOwner()->GetComponents(SkeletalMeshes);
    bool bPrimaryMeshPlaying = false;
    for (USkeletalMeshComponent* Mesh : SkeletalMeshes)
    {
        if (!Mesh || !Mesh->GetSkeletalMeshAsset() ||
            Mesh->GetSkeletalMeshAsset()->GetSkeleton() != MeshSkeleton)
        {
            continue;
        }

        FActionAnimationMeshState& Saved =
            SavedStates.AddDefaulted_GetRef();
        Saved.Mesh = Mesh;
        Saved.AnimationMode = Mesh->GetAnimationMode();
        Saved.AnimClass = Mesh->GetAnimClass();
        UAnimSingleNodeInstance* SingleNode = nullptr;
        if (bUseAdaptivePickup)
        {
            Mesh->SetAnimInstanceClass(
                UNPCAdaptivePickupAnimInstance::StaticClass()
            );
            UNPCAdaptivePickupAnimInstance* PickupInstance =
                Cast<UNPCAdaptivePickupAnimInstance>(Mesh->GetAnimInstance());
            if (PickupInstance)
            {
                PickupInstance->ConfigurePickup(
                    PickupTargetLocation,
                    PickupContactNormalizedTime,
                    bUseSupportHand,
                    bAdjustPickupPelvisAndSpine,
                    GripHalfWidth,
                    AdaptivePickupIKBlendWindow,
                    AdaptivePickupPelvisInfluence,
                    AdaptivePickupMaxPelvisOffset,
                    AdaptivePickupMaxArmStretch
                );
                PickupInstance->SetAnimationAsset(
                    Animation,
                    false,
                    SafePlayRate
                );
                PickupInstance->SetPosition(0.0f, false);
                PickupInstance->SetPlaying(true);
                SingleNode = PickupInstance;
            }
        }
        else
        {
            Mesh->PlayAnimation(Animation, bLooping);
            SingleNode = Mesh->GetSingleNodeInstance();
        }

        if (!SingleNode)
        {
            Mesh->SetAnimationMode(Saved.AnimationMode, true);
            if (Saved.AnimationMode == EAnimationMode::AnimationBlueprint)
            {
                Mesh->SetAnimInstanceClass(Saved.AnimClass);
            }
            SavedStates.Pop(EAllowShrinking::No);
            if (Mesh == PrimaryMesh)
            {
                RestoreAnimationState(SavedStates);
                return false;
            }
            continue;
        }

        SingleNode->SetPlayRate(bReverse ? -SafePlayRate : SafePlayRate);
        if (bReverse)
        {
            SingleNode->SetPosition(Animation->GetPlayLength(), false);
        }
        bPrimaryMeshPlaying = bPrimaryMeshPlaying || Mesh == PrimaryMesh;
    }
    if (!bPrimaryMeshPlaying)
    {
        RestoreAnimationState(SavedStates);
    }
    else if (bUseAdaptivePickup)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NPC_ADAPTIVE_PICKUP_IK started npc=%s target=%s "
                "height=%.1f contact=%.2f support_hand=%d meshes=%d"),
            GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
            *AdaptivePickupTarget->GetName(),
            PickupTargetLocation.Z,
            PickupContactNormalizedTime,
            bUseSupportHand ? 1 : 0,
            SavedStates.Num()
        );
    }
    return bPrimaryMeshPlaying;
}

bool UNPCWorldStateAgentComponent::StartHeldIdleAnimation(
    const FNPCWorldActionDefinition& SourceAction,
    AActor* Target
)
{
    if (!IsValid(Target) || Target != HeldActor.Get())
    {
        return false;
    }

    UAnimSequence* Animation = ResolveHeldIdleAnimation(
        SourceAction,
        Target
    );
    USkeletalMeshComponent* PrimaryMesh = Animation
        ? FindActionAnimationMesh(SourceAction)
        : nullptr;
    if (!Animation || !PrimaryMesh)
    {
        return false;
    }

    RestoreHeldAnimationState();
    if (!PlaySynchronizedAnimation(
        Animation,
        PrimaryMesh,
        true,
        1.0f,
        false,
        HeldAnimationMeshStates
    ))
    {
        return false;
    }

    HeldIdleSourceAction = SourceAction;
    ActiveHeldIdleAnimation = Animation;
    const UNPCWorldStateObjectComponent* ObjectComponent =
        Target->FindComponentByClass<UNPCWorldStateObjectComponent>();
    const FName Category = ObjectComponent
        ? ObjectComponent->Category
        : FName(TEXT("object"));
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION held_idle_started npc=%s object=%s "
            "category=%s animation=%s meshes=%d"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *Target->GetName(),
        *Category.ToString(),
        *Animation->GetPathName(),
        HeldAnimationMeshStates.Num()
    );
    return true;
}

bool UNPCWorldStateAgentComponent::ResumeHeldIdleAnimation()
{
    AActor* Target = HeldActor.Get();
    if (!IsValid(Target))
    {
        return false;
    }
    return StartHeldIdleAnimation(HeldIdleSourceAction, Target);
}

bool UNPCWorldStateAgentComponent::StartAnimatedAction(
    const FNPCWorldActionDefinition& Action,
    AActor* Target,
    FName ObjectId,
    const FString& Parameters,
    FNPCWorldActionResult& OutResult
)
{
    UAnimSequence* Animation = ResolveActionAnimation(Action, Target);
    USkeletalMeshComponent* PrimaryMesh = Animation
        ? FindActionAnimationMesh(Action)
        : nullptr;
    if (!Animation || !PrimaryMesh)
    {
        return false;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    float RequestedPlayRate = Action.AnimationPlayRate;
    float PickupHeightAboveGround = 0.0f;
    const bool bUseTablePickup = Action.ActionId == TEXT("pickup") &&
        Action.ActionAnimation.IsNull() &&
        ShouldUseTablePickupAnimation(Target, &PickupHeightAboveGround);
    const TSoftObjectPtr<UAnimSequence>& HeightPickupAsset = bUseTablePickup
        ? TablePickupAnimation
        : GroundPickupAnimation;
    const bool bUsingHeightSpecificPickup =
        Action.ActionId == TEXT("pickup") &&
        Action.ActionAnimation.IsNull() &&
        !HeightPickupAsset.IsNull() &&
        Animation == HeightPickupAsset.Get();
    if (bUsingHeightSpecificPickup)
    {
        RequestedPlayRate = bUseTablePickup
            ? TablePickupAnimationPlayRate
            : GroundPickupAnimationPlayRate;
    }
    else if (Action.ActionId == TEXT("pickup") &&
        Action.ActionAnimation.IsNull())
    {
        if (const FNPCWorldItemAnimationProfile* Profile =
            FindItemAnimationProfile(Target))
        {
            RequestedPlayRate = Profile->PickupAnimationPlayRate;
        }
    }
    else if (Action.ActionId == TEXT("drop") &&
        Action.ActionAnimation.IsNull())
    {
        if (const FNPCWorldItemAnimationProfile* Profile =
            FindItemAnimationProfile(Target);
            Profile && Profile->DropAnimation == Profile->PickupAnimation)
        {
            RequestedPlayRate = Profile->PickupAnimationPlayRate;
        }
    }
    const float PlayRate = FMath::Max(
        0.01f,
        FMath::Abs(RequestedPlayRate)
    );
    float EffectTriggerNormalizedTime = Action.EffectTriggerNormalizedTime;
    const FNPCWorldItemAnimationProfile* AnimationProfile = nullptr;
    if (bUsingHeightSpecificPickup)
    {
        EffectTriggerNormalizedTime = bUseTablePickup
            ? TablePickupEffectTriggerNormalizedTime
            : GroundPickupEffectTriggerNormalizedTime;
        AnimationProfile = FindItemAnimationProfile(Target);
    }
    else if (Action.ActionId == TEXT("pickup") &&
        Action.ActionAnimation.IsNull())
    {
        AnimationProfile = FindItemAnimationProfile(Target);
        if (AnimationProfile &&
            AnimationProfile->PickupEffectTriggerNormalizedTime >= 0.0f)
        {
            EffectTriggerNormalizedTime =
                AnimationProfile->PickupEffectTriggerNormalizedTime;
        }
    }
    else if (Action.ActionId == TEXT("drop") &&
        Action.ActionAnimation.IsNull())
    {
        AnimationProfile = FindItemAnimationProfile(Target);
        if (AnimationProfile &&
            AnimationProfile->DropAnimation ==
                AnimationProfile->PickupAnimation &&
            AnimationProfile->PickupEffectTriggerNormalizedTime >= 0.0f)
        {
            EffectTriggerNormalizedTime = 1.0f -
                AnimationProfile->PickupEffectTriggerNormalizedTime;
        }
    }
    const bool bAdaptivePickup =
        Action.ActionId == TEXT("pickup") &&
        IsValid(Target) && !Action.bPlayAnimationInReverse;
    const bool bUseSupportHand =
        AnimationProfile && AnimationProfile->bCenterObjectBetweenHands;

    if (bUsingHeightSpecificPickup)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NPC_PICKUP_ANIMATION_VARIANT npc=%s target=%s "
                "variant=%s height=%.1f threshold=%.1f animation=%s"),
            GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
            Target ? *Target->GetName() : TEXT("None"),
            bUseTablePickup ? TEXT("table") : TEXT("ground"),
            PickupHeightAboveGround,
            TablePickupMinimumHeight,
            *Animation->GetPathName()
        );
    }

    const bool bResumeHeldIdleOnFailure =
        !HeldAnimationMeshStates.IsEmpty() && HeldActor.IsValid();
    RestoreHeldAnimationState();
    if (!PlaySynchronizedAnimation(
        Animation,
        PrimaryMesh,
        false,
        PlayRate,
        Action.bPlayAnimationInReverse,
        ActionAnimationMeshStates,
        bAdaptivePickup ? Target : nullptr,
        EffectTriggerNormalizedTime,
        bUseSupportHand,
        !bUsingHeightSpecificPickup || !bUseTablePickup
    ))
    {
        if (bResumeHeldIdleOnFailure)
        {
            ResumeHeldIdleAnimation();
        }
        return false;
    }

    PendingAction = Action;
    PendingActionTarget = Target;
    PendingObjectId = ObjectId;
    PendingParameters = Parameters;
    bActionInProgress = true;
    bPendingEffectApplied = false;
    bPendingEffectSucceeded = false;

    const float Duration = FMath::Max(
        0.01f,
        Animation->GetPlayLength() / PlayRate
    );
    const float EffectDelay = Duration * FMath::Clamp(
        EffectTriggerNormalizedTime,
        0.0f,
        1.0f
    );
    if (EffectDelay <= KINDA_SMALL_NUMBER)
    {
        ApplyPendingActionEffect();
    }
    else
    {
        World->GetTimerManager().SetTimer(
            ActionEffectTimerHandle,
            this,
            &UNPCWorldStateAgentComponent::ApplyPendingActionEffect,
            EffectDelay,
            false
        );
    }
    World->GetTimerManager().SetTimer(
        ActionFinishTimerHandle,
        this,
        &UNPCWorldStateAgentComponent::FinishPendingAction,
        Duration + 0.02f,
        false
    );

    OutResult = MakeResult(
        true,
        ObjectId,
        Action.ActionId,
        FString::Printf(
            TEXT("Started %s animation for %s."),
            *Action.ActionId.ToString(),
            Target ? *Target->GetName() : TEXT("the held object")
        )
    );
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION animation_started npc=%s action=%s object=%s "
            "animation=%s duration=%.3f trigger=%.2f meshes=%d"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *Action.ActionId.ToString(),
        *ObjectId.ToString(),
        *Animation->GetPathName(),
        Duration,
        EffectTriggerNormalizedTime,
        ActionAnimationMeshStates.Num()
    );
    return true;
}

bool UNPCWorldStateAgentComponent::StartApproachForAction(
    const FNPCWorldActionDefinition& Action,
    AActor* Target,
    FName ObjectId,
    const FString& Parameters,
    FNPCWorldActionResult& OutResult
)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!IsValid(Owner) || !IsValid(Target) || !World)
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            Action.ActionId,
            TEXT("The NPC or target object is no longer valid.")
        );
        return false;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const FVector TargetLocation = GetTargetInteractionLocation(Target);
    const float Distance = FVector::Dist2D(OwnerLocation, TargetLocation);
    if (Distance > FMath::Max(100.0f, MaxPickupApproachDistance))
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            Action.ActionId,
            FString::Printf(
                TEXT("The object is too far away to approach (%.0f cm)."),
                Distance
            )
        );
        return false;
    }

    const float ReachDistance = GetPickupReachDistance(Action);

    PendingAction = Action;
    PendingActionTarget = Target;
    PendingObjectId = ObjectId;
    PendingParameters = Parameters;
    bActionInProgress = true;
    bPendingEffectApplied = false;
    bApproachingActionTarget = true;
    ApproachPathPointIndex = 0;
    ApproachElapsedSeconds = 0.0f;
    ApproachRepathElapsedSeconds = 0.0f;
    ApproachStallElapsedSeconds = 0.0f;
    ApproachRepathFailureCount = 0;
    bApproachGroundOffsetInitialized = false;
    ApproachActorGroundOffsetZ = 0.0f;
    ApproachLastTargetLocation = TargetLocation;
    ActiveApproachAnimation.Reset();

    if (!RebuildApproachPath(Target, ReachDistance))
    {
        CancelPendingAction();
        OutResult = MakeResult(
            false,
            ObjectId,
            Action.ActionId,
            TEXT("No navigable route to the object was found.")
        );
        return false;
    }

    const bool bRun = Distance >= PickupApproachRunDistance;
    UpdateApproachAnimation(bRun);
    OutResult = MakeResult(
        true,
        ObjectId,
        Action.ActionId,
        FString::Printf(TEXT("Walking to %s."), *Target->GetName())
    );
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION approach_started npc=%s object=%s "
            "distance=%.1f reach=%.1f path_points=%d navigation=%d"),
        *Owner->GetName(),
        *ObjectId.ToString(),
        Distance,
        ReachDistance,
        ApproachPathPoints.Num(),
        1
    );
    return true;
}

bool UNPCWorldStateAgentComponent::RebuildApproachPath(
    AActor* Target,
    float ReachDistance
)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    UNavigationSystemV1* NavigationSystem = World
        ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)
        : nullptr;
    if (!IsValid(Owner) || !IsValid(Target) || !NavigationSystem)
    {
        return false;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const FVector TargetLocation = GetTargetInteractionLocation(Target);
    FVector PreferredDirection = OwnerLocation - TargetLocation;
    PreferredDirection.Z = 0.0f;
    if (!PreferredDirection.Normalize())
    {
        PreferredDirection = FVector::ForwardVector;
    }

    const float CandidateRadius = FMath::Max(
        45.0f,
        ReachDistance * 0.75f
    );
    const int32 CandidateCount = FMath::Clamp(
        PickupApproachCandidateCount,
        4,
        32
    );
    const FVector ProjectionExtent(140.0f, 140.0f, 250.0f);
    float BestPathLength = TNumericLimits<float>::Max();
    TArray<FVector> BestPathPoints;
    FVector BestGoal = FVector::ZeroVector;

    for (int32 CandidateIndex = 0;
         CandidateIndex < CandidateCount;
         ++CandidateIndex)
    {
        const float AngleDegrees =
            360.0f * static_cast<float>(CandidateIndex) /
            static_cast<float>(CandidateCount);
        const FVector CandidateDirection = PreferredDirection.RotateAngleAxis(
            AngleDegrees,
            FVector::UpVector
        );
        const FVector CandidateLocation =
            TargetLocation + CandidateDirection * CandidateRadius;

        FNavLocation ProjectedLocation;
        if (!NavigationSystem->ProjectPointToNavigation(
                CandidateLocation,
                ProjectedLocation,
                ProjectionExtent))
        {
            continue;
        }
        if (FVector::Dist2D(ProjectedLocation.Location, TargetLocation) >
            ReachDistance * 1.05f)
        {
            continue;
        }

        UNavigationPath* Path =
            UNavigationSystemV1::FindPathToLocationSynchronously(
                World,
                OwnerLocation,
                ProjectedLocation.Location,
                nullptr
            );
        if (!Path || !Path->IsValid() || Path->IsPartial() ||
            Path->PathPoints.Num() < 2)
        {
            continue;
        }

        float PathLength = 0.0f;
        for (int32 PointIndex = 1;
             PointIndex < Path->PathPoints.Num();
             ++PointIndex)
        {
            PathLength += FVector::Dist2D(
                Path->PathPoints[PointIndex - 1],
                Path->PathPoints[PointIndex]
            );
        }
        if (PathLength >= BestPathLength)
        {
            continue;
        }

        BestPathLength = PathLength;
        BestPathPoints = Path->PathPoints;
        BestGoal = ProjectedLocation.Location;
    }

    if (BestPathPoints.IsEmpty())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("NPC_WORLD_ACTION approach_repath_failed npc=%s object=%s"),
            *Owner->GetName(),
            *Target->GetName()
        );
        return false;
    }

    ApproachPathPoints = MoveTemp(BestPathPoints);
    ApproachPathPointIndex = 0;
    ApproachGoalLocation = BestGoal;
    ApproachLastTargetLocation = TargetLocation;
    ApproachRepathElapsedSeconds = 0.0f;
    ApproachStallElapsedSeconds = 0.0f;

    UE_LOG(
        LogTemp,
        Verbose,
        TEXT("NPC_WORLD_ACTION approach_repath npc=%s object=%s "
            "points=%d length=%.1f goal=%s"),
        *Owner->GetName(),
        *Target->GetName(),
        ApproachPathPoints.Num(),
        BestPathLength,
        *ApproachGoalLocation.ToCompactString()
    );
    return true;
}

void UNPCWorldStateAgentComponent::UpdateApproachAnimation(bool bRun)
{
    UAnimSequence* Animation = bRun
        ? PickupApproachRunAnimation.LoadSynchronous()
        : PickupApproachWalkAnimation.LoadSynchronous();
    if (Animation && ActiveApproachAnimation.Get() == Animation &&
        !ActionAnimationMeshStates.IsEmpty())
    {
        return;
    }

    RestoreAnimationState(ActionAnimationMeshStates);
    ActiveApproachAnimation.Reset();
    USkeletalMeshComponent* PrimaryMesh =
        FindActionAnimationMesh(PendingAction);
    if (Animation && PrimaryMesh && PlaySynchronizedAnimation(
        Animation,
        PrimaryMesh,
        true,
        1.0f,
        false,
        ActionAnimationMeshStates
    ))
    {
        ActiveApproachAnimation = Animation;
    }
}

bool UNPCWorldStateAgentComponent::FindApproachGroundHeight(
    const FVector& Location,
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
        SCENE_QUERY_STAT(NaturalNPCPickupApproachGround),
        false,
        Owner
    );
    if (PendingActionTarget.IsValid())
    {
        QueryParams.AddIgnoredActor(PendingActionTarget.Get());
    }
    if (HeldActor.IsValid())
    {
        QueryParams.AddIgnoredActor(HeldActor.Get());
    }

    const float TraceAbove = FMath::Max(
        60.0f,
        PickupApproachMaximumGroundStepUp + 30.0f
    );
    const float TraceBelow = FMath::Max(
        300.0f,
        PickupApproachMaximumGroundStepDown + 100.0f
    );
    const FVector Start(Location.X, Location.Y, Location.Z + TraceAbove);
    const FVector End(Location.X, Location.Y, Location.Z - TraceBelow);
    FHitResult Hit;
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

bool UNPCWorldStateAgentComponent::UpdateApproachGroundHeight(
    FVector& InOutLocation,
    float DeltaTime
)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || DeltaTime <= 0.0f)
    {
        return false;
    }

    const FVector CurrentLocation = Owner->GetActorLocation();
    if (!bApproachGroundOffsetInitialized)
    {
        float CurrentGroundZ = 0.0f;
        if (!FindApproachGroundHeight(CurrentLocation, CurrentGroundZ))
        {
            return false;
        }
        ApproachActorGroundOffsetZ = CurrentLocation.Z - CurrentGroundZ;
        bApproachGroundOffsetInitialized = true;
    }

    FVector GroundProbeLocation = InOutLocation;
    GroundProbeLocation.Z = CurrentLocation.Z;
    float TargetGroundZ = 0.0f;
    if (!FindApproachGroundHeight(GroundProbeLocation, TargetGroundZ))
    {
        return false;
    }

    const float CurrentGroundZ =
        CurrentLocation.Z - ApproachActorGroundOffsetZ;
    const float GroundDelta = TargetGroundZ - CurrentGroundZ;
    if (GroundDelta > FMath::Max(
            10.0f,
            PickupApproachMaximumGroundStepUp) ||
        GroundDelta < -FMath::Max(
            20.0f,
            PickupApproachMaximumGroundStepDown))
    {
        return false;
    }

    const float TargetActorZ =
        TargetGroundZ + ApproachActorGroundOffsetZ;
    InOutLocation.Z = FMath::FInterpTo(
        CurrentLocation.Z,
        TargetActorZ,
        DeltaTime,
        FMath::Max(1.0f, PickupApproachGroundFollowInterpSpeed)
    );
    return true;
}

void UNPCWorldStateAgentComponent::UpdateApproach(float DeltaTime)
{
    if (DeltaTime <= 0.0f)
    {
        return;
    }

    AActor* Owner = GetOwner();
    AActor* Target = PendingActionTarget.Get();
    if (!IsValid(Owner) || !IsValid(Target))
    {
        FailPendingAction(TEXT("The target object is no longer valid."));
        return;
    }

    ApproachElapsedSeconds += DeltaTime;
    ApproachRepathElapsedSeconds += DeltaTime;
    if (ApproachElapsedSeconds >
        FMath::Max(1.0f, PickupApproachTimeoutSeconds))
    {
        FailPendingAction(TEXT("The NPC could not reach the object in time."));
        return;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const FVector TargetLocation = GetTargetInteractionLocation(Target);
    const float ReachDistance = GetPickupReachDistance(PendingAction);
    const float DistanceToTarget = FVector::Dist2D(
        OwnerLocation,
        TargetLocation
    );
    if (DistanceToTarget <= ReachDistance)
    {
        BeginPendingActionAtTarget();
        return;
    }

    const bool bTargetMoved = FVector::Dist2D(
        TargetLocation,
        ApproachLastTargetLocation
    ) > 60.0f;
    if (bTargetMoved &&
        ApproachRepathElapsedSeconds >=
            FMath::Max(0.05f, PickupApproachRepathIntervalSeconds))
    {
        if (!RebuildApproachPath(Target, ReachDistance))
        {
            ++ApproachRepathFailureCount;
        }
        else
        {
            ApproachRepathFailureCount = 0;
        }
    }

    while (ApproachPathPointIndex < ApproachPathPoints.Num() &&
        FVector::Dist2D(
            OwnerLocation,
            ApproachPathPoints[ApproachPathPointIndex]
        ) <= 15.0f)
    {
        ++ApproachPathPointIndex;
    }

    FVector MoveTarget = TargetLocation;
    bool bFollowingPathPoint = false;
    if (ApproachPathPointIndex < ApproachPathPoints.Num())
    {
        MoveTarget = ApproachPathPoints[ApproachPathPointIndex];
        bFollowingPathPoint = true;
    }

    FVector ToMoveTarget = MoveTarget - OwnerLocation;
    ToMoveTarget.Z = 0.0f;
    const float DistanceToMoveTarget = ToMoveTarget.Size();
    if (DistanceToMoveTarget <= UE_SMALL_NUMBER)
    {
        ++ApproachPathPointIndex;
        return;
    }

    const bool bRun = DistanceToTarget >= PickupApproachRunDistance;
    UpdateApproachAnimation(bRun);
    const float Speed = bRun
        ? FMath::Max(20.0f, PickupApproachRunSpeed)
        : FMath::Max(20.0f, PickupApproachWalkSpeed);
    float MoveDistance = FMath::Min(
        DistanceToMoveTarget,
        Speed * DeltaTime
    );
    if (!bFollowingPathPoint)
    {
        MoveDistance = FMath::Min(
            MoveDistance,
            FMath::Max(0.0f, DistanceToTarget - ReachDistance * 0.9f)
        );
    }

    const FVector MoveDirection = ToMoveTarget.GetSafeNormal2D();
    FVector NewLocation = OwnerLocation + MoveDirection * MoveDistance;
    const bool bGroundHeightValid = UpdateApproachGroundHeight(
        NewLocation,
        DeltaTime
    );
    if (!bGroundHeightValid)
    {
        NewLocation = OwnerLocation;
    }
    FHitResult SweepHit;
    Owner->SetActorLocation(NewLocation, true, &SweepHit);
    const float ActualMoveDistance = FVector::Dist2D(
        OwnerLocation,
        Owner->GetActorLocation()
    );
    if (ActualMoveDistance < FMath::Min(1.0f, MoveDistance * 0.2f))
    {
        ApproachStallElapsedSeconds += DeltaTime;
    }
    else
    {
        ApproachStallElapsedSeconds = 0.0f;
        ApproachRepathFailureCount = 0;
    }

    if (SweepHit.bBlockingHit ||
        ApproachStallElapsedSeconds >=
            FMath::Max(0.05f, PickupApproachStallSeconds))
    {
        if (ApproachRepathElapsedSeconds >=
            FMath::Max(0.05f, PickupApproachRepathIntervalSeconds))
        {
            const bool bRebuilt = RebuildApproachPath(
                Target,
                ReachDistance
            );
            if (!bRebuilt)
            {
                ++ApproachRepathFailureCount;
                ApproachRepathElapsedSeconds = 0.0f;
                ApproachStallElapsedSeconds = 0.0f;
            }
            if (ApproachRepathFailureCount >= FMath::Max(
                    1,
                    MaximumPickupApproachRepathFailures))
            {
                FailPendingAction(
                    TEXT("The route to the object is blocked.")
                );
                return;
            }
        }
    }

    const FRotator CurrentRotation = Owner->GetActorRotation();
    const FRotator DesiredDirection = MoveDirection.Rotation();
    Owner->SetActorRotation(FMath::RInterpTo(
        CurrentRotation,
        FRotator(
            CurrentRotation.Pitch,
            DesiredDirection.Yaw + PickupFacingYawOffsetDegrees,
            CurrentRotation.Roll
        ),
        DeltaTime,
        FMath::Max(0.1f, PickupApproachRotationSpeed)
    ));
}

void UNPCWorldStateAgentComponent::BeginPendingActionAtTarget()
{
    AActor* Owner = GetOwner();
    AActor* Target = PendingActionTarget.Get();
    if (!IsValid(Owner) || !IsValid(Target))
    {
        FailPendingAction(TEXT("The target object is no longer valid."));
        return;
    }

    const FNPCWorldActionDefinition Action = PendingAction;
    const FName ObjectId = PendingObjectId;
    const FString Parameters = PendingParameters;
    bApproachingActionTarget = false;
    ApproachPathPoints.Reset();
    ActiveApproachAnimation.Reset();
    RestoreAnimationState(ActionAnimationMeshStates);

    FVector ToTarget = GetTargetInteractionLocation(Target) -
        Owner->GetActorLocation();
    ToTarget.Z = 0.0f;
    if (!ToTarget.IsNearlyZero())
    {
        const FRotator CurrentRotation = Owner->GetActorRotation();
        Owner->SetActorRotation(FRotator(
            CurrentRotation.Pitch,
            ToTarget.Rotation().Yaw + PickupFacingYawOffsetDegrees,
            CurrentRotation.Roll
        ));
    }

    FNPCWorldActionResult Result;
    if (StartAnimatedAction(
        Action,
        Target,
        ObjectId,
        Parameters,
        Result
    ))
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NPC_WORLD_ACTION approach_completed npc=%s object=%s"),
            *Owner->GetName(),
            *ObjectId.ToString()
        );
        return;
    }

    const bool bSuccess = ExecuteActionEffect(
        Action,
        Target,
        ObjectId,
        Parameters,
        Result
    );
    RefreshWorldState();
    OnWorldActionCompleted.Broadcast(Result);
    CancelPendingAction();
    if (bSuccess && Action.ActionId == TEXT("pickup") &&
        HeldActor.Get() == Target)
    {
        StartHeldIdleAnimation(Action, Target);
    }
    if (bNaturalLanguageSequenceActive)
    {
        if (bSuccess)
        {
            ContinueNaturalLanguageActionSequence();
        }
        else
        {
            FailNaturalLanguageActionSequence(Result.Message);
        }
    }
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION approach_effect_without_animation "
            "npc=%s object=%s success=%d"),
        *Owner->GetName(),
        *ObjectId.ToString(),
        bSuccess ? 1 : 0
    );
}

void UNPCWorldStateAgentComponent::FailPendingAction(
    const FString& Message
)
{
    const FNPCWorldActionResult Result = MakeResult(
        false,
        PendingObjectId,
        PendingAction.ActionId,
        Message
    );
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("NPC_WORLD_ACTION approach_failed npc=%s object=%s message=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *PendingObjectId.ToString(),
        *Message
    );
    CancelPendingAction();
    RefreshWorldState();
    OnWorldActionCompleted.Broadcast(Result);
    FailNaturalLanguageActionSequence(Message);
}

void UNPCWorldStateAgentComponent::ApplyPendingActionEffect()
{
    if (!bActionInProgress || bPendingEffectApplied)
    {
        return;
    }
    bPendingEffectApplied = true;

    FNPCWorldActionResult Result;
    ExecuteActionEffect(
        PendingAction,
        PendingActionTarget.Get(),
        PendingObjectId,
        PendingParameters,
        Result
    );
    bPendingEffectSucceeded = Result.bSuccess;
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION effect_completed npc=%s action=%s object=%s "
            "success=%d message=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *PendingAction.ActionId.ToString(),
        *PendingObjectId.ToString(),
        Result.bSuccess ? 1 : 0,
        *Result.Message
    );
    RefreshWorldState();
    OnWorldActionCompleted.Broadcast(Result);
}

void UNPCWorldStateAgentComponent::FinishPendingAction()
{
    if (!bActionInProgress)
    {
        return;
    }
    if (!bPendingEffectApplied)
    {
        ApplyPendingActionEffect();
    }
    const bool bFinishedActionSucceeded = bPendingEffectSucceeded;
    const FNPCWorldActionDefinition FinishedAction = PendingAction;
    AActor* CurrentHeldActor = HeldActor.Get();
    const bool bStartNewHeldIdle =
        FinishedAction.ActionId == TEXT("pickup") &&
        IsValid(CurrentHeldActor) &&
        CurrentHeldActor == PendingActionTarget.Get();
    const bool bResumeExistingHeldIdle =
        !bStartNewHeldIdle && IsValid(CurrentHeldActor);
    CancelPendingAction();
    if (bStartNewHeldIdle)
    {
        StartHeldIdleAnimation(FinishedAction, CurrentHeldActor);
    }
    else if (bResumeExistingHeldIdle)
    {
        ResumeHeldIdleAnimation();
    }
    if (bNaturalLanguageSequenceActive)
    {
        if (bFinishedActionSucceeded)
        {
            ContinueNaturalLanguageActionSequence();
        }
        else
        {
            FailNaturalLanguageActionSequence(
                TEXT("A queued world action failed during execution.")
            );
        }
    }
}

bool UNPCWorldStateAgentComponent::ExecuteWorldAction(
    FName ObjectId,
    FName ActionId,
    const FString& Parameters,
    FNPCWorldActionResult& OutResult
)
{
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION request npc=%s action=%s object=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *ActionId.ToString(),
        *ObjectId.ToString()
    );
    if (bActionInProgress)
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            ActionId,
            TEXT("The NPC is already performing another world action.")
        );
        OnWorldActionCompleted.Broadcast(OutResult);
        return false;
    }

    UNPCWorldStateSubsystem* Subsystem = GetWorld()
        ? GetWorld()->GetSubsystem<UNPCWorldStateSubsystem>()
        : nullptr;
    UNPCWorldStateObjectComponent* ObjectComponent = nullptr;
    if (ActionId == TEXT("drop") || ActionId == TEXT("throw"))
    {
        AActor* Held = HeldActor.Get();
        ObjectComponent = Held
            ? Held->FindComponentByClass<UNPCWorldStateObjectComponent>()
            : nullptr;
        if (ObjectComponent)
        {
            ObjectId = ObjectComponent->GetResolvedObjectId();
        }
    }
    else
    {
        ObjectComponent = Subsystem
            ? Subsystem->FindWorldObject(ObjectId)
            : nullptr;
    }
    AActor* Target = ObjectComponent ? ObjectComponent->GetOwner() : nullptr;
    if (!IsValid(Target))
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            ActionId,
            TEXT("The requested world object was not found.")
        );
        OnWorldActionCompleted.Broadcast(OutResult);
        return false;
    }

    const TArray<FNPCWorldActionDefinition> AvailableActions =
        ObjectComponent->GetAvailableActions(GetOwner());
    const FNPCWorldActionDefinition* Definition =
        AvailableActions.FindByPredicate(
            [ActionId](const FNPCWorldActionDefinition& Candidate)
            {
                return Candidate.ActionId == ActionId;
            }
        );
    if (!Definition)
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            ActionId,
            TEXT("That action is not available for this object right now.")
        );
        OnWorldActionCompleted.Broadcast(OutResult);
        return false;
    }

    if (ActionId == TEXT("pickup") && HeldActor.IsValid())
    {
        OutResult = MakeResult(
            false,
            ObjectId,
            ActionId,
            TEXT("The NPC is already holding another object.")
        );
        OnWorldActionCompleted.Broadcast(OutResult);
        return false;
    }

    if (ActionId == TEXT("pickup") && bApproachPickupTargets && GetOwner())
    {
        const float Distance = FVector::Dist2D(
            GetOwner()->GetActorLocation(),
            GetTargetInteractionLocation(Target)
        );
        if (Distance > GetPickupReachDistance(*Definition))
        {
            const bool bStartedApproach = StartApproachForAction(
                *Definition,
                Target,
                ObjectId,
                Parameters,
                OutResult
            );
            if (!bStartedApproach)
            {
                OnWorldActionCompleted.Broadcast(OutResult);
            }
            return bStartedApproach;
        }
    }

    if (StartAnimatedAction(
        *Definition,
        Target,
        ObjectId,
        Parameters,
        OutResult
    ))
    {
        return true;
    }

    const bool bSuccess = ExecuteActionEffect(
        *Definition,
        Target,
        ObjectId,
        Parameters,
        OutResult
    );
    RefreshWorldState();
    OnWorldActionCompleted.Broadcast(OutResult);
    if (bSuccess && Definition->ActionId == TEXT("pickup") &&
        HeldActor.Get() == Target)
    {
        StartHeldIdleAnimation(*Definition, Target);
    }
    return bSuccess;
}

bool UNPCWorldStateAgentComponent::TextContainsAny(
    const FString& Text,
    const TArray<FString>& Needles
) const
{
    for (const FString& Needle : Needles)
    {
        if (!Needle.IsEmpty() && Text.Contains(Needle, ESearchCase::IgnoreCase))
        {
            return true;
        }
    }
    return false;
}

TArray<FString>
UNPCWorldStateAgentComponent::SplitNaturalLanguageActionClauses(
    const FString& Command
) const
{
    FString Working = Command.TrimStartAndEnd();
    for (const FString& Separator : {
        FString(TEXT(" and then ")),
        FString(TEXT(" followed by ")),
        FString(TEXT(" then ")),
        FString(TEXT(" and ")),
        FString(TEXT(";")),
        FString(TEXT(",")),
        FString(TEXT("\u7136\u540e")),
        FString(TEXT("\u63a5\u7740")),
        FString(TEXT("\u5e76\u4e14")),
        FString(TEXT("\u518d"))})
    {
        Working.ReplaceInline(
            *Separator,
            TEXT("|"),
            ESearchCase::IgnoreCase
        );
    }

    TArray<FString> RawClauses;
    Working.ParseIntoArray(RawClauses, TEXT("|"), true);
    TArray<FString> Clauses;
    for (FString& RawClause : RawClauses)
    {
        RawClause = RawClause.TrimStartAndEnd();
        if (!RawClause.IsEmpty())
        {
            Clauses.Add(MoveTemp(RawClause));
        }
    }
    return Clauses;
}

void UNPCWorldStateAgentComponent::FailNaturalLanguageActionSequence(
    const FString& Reason
)
{
    if (!bNaturalLanguageSequenceActive &&
        QueuedNaturalLanguageActions.IsEmpty())
    {
        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("NPC_WORLD_ACTION sequence_failed npc=%s remaining=%d "
            "reason=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        QueuedNaturalLanguageActions.Num(),
        *Reason
    );
    QueuedNaturalLanguageActions.Reset();
    bNaturalLanguageSequenceActive = false;
    bLastNaturalLanguageSequenceSucceeded = false;
}

bool UNPCWorldStateAgentComponent::ContinueNaturalLanguageActionSequence(
    FNPCWorldActionResult* OutFirstResult
)
{
    bool bWroteFirstResult = false;
    while (bNaturalLanguageSequenceActive &&
        !QueuedNaturalLanguageActions.IsEmpty())
    {
        const FQueuedNaturalLanguageAction Step =
            QueuedNaturalLanguageActions[0];
        QueuedNaturalLanguageActions.RemoveAt(
            0,
            1,
            EAllowShrinking::No
        );

        FNPCWorldActionResult Result;
        const bool bStarted = ExecuteWorldAction(
            Step.ObjectId,
            Step.ActionId,
            Step.Parameters,
            Result
        );
        if (OutFirstResult && !bWroteFirstResult)
        {
            *OutFirstResult = Result;
            bWroteFirstResult = true;
        }

        UE_LOG(
            LogTemp,
            Display,
            TEXT("NPC_WORLD_ACTION sequence_step npc=%s action=%s "
                "object=%s success=%d remaining=%d"),
            GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
            *Step.ActionId.ToString(),
            *Step.ObjectId.ToString(),
            bStarted ? 1 : 0,
            QueuedNaturalLanguageActions.Num()
        );
        if (!bStarted)
        {
            FailNaturalLanguageActionSequence(Result.Message);
            return false;
        }
        if (bActionInProgress)
        {
            return true;
        }
    }

    if (bNaturalLanguageSequenceActive)
    {
        bNaturalLanguageSequenceActive = false;
        bLastNaturalLanguageSequenceSucceeded = true;
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NPC_WORLD_ACTION sequence_completed npc=%s"),
            GetOwner() ? *GetOwner()->GetName() : TEXT("None")
        );
    }
    return bLastNaturalLanguageSequenceSucceeded;
}

FName UNPCWorldStateAgentComponent::ResolveActionIntent(
    const FString& Command
) const
{
    if (TextContainsAny(Command, {
        TEXT("pick up"), TEXT("pick"), TEXT("grab"), TEXT("take"),
        TEXT("hold"), TEXT("fetch"),
        TEXT("\u62ff\u8d77"), TEXT("\u6361\u8d77"), TEXT("\u62fe\u53d6"),
        TEXT("\u62ff")
    }))
    {
        return TEXT("pickup");
    }
    if (TextContainsAny(Command, {
        TEXT("put down"), TEXT("drop"), TEXT("release"),
        TEXT("\u653e\u4e0b"), TEXT("\u4e22\u4e0b")
    }))
    {
        return TEXT("drop");
    }
    if (TextContainsAny(Command, {
        TEXT("throw"), TEXT("toss"), TEXT("\u6254"), TEXT("\u4e22")
    }))
    {
        return TEXT("throw");
    }
    if (TextContainsAny(Command, {
        TEXT("inspect"), TEXT("look at"), TEXT("check"),
        TEXT("\u67e5\u770b"), TEXT("\u68c0\u67e5")
    }))
    {
        return TEXT("inspect");
    }

    for (const FNPCWorldObjectState& Snapshot : GetVisibleWorldObjects())
    {
        for (const FNPCWorldActionDefinition& Action : Snapshot.AvailableActions)
        {
            TArray<FString> Hints = Action.UtteranceHints;
            Hints.Add(Action.ActionId.ToString());
            Hints.Add(Action.DisplayName.ToString());
            if (TextContainsAny(Command, Hints))
            {
                return Action.ActionId;
            }
        }
    }
    return NAME_None;
}

FName UNPCWorldStateAgentComponent::ResolveTargetIntent(
    const FString& Command,
    FName ActionId
) const
{
    const TArray<FNPCWorldObjectState> Snapshots = GetVisibleWorldObjects();
    const auto SupportsAction = [ActionId](
        const FNPCWorldObjectState& Snapshot
    )
    {
        return ActionId.IsNone() ||
            Snapshot.AvailableActions.ContainsByPredicate(
                [ActionId](const FNPCWorldActionDefinition& Action)
                {
                    return Action.ActionId == ActionId;
                }
            );
    };
    const bool bChooseLeft = TextContainsAny(Command, {
        TEXT("left"), TEXT("\u5de6\u8fb9"), TEXT("\u5de6\u4fa7"),
        TEXT("\u5de6\u624b")
    });
    const bool bChooseRight = !bChooseLeft && TextContainsAny(Command, {
        TEXT("right"), TEXT("\u53f3\u8fb9"), TEXT("\u53f3\u4fa7"),
        TEXT("\u53f3\u624b")
    });
    const auto ChooseCandidate = [this, bChooseLeft, bChooseRight](
        const TArray<const FNPCWorldObjectState*>& Candidates
    ) -> FName
    {
        if (Candidates.IsEmpty())
        {
            return NAME_None;
        }
        if ((!bChooseLeft && !bChooseRight) || !GetOwner())
        {
            return Candidates[0]->ObjectId;
        }

        const FVector OwnerLocation = GetOwner()->GetActorLocation();
        const FVector RightVector = GetOwner()->GetActorRightVector();
        const FNPCWorldObjectState* Best = Candidates[0];
        float BestSide = FVector::DotProduct(
            Best->Location - OwnerLocation,
            RightVector
        );
        for (int32 Index = 1; Index < Candidates.Num(); ++Index)
        {
            const float Side = FVector::DotProduct(
                Candidates[Index]->Location - OwnerLocation,
                RightVector
            );
            if ((bChooseLeft && Side < BestSide) ||
                (bChooseRight && Side > BestSide))
            {
                Best = Candidates[Index];
                BestSide = Side;
            }
        }
        return Best->ObjectId;
    };

    TArray<const FNPCWorldObjectState*> NamedCandidates;
    for (const FNPCWorldObjectState& Snapshot : Snapshots)
    {
        if (!SupportsAction(Snapshot))
        {
            continue;
        }
        const UNPCWorldStateObjectComponent* Component = Snapshot.Actor
            ? Snapshot.Actor->FindComponentByClass<UNPCWorldStateObjectComponent>()
            : nullptr;
        TArray<FString> Names = {
            Snapshot.ObjectId.ToString(),
            Snapshot.DisplayName.ToString(),
            Snapshot.Category.ToString()
        };
        if (Component)
        {
            Names.Append(Component->Aliases);
        }
        if (TextContainsAny(Command, Names))
        {
            NamedCandidates.Add(&Snapshot);
        }
    }
    if (!NamedCandidates.IsEmpty())
    {
        return ChooseCandidate(NamedCandidates);
    }

    const FString TrimmedCommand = Command.TrimStartAndEnd();
    const bool bUsesPronoun = TrimmedCommand.Equals(
        TEXT("it"),
        ESearchCase::IgnoreCase
    ) || TextContainsAny(Command, {
        TEXT(" it"), TEXT("that one"), TEXT("this one"),
        TEXT("\u5b83"), TEXT("\u90a3\u4e2a"), TEXT("\u8fd9\u4e2a"),
        TEXT("\u90a3\u4e00\u4e2a"), TEXT("\u8fd9\u4e00\u4e2a")
    });
    if (bUsesPronoun && !LastMentionedObjectId.IsNone())
    {
        const FNPCWorldObjectState* Mentioned = Snapshots.FindByPredicate(
            [this](const FNPCWorldObjectState& Snapshot)
            {
                return Snapshot.ObjectId == LastMentionedObjectId;
            }
        );
        if (Mentioned && SupportsAction(*Mentioned))
        {
            return Mentioned->ObjectId;
        }
    }

    if (bChooseLeft || bChooseRight)
    {
        TArray<const FNPCWorldObjectState*> DirectionalCandidates;
        for (const FNPCWorldObjectState& Snapshot : Snapshots)
        {
            if (SupportsAction(Snapshot))
            {
                DirectionalCandidates.Add(&Snapshot);
            }
        }
        return ChooseCandidate(DirectionalCandidates);
    }

    if (ActionId.IsNone())
    {
        return NAME_None;
    }

    for (const FNPCWorldObjectState& Snapshot : Snapshots)
    {
        if (SupportsAction(Snapshot))
        {
            return Snapshot.ObjectId;
        }
    }
    return NAME_None;
}

bool UNPCWorldStateAgentComponent::TryExecuteNaturalLanguageAction(
    const FString& Command,
    FString& OutReply
)
{
    FString IgnoredRemainingCommand;
    return TryExecuteNaturalLanguageActionDetailed(
        Command,
        OutReply,
        IgnoredRemainingCommand
    );
}

bool UNPCWorldStateAgentComponent::TryExecuteNaturalLanguageActionDetailed(
    const FString& Command,
    FString& OutReply,
    FString& OutRemainingCommand
)
{
    OutReply.Reset();
    OutRemainingCommand.Reset();

    TArray<FQueuedNaturalLanguageAction> ParsedActions;
    TArray<FString> RemainingClauses;
    for (const FString& Clause : SplitNaturalLanguageActionClauses(Command))
    {
        const FName ActionId = ResolveActionIntent(Clause);
        if (ActionId.IsNone())
        {
            RemainingClauses.Add(Clause);
            continue;
        }

        FName TargetId = NAME_None;
        if (ActionId == TEXT("drop") || ActionId == TEXT("throw"))
        {
            AActor* Target = HeldActor.Get();
            if (!IsValid(Target))
            {
                OutReply = TEXT("The NPC is not holding anything.");
                return true;
            }
            TargetId = Target->GetFName();
            if (const UNPCWorldStateObjectComponent* ObjectComponent =
                    Target->FindComponentByClass<
                        UNPCWorldStateObjectComponent>())
            {
                TargetId = ObjectComponent->GetResolvedObjectId();
            }
        }
        else
        {
            TargetId = ResolveTargetIntent(Clause, ActionId);
        }

        if (TargetId.IsNone())
        {
            OutReply = TEXT(
                "I cannot find a nearby object that supports that action."
            );
            return true;
        }

        FQueuedNaturalLanguageAction& Parsed =
            ParsedActions.AddDefaulted_GetRef();
        Parsed.ObjectId = TargetId;
        Parsed.ActionId = ActionId;
        LastMentionedObjectId = TargetId;
    }

    OutRemainingCommand = FString::Join(
        RemainingClauses,
        TEXT(" then ")
    );
    if (ParsedActions.IsEmpty())
    {
        const FName MentionedTarget = ResolveTargetIntent(Command, NAME_None);
        if (!MentionedTarget.IsNone())
        {
            LastMentionedObjectId = MentionedTarget;
            UE_LOG(
                LogTemp,
                Display,
                TEXT("NPC_WORLD_ACTION context_target npc=%s object=%s "
                    "command=%s"),
                GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
                *MentionedTarget.ToString(),
                *Command
            );
        }
        OutRemainingCommand.Reset();
        return false;
    }

    if (bNaturalLanguageSequenceActive || bActionInProgress)
    {
        OutReply = TEXT(
            "The NPC is already performing another world action."
        );
        return true;
    }

    QueuedNaturalLanguageActions = MoveTemp(ParsedActions);
    bNaturalLanguageSequenceActive = true;
    bLastNaturalLanguageSequenceSucceeded = false;
    const int32 StepCount = QueuedNaturalLanguageActions.Num();
    FNPCWorldActionResult FirstResult;
    const bool bStarted = ContinueNaturalLanguageActionSequence(&FirstResult);
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION natural_language_sequence npc=%s command=%s "
            "steps=%d started=%d remaining_command=%s reply=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *Command,
        StepCount,
        bStarted ? 1 : 0,
        *OutRemainingCommand,
        *FirstResult.Message
    );
    OutReply = bStarted ? FString() : FirstResult.Message;
    return true;
}
