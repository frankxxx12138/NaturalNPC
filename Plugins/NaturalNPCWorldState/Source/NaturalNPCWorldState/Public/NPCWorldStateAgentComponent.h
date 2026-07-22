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
        Category = "NPC World State|Approach")
    float PickupFacingYawOffsetDegrees = -90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "0.0"))
    float ThrowImpulse = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultPickupAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultDropAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DefaultHeldIdleAnimation;

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

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldStateUpdatedEvent OnWorldStateUpdated;

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldActionCompletedEvent OnWorldActionCompleted;

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    void RefreshWorldState();

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FString GetWorldStateJson() const { return CachedWorldStateJson; }

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

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    AActor* GetHeldActor() const { return HeldActor.Get(); }

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    bool IsWorldActionInProgress() const { return bActionInProgress; }

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    bool DropHeldActor(bool bThrow, FNPCWorldActionResult& OutResult);

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
        TArray<FActionAnimationMeshState>& SavedStates
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
    bool ResumeHeldIdleAnimation();
    void RestoreHeldAnimationState();
    void CancelPendingAction();
    FName ResolveActionIntent(const FString& Command) const;
    FName ResolveTargetIntent(const FString& Command, FName ActionId) const;
    bool TextContainsAny(
        const FString& Text,
        const TArray<FString>& Needles
    ) const;

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
    bool bApproachingActionTarget = false;
    FNPCWorldActionDefinition PendingAction;
    TWeakObjectPtr<AActor> PendingActionTarget;
    FName PendingObjectId = NAME_None;
    FString PendingParameters;

    TArray<FVector> ApproachPathPoints;
    int32 ApproachPathPointIndex = 0;
    float ApproachElapsedSeconds = 0.0f;
    float ApproachMovementZ = 0.0f;
    TWeakObjectPtr<UAnimSequence> ActiveApproachAnimation;
    TWeakObjectPtr<UAnimSequence> ActiveHeldIdleAnimation;
    FNPCWorldActionDefinition HeldIdleSourceAction;

    TArray<FActionAnimationMeshState> ActionAnimationMeshStates;
    TArray<FActionAnimationMeshState> HeldAnimationMeshStates;
};
