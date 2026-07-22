#pragma once

#include "CoreMinimal.h"
#include "NPCWorldStateTypes.generated.h"

class AActor;
class UAnimSequence;

USTRUCT(BlueprintType)
struct NATURALNPCWORLDSTATE_API FNPCWorldItemAnimationProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    FName Category = TEXT("object");

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> PickupAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> HeldIdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> DropAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> ThrowAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation",
        meta = (ClampMin = "-1.0", ClampMax = "1.0"))
    float PickupEffectTriggerNormalizedTime = -1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    bool bCenterObjectBetweenHands = false;
};

USTRUCT(BlueprintType)
struct NATURALNPCWORLDSTATE_API FNPCWorldActionDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName ActionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    TArray<FString> UtteranceHints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State",
        meta = (ClampMin = "0.0"))
    float MaxDistance = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FName AttachSocket = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FVector AttachLocationOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC World State")
    FRotator AttachRotationOffset = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> ActionAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    TSoftObjectPtr<UAnimSequence> HeldIdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation",
        meta = (ClampMin = "0.01"))
    float AnimationPlayRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation",
        meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EffectTriggerNormalizedTime = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "NPC World State|Animation")
    bool bPlayAnimationInReverse = false;
};

USTRUCT(BlueprintType)
struct NATURALNPCWORLDSTATE_API FNPCWorldObjectState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FName ObjectId = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FText DisplayName;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FName Category = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    TObjectPtr<AActor> Actor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    float Distance = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    TMap<FName, FString> State;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    TArray<FNPCWorldActionDefinition> AvailableActions;
};

USTRUCT(BlueprintType)
struct NATURALNPCWORLDSTATE_API FNPCWorldActionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    bool bSuccess = false;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FName ObjectId = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FName ActionId = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "NPC World State")
    FString Message;
};
