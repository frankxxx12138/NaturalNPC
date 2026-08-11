#include "ConversationTurnManager.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "HAL/PlatformTime.h"

void UConversationTurnManager::Initialize(
    FName InConversationID,
    AActor* InPrimaryNPC
)
{
    ConversationID = InConversationID;
    PrimaryNPC = InPrimaryNPC;
    CurrentTurnOwner.Reset();
    CurrentSpeaker.Reset();
    TurnState = EConversationTurnState::Idle;
    SpeechStartTime = 0.0;
    SpeechEndTime = 0.0;
    bSideCommentPlaying = false;
}

void UConversationTurnManager::Reset()
{
    SetTurn(nullptr, nullptr, EConversationTurnState::Idle);
    ConversationID = NAME_None;
    PrimaryNPC.Reset();
    SpeechStartTime = 0.0;
    SpeechEndTime = 0.0;
    bSideCommentPlaying = false;
}

bool UConversationTurnManager::NotifyPlayerSpeechStarted(AActor* PlayerActor)
{
    return TryBeginSpeech(
        PlayerActor,
        EConversationTurnState::PlayerSpeaking
    );
}

bool UConversationTurnManager::NotifyPlayerSpeechEnded(AActor* PlayerActor)
{
    return TryEndSpeech(PlayerActor);
}

bool UConversationTurnManager::NotifyNPCSpeechStarted(AActor* NPCActor)
{
    if (!IsValid(NPCActor))
    {
        return false;
    }

    const EConversationTurnState NewState =
        NPCActor == PrimaryNPC.Get()
            ? EConversationTurnState::PrimaryNPCSpeaking
            : EConversationTurnState::SecondaryNPCSpeaking;
    return TryBeginSpeech(NPCActor, NewState);
}

bool UConversationTurnManager::NotifyNPCSpeechEnded(AActor* NPCActor)
{
    return TryEndSpeech(NPCActor);
}

bool UConversationTurnManager::BeginSideComment(AActor* NPCActor)
{
    if (!IsValid(NPCActor) ||
        ConversationID.IsNone() ||
        CurrentSpeaker.IsValid() ||
        bSideCommentPlaying)
    {
        return false;
    }

    bSideCommentPlaying = true;
    SpeechStartTime = GetCurrentTimeSeconds();
    SetTurn(
        CurrentTurnOwner.Get(),
        NPCActor,
        EConversationTurnState::TransitionPause
    );
    return true;
}

bool UConversationTurnManager::EndSideComment(AActor* NPCActor)
{
    if (!bSideCommentPlaying || CurrentSpeaker.Get() != NPCActor)
    {
        return false;
    }

    bSideCommentPlaying = false;
    SpeechEndTime = GetCurrentTimeSeconds();
    SetTurn(nullptr, nullptr, EConversationTurnState::Idle);
    return true;
}

bool UConversationTurnManager::BeginEmergencyInterrupt(AActor* NPCActor)
{
    if (!IsValid(NPCActor) ||
        ConversationID.IsNone() ||
        !bInterruptionAllowed)
    {
        return false;
    }

    bSideCommentPlaying = false;
    SpeechStartTime = GetCurrentTimeSeconds();
    SetTurn(
        NPCActor,
        NPCActor,
        EConversationTurnState::EmergencyInterrupt
    );
    return true;
}

bool UConversationTurnManager::EnterPermissionPending()
{
    if (ConversationID.IsNone() || CurrentSpeaker.IsValid())
    {
        return false;
    }

    SetTurn(nullptr, nullptr, EConversationTurnState::PermissionPending);
    return true;
}

bool UConversationTurnManager::ResolvePermissionPending()
{
    if (TurnState != EConversationTurnState::PermissionPending)
    {
        return false;
    }

    SetTurn(nullptr, nullptr, EConversationTurnState::Idle);
    return true;
}

AActor* UConversationTurnManager::GetCurrentTurnOwner() const
{
    return CurrentTurnOwner.Get();
}

AActor* UConversationTurnManager::GetCurrentSpeaker() const
{
    return CurrentSpeaker.Get();
}

bool UConversationTurnManager::IsMainTurnAvailable() const
{
    return TurnState == EConversationTurnState::Idle &&
        !CurrentTurnOwner.IsValid() &&
        !CurrentSpeaker.IsValid();
}

bool UConversationTurnManager::TryBeginSpeech(
    AActor* Speaker,
    EConversationTurnState NewState
)
{
    if (!IsValid(Speaker) || ConversationID.IsNone())
    {
        return false;
    }

    if (CurrentSpeaker.IsValid())
    {
        return false;
    }

    SpeechStartTime = GetCurrentTimeSeconds();
    SetTurn(Speaker, Speaker, NewState);
    return true;
}

bool UConversationTurnManager::TryEndSpeech(AActor* Speaker)
{
    if (!IsValid(Speaker) || CurrentSpeaker.Get() != Speaker)
    {
        return false;
    }

    SpeechEndTime = GetCurrentTimeSeconds();
    SetTurn(nullptr, nullptr, EConversationTurnState::Idle);
    return true;
}

void UConversationTurnManager::SetTurn(
    AActor* NewOwner,
    AActor* NewSpeaker,
    EConversationTurnState NewState
)
{
    AActor* PreviousOwner = CurrentTurnOwner.Get();
    const EConversationTurnState PreviousState = TurnState;

    CurrentTurnOwner = NewOwner;
    CurrentSpeaker = NewSpeaker;
    TurnState = NewState;

    if (PreviousOwner != NewOwner || PreviousState != NewState)
    {
        OnTurnOwnerChanged.Broadcast(
            PreviousOwner,
            NewOwner,
            NewState
        );
    }
}

double UConversationTurnManager::GetCurrentTimeSeconds() const
{
    const UWorld* World = GetWorld();
    return IsValid(World)
        ? static_cast<double>(World->GetTimeSeconds())
        : FPlatformTime::Seconds();
}
