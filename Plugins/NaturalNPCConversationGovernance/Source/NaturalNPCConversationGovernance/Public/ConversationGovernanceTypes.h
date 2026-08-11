#pragma once

#include "CoreMinimal.h"
#include "ConversationGovernanceTypes.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EConversationResponseType : uint8
{
    Emergency,
    RelevantContribution,
    SideComment,
    AssociativeTopic,
    Silence,
    Invalid
};

UENUM(BlueprintType)
enum class EConversationTurnState : uint8
{
    Idle,
    PlayerSpeaking,
    PrimaryNPCSpeaking,
    SecondaryNPCSpeaking,
    PermissionPending,
    EmergencyInterrupt,
    TransitionPause
};

UENUM(BlueprintType)
enum class ENonVerbalReactionType : uint8
{
    None,
    LookAtSpeaker,
    LookAtPlayer,
    Nod,
    ShakeHead,
    Smile,
    Laugh,
    Surprise,
    Concern,
    Fear,
    RaiseHand,
    LeanForward
};

USTRUCT(BlueprintType)
struct NATURALNPCCONVERSATIONGOVERNANCE_API FCandidateNPCResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    FGuid ResponseID;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    FName ConversationID = NAME_None;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    FName NPCID = NAME_None;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    TWeakObjectPtr<AActor> NPCActor;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance",
        meta = (MultiLine = true))
    FString ResponseText;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    EConversationResponseType ResponseType =
        EConversationResponseType::Invalid;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float RelevanceScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float UrgencyScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float TopicShiftScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float TopicAffinityScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float ConversationImpactScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float ConfidenceScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    float PriorityScore = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    double CreatedTime = 0.0;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    double ExpiryTime = 0.0;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    bool bRequiresPermission = false;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    bool bCanOverlapMainTurn = false;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    bool bOwnsTurnWhenSpoken = true;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    bool bHasUniqueInformation = false;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    bool bTopicTransitionReady = false;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance")
    FName VerifiedEmergencyEventID = NAME_None;

    UPROPERTY(BlueprintReadWrite, Category = "Conversation Governance",
        meta = (MultiLine = true))
    FString PermissionRequestText;
};

USTRUCT(BlueprintType)
struct NATURALNPCCONVERSATIONGOVERNANCE_API FConversationRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    FName ConversationID = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    bool bIsActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    TWeakObjectPtr<AActor> PrimaryNPC;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    EConversationTurnState TurnState = EConversationTurnState::Idle;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    TWeakObjectPtr<AActor> CurrentTurnOwner;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    TWeakObjectPtr<AActor> CurrentSpeaker;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    double ConversationStartTime = 0.0;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    double ConversationEndTime = 0.0;
};

USTRUCT(BlueprintType)
struct NATURALNPCCONVERSATIONGOVERNANCE_API FConversationGovernanceDebugSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    FConversationRuntimeState RuntimeState;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    TArray<TObjectPtr<AActor>> EligibleListeners;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    TArray<FCandidateNPCResponse> QueuedResponses;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance")
    FString LastDecision;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FConversationListenerChangedEvent,
    AActor*,
    ListenerActor
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FConversationCandidateEvent,
    const FCandidateNPCResponse&,
    Response
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FConversationPermissionResolvedEvent,
    const FCandidateNPCResponse&,
    Response,
    bool,
    bGranted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FConversationNonVerbalReactionEvent,
    AActor*,
    NPCActor,
    ENonVerbalReactionType,
    ReactionType,
    float,
    Intensity
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FConversationResponseDiscardedEvent,
    const FCandidateNPCResponse&,
    Response,
    const FString&,
    Reason
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FConversationTurnOwnerChangedEvent,
    AActor*,
    PreviousOwner,
    AActor*,
    NewOwner,
    EConversationTurnState,
    NewState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FConversationGovernanceDebugEvent,
    const FString&,
    Message
);
