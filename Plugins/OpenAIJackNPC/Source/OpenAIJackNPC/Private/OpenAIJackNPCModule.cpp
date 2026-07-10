#include "OpenAIJackComponent.h"

#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "Modules/ModuleManager.h"

class FOpenAIJackNPCModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        ConsoleCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.Say"),
            TEXT("Send text to Jack. Usage: Jack.Say hello"),
            FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSay
            ),
            ECVF_Default
        );
        ClearCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.Clear"),
            TEXT("Clear Jack's conversation memory."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackClear
            ),
            ECVF_Default
        );
        ACEWavCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.ACEWav"),
            TEXT("Animate Jack with ACE from a WAV file. Usage: Jack.ACEWav D:\\voice.wav"),
            FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackACEWav
            ),
            ECVF_Default
        );
        STTStartCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTStart"),
            TEXT("Start Windows speech recognition for Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTStart
            ),
            ECVF_Default
        );
        STTStopCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTStop"),
            TEXT("Stop Windows speech recognition for Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTStop
            ),
            ECVF_Default
        );
        STTToggleCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTToggle"),
            TEXT("Toggle Windows speech recognition for Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTToggle
            ),
            ECVF_Default
        );
    }

    virtual void ShutdownModule() override
    {
        if (ConsoleCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(ConsoleCommand);
            ConsoleCommand = nullptr;
        }
        if (ClearCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(ClearCommand);
            ClearCommand = nullptr;
        }
        if (ACEWavCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(ACEWavCommand);
            ACEWavCommand = nullptr;
        }
        if (STTStartCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(STTStartCommand);
            STTStartCommand = nullptr;
        }
        if (STTStopCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(STTStopCommand);
            STTStopCommand = nullptr;
        }
        if (STTToggleCommand)
        {
            IConsoleManager::Get().UnregisterConsoleObject(STTToggleCommand);
            STTToggleCommand = nullptr;
        }
    }

private:
    UOpenAIJackComponent* FindJack(UWorld* World) const
    {
        if (!IsValid(World))
        {
            return nullptr;
        }

        for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
        {
            if (UOpenAIJackComponent* Jack =
                ActorIterator->FindComponentByClass<UOpenAIJackComponent>())
            {
                return Jack;
            }
        }
        return nullptr;
    }

    void HandleJackSay(const TArray<FString>& Args, UWorld* World)
    {
        if (!IsValid(World) || Args.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Usage: Jack.Say <message>"));
            return;
        }

        const FString Message = FString::Join(Args, TEXT(" "));
        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->SendPlayerText(Message);
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    void HandleJackClear(UWorld* World)
    {
        if (!IsValid(World))
        {
            return;
        }

        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->ClearConversation();
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    void HandleJackACEWav(const TArray<FString>& Args, UWorld* World)
    {
        if (!IsValid(World) || Args.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Usage: Jack.ACEWav <wav_path>"));
            return;
        }

        const FString WavPath = FString::Join(Args, TEXT(" "));
        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->AnimateWavFileWithACE(WavPath);
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    void HandleJackSTTStart(UWorld* World)
    {
        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->StartWindowsSTT();
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    void HandleJackSTTStop(UWorld* World)
    {
        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->StopWindowsSTT();
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    void HandleJackSTTToggle(UWorld* World)
    {
        if (UOpenAIJackComponent* Jack = FindJack(World))
        {
            Jack->ToggleWindowsSTT();
            return;
        }

        UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR Jack component not found"));
    }

    IConsoleObject* ConsoleCommand = nullptr;
    IConsoleObject* ClearCommand = nullptr;
    IConsoleObject* ACEWavCommand = nullptr;
    IConsoleObject* STTStartCommand = nullptr;
    IConsoleObject* STTStopCommand = nullptr;
    IConsoleObject* STTToggleCommand = nullptr;
};

IMPLEMENT_MODULE(FOpenAIJackNPCModule, OpenAIJackNPC)
