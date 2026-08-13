#pragma once

#include "ConversationGovernanceTypes.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OpenAINPCConversationSubsystem.generated.h"

class AActor;
class UConversationGovernanceComponent;
class UConversationListenerComponent;
class UOpenAIJackComponent;

USTRUCT(BlueprintType)
struct OPENAIJACKNPC_API FOpenAINPCEmergencyEventState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Local AI|Emergency")
    FName EventID = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "Local AI|Emergency")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Local AI|Emergency")
    TWeakObjectPtr<AActor> SourceActor;

    UPROPERTY(BlueprintReadOnly, Category = "Local AI|Emergency")
    float Severity = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Local AI|Emergency")
    bool bActive = false;
};

UCLASS(BlueprintType)
class OPENAIJACKNPC_API UOpenAINPCConversationSubsystem
    : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

    void RegisterNPC(UOpenAIJackComponent* Component);
    void UnregisterNPC(UOpenAIJackComponent* Component);
    bool CanAcceptPlayerText(
        const UOpenAIJackComponent* TargetComponent
    ) const;

    bool IsSpeechInputCoordinator(
        const UOpenAIJackComponent* Component
    ) const;
    bool RouteRecognizedPlayerText(
        UOpenAIJackComponent* CaptureComponent,
        const FString& PlayerText
    );
    UOpenAIJackComponent* ResolveRecognizedPlayerTarget(
        UOpenAIJackComponent* CaptureComponent,
        const FString& PlayerText,
        FString& OutReason
    ) const;
    bool RouteRecognizedPlayerAction(
        UOpenAIJackComponent* CaptureComponent,
        const FString& PlayerText,
        FString& OutReply
    );

    void NotifyPlayerTextSubmitted(
        UOpenAIJackComponent* PrimaryComponent,
        const FString& PlayerText
    );
    void NotifyNPCReplyReady(
        UOpenAIJackComponent* SourceComponent,
        const FString& ReplyText
    );

    UFUNCTION(BlueprintCallable, Category = "Local AI|Emergency")
    void ReportEmergencyEvent(
        FName EventID,
        const FString& Description,
        AActor* SourceActor,
        float Severity = 1.0f
    );

    UFUNCTION(BlueprintCallable, Category = "Local AI|Emergency")
    void ClearEmergencyEvent(FName EventID);

    UFUNCTION(BlueprintPure, Category = "Local AI|Emergency")
    FOpenAINPCEmergencyEventState GetEmergencyEventState() const
    {
        return EmergencyEvent;
    }

    UFUNCTION(BlueprintCallable, Category = "Local AI|Permission")
    bool GrantPendingSpeakingPermission();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Permission")
    bool DenyPendingSpeakingPermission();

    UFUNCTION(BlueprintPure, Category = "Local AI|Permission")
    bool HasPendingSpeakingPermission() const;

    bool HandlePermissionPlayerText(const FString& PlayerText);

    static bool IsWithinConversationContextRadius(
        const FVector& SourceLocation,
        const FVector& ListenerLocation,
        float Radius
    );

private:
    enum class ESecondaryExecutionMode : uint8
    {
        None,
        MainResponse,
        SideComment,
        PermissionPrompt,
        Emergency
    };

    struct FRegisteredNPC
    {
        TWeakObjectPtr<UOpenAIJackComponent> Component;
        TWeakObjectPtr<UConversationListenerComponent> Listener;
    };

    struct FExchangeRecord
    {
        FName PrimaryNPCID = NAME_None;
        FString PlayerText;
        FString PrimaryReply;
        FName SecondaryNPCID = NAME_None;
        FString SecondaryReply;
        TSet<FName> ContextParticipantNPCIDs;
    };

    void EnsureCoordinator();
    UConversationListenerComponent* EnsureListener(
        UOpenAIJackComponent* Component
    );
    FRegisteredNPC* FindRegisteredNPC(
        const UOpenAIJackComponent* Component
    );
    UOpenAIJackComponent* FindComponentForActor(
        const AActor* Actor
    ) const;
    void RemoveInvalidNPCs();
    UOpenAIJackComponent* ResolvePlayerTextTarget(
        const FString& PlayerText,
        FString& OutReason
    ) const;

    void BeginCandidateGeneration();
    void RequestCandidateFrom(UOpenAIJackComponent* ListenerComponent);
    FString BuildCandidateContext(
        const UOpenAIJackComponent* ListenerComponent
    ) const;
    void HandleCandidatePayload(
        TWeakObjectPtr<UOpenAIJackComponent> ListenerComponent,
        int32 RequestExchangeSerial,
        FName RequestConversationID,
        const FString& Payload,
        const FString& RequestFailure
    );

    void TickConversation();
    void TryExecuteNextResponse();
    bool TryExecuteEmergencyResponse(FGuid ResponseID);
    bool StartResponseExecution(
        const FCandidateNPCResponse& Response,
        ESecondaryExecutionMode Mode,
        const FString& SpokenText
    );
    void ReleaseExecutionTurn(
        AActor* NPCActor,
        ESecondaryExecutionMode Mode
    );
    void FinishActiveSecondaryResponse();
    void SynchronizePrimaryExchangeContext();
    void ArchiveCurrentExchange();
    void DiscardRemainingResponses(
        const FString& Reason,
        FGuid ExcludedResponseID = FGuid()
    );
    void EmitAutonomyLog(
        const FString& EventName,
        const FString& Details
    ) const;
    double GetCurrentTimeSeconds() const;

    UPROPERTY(Transient)
    TObjectPtr<AActor> CoordinatorActor;

    UPROPERTY(Transient)
    TObjectPtr<UConversationGovernanceComponent> Governance;

    TArray<FRegisteredNPC> RegisteredNPCs;
    TArray<FExchangeRecord> RecentExchanges;
    TArray<TWeakObjectPtr<UOpenAIJackComponent>>
        SynchronizedContextListeners;
    FOpenAINPCEmergencyEventState EmergencyEvent;
    TSet<FString> DeliveredEmergencyWarnings;

    TWeakObjectPtr<UOpenAIJackComponent> ActivePrimary;
    TWeakObjectPtr<UOpenAIJackComponent> ActiveSecondary;
    FName ActiveConversationID = NAME_None;
    FString CurrentPlayerText;
    FString CurrentPrimaryReply;
    FString CurrentSecondaryReply;
    FName CurrentSecondaryNPCID = NAME_None;
    FCandidateNPCResponse ActiveResponse;
    ESecondaryExecutionMode ActiveExecutionMode =
        ESecondaryExecutionMode::None;
    int32 ExchangeSerial = 0;
    int32 PendingCandidateRequests = 0;
    int32 ExecutedResponsesThisExchange = 0;
    bool bWaitingForPrimaryReply = false;
    bool bPrimaryTurnStarted = false;
    bool bPrimaryTurnEnded = false;
    bool bExchangeArchived = false;
    double PrimaryReplyReadyTime = 0.0;
    double NaturalPauseReadyTime = 0.0;
    double SecondarySpeechStartTime = 0.0;
    double PermissionResponseDeadline = 0.0;
    FTimerHandle ConversationTickTimer;
};
