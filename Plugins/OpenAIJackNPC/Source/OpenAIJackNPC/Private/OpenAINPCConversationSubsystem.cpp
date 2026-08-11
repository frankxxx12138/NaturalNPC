#include "OpenAINPCConversationSubsystem.h"

#include "ConversationGovernanceComponent.h"
#include "ConversationListenerComponent.h"
#include "ConversationResponseQueue.h"
#include "Dom/JsonObject.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "OpenAIJackComponent.h"
#include "OpenAINPCAutonomy.h"
#include "OpenAINPCRouter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "TimerManager.h"

namespace
{
    constexpr float ConversationTickIntervalSeconds = 0.1f;
    constexpr float NaturalPauseDelaySeconds = 0.65f;
    constexpr float MinimumObservedSpeechSeconds = 0.2f;
    // Candidate generation runs in parallel with the primary NPC's speech.
    // Keep the candidate alive long enough for queued TTS / facial animation
    // to finish before the natural pause and governed hand-off occur.
    constexpr float CandidateExpirySeconds = 120.0f;
    constexpr int32 MaximumCandidateRequestsPerExchange = 2;
    constexpr int32 MaximumAutonomousResponsesPerExchange = 1;
    constexpr int32 MaximumRecentExchanges = 4;
    constexpr float PermissionResponseTimeoutSeconds = 15.0f;

    TAutoConsoleVariable<int32> CVarAutonomousConversationEnabled(
        TEXT("npc.Autonomy.Enabled"),
        1,
        TEXT(
            "Enable autonomous nearby-NPC reactions. "
            "0 disables candidate generation; 1 enables it."
        ),
        ECVF_Default
    );

    FString ExtractOllamaMessage(const FString& ResponsePayload)
    {
        TSharedPtr<FJsonObject> Root;
        const TSharedRef<TJsonReader<>> Reader =
            TJsonReaderFactory<>::Create(ResponsePayload);
        if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
        {
            return FString();
        }

        const TSharedPtr<FJsonObject>* Message = nullptr;
        if (!Root->TryGetObjectField(TEXT("message"), Message) ||
            !Message->IsValid())
        {
            return FString();
        }

        FString Content;
        (*Message)->TryGetStringField(TEXT("content"), Content);
        return Content.TrimStartAndEnd();
    }

    FString SanitizeLogText(const FString& Text)
    {
        return Text
            .Replace(TEXT("\r"), TEXT(" "))
            .Replace(TEXT("\n"), TEXT(" "))
            .Replace(TEXT("\""), TEXT("'"));
    }

    void AddUniqueAlias(TArray<FString>& Aliases, const FString& Value)
    {
        const FString Alias = Value.TrimStartAndEnd();
        if (Alias.Len() < 2 ||
            Alias.Equals(TEXT("OpenAINPC"), ESearchCase::IgnoreCase) ||
            Alias.Equals(TEXT("NPC"), ESearchCase::IgnoreCase))
        {
            return;
        }
        Aliases.AddUnique(Alias);
    }

    TArray<FString> GetAddressAliases(
        const UOpenAIJackComponent* Component
    )
    {
        TArray<FString> Aliases;
        if (!IsValid(Component))
        {
            return Aliases;
        }

        AddUniqueAlias(Aliases, Component->GetResolvedNPCID().ToString());
        AddUniqueAlias(Aliases, Component->SubtitleSpeakerName);
        if (const AActor* Owner = Component->GetOwner())
        {
            for (const FName Tag : Owner->Tags)
            {
                AddUniqueAlias(Aliases, Tag.ToString());
            }
        }
        return Aliases;
    }

    bool HasDirectAddress(
        const FString& PlayerText,
        const UOpenAIJackComponent* Component
    )
    {
        for (const FString& Alias : GetAddressAliases(Component))
        {
            if (OpenAINPCRouter::IsDirectAddress(PlayerText, Alias))
            {
                return true;
            }
        }
        return false;
    }

    bool HasAnyNameMention(
        const FString& PlayerText,
        const UOpenAIJackComponent* Component
    )
    {
        for (const FString& Alias : GetAddressAliases(Component))
        {
            if (OpenAINPCRouter::ContainsNameMention(PlayerText, Alias))
            {
                return true;
            }
        }
        return false;
    }
}

bool UOpenAINPCConversationSubsystem::ShouldCreateSubsystem(
    UObject* Outer
) const
{
    const UWorld* World = Cast<UWorld>(Outer);
    return IsValid(World) &&
        (World->WorldType == EWorldType::Game ||
         World->WorldType == EWorldType::PIE);
}

void UOpenAINPCConversationSubsystem::Initialize(
    FSubsystemCollectionBase& Collection
)
{
    Super::Initialize(Collection);
}

void UOpenAINPCConversationSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    EnsureCoordinator();
    InWorld.GetTimerManager().SetTimer(
        ConversationTickTimer,
        this,
        &UOpenAINPCConversationSubsystem::TickConversation,
        ConversationTickIntervalSeconds,
        true
    );
}

void UOpenAINPCConversationSubsystem::Deinitialize()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ConversationTickTimer);
    }

    ++ExchangeSerial;
    PendingCandidateRequests = 0;
    if (IsValid(Governance))
    {
        Governance->EndConversation();
    }
    if (IsValid(CoordinatorActor))
    {
        CoordinatorActor->Destroy();
    }

    Governance = nullptr;
    CoordinatorActor = nullptr;
    RegisteredNPCs.Reset();
    RecentExchanges.Reset();
    Super::Deinitialize();
}

void UOpenAINPCConversationSubsystem::RegisterNPC(
    UOpenAIJackComponent* Component
)
{
    if (!IsValid(Component) || !IsValid(Component->GetOwner()))
    {
        return;
    }

    RemoveInvalidNPCs();
    if (FindRegisteredNPC(Component))
    {
        return;
    }

    FRegisteredNPC Registration;
    Registration.Component = Component;
    Registration.Listener = EnsureListener(Component);
    RegisteredNPCs.Add(Registration);

    EmitAutonomyLog(
        TEXT("npc_registered"),
        FString::Printf(
            TEXT("npc=%s actor=%s"),
            *Component->GetResolvedNPCID().ToString(),
            *Component->GetOwner()->GetName()
        )
    );
}

void UOpenAINPCConversationSubsystem::UnregisterNPC(
    UOpenAIJackComponent* Component
)
{
    if (!IsValid(Component))
    {
        return;
    }

    if (FRegisteredNPC* Registration = FindRegisteredNPC(Component))
    {
        if (IsValid(Governance) && Registration->Listener.IsValid())
        {
            Governance->UnregisterListener(Registration->Listener.Get());
        }
    }

    RegisteredNPCs.RemoveAll(
        [Component](const FRegisteredNPC& Registration)
        {
            return Registration.Component.Get() == Component;
        }
    );
}

bool UOpenAINPCConversationSubsystem::CanAcceptPlayerText(
    const UOpenAIJackComponent* TargetComponent
) const
{
    return IsValid(TargetComponent) &&
        (!ActiveSecondary.IsValid() ||
         !ActiveSecondary->IsConversationOutputActive());
}

bool UOpenAINPCConversationSubsystem::IsSpeechInputCoordinator(
    const UOpenAIJackComponent* Component
) const
{
    if (!IsValid(Component) || !Component->bEnableKeyboardPushToTalk)
    {
        return false;
    }

    const UOpenAIJackComponent* BestComponent = nullptr;
    FString BestKey;
    for (const FRegisteredNPC& Registration : RegisteredNPCs)
    {
        const UOpenAIJackComponent* Candidate = Registration.Component.Get();
        if (!IsValid(Candidate) || !Candidate->bEnableKeyboardPushToTalk)
        {
            continue;
        }

        const FString CandidateKey = FString::Printf(
            TEXT("%s|%s"),
            *Candidate->GetResolvedNPCID().ToString(),
            Candidate->GetOwner() ? *Candidate->GetOwner()->GetName() : TEXT("")
        ).ToLower();
        if (!BestComponent || CandidateKey < BestKey)
        {
            BestComponent = Candidate;
            BestKey = CandidateKey;
        }
    }
    return BestComponent == Component;
}

UOpenAIJackComponent*
UOpenAINPCConversationSubsystem::ResolvePlayerTextTarget(
    const FString& PlayerText,
    FString& OutReason
) const
{
    OutReason.Reset();
    TArray<UOpenAIJackComponent*> Candidates;
    for (const FRegisteredNPC& Registration : RegisteredNPCs)
    {
        UOpenAIJackComponent* Candidate = Registration.Component.Get();
        if (IsValid(Candidate) && IsValid(Candidate->GetOwner()))
        {
            Candidates.Add(Candidate);
        }
    }
    if (Candidates.IsEmpty())
    {
        OutReason = TEXT("NoRegisteredNPC");
        return nullptr;
    }
    if (Candidates.Num() == 1)
    {
        OutReason = TEXT("OnlyNPC");
        return Candidates[0];
    }

    TArray<UOpenAIJackComponent*> DirectlyAddressed;
    for (UOpenAIJackComponent* Candidate : Candidates)
    {
        if (HasDirectAddress(PlayerText, Candidate))
        {
            DirectlyAddressed.Add(Candidate);
        }
    }
    if (DirectlyAddressed.Num() == 1)
    {
        OutReason = TEXT("DirectAddress");
        return DirectlyAddressed[0];
    }

    UWorld* World = GetWorld();
    APlayerController* PlayerController = World
        ? World->GetFirstPlayerController()
        : nullptr;
    FVector ViewLocation = FVector::ZeroVector;
    FVector ViewDirection = FVector::ForwardVector;
    const bool bHasView = IsValid(PlayerController);
    if (bHasView)
    {
        FRotator ViewRotation;
        PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
        ViewDirection = ViewRotation.Vector().GetSafeNormal();
    }

    UOpenAIJackComponent* BestComponent = nullptr;
    float BestScore = -TNumericLimits<float>::Max();
    FString BestScoreReason;
    const UOpenAIJackComponent* ContextPrimary = ActivePrimary.Get();
    for (UOpenAIJackComponent* Candidate : Candidates)
    {
        AActor* CandidateActor = Candidate->GetOwner();
        float Score = Candidate == ContextPrimary ? 25.0f : 0.0f;
        FString ScoreReason = Candidate == ContextPrimary
            ? TEXT("Context")
            : TEXT("Proximity");

        if (DirectlyAddressed.Contains(Candidate))
        {
            Score += 1000.0f;
            ScoreReason = TEXT("AmbiguousDirectAddress+Gaze");
        }

        if (bHasView)
        {
            const FVector TargetLocation =
                CandidateActor->GetActorLocation() + FVector(0.0, 0.0, 90.0);
            const FVector ToCandidate = TargetLocation - ViewLocation;
            const float Distance = ToCandidate.Size();
            const float Dot = Distance > UE_SMALL_NUMBER
                ? FVector::DotProduct(
                    ViewDirection,
                    ToCandidate / Distance
                )
                : 1.0f;
            Score += FMath::Max(0.0f, Dot) * 180.0f;
            Score += FMath::Max(0.0f, 1.0f - Distance / 2500.0f) * 30.0f;
            if (Dot >= FMath::Cos(FMath::DegreesToRadians(30.0f)))
            {
                Score += 80.0f;
                ScoreReason = TEXT("Gaze");
            }

            FCollisionQueryParams QueryParams(
                SCENE_QUERY_STAT(OpenAINPCSpeechRouting),
                false,
                PlayerController->GetPawn()
            );
            FHitResult Hit;
            if (!World->LineTraceSingleByChannel(
                    Hit,
                    ViewLocation,
                    TargetLocation,
                    ECC_Visibility,
                    QueryParams) ||
                Hit.GetActor() == CandidateActor ||
                (IsValid(Hit.GetActor()) &&
                 Hit.GetActor()->IsAttachedTo(CandidateActor)))
            {
                Score += 15.0f;
            }
            else
            {
                Score -= 35.0f;
            }
        }

        // Internal name mentions are deliberately informational only. They
        // never receive a targeting bonus unless parsed as direct address.
        if (HasAnyNameMention(PlayerText, Candidate) &&
            !DirectlyAddressed.Contains(Candidate))
        {
            ScoreReason += TEXT("+MentionOnly");
        }

        if (Score > BestScore)
        {
            BestScore = Score;
            BestComponent = Candidate;
            BestScoreReason = ScoreReason;
        }
    }

    OutReason = BestScoreReason;
    return BestComponent;
}

bool UOpenAINPCConversationSubsystem::RouteRecognizedPlayerText(
    UOpenAIJackComponent* CaptureComponent,
    const FString& PlayerText
)
{
    const FString TrimmedText = PlayerText.TrimStartAndEnd();
    if (TrimmedText.IsEmpty())
    {
        return false;
    }

    FString Reason;
    UOpenAIJackComponent* Target = ResolvePlayerTextTarget(
        TrimmedText,
        Reason
    );
    if (!IsValid(Target))
    {
        Target = CaptureComponent;
        Reason = TEXT("CaptureFallback");
    }
    if (!IsValid(Target))
    {
        return false;
    }

    EmitAutonomyLog(
        TEXT("speech_routed"),
        FString::Printf(
            TEXT("target=%s reason=%s text=\"%s\""),
            *Target->GetResolvedNPCID().ToString(),
            *Reason,
            *SanitizeLogText(TrimmedText)
        )
    );
    Target->SendPlayerText(TrimmedText);
    return true;
}

void UOpenAINPCConversationSubsystem::NotifyPlayerTextSubmitted(
    UOpenAIJackComponent* PrimaryComponent,
    const FString& PlayerText
)
{
    if (CVarAutonomousConversationEnabled.GetValueOnGameThread() == 0)
    {
        return;
    }

    if (!IsValid(PrimaryComponent) ||
        !IsValid(PrimaryComponent->GetOwner()) ||
        PlayerText.TrimStartAndEnd().IsEmpty())
    {
        return;
    }

    EnsureCoordinator();
    RegisterNPC(PrimaryComponent);
    if (!IsValid(Governance))
    {
        return;
    }

    ++ExchangeSerial;
    PendingCandidateRequests = 0;
    ExecutedResponsesThisExchange = 0;
    bWaitingForPrimaryReply = true;
    bPrimaryTurnStarted = false;
    bPrimaryTurnEnded = false;
    bExchangeArchived = false;
    PrimaryReplyReadyTime = 0.0;
    NaturalPauseReadyTime = 0.0;
    SecondarySpeechStartTime = 0.0;
    PermissionResponseDeadline = 0.0;
    ActiveSecondary.Reset();
    ActiveResponse = FCandidateNPCResponse();
    ActiveExecutionMode = ESecondaryExecutionMode::None;
    CurrentSecondaryReply.Reset();
    CurrentSecondaryNPCID = NAME_None;

    if (Governance->IsConversationActive())
    {
        Governance->EndConversation();
    }

    ActivePrimary = PrimaryComponent;
    CurrentPlayerText = PlayerText.TrimStartAndEnd();
    CurrentPrimaryReply.Reset();
    ActiveConversationID = FName(*FString::Printf(
        TEXT("AutoConversation_%d"),
        ExchangeSerial
    ));

    RemoveInvalidNPCs();
    for (FRegisteredNPC& Registration : RegisteredNPCs)
    {
        UOpenAIJackComponent* RegisteredComponent =
            Registration.Component.Get();
        if (!IsValid(RegisteredComponent))
        {
            continue;
        }

        UConversationListenerComponent* Listener =
            EnsureListener(RegisteredComponent);
        Registration.Listener = Listener;
        if (!IsValid(Listener))
        {
            continue;
        }

        if (RegisteredComponent == PrimaryComponent)
        {
            Governance->UnregisterListener(Listener);
        }
        else
        {
            Governance->RegisterListener(Listener);
        }
    }

    Governance->StartConversation(
        ActiveConversationID,
        PrimaryComponent->GetOwner()
    );

    EmitAutonomyLog(
        TEXT("exchange_started"),
        FString::Printf(
            TEXT("primary=%s player_text=\"%s\" registered_npcs=%d"),
            *PrimaryComponent->GetResolvedNPCID().ToString(),
            *SanitizeLogText(CurrentPlayerText),
            RegisteredNPCs.Num()
        )
    );
}

void UOpenAINPCConversationSubsystem::NotifyNPCReplyReady(
    UOpenAIJackComponent* SourceComponent,
    const FString& ReplyText
)
{
    if (!bWaitingForPrimaryReply ||
        SourceComponent != ActivePrimary.Get() ||
        ReplyText.TrimStartAndEnd().IsEmpty() ||
        !IsValid(Governance))
    {
        return;
    }

    bWaitingForPrimaryReply = false;
    CurrentPrimaryReply = ReplyText.TrimStartAndEnd();
    PrimaryReplyReadyTime = GetCurrentTimeSeconds();
    bPrimaryTurnStarted = Governance->NotifyNPCSpeechStarted(
        SourceComponent->GetOwner()
    );

    EmitAutonomyLog(
        TEXT("primary_reply_ready"),
        FString::Printf(
            TEXT("primary=%s turn_started=%d text=\"%s\""),
            *SourceComponent->GetResolvedNPCID().ToString(),
            bPrimaryTurnStarted ? 1 : 0,
            *SanitizeLogText(CurrentPrimaryReply)
        )
    );

    BeginCandidateGeneration();
}

void UOpenAINPCConversationSubsystem::ReportEmergencyEvent(
    FName EventID,
    const FString& Description,
    AActor* SourceActor,
    float Severity
)
{
    const FString TrimmedDescription = Description.TrimStartAndEnd();
    if (EventID.IsNone() || TrimmedDescription.IsEmpty())
    {
        EmitAutonomyLog(
            TEXT("emergency_event_rejected"),
            TEXT("reason=MissingIdentityOrDescription")
        );
        return;
    }

    if (EmergencyEvent.EventID != EventID)
    {
        DeliveredEmergencyWarnings.Reset();
    }
    EmergencyEvent.EventID = EventID;
    EmergencyEvent.Description = TrimmedDescription;
    EmergencyEvent.SourceActor = SourceActor;
    EmergencyEvent.Severity = FMath::Clamp(Severity, 0.0f, 1.0f);
    EmergencyEvent.bActive = true;
    EmitAutonomyLog(
        TEXT("emergency_event_reported"),
        FString::Printf(
            TEXT("event_id=%s severity=%.2f description=\"%s\""),
            *EventID.ToString(),
            EmergencyEvent.Severity,
            *SanitizeLogText(TrimmedDescription)
        )
    );
    if (IsValid(Governance) &&
        Governance->IsConversationActive() &&
        !CurrentPrimaryReply.IsEmpty())
    {
        BeginCandidateGeneration();
    }
}

void UOpenAINPCConversationSubsystem::ClearEmergencyEvent(FName EventID)
{
    if (!EmergencyEvent.bActive ||
        (!EventID.IsNone() && EmergencyEvent.EventID != EventID))
    {
        return;
    }

    EmitAutonomyLog(
        TEXT("emergency_event_cleared"),
        FString::Printf(
            TEXT("event_id=%s"),
            *EmergencyEvent.EventID.ToString()
        )
    );
    if (IsValid(Governance))
    {
        for (const FCandidateNPCResponse& Response :
             Governance->GetQueuedResponses())
        {
            if (Response.ResponseType ==
                    EConversationResponseType::Emergency &&
                Response.VerifiedEmergencyEventID ==
                    EmergencyEvent.EventID)
            {
                Governance->CancelResponse(Response.ResponseID);
            }
        }
    }
    EmergencyEvent = FOpenAINPCEmergencyEventState();
    DeliveredEmergencyWarnings.Reset();
}

bool UOpenAINPCConversationSubsystem::HasPendingSpeakingPermission() const
{
    return IsValid(Governance) &&
        Governance->HasPendingPermissionRequest();
}

bool UOpenAINPCConversationSubsystem::GrantPendingSpeakingPermission()
{
    if (!HasPendingSpeakingPermission() ||
        ActiveSecondary.IsValid())
    {
        return false;
    }

    const FCandidateNPCResponse Pending =
        Governance->GetPendingPermissionResponse();
    FCandidateNPCResponse Approved;
    if (!Governance->GrantPermission(
            Pending.ResponseID,
            Approved))
    {
        return false;
    }

    PermissionResponseDeadline = 0.0;
    ExecutedResponsesThisExchange =
        FMath::Max(0, ExecutedResponsesThisExchange - 1);
    if (!StartResponseExecution(
            Approved,
            ESecondaryExecutionMode::MainResponse,
            Approved.ResponseText))
    {
        EmitAutonomyLog(
            TEXT("permission_execution_failed"),
            FString::Printf(
                TEXT("npc=%s reason=SpeechUnavailable"),
                *Approved.NPCID.ToString()
            )
        );
        ArchiveCurrentExchange();
        return false;
    }

    EmitAutonomyLog(
        TEXT("permission_granted"),
        FString::Printf(
            TEXT("npc=%s response_id=%s"),
            *Approved.NPCID.ToString(),
            *Approved.ResponseID.ToString(EGuidFormats::DigitsWithHyphens)
        )
    );
    return true;
}

bool UOpenAINPCConversationSubsystem::DenyPendingSpeakingPermission()
{
    if (!HasPendingSpeakingPermission())
    {
        return false;
    }

    const FCandidateNPCResponse Pending =
        Governance->GetPendingPermissionResponse();
    if (!Governance->DenyPermission(Pending.ResponseID))
    {
        return false;
    }

    PermissionResponseDeadline = 0.0;
    EmitAutonomyLog(
        TEXT("permission_denied"),
        FString::Printf(
            TEXT("npc=%s response_id=%s"),
            *Pending.NPCID.ToString(),
            *Pending.ResponseID.ToString(EGuidFormats::DigitsWithHyphens)
        )
    );
    ArchiveCurrentExchange();
    return true;
}

bool UOpenAINPCConversationSubsystem::HandlePermissionPlayerText(
    const FString& PlayerText
)
{
    if (!HasPendingSpeakingPermission())
    {
        return false;
    }

    const FString Normalized =
        PlayerText.TrimStartAndEnd().ToLower();
    const bool bDenied =
        Normalized.Contains(TEXT("no")) ||
        Normalized.Contains(TEXT("not now")) ||
        Normalized.Contains(TEXT("no thanks")) ||
        Normalized.Contains(TEXT("不要")) ||
        Normalized.Contains(TEXT("不用")) ||
        Normalized.Contains(TEXT("不可以"));
    const bool bGranted =
        Normalized.Contains(TEXT("yes")) ||
        Normalized.Contains(TEXT("go ahead")) ||
        Normalized.Contains(TEXT("please do")) ||
        Normalized.Contains(TEXT("sure")) ||
        Normalized.Contains(TEXT("tell me")) ||
        Normalized.Contains(TEXT("可以")) ||
        Normalized.Contains(TEXT("说吧")) ||
        Normalized == TEXT("好");

    if (bDenied)
    {
        DenyPendingSpeakingPermission();
    }
    else if (bGranted)
    {
        GrantPendingSpeakingPermission();
    }
    else
    {
        EmitAutonomyLog(
            TEXT("permission_input_unrecognized"),
            FString::Printf(
                TEXT("text=\"%s\""),
                *SanitizeLogText(PlayerText)
            )
        );
    }
    return true;
}

void UOpenAINPCConversationSubsystem::EnsureCoordinator()
{
    if (IsValid(CoordinatorActor) && IsValid(Governance))
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Name = TEXT("OpenAINPCConversationCoordinator");
    SpawnParameters.ObjectFlags |= RF_Transient;
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    CoordinatorActor = World->SpawnActor<AActor>(
        AActor::StaticClass(),
        FTransform::Identity,
        SpawnParameters
    );
    if (!IsValid(CoordinatorActor))
    {
        return;
    }

    CoordinatorActor->SetActorHiddenInGame(true);
    CoordinatorActor->SetActorEnableCollision(false);
    Governance = NewObject<UConversationGovernanceComponent>(
        CoordinatorActor,
        TEXT("AutonomousConversationGovernance")
    );
    CoordinatorActor->AddInstanceComponent(Governance);
    Governance->RegisterComponent();
    Governance->bEnableDebugLogging = true;
}

UConversationListenerComponent*
UOpenAINPCConversationSubsystem::EnsureListener(
    UOpenAIJackComponent* Component
)
{
    if (!IsValid(Component) || !IsValid(Component->GetOwner()))
    {
        return nullptr;
    }

    AActor* Owner = Component->GetOwner();
    UConversationListenerComponent* Listener =
        Owner->FindComponentByClass<UConversationListenerComponent>();
    const bool bCreatedListener = !IsValid(Listener);
    if (bCreatedListener)
    {
        Listener = NewObject<UConversationListenerComponent>(
            Owner,
            TEXT("AutonomousConversationListener")
        );
        Owner->AddInstanceComponent(Listener);
        Listener->RegisterComponent();
    }

    if (bCreatedListener || Listener->NPCID.IsNone())
    {
        Listener->NPCID = Component->GetResolvedNPCID();
    }
    if (bCreatedListener || Listener->DisplayName.IsEmpty())
    {
        Listener->DisplayName = FText::FromString(
            Component->GetResolvedNPCID().ToString()
        );
    }
    if (bCreatedListener)
    {
        Listener->ListeningRadius = Component->AutonomousListeningRadius;
        Listener->MaximumQueuedResponses = 1;
    }
    Listener->bListenerEnabled = Component->bEnableAutonomousListening;
    return Listener;
}

UOpenAINPCConversationSubsystem::FRegisteredNPC*
UOpenAINPCConversationSubsystem::FindRegisteredNPC(
    const UOpenAIJackComponent* Component
)
{
    return RegisteredNPCs.FindByPredicate(
        [Component](const FRegisteredNPC& Registration)
        {
            return Registration.Component.Get() == Component;
        }
    );
}

UOpenAIJackComponent*
UOpenAINPCConversationSubsystem::FindComponentForActor(
    const AActor* Actor
) const
{
    for (const FRegisteredNPC& Registration : RegisteredNPCs)
    {
        UOpenAIJackComponent* Component = Registration.Component.Get();
        if (IsValid(Component) && Component->GetOwner() == Actor)
        {
            return Component;
        }
    }
    return nullptr;
}

void UOpenAINPCConversationSubsystem::RemoveInvalidNPCs()
{
    RegisteredNPCs.RemoveAll(
        [](const FRegisteredNPC& Registration)
        {
            return !Registration.Component.IsValid() ||
                !IsValid(Registration.Component->GetOwner());
        }
    );
}

void UOpenAINPCConversationSubsystem::BeginCandidateGeneration()
{
    if (!IsValid(Governance) ||
        !Governance->IsConversationActive() ||
        CurrentPrimaryReply.IsEmpty())
    {
        return;
    }

    int32 RequestsStarted = 0;
    for (FRegisteredNPC& Registration : RegisteredNPCs)
    {
        UOpenAIJackComponent* ListenerComponent =
            Registration.Component.Get();
        UConversationListenerComponent* Listener =
            Registration.Listener.Get();
        if (!IsValid(ListenerComponent) ||
            ListenerComponent == ActivePrimary.Get() ||
            !ListenerComponent->bEnableAutonomousListening ||
            !IsValid(Listener) ||
            !Governance->IsListenerEligible(Listener))
        {
            continue;
        }

        if (RequestsStarted >= MaximumCandidateRequestsPerExchange)
        {
            EmitAutonomyLog(
                TEXT("candidate_gated"),
                FString::Printf(
                    TEXT("npc=%s reason=ExchangeRequestLimit"),
                    *ListenerComponent->GetResolvedNPCID().ToString()
                )
            );
            continue;
        }

        ++RequestsStarted;
        ++PendingCandidateRequests;
        RequestCandidateFrom(ListenerComponent);
    }

    EmitAutonomyLog(
        TEXT("candidate_generation_started"),
        FString::Printf(
            TEXT("requests=%d"),
            RequestsStarted
        )
    );
}

void UOpenAINPCConversationSubsystem::RequestCandidateFrom(
    UOpenAIJackComponent* ListenerComponent
)
{
    if (!IsValid(ListenerComponent))
    {
        PendingCandidateRequests =
            FMath::Max(0, PendingCandidateRequests - 1);
        return;
    }

    const int32 RequestExchangeSerial = ExchangeSerial;
    const FName RequestConversationID = ActiveConversationID;
    const FString Context = BuildCandidateContext(ListenerComponent);
    const FString EmergencyContext = EmergencyEvent.bActive
        ? FString::Printf(
            TEXT("ID=%s; severity=%.2f; description=%s"),
            *EmergencyEvent.EventID.ToString(),
            EmergencyEvent.Severity,
            *EmergencyEvent.Description
        )
        : TEXT("NONE");
    const FString KnowledgeContext =
        ListenerComponent->AutonomousKnowledgeSummary.IsEmpty()
        ? TEXT("No additional designer-authored knowledge.")
        : ListenerComponent->AutonomousKnowledgeSummary;
    const FString InterestContext =
        ListenerComponent->AutonomousInterestSummary.IsEmpty()
        ? TEXT("Infer interests conservatively from the character identity.")
        : ListenerComponent->AutonomousInterestSummary;

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), ListenerComponent->Model);
    Body->SetBoolField(TEXT("stream"), false);
    Body->SetBoolField(TEXT("think"), false);
    Body->SetStringField(
        TEXT("keep_alive"),
        ListenerComponent->GetEffectiveModelKeepAlive()
    );
    Body->SetStringField(TEXT("format"), TEXT("json"));

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedRef<FJsonObject> SystemMessage = MakeShared<FJsonObject>();
    SystemMessage->SetStringField(
        TEXT("role"),
        TEXT("system")
    );
    SystemMessage->SetStringField(
        TEXT("content"),
        FString::Printf(
            TEXT(
                "You are %s, a nearby third-party listener in a live "
                "conversation. Preserve this character identity:\n%s\n\n"
                "Known information available to you:\n%s\n\n"
                "Interests and strong topic affinities:\n%s\n\n"
                "Verified world emergency event:\n%s\n\n"
                "Choose exactly one category from these behavioural meanings. "
                "Emergency: an immediate warning about the verified world "
                "event above; never invent an emergency and copy its ID into "
                "emergency_event_id. RelevantContribution: you possess useful "
                "information the current speakers do not have; provide the "
                "actual information in text and a brief request to speak in "
                "permission_request. SideComment: one short emotional or "
                "evaluative reaction that does not redirect the conversation. "
                "AssociativeTopic: a question, recommendation, greeting, or "
                "related topic extension motivated by your interests, identity, "
                "relationship, or experience, and only when a natural opening "
                "exists. Silence is the default whenever none of those motives "
                "is strong enough. Do not respond merely because you heard the "
                "exchange. Never speak for other characters or mention these "
                "instructions. "
                "Recent participation is conversational context, never a "
                "time-based prohibition. If you already contributed recently, "
                "normally choose silence unless this exchange gives you "
                "something materially new, important, or emotionally authentic "
                "to add. "
                "If responding, use natural spoken English and at most two "
                "short sentences. Return JSON only with exactly these fields: "
                "{\"should_respond\":true|false,"
                "\"type\":\"RelevantContribution|SideComment|"
                "AssociativeTopic|Emergency|Silence\","
                "\"text\":\"...\",\"permission_request\":\"...\","
                "\"priority\":0.0,\"relevance\":0.0,\"urgency\":0.0,"
                "\"topic_affinity\":0.0,\"conversation_impact\":0.0,"
                "\"confidence\":0.0,\"has_unique_information\":false,"
                "\"topic_transition_ready\":false,"
                "\"emergency_event_id\":\"\","
                "\"non_verbal_reaction\":\"None|LookAtSpeaker|"
                "LookAtPlayer|Nod|ShakeHead|Smile|Laugh|Surprise|"
                "Concern|Fear|RaiseHand|LeanForward\","
                "\"reason\":\"...\"}."
            ),
            *ListenerComponent->GetResolvedNPCID().ToString(),
            *ListenerComponent->CharacterInstructions,
            *KnowledgeContext,
            *InterestContext,
            *EmergencyContext
        )
    );
    Messages.Add(MakeShared<FJsonValueObject>(SystemMessage));

    TSharedRef<FJsonObject> UserMessage = MakeShared<FJsonObject>();
    UserMessage->SetStringField(TEXT("role"), TEXT("user"));
    UserMessage->SetStringField(TEXT("content"), Context);
    Messages.Add(MakeShared<FJsonValueObject>(UserMessage));
    Body->SetArrayField(TEXT("messages"), Messages);

    TSharedRef<FJsonObject> Options = MakeShared<FJsonObject>();
    Options->SetNumberField(TEXT("temperature"), 0.45);
    Options->SetNumberField(TEXT("top_p"), 0.8);
    Options->SetNumberField(TEXT("top_k"), 32);
    Options->SetNumberField(TEXT("num_ctx"), ListenerComponent->ContextLength);
    Options->SetNumberField(TEXT("num_gpu"), ListenerComponent->GpuLayers);
    Options->SetNumberField(TEXT("num_predict"), 160);
    Body->SetObjectField(TEXT("options"), Options);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(ListenerComponent->OllamaChatUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(FMath::Clamp(
        ListenerComponent->RequestTimeoutSeconds,
        2.0f,
        12.0f
    ));
    Request->SetContentAsString(Json);

    const TWeakObjectPtr<UOpenAINPCConversationSubsystem> WeakThis(this);
    const TWeakObjectPtr<UOpenAIJackComponent> WeakListener(
        ListenerComponent
    );
    Request->OnProcessRequestComplete().BindLambda(
        [
            WeakThis,
            WeakListener,
            RequestExchangeSerial,
            RequestConversationID
        ](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            UOpenAINPCConversationSubsystem* StrongThis = WeakThis.Get();
            if (!IsValid(StrongThis))
            {
                return;
            }

            FString Payload;
            FString Failure;
            if (!bSucceeded ||
                !Response.IsValid() ||
                Response->GetResponseCode() < 200 ||
                Response->GetResponseCode() >= 300)
            {
                Failure = Response.IsValid()
                    ? FString::Printf(
                        TEXT("Http%d"),
                        Response->GetResponseCode()
                    )
                    : TEXT("RequestFailed");
            }
            else
            {
                Payload = ExtractOllamaMessage(
                    Response->GetContentAsString()
                );
                if (Payload.IsEmpty())
                {
                    Failure = TEXT("MissingOllamaMessage");
                }
            }

            StrongThis->HandleCandidatePayload(
                WeakListener,
                RequestExchangeSerial,
                RequestConversationID,
                Payload,
                Failure
            );
        }
    );

    EmitAutonomyLog(
        TEXT("candidate_requested"),
        FString::Printf(
            TEXT("npc=%s"),
            *ListenerComponent->GetResolvedNPCID().ToString()
        )
    );

    if (!Request->ProcessRequest())
    {
        HandleCandidatePayload(
            WeakListener,
            RequestExchangeSerial,
            RequestConversationID,
            FString(),
            TEXT("RequestStartFailed")
        );
    }
}

FString UOpenAINPCConversationSubsystem::BuildCandidateContext(
    const UOpenAIJackComponent* ListenerComponent
) const
{
    FString Context = TEXT(
        "Recent conversation context (oldest to newest):\n"
    );
    const FName ListenerID = IsValid(ListenerComponent)
        ? ListenerComponent->GetResolvedNPCID()
        : NAME_None;
    int32 RecentParticipationCount = 0;
    for (const FExchangeRecord& Exchange : RecentExchanges)
    {
        Context += FString::Printf(
            TEXT("Player: %s\n%s: %s\n"),
            *Exchange.PlayerText,
            *Exchange.PrimaryNPCID.ToString(),
            *Exchange.PrimaryReply
        );
        if (!Exchange.SecondaryReply.IsEmpty())
        {
            if (Exchange.SecondaryNPCID == ListenerID)
            {
                ++RecentParticipationCount;
            }
            Context += FString::Printf(
                TEXT("%s: %s\n"),
                *Exchange.SecondaryNPCID.ToString(),
                *Exchange.SecondaryReply
            );
        }
    }

    const bool bSpokeInPreviousExchange =
        !RecentExchanges.IsEmpty() &&
        RecentExchanges.Last().SecondaryNPCID == ListenerID &&
        !RecentExchanges.Last().SecondaryReply.IsEmpty();
    Context += FString::Printf(
        TEXT(
            "\nParticipation context for %s:\n"
            "- completed exchanges shown: %d\n"
            "- your recent contributions: %d\n"
            "- you spoke in the immediately previous exchange: %s\n"
            "These facts do not prohibit speech. Use them to judge whether "
            "another contribution would feel natural or repetitive.\n"
        ),
        ListenerID.IsNone() ? TEXT("Listener") : *ListenerID.ToString(),
        RecentExchanges.Num(),
        RecentParticipationCount,
        bSpokeInPreviousExchange ? TEXT("yes") : TEXT("no")
    );

    const UOpenAIJackComponent* Primary = ActivePrimary.Get();
    Context += FString::Printf(
        TEXT(
            "\nCurrent exchange:\nPlayer: %s\n%s: %s\n\n"
            "You are %s. Decide whether to react after %s finishes."
        ),
        *CurrentPlayerText,
        IsValid(Primary)
            ? *Primary->GetResolvedNPCID().ToString()
            : TEXT("Primary NPC"),
        *CurrentPrimaryReply,
        IsValid(ListenerComponent)
            ? *ListenerComponent->GetResolvedNPCID().ToString()
            : TEXT("Listener"),
        IsValid(Primary)
            ? *Primary->GetResolvedNPCID().ToString()
            : TEXT("the primary speaker")
    );
    return Context;
}

void UOpenAINPCConversationSubsystem::HandleCandidatePayload(
    TWeakObjectPtr<UOpenAIJackComponent> ListenerComponent,
    int32 RequestExchangeSerial,
    FName RequestConversationID,
    const FString& Payload,
    const FString& RequestFailure
)
{
    if (RequestExchangeSerial != ExchangeSerial ||
        RequestConversationID != ActiveConversationID)
    {
        return;
    }

    PendingCandidateRequests =
        FMath::Max(0, PendingCandidateRequests - 1);
    UOpenAIJackComponent* Listener = ListenerComponent.Get();
    if (!IsValid(Listener) ||
        !IsValid(Listener->GetOwner()) ||
        !IsValid(Governance) ||
        !Governance->IsConversationActive())
    {
        return;
    }

    if (!RequestFailure.IsEmpty())
    {
        EmitAutonomyLog(
            TEXT("candidate_failed"),
            FString::Printf(
                TEXT("npc=%s reason=%s"),
                *Listener->GetResolvedNPCID().ToString(),
                *RequestFailure
            )
        );
        TryExecuteNextResponse();
        return;
    }

    FOpenAINPCAutonomousDecision Decision;
    FString ParseFailure;
    if (!OpenAINPCAutonomy::ParseDecision(
            Payload,
            Listener->MaximumAutonomousResponseCharacters,
            Decision,
            ParseFailure))
    {
        EmitAutonomyLog(
            TEXT("candidate_failed"),
            FString::Printf(
                TEXT("npc=%s reason=%s payload=\"%s\""),
                *Listener->GetResolvedNPCID().ToString(),
                *ParseFailure,
                *SanitizeLogText(Payload)
            )
        );
        TryExecuteNextResponse();
        return;
    }

    if (!Decision.bShouldRespond)
    {
        if (Decision.NonVerbalReaction !=
            ENonVerbalReactionType::None)
        {
            Governance->RequestNonVerbalReaction(
                Listener->GetOwner(),
                Decision.NonVerbalReaction,
                Decision.ConfidenceScore > 0.0f
                    ? Decision.ConfidenceScore
                    : 0.6f
            );
        }
        EmitAutonomyLog(
            TEXT("candidate_silence"),
            FString::Printf(
                TEXT("npc=%s reason=\"%s\""),
                *Listener->GetResolvedNPCID().ToString(),
                *SanitizeLogText(Decision.Reason)
            )
        );
        TryExecuteNextResponse();
        return;
    }

    FOpenAINPCAutonomyValidationContext ValidationContext;
    ValidationContext.bVerifiedEmergencyActive =
        EmergencyEvent.bActive &&
        EmergencyEvent.Severity >= 0.7f;
    ValidationContext.VerifiedEmergencyEventID =
        EmergencyEvent.EventID;
    FString ValidationFailure;
    if (!OpenAINPCAutonomy::ValidateDecision(
            Decision,
            ValidationContext,
            ValidationFailure))
    {
        EmitAutonomyLog(
            TEXT("candidate_silence"),
            FString::Printf(
                TEXT("npc=%s reason=%s category=%d"),
                *Listener->GetResolvedNPCID().ToString(),
                *ValidationFailure,
                static_cast<int32>(Decision.ResponseType)
            )
        );
        Governance->RequestNonVerbalReaction(
            Listener->GetOwner(),
            ENonVerbalReactionType::LookAtSpeaker,
            0.35f
        );
        TryExecuteNextResponse();
        return;
    }

    if (Decision.ResponseType == EConversationResponseType::Emergency)
    {
        const FString WarningKey = FString::Printf(
            TEXT("%s|%s"),
            *Decision.EmergencyEventID.ToString(),
            *Listener->GetResolvedNPCID().ToString()
        );
        if (DeliveredEmergencyWarnings.Contains(WarningKey))
        {
            EmitAutonomyLog(
                TEXT("candidate_silence"),
                FString::Printf(
                    TEXT("npc=%s reason=EmergencyAlreadyAnnounced event_id=%s"),
                    *Listener->GetResolvedNPCID().ToString(),
                    *Decision.EmergencyEventID.ToString()
                )
            );
            TryExecuteNextResponse();
            return;
        }
    }

    if (ExecutedResponsesThisExchange >=
        MaximumAutonomousResponsesPerExchange)
    {
        EmitAutonomyLog(
            TEXT("candidate_discarded"),
            FString::Printf(
                TEXT("npc=%s reason=ExchangeResponseLimit"),
                *Listener->GetResolvedNPCID().ToString()
            )
        );
        return;
    }

    FCandidateNPCResponse Candidate;
    Candidate.NPCActor = Listener->GetOwner();
    Candidate.ResponseText = Decision.ResponseText;
    Candidate.ResponseType = Decision.ResponseType;
    Candidate.PriorityScore = Decision.PriorityScore;
    Candidate.RelevanceScore = Decision.RelevanceScore;
    Candidate.UrgencyScore = Decision.UrgencyScore;
    Candidate.TopicAffinityScore = Decision.TopicAffinityScore;
    Candidate.TopicShiftScore =
        1.0f - Decision.TopicAffinityScore;
    Candidate.ConversationImpactScore =
        Decision.ConversationImpactScore;
    Candidate.ConfidenceScore = Decision.ConfidenceScore;
    Candidate.bHasUniqueInformation =
        Decision.bHasUniqueInformation;
    Candidate.bTopicTransitionReady =
        Decision.bTopicTransitionReady;
    Candidate.VerifiedEmergencyEventID =
        Decision.EmergencyEventID;
    Candidate.PermissionRequestText =
        Decision.PermissionRequestText;
    Candidate.CreatedTime = GetCurrentTimeSeconds();
    Candidate.ExpiryTime =
        Candidate.CreatedTime + CandidateExpirySeconds;
    Candidate.bRequiresPermission =
        Decision.ResponseType ==
            EConversationResponseType::RelevantContribution;
    Candidate.bCanOverlapMainTurn =
        Decision.ResponseType ==
            EConversationResponseType::Emergency;
    Candidate.bOwnsTurnWhenSpoken =
        Decision.ResponseType !=
            EConversationResponseType::SideComment;

    FGuid ResponseID;
    if (!Governance->SubmitCandidateResponseData(
            Candidate,
            ResponseID))
    {
        EmitAutonomyLog(
            TEXT("candidate_discarded"),
            FString::Printf(
                TEXT("npc=%s reason=GovernanceRejected"),
                *Listener->GetResolvedNPCID().ToString()
            )
        );
        TryExecuteNextResponse();
        return;
    }

    EmitAutonomyLog(
        TEXT("candidate_queued"),
        FString::Printf(
            TEXT(
                "npc=%s type=%d priority=%.2f relevance=%.2f "
                "urgency=%.2f affinity=%.2f impact=%.2f "
                "response_id=%s text=\"%s\""
            ),
            *Listener->GetResolvedNPCID().ToString(),
            static_cast<int32>(Decision.ResponseType),
            Decision.PriorityScore,
            Decision.RelevanceScore,
            Decision.UrgencyScore,
            Decision.TopicAffinityScore,
            Decision.ConversationImpactScore,
            *ResponseID.ToString(EGuidFormats::DigitsWithHyphens),
            *SanitizeLogText(Decision.ResponseText)
        )
    );
    if (Decision.ResponseType ==
        EConversationResponseType::Emergency)
    {
        TryExecuteEmergencyResponse(ResponseID);
        return;
    }
    TryExecuteNextResponse();
}

void UOpenAINPCConversationSubsystem::TickConversation()
{
    RemoveInvalidNPCs();
    if (!IsValid(Governance) ||
        !Governance->IsConversationActive() ||
        ActiveConversationID.IsNone())
    {
        return;
    }

    const double Now = GetCurrentTimeSeconds();
    UOpenAIJackComponent* Primary = ActivePrimary.Get();
    if (bPrimaryTurnStarted &&
        !bPrimaryTurnEnded &&
        IsValid(Primary) &&
        Now - PrimaryReplyReadyTime >= MinimumObservedSpeechSeconds &&
        !Primary->IsConversationOutputActive())
    {
        Governance->NotifyNPCSpeechEnded(Primary->GetOwner());
        bPrimaryTurnEnded = true;
        NaturalPauseReadyTime = Now + NaturalPauseDelaySeconds;
        EmitAutonomyLog(
            TEXT("primary_speech_finished"),
            FString::Printf(
                TEXT("primary=%s pause_until=%.3f"),
                *Primary->GetResolvedNPCID().ToString(),
                NaturalPauseReadyTime
            )
        );
    }

    if (ActiveSecondary.IsValid())
    {
        if (Now - SecondarySpeechStartTime >=
                MinimumObservedSpeechSeconds &&
            !ActiveSecondary->IsConversationOutputActive())
        {
            FinishActiveSecondaryResponse();
        }
        return;
    }

    if (HasPendingSpeakingPermission())
    {
        if (PermissionResponseDeadline > 0.0 &&
            Now >= PermissionResponseDeadline)
        {
            EmitAutonomyLog(
                TEXT("permission_timed_out"),
                FString::Printf(
                    TEXT("response_id=%s"),
                    *Governance->GetPendingPermissionResponse()
                        .ResponseID.ToString(
                            EGuidFormats::DigitsWithHyphens
                        )
                )
            );
            DenyPendingSpeakingPermission();
        }
        return;
    }

    TryExecuteNextResponse();
    if (bPrimaryTurnEnded &&
        PendingCandidateRequests == 0 &&
        Governance->GetQueuedResponses().IsEmpty() &&
        !HasPendingSpeakingPermission() &&
        !bExchangeArchived)
    {
        ArchiveCurrentExchange();
    }
}

bool UOpenAINPCConversationSubsystem::TryExecuteEmergencyResponse(
    FGuid ResponseID
)
{
    if (!ResponseID.IsValid() ||
        !IsValid(Governance) ||
        !IsValid(Governance->GetResponseQueue()))
    {
        return false;
    }

    if (HasPendingSpeakingPermission())
    {
        const FCandidateNPCResponse Pending =
            Governance->GetPendingPermissionResponse();
        Governance->DenyPermission(Pending.ResponseID);
        PermissionResponseDeadline = 0.0;
        EmitAutonomyLog(
            TEXT("permission_preempted"),
            FString::Printf(
                TEXT("npc=%s reason=Emergency"),
                *Pending.NPCID.ToString()
            )
        );
    }

    if (ActiveSecondary.IsValid())
    {
        UOpenAIJackComponent* Interrupted = ActiveSecondary.Get();
        if (ActiveExecutionMode == ESecondaryExecutionMode::Emergency)
        {
            return false;
        }

        const FCandidateNPCResponse InterruptedResponse = ActiveResponse;
        Interrupted->InterruptConversationOutput();
        ReleaseExecutionTurn(
            Interrupted->GetOwner(),
            ActiveExecutionMode
        );
        EmitAutonomyLog(
            TEXT("execution_interrupted"),
            FString::Printf(
                TEXT("npc=%s response_id=%s reason=Emergency"),
                *InterruptedResponse.NPCID.ToString(),
                *InterruptedResponse.ResponseID.ToString(
                    EGuidFormats::DigitsWithHyphens
                )
            )
        );
        ActiveSecondary.Reset();
        ActiveResponse = FCandidateNPCResponse();
        ActiveExecutionMode = ESecondaryExecutionMode::None;
    }
    ExecutedResponsesThisExchange = 0;

    FCandidateNPCResponse Response;
    if (!Governance->GetResponseQueue()->GetResponseByID(
            ResponseID,
            Response) ||
        Response.ResponseType != EConversationResponseType::Emergency ||
        Response.VerifiedEmergencyEventID != EmergencyEvent.EventID)
    {
        return false;
    }

    UOpenAIJackComponent* Primary = ActivePrimary.Get();
    if (IsValid(Primary))
    {
        Primary->InterruptConversationOutput();
        if (bPrimaryTurnStarted && !bPrimaryTurnEnded)
        {
            Governance->NotifyNPCSpeechEnded(Primary->GetOwner());
        }
    }
    bPrimaryTurnEnded = true;
    NaturalPauseReadyTime = GetCurrentTimeSeconds();

    if (!Governance->GetResponseQueue()->RemoveByResponseID(ResponseID) ||
        !StartResponseExecution(
            Response,
            ESecondaryExecutionMode::Emergency,
            Response.ResponseText))
    {
        EmitAutonomyLog(
            TEXT("execution_failed"),
            FString::Printf(
                TEXT("npc=%s reason=EmergencySpeechRejected"),
                *Response.NPCID.ToString()
            )
        );
        return false;
    }

    DeliveredEmergencyWarnings.Add(FString::Printf(
        TEXT("%s|%s"),
        *Response.VerifiedEmergencyEventID.ToString(),
        *Response.NPCID.ToString()
    ));
    DiscardRemainingResponses(TEXT("EmergencySupersededQueue"));
    return true;
}

bool UOpenAINPCConversationSubsystem::StartResponseExecution(
    const FCandidateNPCResponse& Response,
    ESecondaryExecutionMode Mode,
    const FString& SpokenText
)
{
    AActor* NPCActor = Response.NPCActor.Get();
    UOpenAIJackComponent* Component =
        FindComponentForActor(NPCActor);
    const FString TrimmedText = SpokenText.TrimStartAndEnd();
    if (!IsValid(NPCActor) ||
        !IsValid(Component) ||
        Component->IsConversationOutputActive() ||
        TrimmedText.IsEmpty() ||
        !IsValid(Governance))
    {
        return false;
    }

    bool bTurnAccepted = false;
    switch (Mode)
    {
    case ESecondaryExecutionMode::Emergency:
        bTurnAccepted = Governance->BeginEmergencyInterrupt(NPCActor);
        break;
    case ESecondaryExecutionMode::SideComment:
        bTurnAccepted = Governance->BeginSideComment(NPCActor);
        break;
    case ESecondaryExecutionMode::MainResponse:
    case ESecondaryExecutionMode::PermissionPrompt:
        bTurnAccepted = Governance->NotifyNPCSpeechStarted(NPCActor);
        break;
    default:
        return false;
    }
    if (!bTurnAccepted)
    {
        return false;
    }

    ActiveSecondary = Component;
    ActiveResponse = Response;
    ActiveExecutionMode = Mode;
    CurrentSecondaryNPCID = Component->GetResolvedNPCID();
    if (Mode != ESecondaryExecutionMode::PermissionPrompt)
    {
        CurrentSecondaryReply = Response.ResponseText;
    }
    SecondarySpeechStartTime = GetCurrentTimeSeconds();
    ++ExecutedResponsesThisExchange;

    if (!Component->SpeakGovernedText(TrimmedText))
    {
        ReleaseExecutionTurn(NPCActor, Mode);
        ActiveSecondary.Reset();
        ActiveResponse = FCandidateNPCResponse();
        ActiveExecutionMode = ESecondaryExecutionMode::None;
        ExecutedResponsesThisExchange =
            FMath::Max(0, ExecutedResponsesThisExchange - 1);
        return false;
    }

    if (Mode == ESecondaryExecutionMode::PermissionPrompt)
    {
        EmitAutonomyLog(
            TEXT("permission_prompt_started"),
            FString::Printf(
                TEXT("npc=%s response_id=%s text=\"%s\""),
                *Response.NPCID.ToString(),
                *Response.ResponseID.ToString(
                    EGuidFormats::DigitsWithHyphens
                ),
                *SanitizeLogText(TrimmedText)
            )
        );
    }
    else
    {
        Governance->NotifyResponseExecutionStarted(Response);
        EmitAutonomyLog(
            TEXT("execution_started"),
            FString::Printf(
                TEXT("npc=%s type=%d mode=%d text=\"%s\""),
                *Response.NPCID.ToString(),
                static_cast<int32>(Response.ResponseType),
                static_cast<int32>(Mode),
                *SanitizeLogText(TrimmedText)
            )
        );
    }
    return true;
}

void UOpenAINPCConversationSubsystem::ReleaseExecutionTurn(
    AActor* NPCActor,
    ESecondaryExecutionMode Mode
)
{
    if (!IsValid(Governance) || !IsValid(NPCActor))
    {
        return;
    }

    if (Mode == ESecondaryExecutionMode::SideComment)
    {
        Governance->EndSideComment(NPCActor);
    }
    else
    {
        Governance->NotifyNPCSpeechEnded(NPCActor);
    }
}

void UOpenAINPCConversationSubsystem::TryExecuteNextResponse()
{
    if (!bPrimaryTurnEnded ||
        ActiveSecondary.IsValid() ||
        HasPendingSpeakingPermission() ||
        PendingCandidateRequests > 0 ||
        ExecutedResponsesThisExchange >=
            MaximumAutonomousResponsesPerExchange ||
        GetCurrentTimeSeconds() < NaturalPauseReadyTime ||
        !IsValid(Governance) ||
        !IsValid(Governance->GetResponseQueue()))
    {
        return;
    }

    FCandidateNPCResponse Response;
    while (Governance->GetResponseQueue()->PeekNextResponse(Response))
    {
        if (Response.ConversationID != ActiveConversationID ||
            (Response.ExpiryTime > 0.0 &&
             Response.ExpiryTime < GetCurrentTimeSeconds()))
        {
            Governance->GetResponseQueue()->PopNextResponse(Response);
            EmitAutonomyLog(
                TEXT("candidate_discarded"),
                FString::Printf(
                    TEXT("npc=%s reason=ExpiredOrWrongConversation"),
                    *Response.NPCID.ToString()
                )
            );
            continue;
        }

        AActor* NPCActor = Response.NPCActor.Get();
        UOpenAIJackComponent* Component =
            FindComponentForActor(NPCActor);
        if (!IsValid(Component) ||
            !IsValid(NPCActor))
        {
            Governance->GetResponseQueue()->PopNextResponse(Response);
            EmitAutonomyLog(
                TEXT("candidate_discarded"),
                FString::Printf(
                    TEXT("npc=%s reason=InvalidOrBusy"),
                    *Response.NPCID.ToString()
                )
            );
            continue;
        }
        if (Component->IsConversationOutputActive())
        {
            EmitAutonomyLog(
                TEXT("candidate_deferred"),
                FString::Printf(
                    TEXT("npc=%s reason=OutputBusy"),
                    *Response.NPCID.ToString()
                )
            );
            return;
        }

        if (Response.ResponseType ==
                EConversationResponseType::RelevantContribution &&
            Response.bRequiresPermission)
        {
            if (!StartResponseExecution(
                    Response,
                    ESecondaryExecutionMode::PermissionPrompt,
                    Response.PermissionRequestText))
            {
                EmitAutonomyLog(
                    TEXT("candidate_deferred"),
                    FString::Printf(
                        TEXT("npc=%s reason=PermissionPromptUnavailable"),
                        *Response.NPCID.ToString()
                    )
                );
                return;
            }
            DiscardRemainingResponses(
                TEXT("ExchangeResponseLimit"),
                Response.ResponseID
            );
            return;
        }

        Governance->GetResponseQueue()->PopNextResponse(Response);
        const ESecondaryExecutionMode Mode =
            Response.ResponseType ==
                EConversationResponseType::SideComment
            ? ESecondaryExecutionMode::SideComment
            : ESecondaryExecutionMode::MainResponse;
        if (!StartResponseExecution(
                Response,
                Mode,
                Response.ResponseText))
        {
            EmitAutonomyLog(
                TEXT("execution_failed"),
                FString::Printf(
                    TEXT("npc=%s reason=SpeechRejected"),
                    *Response.NPCID.ToString()
                )
            );
            continue;
        }

        DiscardRemainingResponses(TEXT("ExchangeResponseLimit"));
        return;
    }
}

void UOpenAINPCConversationSubsystem::FinishActiveSecondaryResponse()
{
    UOpenAIJackComponent* Component = ActiveSecondary.Get();
    if (!IsValid(Component) || !IsValid(Governance))
    {
        ActiveSecondary.Reset();
        return;
    }

    const FCandidateNPCResponse FinishedResponse = ActiveResponse;
    const ESecondaryExecutionMode FinishedMode = ActiveExecutionMode;
    ReleaseExecutionTurn(Component->GetOwner(), FinishedMode);

    if (FinishedMode == ESecondaryExecutionMode::PermissionPrompt)
    {
        EmitAutonomyLog(
            TEXT("permission_prompt_finished"),
            FString::Printf(
                TEXT("npc=%s response_id=%s"),
                *FinishedResponse.NPCID.ToString(),
                *FinishedResponse.ResponseID.ToString(
                    EGuidFormats::DigitsWithHyphens
                )
            )
        );
        ActiveSecondary.Reset();
        ActiveResponse = FCandidateNPCResponse();
        ActiveExecutionMode = ESecondaryExecutionMode::None;
        if (!Governance->RequestSpeakingPermission(
                FinishedResponse.ResponseID))
        {
            Governance->CancelResponse(FinishedResponse.ResponseID);
            EmitAutonomyLog(
                TEXT("candidate_discarded"),
                FString::Printf(
                    TEXT("npc=%s reason=PermissionStateRejected"),
                    *FinishedResponse.NPCID.ToString()
                )
            );
            ArchiveCurrentExchange();
            return;
        }
        PermissionResponseDeadline =
            GetCurrentTimeSeconds() +
            PermissionResponseTimeoutSeconds;
        return;
    }

    Governance->NotifyResponseExecutionFinished(FinishedResponse);
    EmitAutonomyLog(
        TEXT("execution_finished"),
        FString::Printf(
            TEXT("npc=%s type=%d duration=%.2f"),
            *Component->GetResolvedNPCID().ToString(),
            static_cast<int32>(FinishedResponse.ResponseType),
            GetCurrentTimeSeconds() - SecondarySpeechStartTime
        )
    );
    ActiveSecondary.Reset();
    ActiveResponse = FCandidateNPCResponse();
    ActiveExecutionMode = ESecondaryExecutionMode::None;
    ArchiveCurrentExchange();
}

void UOpenAINPCConversationSubsystem::ArchiveCurrentExchange()
{
    if (bExchangeArchived || CurrentPrimaryReply.IsEmpty())
    {
        return;
    }

    FExchangeRecord Record;
    if (const UOpenAIJackComponent* Primary = ActivePrimary.Get())
    {
        Record.PrimaryNPCID = Primary->GetResolvedNPCID();
    }
    Record.PlayerText = CurrentPlayerText;
    Record.PrimaryReply = CurrentPrimaryReply;
    Record.SecondaryNPCID = CurrentSecondaryNPCID;
    Record.SecondaryReply = CurrentSecondaryReply;
    RecentExchanges.Add(MoveTemp(Record));
    if (RecentExchanges.Num() > MaximumRecentExchanges)
    {
        RecentExchanges.RemoveAt(
            0,
            RecentExchanges.Num() - MaximumRecentExchanges
        );
    }
    bExchangeArchived = true;
}

void UOpenAINPCConversationSubsystem::DiscardRemainingResponses(
    const FString& Reason,
    FGuid ExcludedResponseID
)
{
    if (!IsValid(Governance))
    {
        return;
    }

    const TArray<FCandidateNPCResponse> Remaining =
        Governance->GetQueuedResponses();
    for (const FCandidateNPCResponse& Response : Remaining)
    {
        if (ExcludedResponseID.IsValid() &&
            Response.ResponseID == ExcludedResponseID)
        {
            continue;
        }
        Governance->CancelResponse(Response.ResponseID);
        EmitAutonomyLog(
            TEXT("candidate_discarded"),
            FString::Printf(
                TEXT("npc=%s reason=%s"),
                *Response.NPCID.ToString(),
                *Reason
            )
        );
    }
}

void UOpenAINPCConversationSubsystem::EmitAutonomyLog(
    const FString& EventName,
    const FString& Details
) const
{
    UE_LOG(
        LogTemp,
        Display,
        TEXT(
            "NATURALNPC_AUTONOMY event=%s conversation=%s "
            "exchange=%d timestamp=%.3f %s"
        ),
        *EventName,
        *ActiveConversationID.ToString(),
        ExchangeSerial,
        GetCurrentTimeSeconds(),
        *Details
    );
}

double UOpenAINPCConversationSubsystem::GetCurrentTimeSeconds() const
{
    const UWorld* World = GetWorld();
    return IsValid(World)
        ? static_cast<double>(World->GetTimeSeconds())
        : FPlatformTime::Seconds();
}
