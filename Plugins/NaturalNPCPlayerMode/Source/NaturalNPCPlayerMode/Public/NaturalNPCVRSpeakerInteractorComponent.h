#pragma once

#include "Components/ActorComponent.h"
#include "UObject/SoftObjectPtr.h"
#include "NaturalNPCVRSpeakerInteractorComponent.generated.h"

class APlayerController;
class UInputAction;
class UPrimitiveComponent;
class USceneComponent;

UCLASS(ClassGroup = (NaturalNPC), meta = (BlueprintSpawnableComponent))
class NATURALNPCPLAYERMODE_API UNaturalNPCVRSpeakerInteractorComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UNaturalNPCVRSpeakerInteractorComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker")
    bool bSpeakerInteractionEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Input",
        meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float TriggerThreshold = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Targeting",
        meta = (ClampMin = "2.0", ClampMax = "30.0", Units = "cm"))
    float TouchRadius = 9.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Touch",
        meta = (DisplayName = "Enable Speaker Touch Targeting",
            ToolTip = "Allows a nearby hand to select a speaker button. Pressing the trigger is still required to activate it."))
    bool bActivateSpeakerButtonsOnTouch = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Touch",
        meta = (ClampMin = "0.0", ClampMax = "30.0", Units = "cm"))
    float TouchForwardOffset = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Targeting",
        meta = (ClampMin = "50.0", ClampMax = "500.0", Units = "cm"))
    float PointerDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Targeting",
        meta = (ClampMin = "2.0", ClampMax = "25.0", Units = "cm"))
    float PointerRadius = 9.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Input")
    TSoftObjectPtr<UInputAction> SpeakerInteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Input")
    TSoftObjectPtr<UInputAction> LeftTriggerFallbackAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Speaker|Input")
    TSoftObjectPtr<UInputAction> RightTriggerFallbackAction;

    /** Returns true only for a fresh trigger press while a button is targeted. */
    static bool ShouldActivateTarget(
        bool bTriggerDown,
        bool bWasTriggerDown,
        bool bHasTarget
    );

private:
    float ReadActionValue(
        APlayerController* Controller,
        const TSoftObjectPtr<UInputAction>& ActionReference
    ) const;
    float ReadTrigger(
        APlayerController* Controller,
        bool bRightHand
    ) const;
    void ResolveControllerOrigins();
    UPrimitiveComponent* FindTouchButton(
        USceneComponent* Aim,
        USceneComponent* Grip
    ) const;
    UPrimitiveComponent* FindTargetButton(
        USceneComponent* Aim,
        USceneComponent* Grip
    ) const;
    bool ActivateButton(UPrimitiveComponent* Button) const;
    static FName FunctionForButton(const UPrimitiveComponent* Button);

    TWeakObjectPtr<USceneComponent> LeftAim;
    TWeakObjectPtr<USceneComponent> LeftGrip;
    TWeakObjectPtr<USceneComponent> RightAim;
    TWeakObjectPtr<USceneComponent> RightGrip;
    bool bLeftTriggerWasDown = false;
    bool bRightTriggerWasDown = false;
};
