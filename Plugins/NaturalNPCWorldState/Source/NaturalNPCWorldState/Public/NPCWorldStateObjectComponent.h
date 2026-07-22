#pragma once

#include "Components/ActorComponent.h"
#include "NPCWorldStateTypes.h"
#include "NPCWorldStateObjectComponent.generated.h"

UCLASS(ClassGroup = (NPCWorldState), meta = (BlueprintSpawnableComponent))
class NATURALNPCWORLDSTATE_API UNPCWorldStateObjectComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UNPCWorldStateObjectComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName ObjectId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName Category = TEXT("object");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    TArray<FString> Aliases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    TArray<FNPCWorldActionDefinition> Actions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    TMap<FName, FString> State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    bool bVisibleToNPCs = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    bool bAutoInferVRTemplateActions = true;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FName GetResolvedObjectId() const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    FText GetResolvedDisplayName() const;

    UFUNCTION(BlueprintPure, Category = "NPC World State")
    TArray<FNPCWorldActionDefinition> GetAvailableActions(AActor* Observer) const;

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    void SetWorldStateValue(FName Key, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "NPC World State")
    void RemoveWorldStateValue(FName Key);

    bool HasVRGrabComponent() const;
    void SetRuntimeObjectId(FName Value);
    void NotifyStateChanged();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    FNPCWorldActionDefinition MakeInferredAction(FName ActionId) const;
    bool HasAction(
        const TArray<FNPCWorldActionDefinition>& Source,
        FName ActionId
    ) const;

    UPROPERTY(Transient)
    FName RuntimeObjectId = NAME_None;
};
