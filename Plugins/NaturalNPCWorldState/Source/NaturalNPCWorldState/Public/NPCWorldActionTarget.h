#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCWorldActionTarget.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class NATURALNPCWORLDSTATE_API UNPCWorldActionTarget : public UInterface
{
    GENERATED_BODY()
};

class NATURALNPCWORLDSTATE_API INPCWorldActionTarget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC World State")
    bool ExecuteNPCWorldAction(
        AActor* NPC,
        FName ActionId,
        const FString& Parameters,
        FString& ResultMessage
    );
};
