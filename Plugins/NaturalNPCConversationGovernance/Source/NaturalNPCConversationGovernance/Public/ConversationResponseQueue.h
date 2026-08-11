#pragma once

#include "ConversationGovernanceTypes.h"
#include "UObject/Object.h"
#include "ConversationResponseQueue.generated.h"

UCLASS(BlueprintType)
class NATURALNPCCONVERSATIONGOVERNANCE_API UConversationResponseQueue
    : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(int32 InMaximumTotalEntries);

    bool Enqueue(
        const FCandidateNPCResponse& Response,
        int32 MaximumEntriesForNPC,
        FString& OutFailureReason
    );

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    bool RemoveByResponseID(FGuid ResponseID);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    int32 RemoveAllFromNPC(FName NPCID);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    int32 RemoveAllFromConversation(FName ConversationID);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    bool ReprioritizeResponse(FGuid ResponseID, float NewPriorityScore);

    UFUNCTION(BlueprintCallable, Category = "Conversation Governance|Queue")
    bool PopNextResponse(FCandidateNPCResponse& OutResponse);

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    bool PeekNextResponse(FCandidateNPCResponse& OutResponse) const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    bool GetResponseByID(
        FGuid ResponseID,
        FCandidateNPCResponse& OutResponse
    ) const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    TArray<FCandidateNPCResponse> GetQueuedResponses() const
    {
        return Responses;
    }

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    bool HasResponseFromNPC(FName NPCID) const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    int32 CountResponsesFromNPC(FName NPCID) const;

    UFUNCTION(BlueprintPure, Category = "Conversation Governance|Queue")
    int32 Num() const { return Responses.Num(); }

    void Reset();

private:
    void SortByPriority();

    UPROPERTY(Transient)
    TArray<FCandidateNPCResponse> Responses;

    int32 MaximumTotalEntries = 32;
};
