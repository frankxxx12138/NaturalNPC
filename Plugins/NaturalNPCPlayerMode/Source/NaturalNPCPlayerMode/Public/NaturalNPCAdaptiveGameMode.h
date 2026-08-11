#pragma once

#include "GameFramework/GameModeBase.h"
#include "NaturalNPCAdaptiveGameMode.generated.h"

UENUM(BlueprintType)
enum class ENaturalNPCPlayerMode : uint8
{
    Auto,
    VR,
    Desktop
};

UCLASS()
class NATURALNPCPLAYERMODE_API ANaturalNPCAdaptiveGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANaturalNPCAdaptiveGameMode();

    virtual void InitGame(
        const FString& MapName,
        const FString& Options,
        FString& ErrorMessage
    ) override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(
        AController* InController
    ) override;

    virtual APlayerController* SpawnPlayerController(
        ENetRole InRemoteRole,
        const FString& Options
    ) override;

    virtual TSubclassOf<APlayerController>
        GetPlayerControllerClassToSpawnForSeamlessTravel(
            APlayerController* PreviousPlayerController
        ) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    ENaturalNPCPlayerMode PlayerMode = ENaturalNPCPlayerMode::Auto;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    TSubclassOf<APawn> VRPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    TSubclassOf<APawn> DesktopPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    TSubclassOf<APlayerController> VRPlayerControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Mode")
    TSubclassOf<APlayerController> DesktopPlayerControllerClass;

    UFUNCTION(BlueprintPure, Category = "Player Mode")
    bool IsUsingVRPlayer() const;

    static ENaturalNPCPlayerMode ParsePlayerModeOption(const FString& Value);

private:
    void ResolvePlayerMode(const FString& Options);
    TSubclassOf<APawn> GetSelectedPawnClass() const;
    TSubclassOf<APlayerController> GetSelectedPlayerControllerClass() const;

    ENaturalNPCPlayerMode ResolvedPlayerMode = ENaturalNPCPlayerMode::Desktop;
};
