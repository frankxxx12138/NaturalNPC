#pragma once

#include "CoreMinimal.h"

class UOpenAIJackComponent;
class UWorld;

namespace OpenAINPCRouter
{
    OPENAIJACKNPC_API bool ContainsNameMention(
        const FString& PlayerText,
        const FString& CandidateName
    );

    OPENAIJACKNPC_API bool IsDirectAddress(
        const FString& PlayerText,
        const FString& CandidateName
    );

    OPENAIJACKNPC_API UOpenAIJackComponent* FindUniqueNPC(
        UWorld* World,
        FName NPCID,
        int32& OutMatchCount
    );

    OPENAIJACKNPC_API FString DescribeAvailableNPCs(UWorld* World);
}
