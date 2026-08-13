#pragma once

#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "NPCWorldStateTypes.h"
#include "TimerManager.h"
#include "NPCWorldStateAgentComponent.generated.h"

class UPrimitiveComponent;
class USceneComponent;
class USkeletalMeshComponent;
class UAnimInstance;
class UAnimSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FNPCWorldStateUpdatedEvent,
    const FString&,
    WorldStateJson
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FNPCWorldActionCompletedEvent,
    const FNPCWorldActionResult&,
    Result
);

UCLASS(ClassGroup = (NPCWorldState), meta = (BlueprintSpawnableComponent))
class NATURALNPCWORLDSTATE_API UNPCWorldStateAgentComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCWorldStateAgentComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "0.0"))
    float PerceptionRadius = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    bool bAutoRefresh = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "0.1"))
    float RefreshIntervalSeconds = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName HandSocket = TEXT("hand_r");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName SupportHandSocket = TEXT("hand_l");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FString HandMeshNameHint = TEXT("Body");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "20.0"))
    float DefaultPickupDistance = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach")
    bool bApproachPickupTargets = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "100.0"))
    float MaxPickupApproachDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "20.0"))
    float PickupApproachWalkSpeed = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "20.0"))
    float PickupApproachRunSpeed = 320.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "100.0"))
    float PickupApproachRunDistance = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "1.0"))
    float PickupApproachTimeoutSeconds = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "0.1"))
    float PickupApproachRotationSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "0.05", ClampMax = "2.0", Units = "s"))
    float PickupApproachRepathIntervalSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "0.05", ClampMax = "2.0", Units = "s"))
    float PickupApproachStallSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "4", ClampMax = "32"))
    int32 PickupApproachCandidateCount = 16;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach",
        meta = (ClampMin = "1", ClampMax = "10"))
    int32 MaximumPickupApproachRepathFailures = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach|Grounding",
        meta = (ClampMin = "1.0", ClampMax = "40.0"))
    float PickupApproachGroundFollowInterpSpeed = 16.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach|Grounding",
        meta = (ClampMin = "10.0", ClampMax = "100.0", Units = "cm"))
    float PickupApproachMaximumGroundStepUp = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach|Grounding",
        meta = (ClampMin = "20.0", ClampMax = "200.0", Units = "cm"))
    float PickupApproachMaximumGroundStepDown = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Approach")
    float PickupFacingYawOffsetDegrees = -90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "0.0"))
    float ThrowImpulse = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultPickupAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants")
    TSoftObjectPtr<UAnimSequence> GroundPickupAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants")
    TSoftObjectPtr<UAnimSequence> TablePickupAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants",
        meta = (ClampMin = "20.0", ClampMax = "140.0", Units = "cm"))
    float TablePickupMinimumHeight = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants",
        meta = (ClampMin = "0.01"))
    float GroundPickupAnimationPlayRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants",
        meta = (ClampMin = "0.01"))
    float TablePickupAnimationPlayRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants",
        meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float GroundPickupEffectTriggerNormalizedTime = 0.29f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Pickup Variants",
        meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float TablePickupEffectTriggerNormalizedTime = 0.41f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultDropAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultHeldIdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultHeldWalkAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultThrowAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TArray<FNPCWorldItemAnimationProfile> ItemAnimationProfiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> PickupApproachWalkAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> PickupApproachRunAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Adaptive Pickup")
    bool bEnableAdaptivePickupIK = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Adaptive Pickup",
        meta = (ClampMin = "0.05", ClampMax = "0.45"))
    float AdaptivePickupIKBlendWindow = 0.22f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Adaptive Pickup",
        meta = (ClampMin = "0.0", ClampMax = "0.75"))
    float AdaptivePickupPelvisInfluence = 0.30f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Adaptive Pickup",
        meta = (ClampMin = "0.0", ClampMax = "65.0", Units = "cm"))
    float AdaptivePickupMaxPelvisOffset = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation|Adaptive Pickup",
        meta = (ClampMin = "1.0", ClampMax = "1.2"))
    float AdaptivePickupMaxArmStretch = 1.08f;

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldStateUpdatedEvent OnWorldStateUpdated;

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldActionCompletedEvent OnWorldActionCompleted;

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    void RefreshWorldState();

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FString GetWorldStateJson() const { return CachedWorldStateJson; }

    UFUNCTION(BlueprintPure, Category = "NPC World State|Animation")
    UAnimSequence* GetPickupAnimationForTarget(AActor* Target) const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FString GetWorldStateText() const { return CachedWorldStateText; }

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    TArray<FNPCWorldObjectState> GetVisibleWorldObjects() const;

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    bool ExecuteWorldAction(
        FName ObjectId,
        FName ActionId,
        const FString& Parameters,
        FNPCWorldActionResult& OutResult
    );

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    bool TryExecuteNaturalLanguageAction(
        const FString& Command,
        FString& OutReply
    );

    bool TryExecuteNaturalLanguageActionDetailed(
        const FString& Command,
        FString& OutReply,
        FString& OutRemainingCommand
    );

    bool IsNaturalLanguageActionSequenceInProgress() const
    {
        return bNaturalLanguageSequenceActive || bActionInProgress ||
            !QueuedNaturalLanguageActions.IsEmpty();
    }

    bool WasLastNaturalLanguageActionSequenceSuccessful() const
    {
        return bLastNaturalLanguageSequenceSucceeded;
    }

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    AActor* GetHeldActor() const { return HeldActor.Get(); }

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    bool IsWorldActionInProgress() const { return bActionInProgress; }

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    bool DropHeldActor(bool bThrow, FNPCWorldActionResult& OutResult);

    UFUNCTION(BlueprintPure, Category = "NPC World State|Animation")
    UAnimSequence* GetHeldWalkAnimation() const;

    UFUNCTION(BlueprintPure, Category = "NPC World State|Animation")
    UAnimSequence* GetCurrentHeldIdleAnimation() const;

    UFUNCTION(BlueprintCallable, Category = "NPC World State|Animation")
    void SuspendHeldIdleAnimation();

    UFUNCTION(BlueprintCallable, Category = "NPC World State|Animation")
    bool ResumeHeldIdleAnimation();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:
    struct FHeldPrimitiveState
    {
        TWeakObjectPtr<UPrimitiveComponent> Component;
        bool bWasSimulatingPhysics = false;
        bool bWasGravityEnabled = false;
        ECollisionResponse PawnResponse = ECR_Block;
    };

    struct FActionAnimationMeshState
    {
        TWeakObjectPtr<USkeletalMeshComponent> Mesh;
        TSubclassOf<UAnimInstance> AnimClass;
        EAnimationMode::Type AnimationMode =
            EAnimationMode::AnimationBlueprint;
    };

    struct FQueuedNaturalLanguageAction
    {
        FName ObjectId = NAME_None;
        FName ActionId = NAME_None;
        FString Parameters;
    };

    FNPCWorldActionResult MakeResult(
        bool bSuccess,
        FName ObjectId,
        FName ActionId,
        const FString& Message
    ) const;
    USceneComponent* FindHandAnchor(
        FName SocketName,
        FName& OutResolvedSocket
    ) const;
    bool PickupActor(
        const FNPCWorldActionDefinition& Action,
        AActor* Target,
        FNPCWorldActionResult& OutResult
    );
    bool ExecuteCustomAction(
        AActor* Target,
        FName ActionId,
        const FString& Parameters,
        FNPCWorldActionResult& OutResult
    );
    bool ExecuteActionEffect(
        const FNPCWorldActionDefinition& Action,
        AActor* Target,
        FName ObjectId,
        const FString& Parameters,
        FNPCWorldActionResult& OutResult
    );
    UAnimSequence* ResolveActionAnimation(
        const FNPCWorldActionDefinition& Action,
        AActor* Target
    ) const;
    bool ShouldUseTablePickupAnimation(
        AActor* Target,
        float* OutHeightAboveGround = nullptr
    ) const;
    UAnimSequence* ResolveHeldIdleAnimation(
        const FNPCWorldActionDefinition& Action,
        AActor* Target
    ) const;
    const FNPCWorldItemAnimationProfile* FindItemAnimationProfile(
        AActor* Target
    ) const;
    USkeletalMeshComponent* FindActionAnimationMesh(
        const FNPCWorldActionDefinition& Action
    ) const;
    bool StartAnimatedAction(
        const FNPCWorldActionDefinition& Action,
        AActor* Target,
        FName ObjectId,
        const FString& Parameters,
        FNPCWorldActionResult& OutResult
    );
    bool PlaySynchronizedAnimation(
        UAnimSequence* Animation,
        USkeletalMeshComponent* PrimaryMesh,
        bool bLooping,
        float PlayRate,
        bool bReverse,
        TArray<FActionAnimationMeshState>& SavedStates,
        AActor* AdaptivePickupTarget = nullptr,
        float PickupContactNormalizedTime = 0.55f,
        bool bUseSupportHand = false,
        bool bAdjustPickupPelvisAndSpine = true
    );
    FVector GetTargetInteractionLocation(AActor* Target) const;
    float GetPickupReachDistance(
        const FNPCWorldActionDefinition& Action
    ) const;
    bool StartApproachForAction(
        const FNPCWorldActionDefinition& Action,
        AActor* Target,
        FName ObjectId,
        const FString& Parameters,
        FNPCWorldActionResult& OutResult
    );
    bool RebuildApproachPath(AActor* Target, float ReachDistance);
    bool FindApproachGroundHeight(
        const FVector& Location,
        float& OutGroundZ
    ) const;
    bool UpdateApproachGroundHeight(
        FVector& InOutLocation,
        float DeltaTime
    );
    void UpdateApproach(float DeltaTime);
    void UpdateApproachAnimation(bool bRun);
    void BeginPendingActionAtTarget();
    void FailPendingAction(const FString& Message);
    void ApplyPendingActionEffect();
    void FinishPendingAction();
    void RestoreAnimationState(
        TArray<FActionAnimationMeshState>& SavedStates
    );
    bool StartHeldIdleAnimation(
        const FNPCWorldActionDefinition& SourceAction,
        AActor* Target
    );
    void RestoreHeldAnimationState();
    void CancelPendingAction();
    FName ResolveActionIntent(const FString& Command) const;
    FName ResolveTargetIntent(const FString& Command, FName ActionId) const;
    bool TextContainsAny(
        const FString& Text,
        const TArray<FString>& Needles
    ) const;
    TArray<FString> SplitNaturalLanguageActionClauses(
        const FString& Command
    ) const;
    bool ContinueNaturalLanguageActionSequence(
        FNPCWorldActionResult* OutFirstResult = nullptr
    );
    void FailNaturalLanguageActionSequence(const FString& Reason);

    FString CachedWorldStateJson;
    FString CachedWorldStateText;
    FTimerHandle RefreshTimerHandle;
    FTimerHandle ActionEffectTimerHandle;
    FTimerHandle ActionFinishTimerHandle;
    TWeakObjectPtr<AActor> HeldActor;
    TArray<FHeldPrimitiveState> HeldPrimitiveStates;
    FName LastMentionedObjectId = NAME_None;

    bool bActionInProgress = false;
    bool bPendingEffectApplied = false;
    bool bPendingEffectSucceeded = false;
    bool bApproachingActionTarget = false;
    FNPCWorldActionDefinition PendingAction;
    TWeakObjectPtr<AActor> PendingActionTarget;
    FName PendingObjectId = NAME_None;
    FString PendingParameters;

    TArray<FVector> ApproachPathPoints;
    int32 ApproachPathPointIndex = 0;
    float ApproachElapsedSeconds = 0.0f;
    bool bApproachGroundOffsetInitialized = false;
    float ApproachActorGroundOffsetZ = 0.0f;
    float ApproachRepathElapsedSeconds = 0.0f;
    float ApproachStallElapsedSeconds = 0.0f;
    int32 ApproachRepathFailureCount = 0;
    FVector ApproachLastTargetLocation = FVector::ZeroVector;
    FVector ApproachGoalLocation = FVector::ZeroVector;
    TWeakObjectPtr<UAnimSequence> ActiveApproachAnimation;
    TWeakObjectPtr<UAnimSequence> ActiveHeldIdleAnimation;
    FNPCWorldActionDefinition HeldIdleSourceAction;

    TArray<FActionAnimationMeshState> ActionAnimationMeshStates;
    TArray<FActionAnimationMeshState> HeldAnimationMeshStates;
    TArray<FQueuedNaturalLanguageAction> QueuedNaturalLanguageActions;
    bool bNaturalLanguageSequenceActive = false;
    bool bLastNaturalLanguageSequenceSucceeded = false;
};
