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
#include "TimerManager.h"

UNPCWorldStateAgentComponent::UNPCWorldStateAgentComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    const TSoftObjectPtr<UAnimSequence> PistolPickup(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_MM_Pistol_Equip.Human2_MM_Pistol_Equip")
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
    const TSoftObjectPtr<UAnimSequence> TwoHandHeldIdle(
        FSoftObjectPath(
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/")
        TEXT("Human2_Held_Idle_TwoHand.Human2_Held_Idle_TwoHand")
        )
    );

    DefaultPickupAnimation = TwoHandPickup;
    DefaultDropAnimation = TwoHandPickup;
    DefaultHeldIdleAnimation = TwoHandHeldIdle;

    FNPCWorldItemAnimationProfile PistolProfile;
    PistolProfile.Category = TEXT("pistol");
    PistolProfile.PickupAnimation = PistolPickup;
    PistolProfile.HeldIdleAnimation = PistolHeldIdle;
    PistolProfile.DropAnimation = PistolPickup;
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
    bApproachingActionTarget = false;
    PendingAction = FNPCWorldActionDefinition();
    PendingActionTarget.Reset();
    PendingObjectId = NAME_None;
    PendingParameters.Reset();
    ApproachPathPoints.Reset();
    ApproachPathPointIndex = 0;
    ApproachElapsedSeconds = 0.0f;
    ApproachMovementZ = 0.0f;
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
    FVector Origin = Target->GetActorLocation();
    FVector Extent = FVector::ZeroVector;
    Target->GetActorBounds(false, Origin, Extent);
    return Origin;
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
    TArray<FActionAnimationMeshState>& SavedStates
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
        Mesh->PlayAnimation(Animation, bLooping);

        UAnimSingleNodeInstance* SingleNode = Mesh->GetSingleNodeInstance();
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

    const float PlayRate = FMath::Max(
        0.01f,
        FMath::Abs(Action.AnimationPlayRate)
    );

    const bool bResumeHeldIdleOnFailure =
        !HeldAnimationMeshStates.IsEmpty() && HeldActor.IsValid();
    RestoreHeldAnimationState();
    if (!PlaySynchronizedAnimation(
        Animation,
        PrimaryMesh,
        false,
        PlayRate,
        Action.bPlayAnimationInReverse,
        ActionAnimationMeshStates
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

    const float Duration = FMath::Max(
        0.01f,
        Animation->GetPlayLength() / PlayRate
    );
    float EffectTriggerNormalizedTime = Action.EffectTriggerNormalizedTime;
    if (Action.ActionId == TEXT("pickup") &&
        Action.ActionAnimation.IsNull())
    {
        if (const FNPCWorldItemAnimationProfile* Profile =
            FindItemAnimationProfile(Target))
        {
            if (Profile->PickupEffectTriggerNormalizedTime >= 0.0f)
            {
                EffectTriggerNormalizedTime =
                    Profile->PickupEffectTriggerNormalizedTime;
            }
        }
    }
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
    const FVector FromOwner = (TargetLocation - OwnerLocation).GetSafeNormal2D();
    FVector ApproachLocation = TargetLocation -
        FromOwner * FMath::Max(40.0f, ReachDistance * 0.75f);
    ApproachMovementZ = OwnerLocation.Z;

    ApproachPathPoints.Reset();
    bool bUsingNavigationPath = false;
    if (UNavigationPath* Path =
        UNavigationSystemV1::FindPathToLocationSynchronously(
            World,
            OwnerLocation,
            ApproachLocation,
            nullptr
        ); Path && Path->IsValid() && Path->PathPoints.Num() > 1)
    {
        ApproachPathPoints = Path->PathPoints;
        bUsingNavigationPath = true;
    }
    else
    {
        ApproachPathPoints = {OwnerLocation, ApproachLocation};
    }
    for (FVector& Point : ApproachPathPoints)
    {
        Point.Z = ApproachMovementZ;
    }

    PendingAction = Action;
    PendingActionTarget = Target;
    PendingObjectId = ObjectId;
    PendingParameters = Parameters;
    bActionInProgress = true;
    bPendingEffectApplied = false;
    bApproachingActionTarget = true;
    ApproachPathPointIndex = 0;
    ApproachElapsedSeconds = 0.0f;
    ActiveApproachAnimation.Reset();

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
        bUsingNavigationPath ? 1 : 0
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

    while (ApproachPathPointIndex < ApproachPathPoints.Num() &&
        FVector::Dist2D(
            OwnerLocation,
            ApproachPathPoints[ApproachPathPointIndex]
        ) <= 15.0f)
    {
        ++ApproachPathPointIndex;
    }

    FVector MoveTarget = TargetLocation;
    MoveTarget.Z = ApproachMovementZ;
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
    NewLocation.Z = ApproachMovementZ;
    Owner->SetActorLocation(NewLocation, true);

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
    const FName ActionId = ResolveActionIntent(Command);
    if (ActionId.IsNone())
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
        return false;
    }

    if ((ActionId == TEXT("drop") || ActionId == TEXT("throw")) &&
        HeldActor.IsValid())
    {
        AActor* Target = HeldActor.Get();
        FName TargetId = Target ? Target->GetFName() : NAME_None;
        if (const UNPCWorldStateObjectComponent* ObjectComponent = Target
            ? Target->FindComponentByClass<UNPCWorldStateObjectComponent>()
            : nullptr)
        {
            TargetId = ObjectComponent->GetResolvedObjectId();
        }
        FNPCWorldActionResult Result;
        ExecuteWorldAction(TargetId, ActionId, FString(), Result);
        OutReply = Result.Message;
        return true;
    }

    const FName TargetId = ResolveTargetIntent(Command, ActionId);
    if (TargetId.IsNone())
    {
        OutReply = TEXT("I cannot find a nearby object that supports that action.");
        return true;
    }
    LastMentionedObjectId = TargetId;

    FNPCWorldActionResult Result;
    ExecuteWorldAction(TargetId, ActionId, FString(), Result);
    UE_LOG(
        LogTemp,
        Display,
        TEXT("NPC_WORLD_ACTION natural_language npc=%s command=%s action=%s "
            "object=%s success=%d reply=%s"),
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"),
        *Command,
        *ActionId.ToString(),
        *TargetId.ToString(),
        Result.bSuccess ? 1 : 0,
        *Result.Message
    );
    OutReply = Result.Message;
    return true;
}
