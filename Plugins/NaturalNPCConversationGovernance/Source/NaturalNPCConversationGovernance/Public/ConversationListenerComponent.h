#pragma once

#include "Components/ActorComponent.h"
#include "ConversationListenerComponent.generated.h"

UCLASS(
    ClassGroup = (ConversationGovernance),
    meta = (BlueprintSpawnableComponent)
)
class NATURALNPCCONVERSATIONGOVERNANCE_API UConversationListenerComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UConversationListenerComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Listener")
    FName NPCID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Listener")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Listener",
        meta = (ClampMin = "0.0", Units = "cm"))
    float ListeningRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Participation",
        meta = (ClampMin = "1", ClampMax = "32"))
    int32 MaximumQueuedResponses = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Participation")
    bool bSideCommentsEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Participation")
    bool bPermissionRequestsEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Participation")
    bool bEmergencyInterruptionEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Conversation Governance|Listener")
    bool bListenerEnabled = true;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Listener")
    FName GetResolvedNPCID() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Listener")
    FText GetResolvedDisplayName() const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Listener")
    bool CanHearActor(const AActor* SoundSource) const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Listener")
    float GetDistanceToActor(const AActor* OtherActor) const;
};
