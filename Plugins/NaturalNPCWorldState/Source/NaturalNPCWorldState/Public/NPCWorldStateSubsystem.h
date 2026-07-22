#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "NPCWorldStateTypes.h"
#include "NPCWorldStateSubsystem.generated.h"

class UNPCWorldStateObjectComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FNPCWorldObjectEvent,
    FName,
    ObjectId,
    AActor*,
    Actor
);

UCLASS()
class NATURALNPCWORLDSTATE_API UNPCWorldStateSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldObjectEvent OnWorldObjectAdded;

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldObjectEvent OnWorldObjectRemoved;

    UPROPERTY(BlueprintAssignable, Category = "NPC World State")
    FNPCWorldObjectEvent OnWorldObjectChanged;

    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

    void RegisterWorldObject(UNPCWorldStateObjectComponent* Component);
    void UnregisterWorldObject(UNPCWorldStateObjectComponent* Component);
    void NotifyWorldObjectChanged(UNPCWorldStateObjectComponent* Component);

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    void RefreshAutoDiscovery();

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    UNPCWorldStateObjectComponent* FindWorldObject(FName ObjectId) const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    TArray<FNPCWorldObjectState> GetWorldState(
        AActor* Observer,
        float Radius = 0.0f
    ) const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FString BuildWorldStateJson(AActor* Observer, float Radius = 0.0f) const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FString BuildWorldStateText(AActor* Observer, float Radius = 0.0f) const;

private:
    FName MakeUniqueObjectId(FName RequestedId) const;
    bool ShouldAutoDiscoverActor(const AActor* Actor) const;
    bool ShouldAutoAttachAgent(const AActor* Actor) const;
    void ConfigureRuntimeComponent(
        AActor* Actor,
        UNPCWorldStateObjectComponent* Component
    ) const;

    TMap<FName, TWeakObjectPtr<UNPCWorldStateObjectComponent>> Registry;

};
