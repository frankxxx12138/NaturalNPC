#pragma once

#include "Components/ActorComponent.h"
#include "ConversationGovernanceTypes.h"
#include "ConversationGovernanceComponent.generated.h"

class UConversationListenerComponent;
class UConversationResponseQueue;
class UConversationTurnManager;

UCLASS(
    ClassGroup = (ConversationGovernance),
    meta = (BlueprintSpawnableComponent)
)
class NATURALNPCCONVERSATIONGOVERNANCE_API UConversationGovernanceComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UConversationGovernanceComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Queue",
        meta = (ClampMin = "1", ClampMax = "256"))
    int32 MaximumTotalQueueLength = 32;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Debug")
    bool bEnableDebugLogging = true;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationListenerChangedEvent OnListenerEnteredConversation;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationListenerChangedEvent OnListenerLeftConversation;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationCandidateEvent OnCandidateResponseGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationCandidateEvent OnResponseQueued;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationCandidateEvent OnSpeakingPermissionRequested;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationPermissionResolvedEvent OnSpeakingPermissionResolved;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationCandidateEvent OnResponseExecutionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationCandidateEvent OnResponseExecutionFinished;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationNonVerbalReactionEvent OnNonVerbalReactionRequested;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationResponseDiscardedEvent OnResponseDiscarded;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Events")
    FConversationTurnOwnerChangedEvent OnTurnOwnerChanged;

    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Debug")
    FConversationGovernanceDebugEvent OnGovernanceDebugMessage;

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance")
    bool StartConversation(FName ConversationID, AActor* PrimaryNPC);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance")
    void EndConversation();

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Listener")
    bool RegisterListener(UConversationListenerComponent* Listener);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Listener")
    bool UnregisterListener(UConversationListenerComponent* Listener);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Listener")
    void RefreshListenerEligibility();

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Listener")
    bool IsListenerEligible(
        const UConversationListenerComponent* Listener
    ) const;

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Candidate")
    bool SubmitCandidateResponse(
        AActor* NPCActor,
        const FString& ResponseText,
        EConversationResponseType ResponseType,
        float PriorityScore,
        FGuid& OutResponseID
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Candidate")
    bool SubmitCandidateResponseData(
        const FCandidateNPCResponse& Candidate,
        FGuid& OutResponseID
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool NotifyPlayerSpeechStarted(AActor* PlayerActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool NotifyPlayerSpeechEnded(AActor* PlayerActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool NotifyNPCSpeechStarted(AActor* NPCActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool NotifyNPCSpeechEnded(AActor* NPCActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool BeginSideComment(AActor* NPCActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool EndSideComment(AActor* NPCActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool BeginEmergencyInterrupt(AActor* NPCActor);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Permission")
    bool RequestSpeakingPermission(FGuid ResponseID);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Permission")
    bool GrantPermission(
        FGuid ResponseID,
        FCandidateNPCResponse& OutApprovedResponse
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Permission")
    bool DenyPermission(FGuid ResponseID);

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Permission")
    bool HasPendingPermissionRequest() const
    {
        return bHasPendingPermissionResponse;
    }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Permission")
    FCandidateNPCResponse GetPendingPermissionResponse() const
    {
        return PendingPermissionResponse;
    }

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Execution")
    void NotifyResponseExecutionStarted(
        const FCandidateNPCResponse& Response
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Execution")
    void NotifyResponseExecutionFinished(
        const FCandidateNPCResponse& Response
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Reaction")
    void RequestNonVerbalReaction(
        AActor* NPCActor,
        ENonVerbalReactionType ReactionType,
        float Intensity = 1.0f
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    bool CancelResponse(FGuid ResponseID);

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    AActor* GetCurrentTurnOwner() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    EConversationTurnState GetTurnState() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    TArray<FCandidateNPCResponse> GetQueuedResponses() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance")
    FConversationRuntimeState GetRuntimeState() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Debug")
    FConversationGovernanceDebugSnapshot GetDebugSnapshot() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance")
    bool IsConversationActive() const { return RuntimeState.bIsActive; }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance")
    AActor* GetPrimaryNPC() const { return RuntimeState.PrimaryNPC.Get(); }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    UConversationTurnManager* GetTurnManager() const { return TurnManager; }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    UConversationResponseQueue* GetResponseQueue() const
    {
        return ResponseQueue;
    }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void EnsureRuntimeObjects();
    UConversationListenerComponent* FindListenerForActor(
        const AActor* Actor
    ) const;
    void DiscardResponsesFromNPC(FName NPCID, const FString& Reason);
    void BroadcastDiscard(
        const FCandidateNPCResponse& Response,
        const FString& Reason
    );
    void EmitDebug(
        const FString& EventName,
        const FString& Details
    );
    double GetCurrentTimeSeconds() const;

    UFUNCTION()
    void HandleTurnOwnerChanged(
        AActor* PreviousOwner,
        AActor* NewOwner,
        EConversationTurnState NewState
    );

    UPROPERTY(Transient)
    TObjectPtr<UConversationTurnManager> TurnManager;

    UPROPERTY(Transient)
    TObjectPtr<UConversationResponseQueue> ResponseQueue;

    UPROPERTY(Transient)
    FConversationRuntimeState RuntimeState;

    UPROPERTY(Transient)
    TArray<TWeakObjectPtr<UConversationListenerComponent>>
        RegisteredListeners;

    UPROPERTY(Transient)
    TArray<TWeakObjectPtr<UConversationListenerComponent>>
        EligibleListeners;

    UPROPERTY(Transient)
    FCandidateNPCResponse PendingPermissionResponse;

    bool bHasPendingPermissionResponse = false;
    FString LastDecision;
};
