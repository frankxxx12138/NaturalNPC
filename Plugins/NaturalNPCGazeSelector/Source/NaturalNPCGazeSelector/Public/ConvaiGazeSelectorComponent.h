#pragma once

#include "Components/ActorComponent.h"
#include "ConvaiGazeSelectorComponent.generated.h"

class UConvaiChatbotComponent;

UCLASS(ClassGroup = (Convai), meta = (BlueprintSpawnableComponent))
class NATURALNPCGAZESELECTOR_API UConvaiGazeSelectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UConvaiGazeSelectorComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze")
    float MaxSelectionDistance = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze",
        meta = (ClampMin = "1.0", ClampMax = "60.0"))
    float SelectionHalfAngleDegrees = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze",
        meta = (ClampMin = "0.02"))
    float SelectionInterval = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze",
        meta = (ClampMin = "0.0"))
    float GazeHoldDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze",
        meta = (ClampMin = "0.0"))
    float SwitchCooldown = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze")
    bool bBlockSwitchDuringConversation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Convai|Gaze")
    bool bRequireLineOfSight = false;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:
    void UpdateGazeSelection();
    UConvaiChatbotComponent* FindBestVisibleChatbot(
        const FVector& ViewLocation,
        const FVector& ViewDirection
    ) const;
    bool HasLineOfSight(
        const FVector& ViewLocation,
        const UConvaiChatbotComponent* Chatbot
    ) const;
    void SwitchToChatbot(UConvaiChatbotComponent* Chatbot);

    float TimeUntilNextSelection = 0.0f;
    float CandidateGazeStartTime = 0.0f;
    float LastSwitchTime = -1000.0f;

    UPROPERTY(Transient)
    TObjectPtr<UConvaiChatbotComponent> SelectedChatbot;

    UPROPERTY(Transient)
    TObjectPtr<UConvaiChatbotComponent> CandidateChatbot;
};
