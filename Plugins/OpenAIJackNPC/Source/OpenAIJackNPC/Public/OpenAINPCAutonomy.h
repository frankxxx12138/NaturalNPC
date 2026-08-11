#pragma once

#include "ConversationGovernanceTypes.h"
#include "CoreMinimal.h"

struct OPENAIJACKNPC_API FOpenAINPCAutonomousDecision
{
    bool bShouldRespond = false;
    EConversationResponseType ResponseType =
        EConversationResponseType::Silence;
    FString ResponseText;
    FString PermissionRequestText;
    float PriorityScore = 0.0f;
    float RelevanceScore = 0.0f;
    float UrgencyScore = 0.0f;
    float TopicAffinityScore = 0.0f;
    float ConversationImpactScore = 0.0f;
    float ConfidenceScore = 0.0f;
    bool bHasUniqueInformation = false;
    bool bTopicTransitionReady = false;
    FName EmergencyEventID = NAME_None;
    ENonVerbalReactionType NonVerbalReaction =
        ENonVerbalReactionType::None;
    FString Reason;
};

struct OPENAIJACKNPC_API FOpenAINPCAutonomyValidationContext
{
    bool bVerifiedEmergencyActive = false;
    FName VerifiedEmergencyEventID = NAME_None;
};

namespace OpenAINPCAutonomy
{
    OPENAIJACKNPC_API bool ParseDecision(
        const FString& Payload,
        int32 MaximumResponseCharacters,
        FOpenAINPCAutonomousDecision& OutDecision,
        FString& OutFailureReason
    );

    OPENAIJACKNPC_API bool ValidateDecision(
        const FOpenAINPCAutonomousDecision& Decision,
        const FOpenAINPCAutonomyValidationContext& Context,
        FString& OutFailureReason
    );
}
