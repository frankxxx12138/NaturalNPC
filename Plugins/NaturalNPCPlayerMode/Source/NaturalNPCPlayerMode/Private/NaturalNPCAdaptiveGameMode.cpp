#include "NaturalNPCAdaptiveGameMode.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ANaturalNPCAdaptiveGameMode::ANaturalNPCAdaptiveGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> VRPawnFinder(
        TEXT("/Game/XRFramework/Blueprints/BP_XRPawn")
    );
    static ConstructorHelpers::FClassFinder<APawn> DesktopPawnFinder(
        TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter")
    );
    static ConstructorHelpers::FClassFinder<APlayerController> DesktopControllerFinder(
        TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonPlayerController")
    );

    VRPawnClass = VRPawnFinder.Class;
    DesktopPawnClass = DesktopPawnFinder.Class;
    VRPlayerControllerClass = APlayerController::StaticClass();
    DesktopPlayerControllerClass = DesktopControllerFinder.Class;

    DefaultPawnClass = DesktopPawnClass;
    PlayerControllerClass = DesktopPlayerControllerClass;
}

void ANaturalNPCAdaptiveGameMode::InitGame(
    const FString& MapName,
    const FString& Options,
    FString& ErrorMessage
)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    ResolvePlayerMode(Options);

    DefaultPawnClass = GetSelectedPawnClass();
    PlayerControllerClass = GetSelectedPlayerControllerClass();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("NATURALNPC_PLAYER_MODE selected=%s hmd_enabled=%d pawn=%s controller=%s"),
        IsUsingVRPlayer() ? TEXT("vr") : TEXT("desktop"),
        UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() ? 1 : 0,
        *GetNameSafe(DefaultPawnClass),
        *GetNameSafe(PlayerControllerClass)
    );
}

UClass* ANaturalNPCAdaptiveGameMode::GetDefaultPawnClassForController_Implementation(
    AController* InController
)
{
    if (const TSubclassOf<APawn> SelectedClass = GetSelectedPawnClass())
    {
        return SelectedClass.Get();
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APlayerController* ANaturalNPCAdaptiveGameMode::SpawnPlayerController(
    ENetRole InRemoteRole,
    const FString& Options
)
{
    if (Options.Contains(TEXT("SpectatorOnly=1")) &&
        ReplaySpectatorPlayerControllerClass != nullptr)
    {
        return SpawnPlayerControllerCommon(
            InRemoteRole,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            ReplaySpectatorPlayerControllerClass
        );
    }

    TSubclassOf<APlayerController> SelectedClass =
        GetSelectedPlayerControllerClass();
    if (!SelectedClass)
    {
        SelectedClass = APlayerController::StaticClass();
    }
    return SpawnPlayerControllerCommon(
        InRemoteRole,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SelectedClass
    );
}

TSubclassOf<APlayerController>
ANaturalNPCAdaptiveGameMode::GetPlayerControllerClassToSpawnForSeamlessTravel(
    APlayerController* PreviousPlayerController
)
{
    if (PreviousPlayerController &&
        PreviousPlayerController->PlayerState &&
        PreviousPlayerController->PlayerState->IsOnlyASpectator())
    {
        return Super::GetPlayerControllerClassToSpawnForSeamlessTravel(
            PreviousPlayerController
        );
    }

    const TSubclassOf<APlayerController> SelectedClass =
        GetSelectedPlayerControllerClass();
    if (SelectedClass)
    {
        return SelectedClass;
    }
    return TSubclassOf<APlayerController>(APlayerController::StaticClass());
}

bool ANaturalNPCAdaptiveGameMode::IsUsingVRPlayer() const
{
    return ResolvedPlayerMode == ENaturalNPCPlayerMode::VR;
}

ENaturalNPCPlayerMode ANaturalNPCAdaptiveGameMode::ParsePlayerModeOption(
    const FString& Value
)
{
    if (Value.Equals(TEXT("VR"), ESearchCase::IgnoreCase))
    {
        return ENaturalNPCPlayerMode::VR;
    }
    if (Value.Equals(TEXT("Desktop"), ESearchCase::IgnoreCase) ||
        Value.Equals(TEXT("FirstPerson"), ESearchCase::IgnoreCase))
    {
        return ENaturalNPCPlayerMode::Desktop;
    }
    return ENaturalNPCPlayerMode::Auto;
}

void ANaturalNPCAdaptiveGameMode::ResolvePlayerMode(const FString& Options)
{
    const ENaturalNPCPlayerMode URLMode = ParsePlayerModeOption(
        UGameplayStatics::ParseOption(Options, TEXT("PlayerMode"))
    );
    if (URLMode != ENaturalNPCPlayerMode::Auto)
    {
        ResolvedPlayerMode = URLMode;
        return;
    }

    if (PlayerMode != ENaturalNPCPlayerMode::Auto)
    {
        ResolvedPlayerMode = PlayerMode;
        return;
    }

    ResolvedPlayerMode =
        UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()
        ? ENaturalNPCPlayerMode::VR
        : ENaturalNPCPlayerMode::Desktop;
}

TSubclassOf<APawn> ANaturalNPCAdaptiveGameMode::GetSelectedPawnClass() const
{
    return IsUsingVRPlayer() ? VRPawnClass : DesktopPawnClass;
}

TSubclassOf<APlayerController>
ANaturalNPCAdaptiveGameMode::GetSelectedPlayerControllerClass() const
{
    return IsUsingVRPlayer()
        ? VRPlayerControllerClass
        : DesktopPlayerControllerClass;
}
