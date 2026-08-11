#include "ConversationGovernanceComponent.h"

#include "ConversationListenerComponent.h"
#include "ConversationResponseQueue.h"
#include "ConversationTurnManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "HAL/PlatformTime.h"

DEFINE_LOG_CATEGORY_STATIC(
    LogNaturalNPCConversationGovernance,
    Log,
    All
);

UConversationGovernanceComponent::UConversationGovernanceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UConversationGovernanceComponent::BeginPlay()
{
    Super::BeginPlay();
    EnsureRuntimeObjects();
}

void UConversationGovernanceComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    EndConversation();
    Super::EndPlay(EndPlayReason);
}

bool UConversationGovernanceComponent::StartConversation(
    FName ConversationID,
    AActor* PrimaryNPC
)
{
    EnsureRuntimeObjects();

    if (RuntimeState.bIsActive || !IsValid(PrimaryNPC))
    {
        EmitDebug(
            TEXT("conversation_start_rejected"),
            FString::Printf(
                TEXT("reason=%s"),
                RuntimeState.bIsActive
                    ? TEXT("AlreadyActive")
                    : TEXT("InvalidPrimaryNPC")
            )
        );
        return false;
    }

    if (ConversationID.IsNone())
    {
        ConversationID = FName(
            *FGuid::NewGuid().ToString(EGuidFormats::Digits)
        );
    }

    RuntimeState = FConversationRuntimeState();
    RuntimeState.ConversationID = ConversationID;
    RuntimeState.bIsActive = true;
    RuntimeState.PrimaryNPC = PrimaryNPC;
    RuntimeState.ConversationStartTime = GetCurrentTimeSeconds();

    TurnManager->Initialize(ConversationID, PrimaryNPC);
    ResponseQueue->Initialize(MaximumTotalQueueLength);
    PendingPermissionResponse = FCandidateNPCResponse();
    bHasPendingPermissionResponse = false;

    LastDecision = TEXT("ConversationStarted");
    EmitDebug(
        TEXT("conversation_started"),
        FString::Printf(
            TEXT("primary=%s"),
            *PrimaryNPC->GetName()
        )
    );

    RefreshListenerEligibility();
    return true;
}

void UConversationGovernanceComponent::EndConversation()
{
    if (!RuntimeState.bIsActive)
    {
        return;
    }

    EnsureRuntimeObjects();

    const TArray<FCandidateNPCResponse> Pending =
        ResponseQueue->GetQueuedResponses();
    for (const FCandidateNPCResponse& Response : Pending)
    {
        BroadcastDiscard(Response, TEXT("ConversationEnded"));
    }
    if (bHasPendingPermissionResponse)
    {
        BroadcastDiscard(
            PendingPermissionResponse,
            TEXT("ConversationEnded")
        );
        PendingPermissionResponse = FCandidateNPCResponse();
        bHasPendingPermissionResponse = false;
    }

    for (const TWeakObjectPtr<UConversationListenerComponent>& ListenerPtr
        : EligibleListeners)
    {
        UConversationListenerComponent* Listener = ListenerPtr.Get();
        if (IsValid(Listener) && IsValid(Listener->GetOwner()))
        {
            OnListenerLeftConversation.Broadcast(Listener->GetOwner());
        }
    }

    EligibleListeners.Reset();
    ResponseQueue->Reset();
    TurnManager->Reset();

    RuntimeState.bIsActive = false;
    RuntimeState.ConversationEndTime = GetCurrentTimeSeconds();
    RuntimeState.TurnState = EConversationTurnState::Idle;
    RuntimeState.CurrentTurnOwner.Reset();
    RuntimeState.CurrentSpeaker.Reset();

    LastDecision = TEXT("ConversationEnded");
    EmitDebug(TEXT("conversation_ended"), TEXT("reason=ExplicitEnd"));
}

bool UConversationGovernanceComponent::RegisterListener(
    UConversationListenerComponent* Listener
)
{
    if (!IsValid(Listener) || !IsValid(Listener->GetOwner()))
    {
        return false;
    }

    RegisteredListeners.RemoveAll(
        [](const TWeakObjectPtr<UConversationListenerComponent>& Item)
        {
            return !Item.IsValid();
        }
    );

    const bool bWasAlreadyRegistered =
        RegisteredListeners.Contains(Listener);
    if (!bWasAlreadyRegistered)
    {
        RegisteredListeners.Add(Listener);
    }

    RefreshListenerEligibility();
    EmitDebug(
        TEXT("listener_registered"),
        FString::Printf(
            TEXT("npc=%s actor=%s duplicate=%d"),
            *Listener->GetResolvedNPCID().ToString(),
            *Listener->GetOwner()->GetName(),
            bWasAlreadyRegistered ? 1 : 0
        )
    );
    return !bWasAlreadyRegistered;
}

bool UConversationGovernanceComponent::UnregisterListener(
    UConversationListenerComponent* Listener
)
{
    if (!IsValid(Listener))
    {
        return false;
    }

    const bool bWasEligible = EligibleListeners.Remove(Listener) > 0;
    const bool bWasRegistered = RegisteredListeners.Remove(Listener) > 0;
    if (!bWasRegistered)
    {
        return false;
    }

    if (bWasEligible && IsValid(Listener->GetOwner()))
    {
        OnListenerLeftConversation.Broadcast(Listener->GetOwner());
    }

    DiscardResponsesFromNPC(
        Listener->GetResolvedNPCID(),
        TEXT("ListenerUnregistered")
    );
    LastDecision = TEXT("ListenerUnregistered");
    EmitDebug(
        TEXT("listener_unregistered"),
        FString::Printf(
            TEXT("npc=%s"),
            *Listener->GetResolvedNPCID().ToString()
        )
    );
    return true;
}

void UConversationGovernanceComponent::RefreshListenerEligibility()
{
    for (int32 Index = RegisteredListeners.Num() - 1; Index >= 0; --Index)
    {
        if (!RegisteredListeners[Index].IsValid())
        {
            RegisteredListeners.RemoveAt(Index);
        }
    }

    for (int32 Index = EligibleListeners.Num() - 1; Index >= 0; --Index)
    {
        UConversationListenerComponent* Listener =
            EligibleListeners[Index].Get();
        if (!IsValid(Listener) || !IsListenerEligible(Listener))
        {
            EligibleListeners.RemoveAt(Index);
            if (IsValid(Listener))
            {
                if (IsValid(Listener->GetOwner()))
                {
                    OnListenerLeftConversation.Broadcast(
                        Listener->GetOwner()
                    );
                }
                DiscardResponsesFromNPC(
                    Listener->GetResolvedNPCID(),
                    TEXT("ListenerOutOfRange")
                );
            }
        }
    }

    for (const TWeakObjectPtr<UConversationListenerComponent>& ListenerPtr
        : RegisteredListeners)
    {
        UConversationListenerComponent* Listener = ListenerPtr.Get();
        if (IsListenerEligible(Listener) &&
            !EligibleListeners.Contains(Listener))
        {
            EligibleListeners.Add(Listener);
            OnListenerEnteredConversation.Broadcast(Listener->GetOwner());
            EmitDebug(
                TEXT("listener_entered"),
                FString::Printf(
                    TEXT("npc=%s distance=%.1f radius=%.1f"),
                    *Listener->GetResolvedNPCID().ToString(),
                    Listener->GetDistanceToActor(
                        RuntimeState.PrimaryNPC.Get()
                    ),
                    Listener->ListeningRadius
                )
            );
        }
    }
}

bool UConversationGovernanceComponent::IsListenerEligible(
    const UConversationListenerComponent* Listener
) const
{
    if (!RuntimeState.bIsActive ||
        !IsValid(Listener) ||
        !RegisteredListeners.Contains(Listener))
    {
        return false;
    }

    const AActor* ListenerActor = Listener->GetOwner();
    const AActor* PrimaryNPC = RuntimeState.PrimaryNPC.Get();
    return IsValid(ListenerActor) &&
        IsValid(PrimaryNPC) &&
        ListenerActor != PrimaryNPC &&
        Listener->CanHearActor(PrimaryNPC);
}

bool UConversationGovernanceComponent::SubmitCandidateResponse(
    AActor* NPCActor,
    const FString& ResponseText,
    EConversationResponseType ResponseType,
    float PriorityScore,
    FGuid& OutResponseID
)
{
    FCandidateNPCResponse Candidate;
    Candidate.NPCActor = NPCActor;
    Candidate.ResponseText = ResponseText;
    Candidate.ResponseType = ResponseType;
    Candidate.PriorityScore = PriorityScore;
    return SubmitCandidateResponseData(Candidate, OutResponseID);
}

bool UConversationGovernanceComponent::SubmitCandidateResponseData(
    const FCandidateNPCResponse& Candidate,
    FGuid& OutResponseID
)
{
    EnsureRuntimeObjects();
    OutResponseID.Invalidate();

    if (!RuntimeState.bIsActive)
    {
        EmitDebug(
            TEXT("candidate_rejected"),
            TEXT("reason=ConversationNotActive")
        );
        return false;
    }

    AActor* NPCActor = Candidate.NPCActor.Get();
    UConversationListenerComponent* Listener =
        FindListenerForActor(NPCActor);

    RefreshListenerEligibility();
    if (!IsValid(NPCActor) ||
        !IsValid(Listener) ||
        !EligibleListeners.Contains(Listener))
    {
        FCandidateNPCResponse Rejected = Candidate;
        if (!Rejected.ResponseID.IsValid())
        {
            Rejected.ResponseID = FGuid::NewGuid();
        }
        BroadcastDiscard(Rejected, TEXT("ListenerNotEligible"));
        return false;
    }

    FCandidateNPCResponse Prepared = Candidate;
    Prepared.ResponseID = Prepared.ResponseID.IsValid()
        ? Prepared.ResponseID
        : FGuid::NewGuid();
    Prepared.ConversationID = RuntimeState.ConversationID;
    Prepared.NPCID = Listener->GetResolvedNPCID();
    Prepared.NPCActor = NPCActor;
    Prepared.ResponseText = Prepared.ResponseText.TrimStartAndEnd();
    Prepared.CreatedTime = Prepared.CreatedTime > 0.0
        ? Prepared.CreatedTime
        : GetCurrentTimeSeconds();

    switch (Prepared.ResponseType)
    {
    case EConversationResponseType::Emergency:
        if (!Listener->bEmergencyInterruptionEnabled)
        {
            BroadcastDiscard(Prepared, TEXT("EmergencyInterruptionDisabled"));
            return false;
        }
        Prepared.PriorityScore = 1.0f;
        Prepared.bRequiresPermission = false;
        Prepared.bCanOverlapMainTurn = true;
        Prepared.bOwnsTurnWhenSpoken = true;
        break;
    case EConversationResponseType::RelevantContribution:
        Prepared.bRequiresPermission =
            Listener->bPermissionRequestsEnabled;
        Prepared.bCanOverlapMainTurn = false;
        Prepared.bOwnsTurnWhenSpoken = true;
        break;
    case EConversationResponseType::SideComment:
        if (!Listener->bSideCommentsEnabled)
        {
            BroadcastDiscard(Prepared, TEXT("SideCommentsDisabled"));
            return false;
        }
        Prepared.bRequiresPermission = false;
        Prepared.bCanOverlapMainTurn = false;
        Prepared.bOwnsTurnWhenSpoken = false;
        break;
    case EConversationResponseType::AssociativeTopic:
        Prepared.bRequiresPermission = false;
        Prepared.bCanOverlapMainTurn = false;
        Prepared.bOwnsTurnWhenSpoken = true;
        break;
    default:
        break;
    }

    OutResponseID = Prepared.ResponseID;
    OnCandidateResponseGenerated.Broadcast(Prepared);
    EmitDebug(
        TEXT("candidate_generated"),
        FString::Printf(
            TEXT("npc=%s response_id=%s type=%d priority=%.3f text=\"%s\""),
            *Prepared.NPCID.ToString(),
            *Prepared.ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            static_cast<int32>(Prepared.ResponseType),
            Prepared.PriorityScore,
            *Prepared.ResponseText
                .Replace(TEXT("\r"), TEXT(" "))
                .Replace(TEXT("\n"), TEXT(" "))
                .Replace(TEXT("\""), TEXT("'"))
        )
    );

    if (Prepared.ResponseType == EConversationResponseType::Invalid ||
        Prepared.ResponseType == EConversationResponseType::Silence)
    {
        BroadcastDiscard(Prepared, TEXT("NonQueueableResponseType"));
        return false;
    }

    FString FailureReason;
    if (!ResponseQueue->Enqueue(
            Prepared,
            Listener->MaximumQueuedResponses,
            FailureReason))
    {
        BroadcastDiscard(Prepared, FailureReason);
        return false;
    }

    OnResponseQueued.Broadcast(Prepared);
    LastDecision = TEXT("ResponseQueued");
    EmitDebug(
        TEXT("response_queued"),
        FString::Printf(
            TEXT("npc=%s response_id=%s queue_size=%d"),
            *Prepared.NPCID.ToString(),
            *Prepared.ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            ResponseQueue->Num()
        )
    );
    return true;
}

bool UConversationGovernanceComponent::NotifyPlayerSpeechStarted(
    AActor* PlayerActor
)
{
    EnsureRuntimeObjects();
    if (!RuntimeState.bIsActive)
    {
        return false;
    }

    const bool bAccepted =
        TurnManager->NotifyPlayerSpeechStarted(PlayerActor);
    if (!bAccepted)
    {
        EmitDebug(
            TEXT("speech_start_rejected"),
            TEXT("speaker_type=player reason=TurnOccupiedOrInvalid")
        );
    }
    return bAccepted;
}

bool UConversationGovernanceComponent::NotifyPlayerSpeechEnded(
    AActor* PlayerActor
)
{
    EnsureRuntimeObjects();
    if (!RuntimeState.bIsActive)
    {
        return false;
    }

    const bool bAccepted =
        TurnManager->NotifyPlayerSpeechEnded(PlayerActor);
    if (!bAccepted)
    {
        EmitDebug(
            TEXT("speech_end_ignored"),
            TEXT("speaker_type=player reason=DuplicateOrMismatched")
        );
    }
    return bAccepted;
}

bool UConversationGovernanceComponent::NotifyNPCSpeechStarted(AActor* NPCActor)
{
    EnsureRuntimeObjects();
    if (!RuntimeState.bIsActive)
    {
        return false;
    }

    const bool bAccepted = TurnManager->NotifyNPCSpeechStarted(NPCActor);
    if (!bAccepted)
    {
        EmitDebug(
            TEXT("speech_start_rejected"),
            FString::Printf(
                TEXT("speaker_type=npc actor=%s reason=TurnOccupiedOrInvalid"),
                IsValid(NPCActor) ? *NPCActor->GetName() : TEXT("None")
            )
        );
    }
    return bAccepted;
}

bool UConversationGovernanceComponent::NotifyNPCSpeechEnded(AActor* NPCActor)
{
    EnsureRuntimeObjects();
    if (!RuntimeState.bIsActive)
    {
        return false;
    }

    const bool bAccepted = TurnManager->NotifyNPCSpeechEnded(NPCActor);
    if (!bAccepted)
    {
        EmitDebug(
            TEXT("speech_end_ignored"),
            FString::Printf(
                TEXT("speaker_type=npc actor=%s reason=DuplicateOrMismatched"),
                IsValid(NPCActor) ? *NPCActor->GetName() : TEXT("None")
            )
        );
    }
    return bAccepted;
}

bool UConversationGovernanceComponent::BeginSideComment(AActor* NPCActor)
{
    EnsureRuntimeObjects();
    return RuntimeState.bIsActive &&
        TurnManager->BeginSideComment(NPCActor);
}

bool UConversationGovernanceComponent::EndSideComment(AActor* NPCActor)
{
    EnsureRuntimeObjects();
    return RuntimeState.bIsActive &&
        TurnManager->EndSideComment(NPCActor);
}

bool UConversationGovernanceComponent::BeginEmergencyInterrupt(
    AActor* NPCActor
)
{
    EnsureRuntimeObjects();
    return RuntimeState.bIsActive &&
        TurnManager->BeginEmergencyInterrupt(NPCActor);
}

bool UConversationGovernanceComponent::RequestSpeakingPermission(
    FGuid ResponseID
)
{
    EnsureRuntimeObjects();
    if (!RuntimeState.bIsActive ||
        bHasPendingPermissionResponse ||
        !ResponseID.IsValid())
    {
        return false;
    }

    FCandidateNPCResponse Response;
    if (!ResponseQueue->GetResponseByID(ResponseID, Response) ||
        Response.ResponseType !=
            EConversationResponseType::RelevantContribution ||
        !Response.bRequiresPermission)
    {
        return false;
    }
    if (!TurnManager->EnterPermissionPending())
    {
        return false;
    }
    if (!ResponseQueue->RemoveByResponseID(ResponseID))
    {
        TurnManager->ResolvePermissionPending();
        return false;
    }

    PendingPermissionResponse = Response;
    bHasPendingPermissionResponse = true;
    LastDecision = TEXT("SpeakingPermissionRequested");
    OnSpeakingPermissionRequested.Broadcast(Response);
    EmitDebug(
        TEXT("speaking_permission_requested"),
        FString::Printf(
            TEXT("npc=%s response_id=%s"),
            *Response.NPCID.ToString(),
            *Response.ResponseID.ToString(EGuidFormats::DigitsWithHyphens)
        )
    );
    return true;
}

bool UConversationGovernanceComponent::GrantPermission(
    FGuid ResponseID,
    FCandidateNPCResponse& OutApprovedResponse
)
{
    EnsureRuntimeObjects();
    OutApprovedResponse = FCandidateNPCResponse();
    if (!bHasPendingPermissionResponse ||
        PendingPermissionResponse.ResponseID != ResponseID ||
        !TurnManager->ResolvePermissionPending())
    {
        return false;
    }

    OutApprovedResponse = PendingPermissionResponse;
    PendingPermissionResponse = FCandidateNPCResponse();
    bHasPendingPermissionResponse = false;
    LastDecision = TEXT("SpeakingPermissionGranted");
    OnSpeakingPermissionResolved.Broadcast(OutApprovedResponse, true);
    EmitDebug(
        TEXT("speaking_permission_resolved"),
        FString::Printf(
            TEXT("npc=%s response_id=%s granted=1"),
            *OutApprovedResponse.NPCID.ToString(),
            *OutApprovedResponse.ResponseID.ToString(
                EGuidFormats::DigitsWithHyphens
            )
        )
    );
    return true;
}

bool UConversationGovernanceComponent::DenyPermission(FGuid ResponseID)
{
    EnsureRuntimeObjects();
    if (!bHasPendingPermissionResponse ||
        PendingPermissionResponse.ResponseID != ResponseID ||
        !TurnManager->ResolvePermissionPending())
    {
        return false;
    }

    const FCandidateNPCResponse Denied = PendingPermissionResponse;
    PendingPermissionResponse = FCandidateNPCResponse();
    bHasPendingPermissionResponse = false;
    LastDecision = TEXT("SpeakingPermissionDenied");
    OnSpeakingPermissionResolved.Broadcast(Denied, false);
    EmitDebug(
        TEXT("speaking_permission_resolved"),
        FString::Printf(
            TEXT("npc=%s response_id=%s granted=0"),
            *Denied.NPCID.ToString(),
            *Denied.ResponseID.ToString(EGuidFormats::DigitsWithHyphens)
        )
    );
    BroadcastDiscard(Denied, TEXT("PermissionDenied"));
    return true;
}

void UConversationGovernanceComponent::NotifyResponseExecutionStarted(
    const FCandidateNPCResponse& Response
)
{
    LastDecision = TEXT("ResponseExecutionStarted");
    OnResponseExecutionStarted.Broadcast(Response);
    EmitDebug(
        TEXT("response_execution_started"),
        FString::Printf(
            TEXT("npc=%s response_id=%s type=%d"),
            *Response.NPCID.ToString(),
            *Response.ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            static_cast<int32>(Response.ResponseType)
        )
    );
}

void UConversationGovernanceComponent::NotifyResponseExecutionFinished(
    const FCandidateNPCResponse& Response
)
{
    LastDecision = TEXT("ResponseExecutionFinished");
    OnResponseExecutionFinished.Broadcast(Response);
    EmitDebug(
        TEXT("response_execution_finished"),
        FString::Printf(
            TEXT("npc=%s response_id=%s type=%d"),
            *Response.NPCID.ToString(),
            *Response.ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            static_cast<int32>(Response.ResponseType)
        )
    );
}

void UConversationGovernanceComponent::RequestNonVerbalReaction(
    AActor* NPCActor,
    ENonVerbalReactionType ReactionType,
    float Intensity
)
{
    if (!RuntimeState.bIsActive ||
        !IsValid(NPCActor) ||
        ReactionType == ENonVerbalReactionType::None)
    {
        return;
    }

    const float SafeIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
    OnNonVerbalReactionRequested.Broadcast(
        NPCActor,
        ReactionType,
        SafeIntensity
    );
    LastDecision = TEXT("NonVerbalReactionRequested");
    EmitDebug(
        TEXT("non_verbal_reaction_requested"),
        FString::Printf(
            TEXT("actor=%s type=%d intensity=%.2f"),
            *NPCActor->GetName(),
            static_cast<int32>(ReactionType),
            SafeIntensity
        )
    );
}

bool UConversationGovernanceComponent::CancelResponse(FGuid ResponseID)
{
    EnsureRuntimeObjects();

    FCandidateNPCResponse Response;
    if (!ResponseQueue->GetResponseByID(ResponseID, Response) ||
        !ResponseQueue->RemoveByResponseID(ResponseID))
    {
        return false;
    }

    BroadcastDiscard(Response, TEXT("Cancelled"));
    return true;
}

AActor* UConversationGovernanceComponent::GetCurrentTurnOwner() const
{
    return IsValid(TurnManager)
        ? TurnManager->GetCurrentTurnOwner()
        : nullptr;
}

EConversationTurnState UConversationGovernanceComponent::GetTurnState() const
{
    return IsValid(TurnManager)
        ? TurnManager->GetTurnState()
        : EConversationTurnState::Idle;
}

TArray<FCandidateNPCResponse>
UConversationGovernanceComponent::GetQueuedResponses() const
{
    return IsValid(ResponseQueue)
        ? ResponseQueue->GetQueuedResponses()
        : TArray<FCandidateNPCResponse>();
}

FConversationRuntimeState
UConversationGovernanceComponent::GetRuntimeState() const
{
    FConversationRuntimeState Result = RuntimeState;
    if (IsValid(TurnManager))
    {
        Result.TurnState = TurnManager->GetTurnState();
        Result.CurrentTurnOwner = TurnManager->GetCurrentTurnOwner();
        Result.CurrentSpeaker = TurnManager->GetCurrentSpeaker();
    }
    return Result;
}

FConversationGovernanceDebugSnapshot
UConversationGovernanceComponent::GetDebugSnapshot() const
{
    FConversationGovernanceDebugSnapshot Snapshot;
    Snapshot.RuntimeState = GetRuntimeState();
    Snapshot.QueuedResponses = GetQueuedResponses();
    Snapshot.LastDecision = LastDecision;

    for (const TWeakObjectPtr<UConversationListenerComponent>& ListenerPtr
        : EligibleListeners)
    {
        const UConversationListenerComponent* Listener = ListenerPtr.Get();
        if (IsValid(Listener) && IsValid(Listener->GetOwner()))
        {
            Snapshot.EligibleListeners.Add(Listener->GetOwner());
        }
    }

    return Snapshot;
}

void UConversationGovernanceComponent::EnsureRuntimeObjects()
{
    if (!IsValid(TurnManager))
    {
        TurnManager = NewObject<UConversationTurnManager>(
            this,
            TEXT("ConversationTurnManager")
        );
        TurnManager->OnTurnOwnerChanged.AddDynamic(
            this,
            &UConversationGovernanceComponent::HandleTurnOwnerChanged
        );
    }

    if (!IsValid(ResponseQueue))
    {
        ResponseQueue = NewObject<UConversationResponseQueue>(
            this,
            TEXT("ConversationResponseQueue")
        );
        ResponseQueue->Initialize(MaximumTotalQueueLength);
    }
}

UConversationListenerComponent*
UConversationGovernanceComponent::FindListenerForActor(
    const AActor* Actor
) const
{
    if (!IsValid(Actor))
    {
        return nullptr;
    }

    for (const TWeakObjectPtr<UConversationListenerComponent>& ListenerPtr
        : RegisteredListeners)
    {
        UConversationListenerComponent* Listener = ListenerPtr.Get();
        if (IsValid(Listener) && Listener->GetOwner() == Actor)
        {
            return Listener;
        }
    }

    return nullptr;
}

void UConversationGovernanceComponent::DiscardResponsesFromNPC(
    FName NPCID,
    const FString& Reason
)
{
    if (!IsValid(ResponseQueue))
    {
        return;
    }

    const TArray<FCandidateNPCResponse> Pending =
        ResponseQueue->GetQueuedResponses();
    for (const FCandidateNPCResponse& Response : Pending)
    {
        if (Response.NPCID == NPCID)
        {
            ResponseQueue->RemoveByResponseID(Response.ResponseID);
            BroadcastDiscard(Response, Reason);
        }
    }
}

void UConversationGovernanceComponent::BroadcastDiscard(
    const FCandidateNPCResponse& Response,
    const FString& Reason
)
{
    OnResponseDiscarded.Broadcast(Response, Reason);
    LastDecision = FString::Printf(TEXT("Discarded:%s"), *Reason);
    EmitDebug(
        TEXT("response_discarded"),
        FString::Printf(
            TEXT("npc=%s response_id=%s reason=%s"),
            *Response.NPCID.ToString(),
            *Response.ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            *Reason
        )
    );
}

void UConversationGovernanceComponent::EmitDebug(
    const FString& EventName,
    const FString& Details
)
{
    if (!bEnableDebugLogging)
    {
        return;
    }

    const FString Message = FString::Printf(
        TEXT(
            "NATURALNPC_GOV event=%s conversation=%s timestamp=%.3f %s"
        ),
        *EventName,
        *RuntimeState.ConversationID.ToString(),
        GetCurrentTimeSeconds(),
        *Details
    );
    UE_LOG(LogNaturalNPCConversationGovernance, Display, TEXT("%s"), *Message);
    OnGovernanceDebugMessage.Broadcast(Message);
}

double UConversationGovernanceComponent::GetCurrentTimeSeconds() const
{
    const UWorld* World = GetWorld();
    return IsValid(World)
        ? static_cast<double>(World->GetTimeSeconds())
        : FPlatformTime::Seconds();
}

void UConversationGovernanceComponent::HandleTurnOwnerChanged(
    AActor* PreviousOwner,
    AActor* NewOwner,
    EConversationTurnState NewState
)
{
    RuntimeState.TurnState = NewState;
    RuntimeState.CurrentTurnOwner = NewOwner;
    RuntimeState.CurrentSpeaker = IsValid(TurnManager)
        ? TurnManager->GetCurrentSpeaker()
        : nullptr;

    OnTurnOwnerChanged.Broadcast(PreviousOwner, NewOwner, NewState);
    LastDecision = TEXT("TurnOwnerChanged");
    EmitDebug(
        TEXT("turn_owner_changed"),
        FString::Printf(
            TEXT("previous=%s current=%s state=%d"),
            IsValid(PreviousOwner) ? *PreviousOwner->GetName() : TEXT("None"),
            IsValid(NewOwner) ? *NewOwner->GetName() : TEXT("None"),
            static_cast<int32>(NewState)
        )
    );
}
