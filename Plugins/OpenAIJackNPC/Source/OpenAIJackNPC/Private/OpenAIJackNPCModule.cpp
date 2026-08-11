#include "OpenAIJackComponent.h"
#include "OpenAINPCRouter.h"

#include "HAL/IConsoleManager.h"
#include "Modules/ModuleManager.h"

class FOpenAIJackNPCModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        ConsoleCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.Say"),
            TEXT("Send text to the NPC whose ID is Jack."),
            FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSay
            ),
            ECVF_Default
        );
        NPCSayCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("NPC.Say"),
            TEXT("Send text to an NPC. Usage: NPC.Say <NPCID> <message>"),
            FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleNPCSay
            ),
            ECVF_Default
        );
        ClearCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.Clear"),
            TEXT("Clear the NPC with ID Jack's conversation memory."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackClear
            ),
            ECVF_Default
        );
        ACEWavCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.ACEWav"),
            TEXT("Animate the NPC with ID Jack from a WAV file."),
            FConsoleCommandWithWorldAndArgsDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackACEWav
            ),
            ECVF_Default
        );
        STTStartCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTStart"),
            TEXT("Start speech recognition for the NPC with ID Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTStart
            ),
            ECVF_Default
        );
        STTStopCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTStop"),
            TEXT("Stop speech recognition for the NPC with ID Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTStop
            ),
            ECVF_Default
        );
        STTToggleCommand = IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("Jack.STTToggle"),
            TEXT("Toggle speech recognition for the NPC with ID Jack."),
            FConsoleCommandWithWorldDelegate::CreateRaw(
                this,
                &FOpenAIJackNPCModule::HandleJackSTTToggle
            ),
            ECVF_Default
        );
    }

    virtual void ShutdownModule() override
    {
        UnregisterCommand(ConsoleCommand);
        UnregisterCommand(NPCSayCommand);
        UnregisterCommand(ClearCommand);
        UnregisterCommand(ACEWavCommand);
        UnregisterCommand(STTStartCommand);
        UnregisterCommand(STTStopCommand);
        UnregisterCommand(STTToggleCommand);
    }

private:
    static void UnregisterCommand(IConsoleObject*& Command)
    {
        if (Command)
        {
            IConsoleManager::Get().UnregisterConsoleObject(Command);
            Command = nullptr;
        }
    }

    UOpenAIJackComponent* ResolveNPC(
        UWorld* World,
        FName NPCID,
        const TCHAR* CommandName
    ) const
    {
        int32 MatchCount = 0;
        UOpenAIJackComponent* Component =
            OpenAINPCRouter::FindUniqueNPC(
                World,
                NPCID,
                MatchCount
            );

        if (!IsValid(Component))
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT(
                    "OPENAI_NPC_ROUTE_ERROR command=%s target=%s "
                    "matches=%d available=%s"
                ),
                CommandName,
                *NPCID.ToString(),
                MatchCount,
                *OpenAINPCRouter::DescribeAvailableNPCs(World)
            );
            return nullptr;
        }

        const AActor* Owner = Component->GetOwner();
        UE_LOG(
            LogTemp,
            Display,
            TEXT(
                "OPENAI_NPC_ROUTE command=%s target=%s actor=%s "
                "resolved_id=%s"
            ),
            CommandName,
            *NPCID.ToString(),
            IsValid(Owner) ? *Owner->GetName() : TEXT("None"),
            *Component->GetResolvedNPCID().ToString()
        );
        return Component;
    }

    void SendTextToNPC(
        FName NPCID,
        const TArray<FString>& MessageArgs,
        UWorld* World,
        const TCHAR* CommandName
    ) const
    {
        if (!IsValid(World) || MessageArgs.IsEmpty())
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("OPENAI_NPC_ROUTE_ERROR command=%s reason=MissingMessage"),
                CommandName
            );
            return;
        }

        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, NPCID, CommandName))
        {
            Component->SendPlayerText(
                FString::Join(MessageArgs, TEXT(" "))
            );
        }
    }

    void HandleJackSay(const TArray<FString>& Args, UWorld* World)
    {
        SendTextToNPC(TEXT("Jack"), Args, World, TEXT("Jack.Say"));
    }

    void HandleNPCSay(const TArray<FString>& Args, UWorld* World)
    {
        if (Args.Num() < 2)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Usage: NPC.Say <NPCID> <message>")
            );
            return;
        }

        TArray<FString> MessageArgs = Args;
        const FName TargetNPCID(*MessageArgs[0]);
        MessageArgs.RemoveAt(0);
        SendTextToNPC(
            TargetNPCID,
            MessageArgs,
            World,
            TEXT("NPC.Say")
        );
    }

    void HandleJackClear(UWorld* World)
    {
        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, TEXT("Jack"), TEXT("Jack.Clear")))
        {
            Component->ClearConversation();
        }
    }

    void HandleJackACEWav(
        const TArray<FString>& Args,
        UWorld* World
    )
    {
        if (!IsValid(World) || Args.IsEmpty())
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Usage: Jack.ACEWav <wav_path>")
            );
            return;
        }

        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, TEXT("Jack"), TEXT("Jack.ACEWav")))
        {
            Component->AnimateWavFileWithACE(
                FString::Join(Args, TEXT(" "))
            );
        }
    }

    void HandleJackSTTStart(UWorld* World)
    {
        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, TEXT("Jack"), TEXT("Jack.STTStart")))
        {
            Component->StartWindowsSTT();
        }
    }

    void HandleJackSTTStop(UWorld* World)
    {
        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, TEXT("Jack"), TEXT("Jack.STTStop")))
        {
            Component->StopWindowsSTT();
        }
    }

    void HandleJackSTTToggle(UWorld* World)
    {
        if (UOpenAIJackComponent* Component =
            ResolveNPC(World, TEXT("Jack"), TEXT("Jack.STTToggle")))
        {
            Component->ToggleWindowsSTT();
        }
    }

    IConsoleObject* ConsoleCommand = nullptr;
    IConsoleObject* NPCSayCommand = nullptr;
    IConsoleObject* ClearCommand = nullptr;
    IConsoleObject* ACEWavCommand = nullptr;
    IConsoleObject* STTStartCommand = nullptr;
    IConsoleObject* STTStopCommand = nullptr;
    IConsoleObject* STTToggleCommand = nullptr;
};

IMPLEMENT_MODULE(FOpenAIJackNPCModule, OpenAIJackNPC)
