#pragma once

#include "ConversationGovernanceTypes.h"
#include "UObject/Object.h"
#include "ConversationTurnManager.generated.h"

UCLASS(BlueprintType)
class NATURALNPCCONVERSATIONGOVERNANCE_API UConversationTurnManager
    : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Conversation Governance|Turn")
    FConversationTurnOwnerChangedEvent OnTurnOwnerChanged;

    void Initialize(FName InConversationID, AActor* InPrimaryNPC);
    void Reset();

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

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool EnterPermissionPending();

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Turn")
    bool ResolvePermissionPending();

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    AActor* GetCurrentTurnOwner() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    AActor* GetCurrentSpeaker() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    EConversationTurnState GetTurnState() const { return TurnState; }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    bool IsMainTurnAvailable() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    double GetSpeechStartTime() const { return SpeechStartTime; }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Turn")
    double GetSpeechEndTime() const { return SpeechEndTime; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Turn",
        meta = (ClampMin = "0.0", Units = "s"))
    float MinimumTransitionPauseSeconds = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Turn")
    bool bInterruptionAllowed = true;

    UPROPERTY(BlueprintReadOnly, Category = "Conversation Governance|Turn")
    bool bSideCommentPlaying = false;

private:
    bool TryBeginSpeech(
        AActor* Speaker,
        EConversationTurnState NewState
    );
    bool TryEndSpeech(AActor* Speaker);
    void SetTurn(
        AActor* NewOwner,
        AActor* NewSpeaker,
        EConversationTurnState NewState
    );
    double GetCurrentTimeSeconds() const;

    FName ConversationID = NAME_None;
    TWeakObjectPtr<AActor> PrimaryNPC;
    TWeakObjectPtr<AActor> CurrentTurnOwner;
    TWeakObjectPtr<AActor> CurrentSpeaker;
    EConversationTurnState TurnState = EConversationTurnState::Idle;
    double SpeechStartTime = 0.0;
    double SpeechEndTime = 0.0;
};
