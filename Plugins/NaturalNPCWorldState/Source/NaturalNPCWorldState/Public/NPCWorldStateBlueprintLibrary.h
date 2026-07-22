#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "NPCWorldStateBlueprintLibrary.generated.h"

class UNPCWorldStateSubsystem;
class UNPCWorldStateAgentComponent;
class AActor;

UCLASS()
class NATURALNPCWORLDSTATE_API UNPCWorldStateBlueprintLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "NPC World State",
        meta = (WorldContext = "WorldContextObject"))
    static UNPCWorldStateSubsystem* GetNPCWorldStateSubsystem(
        const UObject* WorldContextObject
    );

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    static UNPCWorldStateAgentComponent* EnsureNPCWorldStateAgent(
        AActor* Actor
    );
};
