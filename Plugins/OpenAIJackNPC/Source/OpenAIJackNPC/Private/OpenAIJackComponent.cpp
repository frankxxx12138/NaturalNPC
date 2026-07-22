#include "OpenAIJackComponent.h"

#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Dom/JsonObject.h"
#include "Async/Async.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#include "Containers/StringConv.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Sound/SoundWaveProcedural.h"
#include "Animation/AnimSequence.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateTypes.h"
#include "Styling/CoreStyle.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#include "ACEBlueprintLibrary.h"
#include "ACEAudioCurveSourceComponent.h"
#include "AsyncActionAnimateCharacter.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <sapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

namespace
{
    bool ParseWav(
        const TArray<uint8>& Bytes,
        int32& OutSampleRate,
        int32& OutChannels,
        TArray<uint8>& OutPcm
    )
    {
        if (Bytes.Num() < 44 ||
            FMemory::Memcmp(Bytes.GetData(), "RIFF", 4) != 0 ||
            FMemory::Memcmp(Bytes.GetData() + 8, "WAVE", 4) != 0)
        {
            return false;
        }

        int32 Offset = 12;
        int16 BitsPerSample = 0;
        while (Offset + 8 <= Bytes.Num())
        {
            const uint8* Chunk = Bytes.GetData() + Offset;
            uint32 ChunkSize = 0;
            FMemory::Memcpy(&ChunkSize, Chunk + 4, sizeof(uint32));
            const int32 DataOffset = Offset + 8;
            if (DataOffset + static_cast<int32>(ChunkSize) > Bytes.Num())
            {
                return false;
            }

            if (FMemory::Memcmp(Chunk, "fmt ", 4) == 0 && ChunkSize >= 16)
            {
                int16 Format = 0;
                FMemory::Memcpy(&Format, Bytes.GetData() + DataOffset, 2);
                FMemory::Memcpy(&OutChannels, Bytes.GetData() + DataOffset + 2, 2);
                FMemory::Memcpy(&OutSampleRate, Bytes.GetData() + DataOffset + 4, 4);
                FMemory::Memcpy(&BitsPerSample, Bytes.GetData() + DataOffset + 14, 2);
                if (Format != 1 || BitsPerSample != 16)
                {
                    return false;
                }
            }
            else if (FMemory::Memcmp(Chunk, "data", 4) == 0)
            {
                OutPcm.Append(Bytes.GetData() + DataOffset, ChunkSize);
                return OutSampleRate > 0 && OutChannels > 0 && BitsPerSample == 16;
            }

            Offset = DataOffset + static_cast<int32>(ChunkSize) + (ChunkSize & 1);
        }
        return false;
    }

    float EstimateWavDurationSeconds(const TArray<uint8>& Bytes)
    {
        int32 SampleRate = 0;
        int32 Channels = 0;
        TArray<uint8> Pcm;
        if (!ParseWav(Bytes, SampleRate, Channels, Pcm) ||
            SampleRate <= 0 ||
            Channels <= 0)
        {
            return 0.0f;
        }

        const int32 BytesPerFrame = Channels * sizeof(int16);
        return BytesPerFrame > 0
            ? static_cast<float>(Pcm.Num()) /
                static_cast<float>(BytesPerFrame * SampleRate)
            : 0.0f;
    }

#if PLATFORM_WINDOWS
    FString GetSapiRecoText(LPARAM LParam)
    {
        if (!LParam)
        {
            return FString();
        }

        ISpRecoResult* RecoResult =
            reinterpret_cast<ISpRecoResult*>(LParam);
        WCHAR* RawText = nullptr;
        if (!SUCCEEDED(RecoResult->GetText(
                SP_GETWHOLEPHRASE,
                SP_GETWHOLEPHRASE,
                false,
                &RawText,
                nullptr
            )) ||
            !RawText)
        {
            return FString();
        }

        FString Result = FString(RawText).TrimStartAndEnd();
        CoTaskMemFree(RawText);
        return Result;
    }

    void ClearSapiEvent(SPEVENT& Event)
    {
        if (Event.lParam)
        {
            switch (Event.elParamType)
            {
            case SPET_LPARAM_IS_TOKEN:
            case SPET_LPARAM_IS_OBJECT:
                reinterpret_cast<IUnknown*>(Event.lParam)->Release();
                break;
            case SPET_LPARAM_IS_POINTER:
            case SPET_LPARAM_IS_STRING:
                CoTaskMemFree(reinterpret_cast<void*>(Event.lParam));
                break;
            default:
                break;
            }
        }

        Event.eEventId = SPEI_UNDEFINED;
        Event.elParamType = SPET_LPARAM_IS_UNDEFINED;
        Event.wParam = 0;
        Event.lParam = 0;
    }
#endif

    void AppendWaveUInt16(TArray<uint8>& Bytes, uint16 Value)
    {
        Bytes.Add(static_cast<uint8>(Value & 0xff));
        Bytes.Add(static_cast<uint8>((Value >> 8) & 0xff));
    }

    void AppendWaveUInt32(TArray<uint8>& Bytes, uint32 Value)
    {
        Bytes.Add(static_cast<uint8>(Value & 0xff));
        Bytes.Add(static_cast<uint8>((Value >> 8) & 0xff));
        Bytes.Add(static_cast<uint8>((Value >> 16) & 0xff));
        Bytes.Add(static_cast<uint8>((Value >> 24) & 0xff));
    }

    TArray<uint8> CreateSilentPcm16WavBytes(
        int32 SampleRate,
        float DurationSeconds
    )
    {
        const uint16 NumChannels = 1;
        const uint16 BitsPerSample = 16;
        const uint16 BlockAlign =
            static_cast<uint16>(NumChannels * BitsPerSample / 8);
        const uint32 ByteRate =
            static_cast<uint32>(SampleRate) * BlockAlign;
        const int32 SampleCount = FMath::Max(
            1,
            FMath::RoundToInt(
                FMath::Max(0.01f, DurationSeconds) *
                static_cast<float>(SampleRate)
            )
        );
        const uint32 DataSize =
            static_cast<uint32>(SampleCount * BlockAlign);

        TArray<uint8> Bytes;
        Bytes.Reserve(44 + DataSize);
        Bytes.Append(reinterpret_cast<const uint8*>("RIFF"), 4);
        AppendWaveUInt32(Bytes, 36 + DataSize);
        Bytes.Append(reinterpret_cast<const uint8*>("WAVE"), 4);
        Bytes.Append(reinterpret_cast<const uint8*>("fmt "), 4);
        AppendWaveUInt32(Bytes, 16);
        AppendWaveUInt16(Bytes, 1);
        AppendWaveUInt16(Bytes, NumChannels);
        AppendWaveUInt32(Bytes, static_cast<uint32>(SampleRate));
        AppendWaveUInt32(Bytes, ByteRate);
        AppendWaveUInt16(Bytes, BlockAlign);
        AppendWaveUInt16(Bytes, BitsPerSample);
        Bytes.Append(reinterpret_cast<const uint8*>("data"), 4);
        AppendWaveUInt32(Bytes, DataSize);
        Bytes.AddZeroed(DataSize);
        return Bytes;
    }

    bool IsHardSpeechBoundary(TCHAR Character)
    {
        return Character == TEXT('.') ||
            Character == TEXT('!') ||
            Character == TEXT('?') ||
            Character == TEXT('\n') ||
            Character == TCHAR(0x3002) ||
            Character == TCHAR(0xff01) ||
            Character == TCHAR(0xff1f);
    }

    bool IsSoftSpeechBoundary(TCHAR Character)
    {
        return Character == TEXT(',') ||
            Character == TEXT(';') ||
            Character == TEXT(':') ||
            Character == TCHAR(0xff0c) ||
            Character == TCHAR(0xff1b) ||
            Character == TCHAR(0xff1a);
    }

    struct FOllamaStreamState
    {
        FCriticalSection Mutex;
        TArray<uint8> PendingBytes;
        FString ReplyText;
        FString PendingSpeechText;
        int32 PendingSpeechSentenceCount = 0;
        bool bHasDispatchedSpeechSegment = false;
        double PromptDurationNanoseconds = 0.0;
        double EvalDurationNanoseconds = 0.0;
        double PromptTokenCount = 0.0;
        double OutputTokenCount = 0.0;
        bool bSawDone = false;
    };

    FString Utf8BytesToString(const TArray<uint8>& Bytes)
    {
        if (Bytes.IsEmpty())
        {
            return FString();
        }

        FUTF8ToTCHAR Converter(
            reinterpret_cast<const ANSICHAR*>(Bytes.GetData()),
            Bytes.Num()
        );
        return FString(Converter.Length(), Converter.Get());
    }

    int32 FindLineBreakIndex(const TArray<uint8>& Bytes)
    {
        for (int32 Index = 0; Index < Bytes.Num(); ++Index)
        {
            if (Bytes[Index] == static_cast<uint8>('\n'))
            {
                return Index;
            }
        }
        return INDEX_NONE;
    }

    void FlushStreamingSpeechSegment(
        FString& PendingSpeechText,
        int32& PendingSpeechSentenceCount,
        TArray<FString>& OutSegments
    )
    {
        const FString Segment = PendingSpeechText.TrimStartAndEnd();
        if (!Segment.IsEmpty())
        {
            OutSegments.Add(Segment);
        }
        PendingSpeechText.Reset();
        PendingSpeechSentenceCount = 0;
    }

    void AppendStreamingSpeechContent(
        const FString& Content,
        int32 FirstSentencesPerSegment,
        int32 MaxSentencesPerSegment,
        int32 MaxSegmentCharacters,
        bool bEnableFirstSoftBoundary,
        int32 FirstSoftMinCharacters,
        FString& PendingSpeechText,
        int32& PendingSpeechSentenceCount,
        bool& bHasDispatchedSpeechSegment,
        TArray<FString>& OutSegments
    )
    {
        const int32 ClampedFirstSentences =
            FMath::Clamp(FirstSentencesPerSegment, 1, 6);
        const int32 ClampedMaxSentences =
            FMath::Clamp(MaxSentencesPerSegment, 1, 6);
        const int32 ClampedMaxSegmentCharacters =
            FMath::Clamp(MaxSegmentCharacters, 8, 120);
        const int32 ClampedFirstSoftMinCharacters =
            FMath::Clamp(FirstSoftMinCharacters, 16, 120);

        for (int32 Index = 0; Index < Content.Len(); ++Index)
        {
            const TCHAR Character = Content[Index];
            PendingSpeechText.AppendChar(Character);
            const int32 PendingLength =
                PendingSpeechText.TrimStartAndEnd().Len();

            if (IsHardSpeechBoundary(Character))
            {
                ++PendingSpeechSentenceCount;
                const int32 EffectiveMaxSentences =
                    bHasDispatchedSpeechSegment
                        ? ClampedMaxSentences
                        : ClampedFirstSentences;
                if (PendingSpeechSentenceCount >= EffectiveMaxSentences ||
                    PendingLength >= ClampedMaxSegmentCharacters)
                {
                    FlushStreamingSpeechSegment(
                        PendingSpeechText,
                        PendingSpeechSentenceCount,
                        OutSegments
                    );
                    bHasDispatchedSpeechSegment = true;
                }
            }
            else if (
                bEnableFirstSoftBoundary &&
                !bHasDispatchedSpeechSegment &&
                PendingSpeechSentenceCount == 0 &&
                IsSoftSpeechBoundary(Character) &&
                PendingLength >= ClampedFirstSoftMinCharacters)
            {
                FlushStreamingSpeechSegment(
                    PendingSpeechText,
                    PendingSpeechSentenceCount,
                    OutSegments
                );
                bHasDispatchedSpeechSegment = true;
            }
            else if (
                IsSoftSpeechBoundary(Character) &&
                PendingLength >= ClampedMaxSegmentCharacters)
            {
                FlushStreamingSpeechSegment(
                    PendingSpeechText,
                    PendingSpeechSentenceCount,
                    OutSegments
                );
                bHasDispatchedSpeechSegment = true;
            }
        }
    }

    FString ExtractOllamaReply(const TSharedPtr<FJsonObject>& Root)
    {
        const TSharedPtr<FJsonObject>* Message = nullptr;
        if (!Root->TryGetObjectField(TEXT("message"), Message) ||
            !Message->IsValid())
        {
            return FString();
        }

        FString Content;
        (*Message)->TryGetStringField(TEXT("content"), Content);
        return Content.TrimStartAndEnd();
    }

    bool ExtractEmbedding(
        const TSharedPtr<FJsonObject>& Root,
        TArray<float>& OutEmbedding
    )
    {
        const TArray<TSharedPtr<FJsonValue>>* Embeddings = nullptr;
        if (!Root->TryGetArrayField(TEXT("embeddings"), Embeddings) ||
            Embeddings->IsEmpty())
        {
            return false;
        }

        const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
        if (!(*Embeddings)[0]->TryGetArray(Values))
        {
            return false;
        }

        OutEmbedding.Reset(Values->Num());
        for (const TSharedPtr<FJsonValue>& Value : *Values)
        {
            OutEmbedding.Add(static_cast<float>(Value->AsNumber()));
        }
        return !OutEmbedding.IsEmpty();
    }

    float CosineSimilarity(
        const TArray<float>& Left,
        const TArray<float>& Right
    )
    {
        if (Left.Num() != Right.Num() || Left.IsEmpty())
        {
            return -1.0f;
        }

        double DotProduct = 0.0;
        double LeftLengthSquared = 0.0;
        double RightLengthSquared = 0.0;
        for (int32 Index = 0; Index < Left.Num(); ++Index)
        {
            DotProduct += Left[Index] * Right[Index];
            LeftLengthSquared += Left[Index] * Left[Index];
            RightLengthSquared += Right[Index] * Right[Index];
        }

        const double Denominator =
            FMath::Sqrt(LeftLengthSquared) * FMath::Sqrt(RightLengthSquared);
        return Denominator > UE_SMALL_NUMBER
            ? static_cast<float>(DotProduct / Denominator)
            : -1.0f;
    }
}

UOpenAIJackComponent::UOpenAIJackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOpenAIJackComponent::BeginPlay()
{
    Super::BeginPlay();
    EnsureWorldStateAgent();
    AudioComponent = NewObject<UAudioComponent>(GetOwner(), TEXT("JackLocalAIAudio"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->bAllowSpatialization = true;
    AudioComponent->RegisterComponent();
    AudioComponent->AttachToComponent(
        GetOwner()->GetRootComponent(),
        FAttachmentTransformRules::KeepRelativeTransform
    );

    if (bEnableACEAudio2Face)
    {
        UACEBlueprintLibrary::OverrideA2F3DInferenceMode(false);
        UACEBlueprintLibrary::OverrideA2F3DRealtimeInitialChunkSize(
            ACERealtimeInitialChunkSeconds
        );
        if (bPreAllocateACEAudio2FaceResources)
        {
            UACEBlueprintLibrary::AllocateA2F3DResources(
                ACEAudio2FaceProviderName
            );
        }
        BindACEPlaybackDelegates();
        ScheduleACEWarmup();
    }

    InitializeSessionMemory();
    WarmEmbeddingModel();
}

void UOpenAIJackComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    ++HttpSTTRequestGeneration;
    bHttpSTTListening = false;
    bHttpSTTRequestInFlight = false;
    CleanupWindowsSTT();
    StopFollowingPlayer();
    ResetSpeechQueue();
    HideScreenSubtitle();
    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
    Super::EndPlay(EndPlayReason);
}

void UOpenAIJackComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bEnableACEDirectMorphBridge)
    {
        ApplyACEDirectMorphBridge();
    }
    else if (bACEDirectMorphsActive)
    {
        ClearACEDirectMorphBridge();
    }

    UpdateKeyboardPushToTalk(DeltaTime);
    PollWindowsSTT();
    UpdateActionMovement(DeltaTime);
    UpdateActionJump(DeltaTime);
}

FString UOpenAIJackComponent::GetApiKey() const
{
    return FPlatformMisc::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"));
}

void UOpenAIJackComponent::SendPlayerText(const FString& PlayerText)
{
    const FString TrimmedPlayerText = PlayerText.TrimStartAndEnd();
    if (TrimmedPlayerText.IsEmpty())
    {
        return;
    }

    EnsureWorldStateAgent();
    FString WorldActionReply;
    if (bEnableWorldStateNaturalLanguageActions &&
        IsValid(WorldStateAgent) &&
        WorldStateAgent->TryExecuteNaturalLanguageAction(
            TrimmedPlayerText,
            WorldActionReply
        ))
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_WORLD_ACTION handled text=%s reply=%s"),
            *TrimmedPlayerText,
            *WorldActionReply
        );
        if (!bBusy)
        {
            SpeakLocalActionReply(TrimmedPlayerText, WorldActionReply);
        }
        return;
    }

    FString ActionReply;
    if (TryHandleNaturalLanguageAction(TrimmedPlayerText, ActionReply))
    {
        if (!bBusy)
        {
            SpeakLocalActionReply(TrimmedPlayerText, ActionReply);
        }
        else
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_ACTION handled_while_busy text=%s"),
                *TrimmedPlayerText
            );
        }
        return;
    }

    if (bBusy)
    {
        return;
    }

    ResetSpeechQueue();
    bBusy = true;
    if (bEnableQueuedSpeech &&
        bEnableHttpTTS &&
        bEnableInstantAcknowledgement)
    {
        ScheduleInstantAcknowledgement();
    }

    RequestRelevantMemory(TrimmedPlayerText);
}

bool UOpenAIJackComponent::StartWindowsSTT()
{
#if PLATFORM_WINDOWS
    if (bWindowsSTTListening)
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_WINDOWS_STT already_listening"));
        return true;
    }

    CleanupWindowsSTT();

    const HRESULT InitResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    bWindowsSTTShouldUninitializeCOM = SUCCEEDED(InitResult);
    const bool bCOMReady =
        SUCCEEDED(InitResult) || InitResult == RPC_E_CHANGED_MODE;

    ISpRecognizer* Recognizer = nullptr;
    ISpAudio* AudioInput = nullptr;
    ISpRecoContext* Context = nullptr;
    ISpRecoGrammar* Grammar = nullptr;
    HRESULT Result = bCOMReady ? S_OK : InitResult;

    if (SUCCEEDED(Result))
    {
        Result = CoCreateInstance(
            CLSID_SpInprocRecognizer,
            nullptr,
            CLSCTX_ALL,
            IID_ISpRecognizer,
            reinterpret_cast<void**>(&Recognizer)
        );
    }
    if (SUCCEEDED(Result))
    {
        Result = CoCreateInstance(
            CLSID_SpMMAudioIn,
            nullptr,
            CLSCTX_ALL,
            IID_ISpAudio,
            reinterpret_cast<void**>(&AudioInput)
        );
    }
    if (SUCCEEDED(Result))
    {
        Result = Recognizer->SetInput(AudioInput, true);
    }
    if (SUCCEEDED(Result))
    {
        Result = Recognizer->CreateRecoContext(&Context);
    }
    if (SUCCEEDED(Result))
    {
        Result = Context->SetNotifyWin32Event();
    }
    if (SUCCEEDED(Result))
    {
        const ULONGLONG Interest =
            SPFEI(SPEI_SOUND_START) |
            SPFEI(SPEI_SOUND_END) |
            SPFEI(SPEI_HYPOTHESIS) |
            SPFEI(SPEI_RECOGNITION) |
            SPFEI(SPEI_FALSE_RECOGNITION);
        Result = Context->SetInterest(Interest, Interest);
    }
    if (SUCCEEDED(Result))
    {
        Result = Context->CreateGrammar(1, &Grammar);
    }
    if (SUCCEEDED(Result))
    {
        Result = Grammar->LoadDictation(nullptr, SPLO_STATIC);
    }
    if (SUCCEEDED(Result))
    {
        Result = Grammar->SetDictationState(SPRS_ACTIVE);
    }
    if (SUCCEEDED(Result))
    {
        Result = Recognizer->SetRecoState(SPRST_ACTIVE_ALWAYS);
    }

    if (FAILED(Result))
    {
        if (Grammar)
        {
            Grammar->Release();
        }
        if (Context)
        {
            Context->Release();
        }
        if (Recognizer)
        {
            Recognizer->Release();
        }
        if (AudioInput)
        {
            AudioInput->Release();
        }
        if (bWindowsSTTShouldUninitializeCOM)
        {
            CoUninitialize();
        }
        bWindowsSTTShouldUninitializeCOM = false;
        Fail(FString::Printf(
            TEXT("Windows STT failed with HRESULT 0x%08X."),
            static_cast<uint32>(Result)
        ));
        return false;
    }

    WindowsSTTRecognizer = Recognizer;
    WindowsSTTAudioInput = AudioInput;
    WindowsSTTContext = Context;
    WindowsSTTGrammar = Grammar;
    bWindowsSTTListening = true;
    LastWindowsSTTHypothesis.Reset();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_WINDOWS_STT started mode=inproc_default_mic")
    );
    return true;
#else
    Fail(TEXT("Windows STT is only available on Windows."));
    return false;
#endif
}

void UOpenAIJackComponent::StopWindowsSTT()
{
    const bool bWasListening = bWindowsSTTListening;
    CleanupWindowsSTT();
    if (bWasListening)
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_WINDOWS_STT stopped"));
    }
}

bool UOpenAIJackComponent::ToggleWindowsSTT()
{
    if (bWindowsSTTListening)
    {
        StopWindowsSTT();
        return false;
    }
    return StartWindowsSTT();
}

void UOpenAIJackComponent::ClearConversation()
{
    ConversationHistory.Reset();
    PersistentHistory.Reset();
    MemoryTurns.Reset();

    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
    UE_LOG(LogTemp, Display, TEXT("JACK_LOCAL_AI_MEMORY cleared session=true"));
}

void UOpenAIJackComponent::StartFollowingPlayer(bool bRun)
{
    FollowTargetActor = ResolveFollowTarget();
    bFollowingPlayer = true;
    bFollowUsingRun = bRun;
    CurrentActionAnimationState = EActionAnimationState::None;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION follow started mode=%s target=%s"),
        bFollowUsingRun ? TEXT("run") : TEXT("walk"),
        FollowTargetActor.IsValid()
            ? *FollowTargetActor->GetName()
            : TEXT("none")
    );
}

void UOpenAIJackComponent::EnsureWorldStateAgent()
{
    if (!bEnableWorldState || IsValid(WorldStateAgent) || !GetOwner())
    {
        return;
    }
    WorldStateAgent =
        GetOwner()->FindComponentByClass<UNPCWorldStateAgentComponent>();
    if (!WorldStateAgent)
    {
        WorldStateAgent = NewObject<UNPCWorldStateAgentComponent>(
            GetOwner(),
            UNPCWorldStateAgentComponent::StaticClass(),
            TEXT("NPCWorldStateAgent")
        );
        GetOwner()->AddInstanceComponent(WorldStateAgent);
        WorldStateAgent->PerceptionRadius = WorldStatePerceptionRadius;
        WorldStateAgent->RegisterComponent();
    }
    else
    {
        WorldStateAgent->PerceptionRadius = WorldStatePerceptionRadius;
    }
    WorldStateAgent->RefreshWorldState();
}

FString UOpenAIJackComponent::GetWorldStateJson() const
{
    return IsValid(WorldStateAgent)
        ? WorldStateAgent->GetWorldStateJson()
        : FString(TEXT("{\"objects\":[]}"));
}

bool UOpenAIJackComponent::ExecuteWorldAction(
    FName ObjectId,
    FName ActionId,
    const FString& Parameters,
    FString& OutMessage
)
{
    EnsureWorldStateAgent();
    if (!IsValid(WorldStateAgent))
    {
        OutMessage = TEXT("World-state support is not available.");
        return false;
    }
    FNPCWorldActionResult Result;
    const bool bSuccess = WorldStateAgent->ExecuteWorldAction(
        ObjectId,
        ActionId,
        Parameters,
        Result
    );
    OutMessage = Result.Message;
    return bSuccess;
}

void UOpenAIJackComponent::StopFollowingPlayer()
{
    if (!bFollowingPlayer &&
        CurrentActionAnimationState == EActionAnimationState::None)
    {
        return;
    }

    bFollowingPlayer = false;
    bFollowUsingRun = false;
    FollowTargetActor.Reset();
    RestoreActionAnimation();

    UE_LOG(LogTemp, Display, TEXT("JACK_ACTION follow stopped"));
}

void UOpenAIJackComponent::PlayJumpAction()
{
    PlayActionAnimation(EActionAnimationState::Jump, false);

    UAnimSequence* JumpSequence =
        ResolveActionAnimation(EActionAnimationState::Jump);
    const float AnimationDurationSeconds = JumpSequence
        ? FMath::Clamp(JumpSequence->GetPlayLength(), 0.2f, 3.0f)
        : 0.8f;
    const float JumpDurationSeconds = FMath::Clamp(
        ActionJumpDurationSeconds > UE_SMALL_NUMBER
            ? ActionJumpDurationSeconds
            : AnimationDurationSeconds,
        0.2f,
        3.0f
    );

    if (AActor* Owner = GetOwner())
    {
        bActionJumpInProgress = true;
        ActionJumpElapsedSeconds = 0.0f;
        ActionJumpActiveDurationSeconds = JumpDurationSeconds;
        ActionJumpBaseZ = Owner->GetActorLocation().Z;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ActionAnimationTimerHandle);
        World->GetTimerManager().SetTimer(
            ActionAnimationTimerHandle,
            FTimerDelegate::CreateWeakLambda(
                this,
                [this]()
                {
                    bActionJumpInProgress = false;
                    if (AActor* Owner = GetOwner())
                    {
                        FVector Location = Owner->GetActorLocation();
                        Location.Z = ActionJumpBaseZ;
                        Owner->SetActorLocation(Location, true);
                    }
                    CurrentActionAnimationState = EActionAnimationState::None;
                    if (!bFollowingPlayer)
                    {
                        RestoreActionAnimation();
                    }
                }
            ),
            JumpDurationSeconds,
            false
        );
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION jump duration=%.2f height=%.1f"),
        JumpDurationSeconds,
        ActionJumpHeight
    );
}

bool UOpenAIJackComponent::TryHandleNaturalLanguageAction(
    const FString& PlayerText,
    FString& OutReply
)
{
    if (!bEnableNaturalLanguageActions)
    {
        return false;
    }

    const FString LowerText = PlayerText.ToLower();
    const bool bStopFollowIntent =
        LowerText.Contains(TEXT("stop following")) ||
        LowerText.Contains(TEXT("don't follow")) ||
        LowerText.Contains(TEXT("dont follow")) ||
        LowerText.Contains(TEXT("stay there")) ||
        LowerText.Contains(TEXT("wait there")) ||
        LowerText.Contains(TEXT("stop there")) ||
        PlayerText.Contains(TEXT("停止跟随")) ||
        PlayerText.Contains(TEXT("别跟")) ||
        PlayerText.Contains(TEXT("不要跟")) ||
        PlayerText.Contains(TEXT("原地")) ||
        PlayerText.Contains(TEXT("站住")) ||
        PlayerText.Contains(TEXT("停下"));

    if (bStopFollowIntent || (bFollowingPlayer && LowerText == TEXT("stop")))
    {
        StopFollowingPlayer();
        OutReply = TEXT("I'll stay here.");
        return true;
    }

    const bool bRunIntent =
        LowerText.Contains(TEXT("run")) ||
        LowerText.Contains(TEXT("hurry")) ||
        LowerText.Contains(TEXT("faster")) ||
        LowerText.Contains(TEXT("quick")) ||
        LowerText.Contains(TEXT("catch up")) ||
        PlayerText.Contains(TEXT("跑")) ||
        PlayerText.Contains(TEXT("快点")) ||
        PlayerText.Contains(TEXT("快些"));

    const bool bWalkIntent =
        LowerText.Contains(TEXT("walk")) ||
        LowerText.Contains(TEXT("slow down")) ||
        LowerText.Contains(TEXT("slower")) ||
        PlayerText.Contains(TEXT("走")) ||
        PlayerText.Contains(TEXT("慢点")) ||
        PlayerText.Contains(TEXT("慢些"));

    const bool bFollowIntent =
        LowerText.Contains(TEXT("follow me")) ||
        LowerText.Contains(TEXT("follow with me")) ||
        LowerText.Contains(TEXT("come with me")) ||
        LowerText.Contains(TEXT("come after me")) ||
        LowerText.Contains(TEXT("stay with me")) ||
        LowerText.Contains(TEXT("walk with me")) ||
        LowerText.Contains(TEXT("run with me")) ||
        PlayerText.Contains(TEXT("跟着我")) ||
        PlayerText.Contains(TEXT("跟随我")) ||
        PlayerText.Contains(TEXT("跟我")) ||
        PlayerText.Contains(TEXT("跟上")) ||
        PlayerText.Contains(TEXT("跟过来"));

    const bool bJumpIntent =
        LowerText.Contains(TEXT("jump")) ||
        LowerText.Contains(TEXT("hop")) ||
        PlayerText.Contains(TEXT("跳"));

    if (bJumpIntent)
    {
        PlayJumpAction();
        OutReply = TEXT("Sure.");
        return true;
    }

    if (bFollowIntent || (bFollowingPlayer && (bRunIntent || bWalkIntent)))
    {
        const bool bUseRun = bRunIntent || LowerText.Contains(TEXT("run with me"));
        StartFollowingPlayer(bUseRun);
        OutReply = bUseRun
            ? TEXT("I'll keep up with you.")
            : TEXT("I'll follow you.");
        return true;
    }

    if (bRunIntent && !bFollowingPlayer)
    {
        StartFollowingPlayer(true);
        OutReply = TEXT("I'll keep up with you.");
        return true;
    }

    if (bWalkIntent && !bFollowingPlayer)
    {
        StartFollowingPlayer(false);
        OutReply = TEXT("I'll follow you.");
        return true;
    }

    return false;
}

void UOpenAIJackComponent::SpeakLocalActionReply(
    const FString& PlayerText,
    const FString& ReplyText
)
{
    const FString TrimmedReply = ReplyText.TrimStartAndEnd();
    if (TrimmedReply.IsEmpty())
    {
        return;
    }

    ResetSpeechQueue();
    bBusy = true;
    AddConversationTurn(PlayerText, TrimmedReply);
    OnReplyText.Broadcast(TrimmedReply);

    UE_LOG(LogTemp, Display, TEXT("JACK_ACTION_REPLY %s"), *TrimmedReply);

    if (bEnableQueuedSpeech && bEnableHttpTTS)
    {
        bFinalSpeechQueuedForCurrentTurn = true;
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(
                InstantAcknowledgementTimerHandle
            );
        }
        EnqueueSpeechText(TrimmedReply);
        bFinalReplyReadyForCurrentTurn = true;
        PumpSpeechQueue();
    }
    else if (bEnableHttpTTS)
    {
        RequestHttpSpeech(TrimmedReply);
    }
    else if (bEnableWindowsTTS)
    {
        RequestWindowsSpeech(TrimmedReply);
    }
    else if (bEnableOpenAITTS)
    {
        if (GetApiKey().IsEmpty())
        {
            Fail(TEXT("OPENAI_API_KEY is required when OpenAI TTS is enabled."));
            return;
        }
        RequestSpeech(TrimmedReply);
    }
    else
    {
        bBusy = false;
    }
}

void UOpenAIJackComponent::UpdateActionMovement(float DeltaTime)
{
    if (!bFollowingPlayer || DeltaTime <= 0.0f)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return;
    }

    AActor* Target = FollowTargetActor.Get();
    if (!IsValid(Target))
    {
        Target = ResolveFollowTarget();
        FollowTargetActor = Target;
    }
    if (!IsValid(Target))
    {
        if (!bActionJumpInProgress)
        {
            PlayActionAnimation(EActionAnimationState::Idle, true);
        }
        return;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const FVector TargetLocation = Target->GetActorLocation();
    FVector ToTarget = TargetLocation - OwnerLocation;
    ToTarget.Z = 0.0f;

    const float Distance = ToTarget.Size();
    const float StopDistance = FMath::Max(40.0f, FollowStopDistance);
    if (Distance <= StopDistance)
    {
        if (!bActionJumpInProgress)
        {
            PlayActionAnimation(EActionAnimationState::Idle, true);
        }
        if (Distance > 20.0f)
        {
            const FRotator DesiredRotation = ToTarget.GetSafeNormal2D().Rotation();
            const FRotator CurrentRotation = Owner->GetActorRotation();
            Owner->SetActorRotation(FMath::RInterpTo(
                CurrentRotation,
                FRotator(
                    CurrentRotation.Pitch,
                    DesiredRotation.Yaw + ActionFacingYawOffsetDegrees,
                    CurrentRotation.Roll
                ),
                DeltaTime,
                FMath::Max(0.1f, FollowRotationInterpSpeed)
            ));
        }
        return;
    }

    const FVector MoveDirection = ToTarget.GetSafeNormal2D();
    const bool bShouldRun =
        bFollowUsingRun ||
        Distance >= FMath::Max(StopDistance, FollowRunDistance);
    const float Speed = bShouldRun
        ? FMath::Max(20.0f, FollowRunSpeed)
        : FMath::Max(20.0f, FollowWalkSpeed);
    const float MoveDistance =
        FMath::Min(Distance - StopDistance, Speed * DeltaTime);
    const FVector NewLocation =
        OwnerLocation + MoveDirection * FMath::Max(0.0f, MoveDistance);

    Owner->SetActorLocation(NewLocation, true);

    const FRotator DesiredRotation = MoveDirection.Rotation();
    const FRotator CurrentRotation = Owner->GetActorRotation();
    Owner->SetActorRotation(FMath::RInterpTo(
        CurrentRotation,
        FRotator(
            CurrentRotation.Pitch,
            DesiredRotation.Yaw + ActionFacingYawOffsetDegrees,
            CurrentRotation.Roll
        ),
        DeltaTime,
        FMath::Max(0.1f, FollowRotationInterpSpeed)
    ));

    if (!bActionJumpInProgress)
    {
        PlayActionAnimation(
            bShouldRun
                ? EActionAnimationState::Run
                : EActionAnimationState::Walk,
            true
        );
    }
}

void UOpenAIJackComponent::UpdateActionJump(float DeltaTime)
{
    if (!bActionJumpInProgress || DeltaTime <= 0.0f)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        bActionJumpInProgress = false;
        return;
    }

    const float DurationSeconds =
        FMath::Max(0.2f, ActionJumpActiveDurationSeconds);
    ActionJumpElapsedSeconds += DeltaTime;

    const float Alpha = FMath::Clamp(
        ActionJumpElapsedSeconds / DurationSeconds,
        0.0f,
        1.0f
    );
    const float Height =
        FMath::Sin(Alpha * UE_PI) * FMath::Max(0.0f, ActionJumpHeight);

    FVector Location = Owner->GetActorLocation();
    Location.Z = ActionJumpBaseZ + Height;
    Owner->SetActorLocation(Location, true);

    if (Alpha >= 1.0f - UE_KINDA_SMALL_NUMBER)
    {
        Location = Owner->GetActorLocation();
        Location.Z = ActionJumpBaseZ;
        Owner->SetActorLocation(Location, true);
        bActionJumpInProgress = false;
        ActionJumpElapsedSeconds = 0.0f;

        UE_LOG(LogTemp, Display, TEXT("JACK_ACTION jump landed"));
    }
}

AActor* UOpenAIJackComponent::ResolveFollowTarget() const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
    {
        return PlayerPawn;
    }

    if (APlayerController* PlayerController =
            UGameplayStatics::GetPlayerController(World, 0))
    {
        return PlayerController->GetViewTarget();
    }

    return nullptr;
}

TArray<USkeletalMeshComponent*> UOpenAIJackComponent::ResolveActionAnimationMeshes()
{
    TArray<USkeletalMeshComponent*> Result;

    bool bCacheValid = !ActionAnimationMeshes.IsEmpty();
    for (const TWeakObjectPtr<USkeletalMeshComponent>& MeshPtr :
         ActionAnimationMeshes)
    {
        if (USkeletalMeshComponent* Mesh = MeshPtr.Get())
        {
            Result.Add(Mesh);
        }
        else
        {
            bCacheValid = false;
        }
    }

    if (bCacheValid && !Result.IsEmpty())
    {
        return Result;
    }

    ActionAnimationMeshes.Reset();
    Result.Reset();

    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return Result;
    }

    TArray<USkeletalMeshComponent*> Meshes;
    Owner->GetComponents<USkeletalMeshComponent>(Meshes);
    for (USkeletalMeshComponent* Mesh : Meshes)
    {
        if (ShouldUseMeshForActionAnimation(Mesh))
        {
            ActionAnimationMeshes.Add(Mesh);
            Result.Add(Mesh);
        }
    }

    if (Result.IsEmpty())
    {
        for (USkeletalMeshComponent* Mesh : Meshes)
        {
            if (IsValid(Mesh) &&
                !Mesh->GetName().Contains(
                    TEXT("Face"),
                    ESearchCase::IgnoreCase
                ))
            {
                ActionAnimationMeshes.Add(Mesh);
                Result.Add(Mesh);
                break;
            }
        }
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION animation_meshes count=%d"),
        Result.Num()
    );

    return Result;
}

bool UOpenAIJackComponent::ShouldUseMeshForActionAnimation(
    const USkeletalMeshComponent* Mesh
) const
{
    if (!IsValid(Mesh))
    {
        return false;
    }

    const FString MeshName = Mesh->GetName();
    if (MeshName.Contains(TEXT("Face"), ESearchCase::IgnoreCase))
    {
        return false;
    }

    if (MeshName.Contains(TEXT("Body"), ESearchCase::IgnoreCase) ||
        MeshName.Contains(TEXT("SkeletalMesh"), ESearchCase::IgnoreCase))
    {
        return true;
    }

    const FString Hint = ActionBodyMeshNameHint.TrimStartAndEnd();
    return !Hint.IsEmpty() &&
        MeshName.Contains(Hint, ESearchCase::IgnoreCase);
}

void UOpenAIJackComponent::PlayActionAnimation(
    EActionAnimationState State,
    bool bLooping
)
{
    if (!bUseActionAnimationOverride ||
        State == EActionAnimationState::None ||
        CurrentActionAnimationState == State)
    {
        return;
    }

    UAnimSequence* Animation = ResolveActionAnimation(State);
    if (!Animation)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACTION animation_missing state=%d"),
            static_cast<int32>(State)
        );
        return;
    }

    TArray<USkeletalMeshComponent*> Meshes = ResolveActionAnimationMeshes();
    if (Meshes.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("JACK_ACTION animation_mesh_missing"));
        return;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ActionAnimationTimerHandle);
    }

    if (!bActionAnimationStateSaved)
    {
        SavedActionAnimationMeshStates.Reset();
        for (USkeletalMeshComponent* Mesh : Meshes)
        {
            if (!IsValid(Mesh))
            {
                continue;
            }

            FActionAnimationMeshState MeshState;
            MeshState.Mesh = Mesh;
            MeshState.SavedAnimationMode = Mesh->GetAnimationMode();
            MeshState.SavedAnimClass = Mesh->GetAnimClass();
            SavedActionAnimationMeshStates.Add(MeshState);
        }
        bActionAnimationStateSaved = true;
    }

    for (USkeletalMeshComponent* Mesh : Meshes)
    {
        if (IsValid(Mesh))
        {
            Mesh->PlayAnimation(Animation, bLooping);
        }
    }
    CurrentActionAnimationState = State;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION animation state=%d loop=%d asset=%s meshes=%d"),
        static_cast<int32>(State),
        bLooping ? 1 : 0,
        *Animation->GetPathName(),
        Meshes.Num()
    );
}

void UOpenAIJackComponent::RestoreActionAnimation()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ActionAnimationTimerHandle);
    }

    if (bActionAnimationStateSaved)
    {
        for (const FActionAnimationMeshState& MeshState :
             SavedActionAnimationMeshStates)
        {
            USkeletalMeshComponent* Mesh = MeshState.Mesh.Get();
            if (!IsValid(Mesh))
            {
                continue;
            }

            Mesh->SetAnimationMode(MeshState.SavedAnimationMode);
            if (MeshState.SavedAnimationMode ==
                EAnimationMode::AnimationBlueprint)
            {
                Mesh->SetAnimInstanceClass(MeshState.SavedAnimClass);
            }
        }
    }

    bActionAnimationStateSaved = false;
    SavedActionAnimationMeshStates.Reset();
    CurrentActionAnimationState = EActionAnimationState::None;
}

UAnimSequence* UOpenAIJackComponent::ResolveActionAnimation(
    EActionAnimationState State
) const
{
    switch (State)
    {
    case EActionAnimationState::Idle:
        return IdleAnimation.LoadSynchronous();
    case EActionAnimationState::Walk:
        return WalkAnimation.LoadSynchronous();
    case EActionAnimationState::Run:
        return RunAnimation.LoadSynchronous();
    case EActionAnimationState::Jump:
        return JumpAnimation.LoadSynchronous();
    default:
        return nullptr;
    }
}

int32 UOpenAIJackComponent::GetConversationTurnCount() const
{
    return ConversationHistory.Num() / 2;
}

void UOpenAIJackComponent::RequestRelevantMemory(const FString& PlayerText)
{
    const int32 RecentTurnCount = ConversationHistory.Num() / 2;
    if (MemoryTurns.Num() <= RecentTurnCount ||
        MaxRelevantPastTurns <= 0)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_LOCAL_AI_TIMING embed_ms=0 reason=no_old_memory")
        );
        RequestResponse(PlayerText, {});
        return;
    }

    const double EmbedStartSeconds = FPlatformTime::Seconds();
    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), EmbeddingModel);
    Body->SetStringField(TEXT("input"), PlayerText);
    Body->SetStringField(TEXT("keep_alive"), KeepAlive);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(OllamaEmbedUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [this, PlayerText, EmbedStartSeconds](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            TArray<FConversationMessage> RelevantMessages;
            if (bSucceeded && Response.IsValid() &&
                Response->GetResponseCode() >= 200 &&
                Response->GetResponseCode() < 300)
            {
                TSharedPtr<FJsonObject> Root;
                const TSharedRef<TJsonReader<>> Reader =
                    TJsonReaderFactory<>::Create(
                        Response->GetContentAsString()
                    );
                TArray<float> QueryEmbedding;
                if (FJsonSerializer::Deserialize(Reader, Root) &&
                    Root.IsValid() &&
                    ExtractEmbedding(Root, QueryEmbedding))
                {
                    RelevantMessages =
                        FindRelevantPastMessages(QueryEmbedding);
                }
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_LOCAL_AI_EMBED query_failed; using recent memory")
                );
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_LOCAL_AI_TIMING embed_ms=%.1f recalled_turns=%d"),
                (FPlatformTime::Seconds() - EmbedStartSeconds) * 1000.0,
                RelevantMessages.Num() / 2
            );
            RequestResponse(PlayerText, RelevantMessages);
        }
    );
    if (!Request->ProcessRequest())
    {
        RequestResponse(PlayerText, {});
    }
}

void UOpenAIJackComponent::RequestResponse(
    const FString& PlayerText,
    const TArray<FConversationMessage>& RelevantPastMessages
)
{
    const double ChatStartSeconds = FPlatformTime::Seconds();
    const bool bUseStreamingResponse =
        bEnableStreamingResponses &&
        bEnableQueuedSpeech &&
        bEnableHttpTTS;
    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), Model);
    Body->SetBoolField(TEXT("stream"), bUseStreamingResponse);
    Body->SetBoolField(TEXT("think"), false);
    Body->SetStringField(TEXT("keep_alive"), KeepAlive);

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedRef<FJsonObject> SystemMessage = MakeShared<FJsonObject>();
    SystemMessage->SetStringField(TEXT("role"), TEXT("system"));
    SystemMessage->SetStringField(
        TEXT("content"),
        CharacterInstructions
    );
    Messages.Add(MakeShared<FJsonValueObject>(SystemMessage));

    if (bEnableWorldState && IsValid(WorldStateAgent))
    {
        WorldStateAgent->RefreshWorldState();
        TSharedRef<FJsonObject> WorldMessage = MakeShared<FJsonObject>();
        WorldMessage->SetStringField(TEXT("role"), TEXT("system"));
        WorldMessage->SetStringField(
            TEXT("content"),
            TEXT("Use this live world state when discussing nearby objects. ")
            TEXT("The listed actions are capabilities, not completed events.\n") +
            WorldStateAgent->GetWorldStateText()
        );
        Messages.Add(MakeShared<FJsonValueObject>(WorldMessage));
    }

    if (!RelevantPastMessages.IsEmpty())
    {
        FString RelevantMemory = TEXT(
            "Relevant memories from earlier conversations. Use them only "
            "when they help answer the player:\n"
        );
        for (const FConversationMessage& MemoryMessage : RelevantPastMessages)
        {
            RelevantMemory += FString::Printf(
                TEXT("%s: %s\n"),
                MemoryMessage.Role == TEXT("user")
                    ? TEXT("Player")
                    : TEXT("Jack"),
                *MemoryMessage.Content
            );
        }

        TSharedRef<FJsonObject> MemoryMessage = MakeShared<FJsonObject>();
        MemoryMessage->SetStringField(TEXT("role"), TEXT("system"));
        MemoryMessage->SetStringField(TEXT("content"), RelevantMemory);
        Messages.Add(MakeShared<FJsonValueObject>(MemoryMessage));
    }

    for (const FConversationMessage& HistoryMessage : ConversationHistory)
    {
        TSharedRef<FJsonObject> Message = MakeShared<FJsonObject>();
        Message->SetStringField(TEXT("role"), HistoryMessage.Role);
        Message->SetStringField(TEXT("content"), HistoryMessage.Content);
        Messages.Add(MakeShared<FJsonValueObject>(Message));
    }

    TSharedRef<FJsonObject> UserMessage = MakeShared<FJsonObject>();
    UserMessage->SetStringField(TEXT("role"), TEXT("user"));
    UserMessage->SetStringField(TEXT("content"), PlayerText);
    Messages.Add(MakeShared<FJsonValueObject>(UserMessage));
    Body->SetArrayField(TEXT("messages"), Messages);

    TSharedRef<FJsonObject> Options = MakeShared<FJsonObject>();
    Options->SetNumberField(TEXT("temperature"), 0.4);
    Options->SetNumberField(TEXT("top_p"), 0.8);
    Options->SetNumberField(TEXT("top_k"), 32);
    Options->SetNumberField(TEXT("num_ctx"), ContextLength);
    Options->SetNumberField(TEXT("num_gpu"), GpuLayers);
    if (MaxOutputTokens > 0)
    {
        Options->SetNumberField(TEXT("num_predict"), MaxOutputTokens);
    }
    Body->SetObjectField(TEXT("options"), Options);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(OllamaChatUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);

    TSharedPtr<FOllamaStreamState, ESPMode::ThreadSafe> StreamState;
    if (bUseStreamingResponse)
    {
        StreamState = MakeShared<FOllamaStreamState, ESPMode::ThreadSafe>();
        const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);
        const int32 StreamingFirstSentencesPerSegment =
            FMath::Clamp(FirstSpeechSegmentSentences, 1, 6);
        const int32 StreamingMaxSentencesPerSegment =
            FMath::Clamp(MaxSentencesPerSpeechSegment, 1, 6);
        const int32 StreamingMaxSegmentCharacters =
            FMath::Clamp(MaxSpeechSegmentCharacters, 8, 120);
        const bool bStreamingEnableFirstSoftBoundary =
            bEnableFirstSpeechSegmentSoftBoundary;
        const int32 StreamingFirstSoftMinCharacters =
            FMath::Clamp(FirstSpeechSegmentSoftMinCharacters, 16, 120);

        Request->SetResponseBodyReceiveStreamDelegateV2(
            FHttpRequestStreamDelegateV2::CreateLambda(
                [
                    StreamState,
                    WeakThis,
                    StreamingFirstSentencesPerSegment,
                    StreamingMaxSentencesPerSegment,
                    StreamingMaxSegmentCharacters,
                    bStreamingEnableFirstSoftBoundary,
                    StreamingFirstSoftMinCharacters
                ](
                    void* Ptr,
                    int64& InOutLength
                )
                {
                    if (!Ptr || InOutLength <= 0 || !StreamState.IsValid())
                    {
                        return;
                    }

                    TArray<FString> SegmentsToDispatch;
                    {
                        FScopeLock Lock(&StreamState->Mutex);
                        StreamState->PendingBytes.Append(
                            static_cast<const uint8*>(Ptr),
                            InOutLength
                        );

                        int32 LineBreakIndex =
                            FindLineBreakIndex(StreamState->PendingBytes);
                        while (LineBreakIndex != INDEX_NONE)
                        {
                            TArray<uint8> LineBytes;
                            if (LineBreakIndex > 0)
                            {
                                LineBytes.Append(
                                    StreamState->PendingBytes.GetData(),
                                    LineBreakIndex
                                );
                            }
                            StreamState->PendingBytes.RemoveAt(
                                0,
                                LineBreakIndex + 1,
                                EAllowShrinking::No
                            );
                            if (!LineBytes.IsEmpty() &&
                                LineBytes.Last() == static_cast<uint8>('\r'))
                            {
                                LineBytes.RemoveAt(LineBytes.Num() - 1);
                            }

                            const FString Line =
                                Utf8BytesToString(LineBytes).TrimStartAndEnd();
                            if (!Line.IsEmpty())
                            {
                                TSharedPtr<FJsonObject> Root;
                                const TSharedRef<TJsonReader<>> Reader =
                                    TJsonReaderFactory<>::Create(Line);
                                if (FJsonSerializer::Deserialize(Reader, Root) &&
                                    Root.IsValid())
                                {
                                    const TSharedPtr<FJsonObject>* Message =
                                        nullptr;
                                    FString Delta;
                                    if (Root->TryGetObjectField(
                                            TEXT("message"),
                                            Message
                                        ) &&
                                        Message->IsValid())
                                    {
                                        (*Message)->TryGetStringField(
                                            TEXT("content"),
                                            Delta
                                        );
                                    }

                                    if (!Delta.IsEmpty())
                                    {
                                        StreamState->ReplyText += Delta;
                                        AppendStreamingSpeechContent(
                                            Delta,
                                            StreamingFirstSentencesPerSegment,
                                            StreamingMaxSentencesPerSegment,
                                            StreamingMaxSegmentCharacters,
                                            bStreamingEnableFirstSoftBoundary,
                                            StreamingFirstSoftMinCharacters,
                                            StreamState->PendingSpeechText,
                                            StreamState
                                                ->PendingSpeechSentenceCount,
                                            StreamState
                                                ->bHasDispatchedSpeechSegment,
                                            SegmentsToDispatch
                                        );
                                    }

                                    bool bDone = false;
                                    if (Root->TryGetBoolField(
                                            TEXT("done"),
                                            bDone
                                        ) &&
                                        bDone)
                                    {
                                        StreamState->bSawDone = true;
                                        Root->TryGetNumberField(
                                            TEXT("prompt_eval_duration"),
                                            StreamState->PromptDurationNanoseconds
                                        );
                                        Root->TryGetNumberField(
                                            TEXT("eval_duration"),
                                            StreamState->EvalDurationNanoseconds
                                        );
                                        Root->TryGetNumberField(
                                            TEXT("prompt_eval_count"),
                                            StreamState->PromptTokenCount
                                        );
                                        Root->TryGetNumberField(
                                            TEXT("eval_count"),
                                            StreamState->OutputTokenCount
                                        );
                                    }
                                }
                            }

                            LineBreakIndex =
                                FindLineBreakIndex(StreamState->PendingBytes);
                        }
                    }

                    if (!SegmentsToDispatch.IsEmpty())
                    {
                        AsyncTask(
                            ENamedThreads::GameThread,
                            [
                                WeakThis,
                                Segments = MoveTemp(SegmentsToDispatch)
                            ]() mutable
                            {
                                UOpenAIJackComponent* StrongThis =
                                    WeakThis.Get();
                                if (!IsValid(StrongThis) || !StrongThis->bBusy)
                                {
                                    return;
                                }

                                if (!Segments.IsEmpty())
                                {
                                    StrongThis->bFinalSpeechQueuedForCurrentTurn =
                                        true;
                                    if (UWorld* World = StrongThis->GetWorld())
                                    {
                                        World->GetTimerManager().ClearTimer(
                                            StrongThis
                                                ->InstantAcknowledgementTimerHandle
                                        );
                                    }
                                }
                                for (const FString& Segment : Segments)
                                {
                                    StrongThis->EnqueueSpeechText(Segment);
                                }
                                StrongThis->PumpSpeechQueue();
                            }
                        );
                    }
                }
            )
        );
    }

    Request->OnProcessRequestComplete().BindLambda(
        [this, PlayerText, ChatStartSeconds, bUseStreamingResponse, StreamState](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (!bSucceeded || !Response.IsValid() ||
                Response->GetResponseCode() < 200 ||
                Response->GetResponseCode() >= 300)
            {
                Fail(Response.IsValid()
                    ? Response->GetContentAsString()
                    : TEXT("Ollama request failed. Is Ollama running?"));
                return;
            }

            if (bUseStreamingResponse)
            {
                if (!StreamState.IsValid())
                {
                    Fail(TEXT("Ollama streaming state was not initialized."));
                    return;
                }

                FString Reply;
                TArray<FString> FinalSegments;
                double PromptDurationNanoseconds = 0.0;
                double EvalDurationNanoseconds = 0.0;
                double PromptTokenCount = 0.0;
                double OutputTokenCount = 0.0;
                {
                    FScopeLock Lock(&StreamState->Mutex);
                    FlushStreamingSpeechSegment(
                        StreamState->PendingSpeechText,
                        StreamState->PendingSpeechSentenceCount,
                        FinalSegments
                    );
                    Reply = StreamState->ReplyText.TrimStartAndEnd();
                    PromptDurationNanoseconds =
                        StreamState->PromptDurationNanoseconds;
                    EvalDurationNanoseconds =
                        StreamState->EvalDurationNanoseconds;
                    PromptTokenCount = StreamState->PromptTokenCount;
                    OutputTokenCount = StreamState->OutputTokenCount;
                }

                if (Reply.IsEmpty())
                {
                    Fail(TEXT("Ollama returned no streamed reply text."));
                    return;
                }

                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT(
                        "JACK_LOCAL_AI_TIMING chat_ms=%.1f prompt_ms=%.1f "
                        "generation_ms=%.1f prompt_tokens=%d output_tokens=%d "
                        "stream=1"
                    ),
                    (FPlatformTime::Seconds() - ChatStartSeconds) * 1000.0,
                    PromptDurationNanoseconds / 1000000.0,
                    EvalDurationNanoseconds / 1000000.0,
                    static_cast<int32>(PromptTokenCount),
                    static_cast<int32>(OutputTokenCount)
                );
                AddConversationTurn(PlayerText, Reply);
                UE_LOG(LogTemp, Display, TEXT("JACK_LOCAL_AI_REPLY %s"), *Reply);
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_LOCAL_AI_MEMORY turns=%d"),
                    GetConversationTurnCount()
                );
                OnReplyText.Broadcast(Reply);

                const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);
                AsyncTask(
                    ENamedThreads::GameThread,
                    [
                        WeakThis,
                        Segments = MoveTemp(FinalSegments)
                    ]() mutable
                    {
                        UOpenAIJackComponent* StrongThis = WeakThis.Get();
                        if (!IsValid(StrongThis) || !StrongThis->bBusy)
                        {
                            return;
                        }

                        StrongThis->bFinalSpeechQueuedForCurrentTurn = true;
                        if (UWorld* World = StrongThis->GetWorld())
                        {
                            World->GetTimerManager().ClearTimer(
                                StrongThis->InstantAcknowledgementTimerHandle
                            );
                        }
                        for (const FString& Segment : Segments)
                        {
                            StrongThis->EnqueueSpeechText(Segment);
                        }
                        StrongThis->bFinalReplyReadyForCurrentTurn = true;
                        StrongThis->PumpSpeechQueue();
                    }
                );
                return;
            }

            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
            {
                Fail(TEXT("Could not parse the Ollama response."));
                return;
            }

            const FString Reply = ExtractOllamaReply(Root);
            if (Reply.IsEmpty())
            {
                Fail(TEXT("Ollama returned no reply text."));
                return;
            }

            double PromptDurationNanoseconds = 0.0;
            double EvalDurationNanoseconds = 0.0;
            double PromptTokenCount = 0.0;
            double OutputTokenCount = 0.0;
            Root->TryGetNumberField(
                TEXT("prompt_eval_duration"),
                PromptDurationNanoseconds
            );
            Root->TryGetNumberField(
                TEXT("eval_duration"),
                EvalDurationNanoseconds
            );
            Root->TryGetNumberField(
                TEXT("prompt_eval_count"),
                PromptTokenCount
            );
            Root->TryGetNumberField(
                TEXT("eval_count"),
                OutputTokenCount
            );
            UE_LOG(
                LogTemp,
                Display,
                TEXT(
                    "JACK_LOCAL_AI_TIMING chat_ms=%.1f prompt_ms=%.1f "
                    "generation_ms=%.1f prompt_tokens=%d output_tokens=%d"
                ),
                (FPlatformTime::Seconds() - ChatStartSeconds) * 1000.0,
                PromptDurationNanoseconds / 1000000.0,
                EvalDurationNanoseconds / 1000000.0,
                static_cast<int32>(PromptTokenCount),
                static_cast<int32>(OutputTokenCount)
            );
            AddConversationTurn(PlayerText, Reply);
            UE_LOG(LogTemp, Display, TEXT("JACK_LOCAL_AI_REPLY %s"), *Reply);
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_LOCAL_AI_MEMORY turns=%d"),
                GetConversationTurnCount()
            );
            OnReplyText.Broadcast(Reply);
            if (bEnableQueuedSpeech && bEnableHttpTTS)
            {
                bFinalSpeechQueuedForCurrentTurn = true;
                if (UWorld* World = GetWorld())
                {
                    World->GetTimerManager().ClearTimer(
                        InstantAcknowledgementTimerHandle
                    );
                }
                for (const FString& Segment : SplitReplyIntoSpeechSegments(Reply))
                {
                    EnqueueSpeechText(Segment);
                }
                bFinalReplyReadyForCurrentTurn = true;
                PumpSpeechQueue();
            }
            else if (bEnableHttpTTS)
            {
                RequestHttpSpeech(Reply);
            }
            else if (bEnableWindowsTTS)
            {
                RequestWindowsSpeech(Reply);
            }
            else if (bEnableOpenAITTS)
            {
                if (GetApiKey().IsEmpty())
                {
                    Fail(TEXT("OPENAI_API_KEY is required when OpenAI TTS is enabled."));
                    return;
                }
                RequestSpeech(Reply);
            }
            else
            {
                bBusy = false;
            }
        }
    );
    if (!Request->ProcessRequest())
    {
        Fail(TEXT("Could not start the Ollama HTTP request."));
    }
}

void UOpenAIJackComponent::AddConversationTurn(
    const FString& PlayerText,
    const FString& ReplyText
)
{
    ConversationHistory.Add({TEXT("user"), PlayerText});
    ConversationHistory.Add({TEXT("assistant"), ReplyText});
    PersistentHistory.Add({TEXT("user"), PlayerText});
    PersistentHistory.Add({TEXT("assistant"), ReplyText});
    MemoryTurns.Add({PlayerText, ReplyText, {}});

    const int32 MaximumMessages = FMath::Max(1, MaxConversationTurns) * 2;
    if (ConversationHistory.Num() > MaximumMessages)
    {
        ConversationHistory.RemoveAt(
            0,
            ConversationHistory.Num() - MaximumMessages
        );
    }

    const int32 MaximumStoredMessages =
        FMath::Max(1, MaxStoredConversationTurns) * 2;
    if (PersistentHistory.Num() > MaximumStoredMessages)
    {
        PersistentHistory.RemoveAt(
            0,
            PersistentHistory.Num() - MaximumStoredMessages
        );
    }
    if (MemoryTurns.Num() > FMath::Max(1, MaxStoredConversationTurns))
    {
        MemoryTurns.RemoveAt(
            0,
            MemoryTurns.Num() - FMath::Max(1, MaxStoredConversationTurns)
        );
    }
    const int32 NewTurnIndex = MemoryTurns.Num() - 1;

    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
    RequestTurnEmbedding(NewTurnIndex);
}

void UOpenAIJackComponent::InitializeSessionMemory()
{
    ConversationHistory.Reset();
    PersistentHistory.Reset();
    MemoryTurns.Reset();

    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_LOCAL_AI_MEMORY_SESSION initialized path=%s"),
        *GetMemoryFilePath()
    );
}

FString UOpenAIJackComponent::GetMemoryFilePath() const
{
    return FPaths::Combine(
        FPaths::ProjectSavedDir(),
        TEXT("NPCMemory"),
        FPaths::GetCleanFilename(MemoryFileName)
    );
}

bool UOpenAIJackComponent::SaveMemory()
{
    const FString FilePath = GetMemoryFilePath();
    IFileManager::Get().MakeDirectory(
        *FPaths::GetPath(FilePath),
        true
    );

    TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetNumberField(TEXT("version"), 1);
    Root->SetStringField(TEXT("npc"), TEXT("Jack"));

    TArray<TSharedPtr<FJsonValue>> Messages;
    for (const FConversationMessage& Message : PersistentHistory)
    {
        TSharedRef<FJsonObject> JsonMessage = MakeShared<FJsonObject>();
        JsonMessage->SetStringField(TEXT("role"), Message.Role);
        JsonMessage->SetStringField(TEXT("content"), Message.Content);
        Messages.Add(MakeShared<FJsonValueObject>(JsonMessage));
    }
    Root->SetArrayField(TEXT("messages"), Messages);

    TArray<TSharedPtr<FJsonValue>> Turns;
    for (const FMemoryTurn& Turn : MemoryTurns)
    {
        TSharedRef<FJsonObject> JsonTurn = MakeShared<FJsonObject>();
        JsonTurn->SetStringField(TEXT("player"), Turn.PlayerText);
        JsonTurn->SetStringField(TEXT("jack"), Turn.ReplyText);
        Turns.Add(MakeShared<FJsonValueObject>(JsonTurn));
    }
    Root->SetArrayField(TEXT("turns"), Turns);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Root, Writer);

    const bool bSaved = FFileHelper::SaveStringToFile(
        Json,
        *FilePath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM
    );
    if (bSaved)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_LOCAL_AI_MEMORY_SAVE success=1 turns=%d path=%s"),
            PersistentHistory.Num() / 2,
            *FilePath
        );
    }
    else
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JACK_LOCAL_AI_MEMORY_SAVE success=0 path=%s"),
            *FilePath
        );
    }
    return bSaved;
}

void UOpenAIJackComponent::WarmEmbeddingModel()
{
    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), EmbeddingModel);
    Body->SetStringField(TEXT("input"), TEXT("conversation memory"));
    Body->SetStringField(TEXT("keep_alive"), KeepAlive);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(OllamaEmbedUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr, FHttpResponsePtr Response, bool bSucceeded)
        {
            const bool bWarm = bSucceeded && Response.IsValid() &&
                Response->GetResponseCode() >= 200 &&
                Response->GetResponseCode() < 300;
            if (bWarm)
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_LOCAL_AI_EMBED warm=1")
                );
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_LOCAL_AI_EMBED warm=0")
                );
            }
        }
    );
    Request->ProcessRequest();
}

void UOpenAIJackComponent::RequestTurnEmbedding(int32 TurnIndex)
{
    if (!MemoryTurns.IsValidIndex(TurnIndex))
    {
        return;
    }

    const FString MemoryText = FString::Printf(
        TEXT("Player: %s\nJack: %s"),
        *MemoryTurns[TurnIndex].PlayerText,
        *MemoryTurns[TurnIndex].ReplyText
    );

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), EmbeddingModel);
    Body->SetStringField(TEXT("input"), MemoryText);
    Body->SetStringField(TEXT("keep_alive"), KeepAlive);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(OllamaEmbedUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [this, TurnIndex](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (!bSucceeded || !Response.IsValid() ||
                !MemoryTurns.IsValidIndex(TurnIndex))
            {
                return;
            }

            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            TArray<float> Embedding;
            if (FJsonSerializer::Deserialize(Reader, Root) &&
                Root.IsValid() &&
                ExtractEmbedding(Root, Embedding))
            {
                MemoryTurns[TurnIndex].Embedding = MoveTemp(Embedding);
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_LOCAL_AI_EMBED indexed_turn=%d dimensions=%d"),
                    TurnIndex + 1,
                    MemoryTurns[TurnIndex].Embedding.Num()
                );
            }
        }
    );
    Request->ProcessRequest();
}

TArray<UOpenAIJackComponent::FConversationMessage>
UOpenAIJackComponent::FindRelevantPastMessages(
    const TArray<float>& QueryEmbedding
) const
{
    struct FScoredTurn
    {
        float Score = -1.0f;
        int32 TurnIndex = 0;
    };

    TArray<FScoredTurn> ScoredTurns;
    const int32 RecentTurnCount = ConversationHistory.Num() / 2;
    const int32 SearchTurnCount = FMath::Max(
        0,
        MemoryTurns.Num() - RecentTurnCount
    );

    for (int32 Index = 0; Index < SearchTurnCount; ++Index)
    {
        if (MemoryTurns[Index].Embedding.IsEmpty())
        {
            continue;
        }

        const float Score = CosineSimilarity(
            QueryEmbedding,
            MemoryTurns[Index].Embedding
        );
        if (Score >= 0.25f)
        {
            ScoredTurns.Add({Score, Index});
        }
    }

    ScoredTurns.Sort(
        [](const FScoredTurn& Left, const FScoredTurn& Right)
        {
            return Left.Score > Right.Score;
        }
    );

    TArray<FConversationMessage> Result;
    const int32 TurnCount = FMath::Min(
        FMath::Max(0, MaxRelevantPastTurns),
        ScoredTurns.Num()
    );
    for (int32 Index = 0; Index < TurnCount; ++Index)
    {
        const FMemoryTurn& Turn =
            MemoryTurns[ScoredTurns[Index].TurnIndex];
        Result.Add({TEXT("user"), Turn.PlayerText});
        Result.Add({TEXT("assistant"), Turn.ReplyText});
    }
    return Result;
}

void UOpenAIJackComponent::RequestSpeech(const FString& ReplyText)
{
    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("model"), SpeechModel);
    Body->SetStringField(TEXT("voice"), Voice);
    Body->SetStringField(TEXT("input"), ReplyText);
    Body->SetStringField(TEXT("response_format"), TEXT("wav"));

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(TEXT("https://api.openai.com/v1/audio/speech"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + GetApiKey());
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [this, ReplyText](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (!bSucceeded || !Response.IsValid() ||
                Response->GetResponseCode() < 200 ||
                Response->GetResponseCode() >= 300)
            {
                Fail(Response.IsValid()
                    ? Response->GetContentAsString()
                    : TEXT("OpenAI speech request failed."));
                return;
            }

            const TArray<uint8>& WavBytes = Response->GetContent();
            const float SubtitleDurationSeconds =
                EstimateWavDurationSeconds(WavBytes);
            ShowScreenSubtitle(ReplyText, SubtitleDurationSeconds);

            if (bEnableACEAudio2Face && TryPlayWavWithACE(WavBytes))
            {
                bBusy = false;
                return;
            }

            PlayWavBytes(WavBytes);
            bBusy = false;
        }
    );
    Request->ProcessRequest();
}

void UOpenAIJackComponent::RequestHttpSpeech(const FString& ReplyText)
{
    RequestHttpSpeechInternal(ReplyText, false);
}

void UOpenAIJackComponent::RequestQueuedHttpSpeech(
    int32 SpeechQueueItemId,
    const FString& ReplyText
)
{
    RequestHttpSpeechInternal(
        ReplyText,
        true,
        SpeechQueueItemId,
        SpeechQueueGeneration
    );
}

void UOpenAIJackComponent::RequestHttpSpeechInternal(
    const FString& ReplyText,
    bool bFromSpeechQueue,
    int32 SpeechQueueItemId,
    int32 QueueGeneration
)
{
    if (HttpTTSUrl.TrimStartAndEnd().IsEmpty())
    {
        const FString Message =
            TEXT("HTTP TTS is enabled but HttpTTSUrl is empty.");
        if (bFromSpeechQueue)
        {
            UE_LOG(LogTemp, Warning, TEXT("JACK_SPEECH_QUEUE_ERROR %s"), *Message);
            OnError.Broadcast(Message);
            if (FSpeechQueueItem* Item = FindSpeechQueueItem(SpeechQueueItemId))
            {
                Item->bTTSFailed = true;
            }
            SpeechRequestsInFlight = FMath::Max(0, SpeechRequestsInFlight - 1);
            PumpSpeechQueue();
        }
        else
        {
            Fail(Message);
        }
        return;
    }

    const double TTSStartSeconds = FPlatformTime::Seconds();

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("text"), ReplyText);
    Body->SetStringField(TEXT("voice"), HttpTTSVoice);
    Body->SetStringField(TEXT("language_code"), HttpTTSLanguageCode);
    Body->SetNumberField(TEXT("sample_rate_hz"), HttpTTSSampleRateHz);
    Body->SetStringField(TEXT("format"), TEXT("wav"));

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(HttpTTSUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("audio/wav, application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [
            this,
            TTSStartSeconds,
            bFromSpeechQueue,
            ReplyText,
            SpeechQueueItemId,
            QueueGeneration
        ](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            auto HandleFailure =
                [
                    this,
                    bFromSpeechQueue,
                    SpeechQueueItemId,
                    QueueGeneration
                ](const FString& Message)
            {
                if (bFromSpeechQueue)
                {
                    if (QueueGeneration != SpeechQueueGeneration)
                    {
                        return;
                    }

                    UE_LOG(
                        LogTemp,
                        Warning,
                        TEXT("JACK_SPEECH_QUEUE_ERROR %s"),
                        *Message
                    );
                    OnError.Broadcast(Message);
                    if (FSpeechQueueItem* Item =
                            FindSpeechQueueItem(SpeechQueueItemId))
                    {
                        Item->bTTSFailed = true;
                    }
                    SpeechRequestsInFlight =
                        FMath::Max(0, SpeechRequestsInFlight - 1);
                    PumpSpeechQueue();
                }
                else
                {
                    Fail(Message);
                }
            };

            if (!bSucceeded || !Response.IsValid() ||
                Response->GetResponseCode() < 200 ||
                Response->GetResponseCode() >= 300)
            {
                HandleFailure(Response.IsValid()
                    ? Response->GetContentAsString()
                    : TEXT("HTTP TTS request failed. Is the TTS service running?"));
                return;
            }

            TArray<uint8> WavBytes = Response->GetContent();
            const bool bLooksLikeWav =
                WavBytes.Num() >= 12 &&
                FMemory::Memcmp(WavBytes.GetData(), "RIFF", 4) == 0 &&
                FMemory::Memcmp(WavBytes.GetData() + 8, "WAVE", 4) == 0;

            if (!bLooksLikeWav)
            {
                TSharedPtr<FJsonObject> Root;
                const TSharedRef<TJsonReader<>> Reader =
                    TJsonReaderFactory<>::Create(Response->GetContentAsString());
                if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
                {
                    HandleFailure(
                        TEXT("HTTP TTS returned neither WAV bytes nor valid JSON.")
                    );
                    return;
                }

                FString AudioBase64;
                Root->TryGetStringField(TEXT("audio_wav_base64"), AudioBase64);
                if (AudioBase64.IsEmpty())
                {
                    Root->TryGetStringField(TEXT("wav_base64"), AudioBase64);
                }
                if (AudioBase64.IsEmpty())
                {
                    Root->TryGetStringField(TEXT("audio_base64"), AudioBase64);
                }
                if (AudioBase64.IsEmpty() ||
                    !FBase64::Decode(AudioBase64, WavBytes))
                {
                    HandleFailure(
                        TEXT("HTTP TTS JSON did not contain a valid base64 WAV field.")
                    );
                    return;
                }
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_HTTP_TTS generated bytes=%d tts_ms=%.1f url=%s"),
                WavBytes.Num(),
                (FPlatformTime::Seconds() - TTSStartSeconds) * 1000.0,
                *HttpTTSUrl
            );

            if (bFromSpeechQueue)
            {
                if (QueueGeneration != SpeechQueueGeneration)
                {
                    return;
                }

                FSpeechQueueItem* Item =
                    FindSpeechQueueItem(SpeechQueueItemId);
                if (!Item)
                {
                    SpeechRequestsInFlight =
                        FMath::Max(0, SpeechRequestsInFlight - 1);
                    PumpSpeechQueue();
                    return;
                }

                Item->WavBytes = MoveTemp(WavBytes);
                Item->DurationSeconds =
                    EstimateWavDurationSeconds(Item->WavBytes);
                Item->bTTSReady = true;
                SpeechRequestsInFlight =
                    FMath::Max(0, SpeechRequestsInFlight - 1);
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT(
                        "JACK_SPEECH_QUEUE segment_tts_done chars=%d "
                        "duration=%.2f pending=%d in_flight=%d text=%s"
                    ),
                    ReplyText.Len(),
                    Item->DurationSeconds,
                    SpeechQueue.Num(),
                    SpeechRequestsInFlight,
                    *ReplyText
                );
                PumpSpeechQueue();
                return;
            }

            const float SubtitleDurationSeconds =
                EstimateWavDurationSeconds(WavBytes);
            ShowScreenSubtitle(ReplyText, SubtitleDurationSeconds);

            if (bEnableACEAudio2Face && TryPlayWavWithACE(WavBytes))
            {
                return;
            }

            PlayWavBytes(WavBytes);
            bBusy = false;
        }
    );
    if (!Request->ProcessRequest())
    {
        const FString Message = TEXT("Could not start the HTTP TTS request.");
        if (bFromSpeechQueue)
        {
            UE_LOG(LogTemp, Warning, TEXT("JACK_SPEECH_QUEUE_ERROR %s"), *Message);
            OnError.Broadcast(Message);
            if (QueueGeneration == SpeechQueueGeneration)
            {
                if (FSpeechQueueItem* Item =
                        FindSpeechQueueItem(SpeechQueueItemId))
                {
                    Item->bTTSFailed = true;
                }
                SpeechRequestsInFlight =
                    FMath::Max(0, SpeechRequestsInFlight - 1);
                PumpSpeechQueue();
            }
        }
        else
        {
            Fail(Message);
        }
    }
}

void UOpenAIJackComponent::ResetSpeechQueue()
{
    SpeechQueue.Reset();
    SpeechRequestsInFlight = 0;
    ++SpeechQueueGeneration;
    NextSpeechQueueItemId = 1;
    bACEPlaybackInFlight = false;
    bFinalReplyReadyForCurrentTurn = false;
    bCurrentACEFromSpeechQueue = false;
    bQueuedACESendCompleted = false;
    bQueuedACEPlaybackStarted = false;
    bQueuedACEPlaybackEnded = false;
    bFinalSpeechQueuedForCurrentTurn = false;
    PendingQueuedACESubtitleText.Reset();
    PendingQueuedACESubtitleDurationSeconds = 0.0f;
    QueuedSpeechDurationSeconds = 0.0f;
    HideScreenSubtitle();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ACEWarmupTimerHandle);
        World->GetTimerManager().ClearTimer(SpeechQueueTimerHandle);
        World->GetTimerManager().ClearTimer(InstantAcknowledgementTimerHandle);
    }
}

void UOpenAIJackComponent::EnqueueSpeechText(const FString& Text)
{
    const FString TrimmedText = Text.TrimStartAndEnd();
    if (TrimmedText.IsEmpty())
    {
        return;
    }

    FSpeechQueueItem Item;
    Item.Id = NextSpeechQueueItemId++;
    Item.Text = TrimmedText;
    SpeechQueue.Add(MoveTemp(Item));
    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_SPEECH_QUEUE enqueue chars=%d pending=%d text=%s"),
        TrimmedText.Len(),
        SpeechQueue.Num(),
        *TrimmedText
    );
    StartSpeechPrefetches();
}

void UOpenAIJackComponent::StartSpeechPrefetches()
{
    const int32 MaxRequests =
        FMath::Clamp(MaxConcurrentSpeechRequests, 1, 4);
    if (SpeechQueue.IsEmpty())
    {
        return;
    }

    FSpeechQueueItem& FirstItem = SpeechQueue[0];
    if (!FirstItem.bTTSReady && !FirstItem.bTTSFailed)
    {
        if (!FirstItem.bTTSRequested && SpeechRequestsInFlight < MaxRequests)
        {
            FirstItem.bTTSRequested = true;
            ++SpeechRequestsInFlight;
            UE_LOG(
                LogTemp,
                Display,
                TEXT(
                    "JACK_SPEECH_QUEUE prefetch id=%d in_flight=%d "
                    "pending=%d text=%s"
                ),
                FirstItem.Id,
                SpeechRequestsInFlight,
                SpeechQueue.Num(),
                *FirstItem.Text
            );
            RequestQueuedHttpSpeech(FirstItem.Id, FirstItem.Text);
        }
        return;
    }

    if (!bACEPlaybackInFlight)
    {
        return;
    }

    for (int32 ItemIndex = 1; ItemIndex < SpeechQueue.Num(); ++ItemIndex)
    {
        FSpeechQueueItem& Item = SpeechQueue[ItemIndex];
        if (SpeechRequestsInFlight >= MaxRequests)
        {
            return;
        }

        if (Item.bTTSRequested || Item.bTTSReady || Item.bTTSFailed)
        {
            continue;
        }

        Item.bTTSRequested = true;
        ++SpeechRequestsInFlight;
        UE_LOG(
            LogTemp,
            Display,
            TEXT(
                "JACK_SPEECH_QUEUE prefetch id=%d in_flight=%d pending=%d "
                "text=%s"
            ),
            Item.Id,
            SpeechRequestsInFlight,
            SpeechQueue.Num(),
            *Item.Text
        );
        RequestQueuedHttpSpeech(Item.Id, Item.Text);
    }
}

UOpenAIJackComponent::FSpeechQueueItem*
UOpenAIJackComponent::FindSpeechQueueItem(int32 SpeechQueueItemId)
{
    for (FSpeechQueueItem& Item : SpeechQueue)
    {
        if (Item.Id == SpeechQueueItemId)
        {
            return &Item;
        }
    }
    return nullptr;
}

void UOpenAIJackComponent::PumpSpeechQueue()
{
    if (!bEnableQueuedSpeech || !bEnableHttpTTS)
    {
        return;
    }

    StartSpeechPrefetches();

    if (bACEPlaybackInFlight)
    {
        return;
    }

    while (!SpeechQueue.IsEmpty())
    {
        FSpeechQueueItem& NextItem = SpeechQueue[0];
        if (NextItem.bTTSFailed)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("JACK_SPEECH_QUEUE skip_failed id=%d text=%s"),
                NextItem.Id,
                *NextItem.Text
            );
            SpeechQueue.RemoveAt(0);
            StartSpeechPrefetches();
            continue;
        }

        if (!NextItem.bTTSReady)
        {
            return;
        }

        const FString TrimmedText = NextItem.Text.TrimStartAndEnd();
        TArray<uint8> WavBytes = MoveTemp(NextItem.WavBytes);
        QueuedSpeechDurationSeconds = NextItem.DurationSeconds;
        const float SubtitleDurationSeconds =
            QueuedSpeechDurationSeconds + SpeechSegmentPaddingSeconds;
        const int32 ItemId = NextItem.Id;
        SpeechQueue.RemoveAt(0);

        UE_LOG(
            LogTemp,
            Display,
            TEXT(
                "JACK_SPEECH_QUEUE play id=%d pending_after_pop=%d "
                "duration=%.2f text=%s"
            ),
            ItemId,
            SpeechQueue.Num(),
            QueuedSpeechDurationSeconds,
            *TrimmedText
        );

        if (bEnableACEAudio2Face)
        {
            PendingQueuedACESubtitleText = TrimmedText;
            PendingQueuedACESubtitleDurationSeconds = SubtitleDurationSeconds;
            bCurrentACEFromSpeechQueue = true;
            bQueuedACESendCompleted = false;
            bQueuedACEPlaybackStarted = false;
            bQueuedACEPlaybackEnded = false;
            BindACEPlaybackDelegates();
            if (TryPlayWavWithACE(WavBytes))
            {
                bACEPlaybackInFlight = true;
                ScheduleQueuedACEFallbackCompletion(
                    QueuedSpeechDurationSeconds +
                    SpeechSegmentPaddingSeconds +
                    1.0f
                );
                return;
            }
            bCurrentACEFromSpeechQueue = false;
            bQueuedACESendCompleted = false;
            bQueuedACEPlaybackStarted = false;
            bQueuedACEPlaybackEnded = false;
            PendingQueuedACESubtitleText.Reset();
            PendingQueuedACESubtitleDurationSeconds = 0.0f;
        }

        ShowScreenSubtitle(TrimmedText, SubtitleDurationSeconds);
        PlayWavBytes(WavBytes);
        ScheduleQueuedSpeechCompletion(
            QueuedSpeechDurationSeconds + SpeechSegmentPaddingSeconds
        );
        StartSpeechPrefetches();
        return;
    }

    if (bFinalReplyReadyForCurrentTurn && SpeechRequestsInFlight <= 0)
    {
        bFinalReplyReadyForCurrentTurn = false;
        bBusy = false;
        UE_LOG(LogTemp, Display, TEXT("JACK_SPEECH_QUEUE complete"));
    }
}

void UOpenAIJackComponent::CompleteQueuedSpeechSegment()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SpeechQueueTimerHandle);
    }

    bACEPlaybackInFlight = false;
    bCurrentACEFromSpeechQueue = false;
    bQueuedACESendCompleted = false;
    bQueuedACEPlaybackStarted = false;
    bQueuedACEPlaybackEnded = false;
    PendingQueuedACESubtitleText.Reset();
    PendingQueuedACESubtitleDurationSeconds = 0.0f;
    QueuedSpeechDurationSeconds = 0.0f;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_SPEECH_QUEUE segment_complete pending=%d final_ready=%d"),
        SpeechQueue.Num(),
        bFinalReplyReadyForCurrentTurn ? 1 : 0
    );
    PumpSpeechQueue();
}

void UOpenAIJackComponent::ScheduleQueuedSpeechCompletion(float DelaySeconds)
{
    bACEPlaybackInFlight = true;

    const float SafeDelaySeconds = FMath::Max(0.2f, DelaySeconds);
    UWorld* World = GetWorld();
    if (!World)
    {
        CompleteQueuedSpeechSegment();
        return;
    }

    World->GetTimerManager().SetTimer(
        SpeechQueueTimerHandle,
        FTimerDelegate::CreateWeakLambda(
            this,
            [this]()
            {
                CompleteQueuedSpeechSegment();
            }
        ),
        SafeDelaySeconds,
        false
    );
}

void UOpenAIJackComponent::TryCompleteQueuedACESpeechSegment()
{
    if (!bACEPlaybackInFlight || !bCurrentACEFromSpeechQueue)
    {
        return;
    }

    if (bQueuedACESendCompleted && bQueuedACEPlaybackEnded)
    {
        CompleteQueuedSpeechSegment();
    }
}

void UOpenAIJackComponent::ScheduleQueuedACEFallbackCompletion(
    float DelaySeconds
)
{
    const float SafeDelaySeconds = FMath::Max(0.2f, DelaySeconds);
    UWorld* World = GetWorld();
    if (!World)
    {
        bQueuedACEPlaybackEnded = true;
        TryCompleteQueuedACESpeechSegment();
        return;
    }

    World->GetTimerManager().SetTimer(
        SpeechQueueTimerHandle,
        FTimerDelegate::CreateWeakLambda(
            this,
            [this]()
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_ACE_A2F_PLAYBACK fallback_end")
                );
                bQueuedACEPlaybackEnded = true;
                TryCompleteQueuedACESpeechSegment();
            }
        ),
        SafeDelaySeconds,
        false
    );
}

void UOpenAIJackComponent::BindACEPlaybackDelegates()
{
    ResolveACEDirectMorphBridge();
    if (!IsValid(ACECurveSource))
    {
        return;
    }

    ACECurveSource->BufferLengthInSeconds =
        FMath::Clamp(ACEAudioBufferSeconds, 0.01f, 0.5f);

    ACECurveSource->OnAnimationStarted.RemoveDynamic(
        this,
        &UOpenAIJackComponent::HandleACEAnimationStarted
    );
    ACECurveSource->OnAnimationStarted.AddDynamic(
        this,
        &UOpenAIJackComponent::HandleACEAnimationStarted
    );
    ACECurveSource->OnAnimationEnded.RemoveDynamic(
        this,
        &UOpenAIJackComponent::HandleACEAnimationEnded
    );
    ACECurveSource->OnAnimationEnded.AddDynamic(
        this,
        &UOpenAIJackComponent::HandleACEAnimationEnded
    );
}

void UOpenAIJackComponent::ScheduleACEWarmup()
{
    if (!bWarmUpACEAudio2FaceOnBeginPlay || !bEnableACEAudio2Face)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        TryStartACEWarmup();
        return;
    }

    const float SafeDelaySeconds =
        FMath::Clamp(ACEWarmupDelaySeconds, 0.0f, 10.0f);
    World->GetTimerManager().SetTimer(
        ACEWarmupTimerHandle,
        FTimerDelegate::CreateWeakLambda(
            this,
            [this]()
            {
                TryStartACEWarmup();
            }
        ),
        SafeDelaySeconds,
        false
    );
}

void UOpenAIJackComponent::TryStartACEWarmup()
{
    if (!bWarmUpACEAudio2FaceOnBeginPlay ||
        !bEnableACEAudio2Face ||
        bBusy ||
        bACEPlaybackInFlight ||
        IsValid(ActiveACEAction))
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_ACE_A2F_WARMUP skipped"));
        return;
    }

    BindACEPlaybackDelegates();

    TArray<uint8> WavBytes = CreateSilentPcm16WavBytes(
        16000,
        FMath::Clamp(ACEWarmupDurationSeconds, 0.05f, 2.0f)
    );
    const FString OutputPath = FPaths::CreateTempFilename(
        *FPaths::ProjectIntermediateDir(),
        TEXT("JackACEWarmup_"),
        TEXT(".wav")
    );
    if (!FFileHelper::SaveArrayToFile(WavBytes, *OutputPath))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACE_A2F_WARMUP failed_to_write_wav path=%s"),
            *OutputPath
        );
        return;
    }

    bACEWarmupInFlight = true;
    const bool bStarted = StartACEWavFileAsync(OutputPath, true);
    if (!bStarted)
    {
        bACEWarmupInFlight = false;
        IFileManager::Get().Delete(*OutputPath);
        UE_LOG(LogTemp, Display, TEXT("JACK_ACE_A2F_WARMUP not_started"));
        return;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACE_A2F_WARMUP started duration=%.2f"),
        FMath::Clamp(ACEWarmupDurationSeconds, 0.05f, 2.0f)
    );
}

void UOpenAIJackComponent::ScheduleInstantAcknowledgement()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        TryEnqueueInstantAcknowledgement();
        return;
    }

    const float SafeDelaySeconds =
        FMath::Clamp(InstantAcknowledgementDelaySeconds, 0.0f, 3.0f);
    if (SafeDelaySeconds <= UE_SMALL_NUMBER)
    {
        TryEnqueueInstantAcknowledgement();
        return;
    }

    World->GetTimerManager().SetTimer(
        InstantAcknowledgementTimerHandle,
        FTimerDelegate::CreateWeakLambda(
            this,
            [this]()
            {
                TryEnqueueInstantAcknowledgement();
            }
        ),
        SafeDelaySeconds,
        false
    );
}

void UOpenAIJackComponent::TryEnqueueInstantAcknowledgement()
{
    if (!bBusy ||
        !bEnableQueuedSpeech ||
        !bEnableHttpTTS ||
        !bEnableInstantAcknowledgement ||
        bFinalSpeechQueuedForCurrentTurn)
    {
        return;
    }

    EnqueueSpeechText(InstantAcknowledgementText);
    PumpSpeechQueue();
}

TArray<FString> UOpenAIJackComponent::SplitReplyIntoSpeechSegments(
    const FString& ReplyText
) const
{
    TArray<FString> Segments;
    const FString TrimmedReply = ReplyText.TrimStartAndEnd();
    if (TrimmedReply.IsEmpty())
    {
        return Segments;
    }

    const int32 MaxSentencesPerSegment =
        FMath::Clamp(MaxSentencesPerSpeechSegment, 1, 6);
    const int32 FirstSentencesPerSegment =
        FMath::Clamp(FirstSpeechSegmentSentences, 1, 6);
    const int32 MaxSegmentCharacters =
        FMath::Clamp(MaxSpeechSegmentCharacters, 8, 120);
    const int32 FirstSoftMinCharacters =
        FMath::Clamp(FirstSpeechSegmentSoftMinCharacters, 16, 120);

    FString CurrentSegment;
    int32 CurrentSentenceCount = 0;
    bool bIsFirstSegment = true;
    auto FlushSegment = [&Segments, &CurrentSegment, &bIsFirstSegment]()
    {
        const FString Segment = CurrentSegment.TrimStartAndEnd();
        if (!Segment.IsEmpty())
        {
            Segments.Add(Segment);
            bIsFirstSegment = false;
        }
        CurrentSegment.Reset();
    };

    for (int32 Index = 0; Index < TrimmedReply.Len(); ++Index)
    {
        const TCHAR Character = TrimmedReply[Index];
        CurrentSegment.AppendChar(Character);

        const bool bHardBoundary = IsHardSpeechBoundary(Character);
        const bool bSoftBoundary = IsSoftSpeechBoundary(Character);
        const int32 CurrentLength = CurrentSegment.TrimStartAndEnd().Len();
        if (bHardBoundary)
        {
            ++CurrentSentenceCount;
            const int32 EffectiveMaxSentences =
                bIsFirstSegment
                    ? FirstSentencesPerSegment
                    : MaxSentencesPerSegment;
            if (CurrentSentenceCount >= EffectiveMaxSentences ||
                CurrentLength >= MaxSegmentCharacters)
            {
                FlushSegment();
                CurrentSentenceCount = 0;
            }
        }
        else if (
            bEnableFirstSpeechSegmentSoftBoundary &&
            bIsFirstSegment &&
            CurrentSentenceCount == 0 &&
            bSoftBoundary &&
            CurrentLength >= FirstSoftMinCharacters)
        {
            FlushSegment();
            CurrentSentenceCount = 0;
        }
        else if (bSoftBoundary && CurrentLength >= MaxSegmentCharacters)
        {
            FlushSegment();
            CurrentSentenceCount = 0;
        }
    }

    FlushSegment();
    return Segments;
}

void UOpenAIJackComponent::RequestWindowsSpeech(const FString& ReplyText)
{
#if PLATFORM_WINDOWS
    const FString VoiceName = WindowsVoiceName;
    const int32 VoiceRate = WindowsVoiceRate;
    const int32 VoiceVolume = WindowsVoiceVolume;
    const FString OutputPath = FPaths::CreateTempFilename(
        *FPaths::ProjectIntermediateDir(),
        TEXT("JackTTS_"),
        TEXT(".wav")
    );
    const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);

    Async(
        EAsyncExecution::ThreadPool,
        [WeakThis, ReplyText, VoiceName, VoiceRate, VoiceVolume, OutputPath]()
        {
            const double TTSStartSeconds = FPlatformTime::Seconds();
            TArray<uint8> WavBytes;
            FString Error;
            const HRESULT InitResult = CoInitializeEx(
                nullptr,
                COINIT_MULTITHREADED
            );
            const bool bShouldUninitialize =
                SUCCEEDED(InitResult);
            const bool bCOMReady =
                SUCCEEDED(InitResult) || InitResult == RPC_E_CHANGED_MODE;

            ISpVoice* Voice = nullptr;
            ISpStream* Stream = nullptr;
            ISpObjectToken* VoiceToken = nullptr;
            ISpObjectTokenCategory* VoiceCategory = nullptr;
            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 1;
            WaveFormat.nSamplesPerSec = 22050;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nBlockAlign =
                WaveFormat.nChannels * WaveFormat.wBitsPerSample / 8;
            WaveFormat.nAvgBytesPerSec =
                WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

            HRESULT Result = bCOMReady ? S_OK : InitResult;
            if (SUCCEEDED(Result))
            {
                Result = CoCreateInstance(
                    CLSID_SpVoice,
                    nullptr,
                    CLSCTX_ALL,
                    IID_ISpVoice,
                    reinterpret_cast<void**>(&Voice)
                );
            }
            if (SUCCEEDED(Result) && !VoiceName.IsEmpty())
            {
                IEnumSpObjectTokens* VoiceTokens = nullptr;
                if (SUCCEEDED(CoCreateInstance(
                        CLSID_SpObjectTokenCategory,
                        nullptr,
                        CLSCTX_INPROC_SERVER,
                        IID_ISpObjectTokenCategory,
                        reinterpret_cast<void**>(&VoiceCategory)
                    )) &&
                    SUCCEEDED(VoiceCategory->SetId(SPCAT_VOICES, false)) &&
                    SUCCEEDED(VoiceCategory->EnumTokens(
                        nullptr,
                        nullptr,
                        &VoiceTokens
                    )))
                {
                    ULONG TokenCount = 0;
                    VoiceTokens->GetCount(&TokenCount);
                    for (ULONG Index = 0; Index < TokenCount; ++Index)
                    {
                        ISpObjectToken* Candidate = nullptr;
                        if (VoiceTokens->Next(1, &Candidate, nullptr) != S_OK)
                        {
                            break;
                        }

                        WCHAR* Description = nullptr;
                        if (FAILED(Candidate->GetStringValue(nullptr, &Description)))
                        {
                            Candidate->GetStringValue(L"Name", &Description);
                        }
                        if (Description)
                        {
                            const FString CandidateName(Description);
                            CoTaskMemFree(Description);
                            if (CandidateName.Contains(
                                VoiceName,
                                ESearchCase::IgnoreCase
                            ))
                            {
                                VoiceToken = Candidate;
                                break;
                            }
                        }
                        Candidate->Release();
                    }
                    VoiceTokens->Release();
                }
            }

            if (SUCCEEDED(Result) && VoiceToken)
            {
                Voice->SetVoice(VoiceToken);
            }
            else if (SUCCEEDED(Result) && !VoiceName.IsEmpty())
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_WINDOWS_TTS voice_not_found=%s using_default_voice"),
                    *VoiceName
                );
            }
            if (SUCCEEDED(Result))
            {
                Voice->SetRate(FMath::Clamp(VoiceRate, -10, 10));
                Voice->SetVolume(
                    static_cast<USHORT>(FMath::Clamp(VoiceVolume, 0, 100))
                );
                Result = CoCreateInstance(
                    CLSID_SpStream,
                    nullptr,
                    CLSCTX_INPROC_SERVER,
                    IID_ISpStream,
                    reinterpret_cast<void**>(&Stream)
                );
            }
            if (SUCCEEDED(Result))
            {
                Result = Stream->BindToFile(
                    *OutputPath,
                    SPFM_CREATE_ALWAYS,
                    &SPDFID_WaveFormatEx,
                    &WaveFormat,
                    0
                );
            }
            if (SUCCEEDED(Result))
            {
                Result = Voice->SetOutput(Stream, true);
            }
            if (SUCCEEDED(Result))
            {
                Result = Voice->Speak(*ReplyText, SPF_DEFAULT, nullptr);
            }

            if (Stream)
            {
                Stream->Close();
                Stream->Release();
            }
            if (VoiceCategory)
            {
                VoiceCategory->Release();
            }
            if (VoiceToken)
            {
                VoiceToken->Release();
            }
            if (Voice)
            {
                Voice->Release();
            }
            if (bShouldUninitialize)
            {
                CoUninitialize();
            }

            if (SUCCEEDED(Result) &&
                !FFileHelper::LoadFileToArray(WavBytes, *OutputPath))
            {
                Error = TEXT("Windows TTS could not read its WAV output.");
            }
            else if (FAILED(Result))
            {
                Error = FString::Printf(
                    TEXT("Windows TTS failed with HRESULT 0x%08X."),
                    static_cast<uint32>(Result)
                );
            }
            IFileManager::Get().Delete(*OutputPath);

            AsyncTask(
                ENamedThreads::GameThread,
                [WeakThis,
                 WavBytes = MoveTemp(WavBytes),
                 ReplyText,
                 Error,
                 TTSDurationMs =
                    (FPlatformTime::Seconds() - TTSStartSeconds) * 1000.0]()
                {
                    if (!WeakThis.IsValid())
                    {
                        return;
                    }
                    if (!Error.IsEmpty())
                    {
                        WeakThis->Fail(Error);
                        return;
                    }

                    UE_LOG(
                        LogTemp,
                        Display,
                        TEXT("JACK_WINDOWS_TTS generated bytes=%d tts_ms=%.1f"),
                        WavBytes.Num(),
                        TTSDurationMs
                    );
                    const float SubtitleDurationSeconds =
                        EstimateWavDurationSeconds(WavBytes);
                    WeakThis->ShowScreenSubtitle(
                        ReplyText,
                        SubtitleDurationSeconds
                    );
                    if (WeakThis->bEnableACEAudio2Face &&
                        WeakThis->TryPlayWavWithACE(WavBytes))
                    {
                        return;
                    }

                    WeakThis->PlayWavBytes(WavBytes);
                    WeakThis->bBusy = false;
                    UE_LOG(
                        LogTemp,
                        Display,
                        TEXT("JACK_LOCAL_TTS played bytes=%d"),
                        WavBytes.Num()
                    );
                }
            );
        }
    );
#else
    Fail(TEXT("Windows TTS is only available on Windows."));
#endif
}

bool UOpenAIJackComponent::TryPlayWavWithACE(const TArray<uint8>& WavBytes)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || WavBytes.IsEmpty())
    {
        return false;
    }

    const FString OutputPath = FPaths::CreateTempFilename(
        *FPaths::ProjectIntermediateDir(),
        TEXT("JackACE_"),
        TEXT(".wav")
    );
    if (!FFileHelper::SaveArrayToFile(WavBytes, *OutputPath))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACE_A2F failed_to_write_wav path=%s"),
            *OutputPath
        );
        return false;
    }

    const bool bStarted = StartACEWavFileAsync(OutputPath, true);
    if (!bStarted)
    {
        IFileManager::Get().Delete(*OutputPath);
    }
    return bStarted;
}

bool UOpenAIJackComponent::StartACEWavFileAsync(
    const FString& WavFilePath,
    bool bDeleteWhenComplete
)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || WavFilePath.IsEmpty())
    {
        return false;
    }
    if (IsValid(ActiveACEAction))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACE_A2F_ASYNC busy path=%s"),
            *WavFilePath
        );
        return false;
    }

    UAsyncActionAnimateCharacter* Action =
        UAsyncActionAnimateCharacter::AnimateCharacterFromWavFileAsync(
            this,
            Owner,
            WavFilePath,
            FAudio2FaceEmotion(),
            nullptr,
            ACEAudio2FaceProviderName
        );
    if (!IsValid(Action))
    {
        return false;
    }

    ActiveACEAction = Action;
    PendingACEDeletePath = bDeleteWhenComplete ? WavFilePath : FString();
    Action->AudioSendCompleted.AddDynamic(
        this,
        &UOpenAIJackComponent::HandleACEAudioSendCompleted
    );
    Action->Activate();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACE_A2F_ASYNC started path=%s provider=%s"),
        *WavFilePath,
        *ACEAudio2FaceProviderName.ToString()
    );
    return true;
}

void UOpenAIJackComponent::HandleACEAudioSendCompleted(bool bSuccess)
{
    const bool bWasQueuedSpeech = bCurrentACEFromSpeechQueue;
    const bool bWasWarmup = bACEWarmupInFlight;
    if (bSuccess)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACE_A2F_ASYNC completed=1 provider=%s"),
            *ACEAudio2FaceProviderName.ToString()
        );
    }
    else
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACE_A2F_ASYNC completed=0 provider=%s"),
            *ACEAudio2FaceProviderName.ToString()
        );
    }

    if (!PendingACEDeletePath.IsEmpty())
    {
        IFileManager::Get().Delete(*PendingACEDeletePath);
        PendingACEDeletePath.Reset();
    }

    ActiveACEAction = nullptr;
    if (bWasWarmup)
    {
        bACEWarmupInFlight = false;
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACE_A2F_WARMUP completed=%d"),
            bSuccess ? 1 : 0
        );
        return;
    }

    if (bWasQueuedSpeech)
    {
        if (bSuccess)
        {
            bQueuedACESendCompleted = true;
            TryCompleteQueuedACESpeechSegment();
        }
        else
        {
            CompleteQueuedSpeechSegment();
        }
        return;
    }

    bBusy = false;
}

void UOpenAIJackComponent::HandleACEAnimationStarted()
{
    if (!bACEPlaybackInFlight || !bCurrentACEFromSpeechQueue)
    {
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("JACK_ACE_A2F_PLAYBACK started"));
    bQueuedACEPlaybackStarted = true;
    if (!PendingQueuedACESubtitleText.IsEmpty())
    {
        ShowScreenSubtitle(
            PendingQueuedACESubtitleText,
            PendingQueuedACESubtitleDurationSeconds
        );
        PendingQueuedACESubtitleText.Reset();
        PendingQueuedACESubtitleDurationSeconds = 0.0f;
    }
    StartSpeechPrefetches();
}

void UOpenAIJackComponent::HandleACEAnimationEnded()
{
    if (!bACEPlaybackInFlight || !bCurrentACEFromSpeechQueue)
    {
        return;
    }

    if (!bQueuedACEPlaybackStarted)
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_ACE_A2F_PLAYBACK stale_end_ignored"));
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("JACK_ACE_A2F_PLAYBACK ended"));
    bQueuedACEPlaybackEnded = true;
    TryCompleteQueuedACESpeechSegment();
}

void UOpenAIJackComponent::EnsureScreenSubtitleWidget()
{
    if (!bEnableScreenSubtitles || !GEngine || !GEngine->GameViewport)
    {
        return;
    }

    if (!SubtitleRootWidget.IsValid())
    {
        SAssignNew(SubtitleRootWidget, SOverlay)
        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Bottom)
        .Padding(FMargin(16.0f, 0.0f, 16.0f, SubtitleBottomPadding))
        [
            SNew(SBox)
            .HAlign(HAlign_Fill)
            .MinDesiredWidth(1200.0f)
            .MaxDesiredWidth(3500.0f)
            [
                SNew(SBorder)
                .HAlign(HAlign_Fill)
                .Padding(FMargin(18.0f, 10.0f))
                .BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
                .BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.72f))
                [
                    SAssignNew(SubtitleTextBlock, STextBlock)
                    .AutoWrapText(true)
                    .Justification(ETextJustify::Center)
                    .ColorAndOpacity(FLinearColor::White)
                    .ShadowOffset(FVector2D(1.0f, 1.0f))
                    .ShadowColorAndOpacity(FLinearColor::Black)
                    .Font(FCoreStyle::GetDefaultFontStyle(
                        FName(TEXT("Regular")),
                        FMath::Clamp(SubtitleFontSize, 12, 48)
                    ))
                ]
            ]
        ];
    }

    if (!bSubtitleWidgetAdded && SubtitleRootWidget.IsValid())
    {
        GEngine->GameViewport->AddViewportWidgetContent(
            SubtitleRootWidget.ToSharedRef(),
            1000
        );
        bSubtitleWidgetAdded = true;
    }
}

void UOpenAIJackComponent::ShowScreenSubtitle(
    const FString& Text,
    float AudioDurationSeconds
)
{
    if (!bEnableScreenSubtitles)
    {
        return;
    }

    const FString TrimmedText = Text.TrimStartAndEnd();
    if (TrimmedText.IsEmpty())
    {
        return;
    }

    EnsureScreenSubtitleWidget();
    if (!SubtitleTextBlock.IsValid())
    {
        return;
    }

    FString DisplayText = TrimmedText;
    const FString TrimmedSpeakerName = SubtitleSpeakerName.TrimStartAndEnd();
    if (bShowSubtitleSpeakerName && !TrimmedSpeakerName.IsEmpty())
    {
        DisplayText = FString::Printf(
            TEXT("%s: %s"),
            *TrimmedSpeakerName,
            *TrimmedText
        );
    }

    SubtitleTextBlock->SetText(FText::FromString(DisplayText));

    const float DisplaySeconds =
        GetSubtitleDisplayDuration(TrimmedText, AudioDurationSeconds);
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SubtitleTimerHandle);
        World->GetTimerManager().SetTimer(
            SubtitleTimerHandle,
            FTimerDelegate::CreateWeakLambda(
                this,
                [this]()
                {
                    HideScreenSubtitle();
                }
            ),
            DisplaySeconds,
            false
        );
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_SUBTITLE show duration=%.2f text=%s"),
        DisplaySeconds,
        *DisplayText
    );
}

void UOpenAIJackComponent::HideScreenSubtitle()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SubtitleTimerHandle);
    }

    if (SubtitleTextBlock.IsValid())
    {
        SubtitleTextBlock->SetText(FText::GetEmpty());
    }

    if (bSubtitleWidgetAdded &&
        SubtitleRootWidget.IsValid() &&
        GEngine &&
        GEngine->GameViewport)
    {
        GEngine->GameViewport->RemoveViewportWidgetContent(
            SubtitleRootWidget.ToSharedRef()
        );
        bSubtitleWidgetAdded = false;
    }
}

float UOpenAIJackComponent::GetSubtitleDisplayDuration(
    const FString& Text,
    float AudioDurationSeconds
) const
{
    const float MinimumDisplaySeconds =
        FMath::Clamp(SubtitleDisplaySeconds, 1.0f, 30.0f);
    const float ReadingDurationSeconds =
        static_cast<float>(Text.Len()) * 0.045f;
    const float AudioDisplaySeconds =
        AudioDurationSeconds > UE_SMALL_NUMBER
            ? AudioDurationSeconds + 0.2f
            : 0.0f;
    return FMath::Clamp(
        FMath::Max3(
            MinimumDisplaySeconds,
            ReadingDurationSeconds,
            AudioDisplaySeconds
        ),
        1.0f,
        30.0f
    );
}

void UOpenAIJackComponent::ResolveACEDirectMorphBridge()
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        ACECurveSource = nullptr;
        ACEFaceMesh = nullptr;
        return;
    }

    if (!IsValid(ACECurveSource))
    {
        ACECurveSource = Owner->FindComponentByClass<UACEAudioCurveSourceComponent>();
    }

    if (!IsValid(ACEFaceMesh))
    {
        TArray<USkeletalMeshComponent*> MeshComponents;
        Owner->GetComponents<USkeletalMeshComponent>(MeshComponents);
        for (USkeletalMeshComponent* MeshComponent : MeshComponents)
        {
            if (IsValid(MeshComponent) &&
                MeshComponent->GetName().Contains(TEXT("Face")))
            {
                ACEFaceMesh = MeshComponent;
                break;
            }
        }
    }
}

namespace
{
    float GetACECurveWeight(
        const TArray<float>& Weights,
        const TCHAR* CurveName
    )
    {
        const FName TargetName(CurveName);
        const int32 MaxIndex = FMath::Min(
            Weights.Num(),
            static_cast<int32>(UE_ARRAY_COUNT(UACEAudioCurveSourceComponent::CurveNames))
        );
        for (int32 Index = 0; Index < MaxIndex; ++Index)
        {
            if (UACEAudioCurveSourceComponent::CurveNames[Index] == TargetName)
            {
                return FMath::Clamp(Weights[Index], 0.0f, 1.0f);
            }
        }
        return 0.0f;
    }

    void SetMorphTargetScaled(
        USkeletalMeshComponent* Mesh,
        const TCHAR* MorphName,
        float Value,
        float Scale
    )
    {
        if (IsValid(Mesh))
        {
            Mesh->SetMorphTarget(
                FName(MorphName),
                FMath::Clamp(Value * Scale, 0.0f, 1.0f)
            );
        }
    }

    void ApplyACEToMetaHumanMorphs(
        USkeletalMeshComponent* Mesh,
        const TArray<float>& Weights,
        float Scale
    )
    {
        const float JawOpen = GetACECurveWeight(Weights, TEXT("JawOpen"));
        const float JawLeft = GetACECurveWeight(Weights, TEXT("JawLeft"));
        const float JawRight = GetACECurveWeight(Weights, TEXT("JawRight"));
        const float JawForward = GetACECurveWeight(Weights, TEXT("JawForward"));
        const float MouthClose = GetACECurveWeight(Weights, TEXT("MouthClose"));
        const float MouthFunnel = GetACECurveWeight(Weights, TEXT("MouthFunnel"));
        const float MouthPucker = GetACECurveWeight(Weights, TEXT("MouthPucker"));
        const float MouthSmileLeft = GetACECurveWeight(Weights, TEXT("MouthSmileLeft"));
        const float MouthSmileRight = GetACECurveWeight(Weights, TEXT("MouthSmileRight"));
        const float MouthFrownLeft = GetACECurveWeight(Weights, TEXT("MouthFrownLeft"));
        const float MouthFrownRight = GetACECurveWeight(Weights, TEXT("MouthFrownRight"));
        const float MouthDimpleLeft = GetACECurveWeight(Weights, TEXT("MouthDimpleLeft"));
        const float MouthDimpleRight = GetACECurveWeight(Weights, TEXT("MouthDimpleRight"));
        const float MouthStretchLeft = GetACECurveWeight(Weights, TEXT("MouthStretchLeft"));
        const float MouthStretchRight = GetACECurveWeight(Weights, TEXT("MouthStretchRight"));
        const float MouthPressLeft = GetACECurveWeight(Weights, TEXT("MouthPressLeft"));
        const float MouthPressRight = GetACECurveWeight(Weights, TEXT("MouthPressRight"));
        const float MouthLowerDownLeft = GetACECurveWeight(Weights, TEXT("MouthLowerDownLeft"));
        const float MouthLowerDownRight = GetACECurveWeight(Weights, TEXT("MouthLowerDownRight"));
        const float MouthUpperUpLeft = GetACECurveWeight(Weights, TEXT("MouthUpperUpLeft"));
        const float MouthUpperUpRight = GetACECurveWeight(Weights, TEXT("MouthUpperUpRight"));
        const float EyeBlinkLeft = GetACECurveWeight(Weights, TEXT("EyeBlinkLeft"));
        const float EyeBlinkRight = GetACECurveWeight(Weights, TEXT("EyeBlinkRight"));
        const float EyeWideLeft = GetACECurveWeight(Weights, TEXT("EyeWideLeft"));
        const float EyeWideRight = GetACECurveWeight(Weights, TEXT("EyeWideRight"));
        const float EyeSquintLeft = GetACECurveWeight(Weights, TEXT("EyeSquintLeft"));
        const float EyeSquintRight = GetACECurveWeight(Weights, TEXT("EyeSquintRight"));
        const float BrowDownLeft = GetACECurveWeight(Weights, TEXT("BrowDownLeft"));
        const float BrowDownRight = GetACECurveWeight(Weights, TEXT("BrowDownRight"));
        const float BrowInnerUp = GetACECurveWeight(Weights, TEXT("BrowInnerUp"));
        const float BrowOuterUpLeft = GetACECurveWeight(Weights, TEXT("BrowOuterUpLeft"));
        const float BrowOuterUpRight = GetACECurveWeight(Weights, TEXT("BrowOuterUpRight"));
        const float CheekPuff = GetACECurveWeight(Weights, TEXT("CheekPuff"));
        const float CheekSquintLeft = GetACECurveWeight(Weights, TEXT("CheekSquintLeft"));
        const float CheekSquintRight = GetACECurveWeight(Weights, TEXT("CheekSquintRight"));

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__jaw_open"), JawOpen, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__jaw_left"), JawLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__jaw_right"), JawRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__jaw_fwd"), JawForward, Scale);

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_funnel_UL"), MouthFunnel, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_funnel_UR"), MouthFunnel, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_funnel_DL"), MouthFunnel, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_funnel_DR"), MouthFunnel, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPurse_UL"), MouthPucker, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPurse_UR"), MouthPucker, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPurse_DL"), MouthPucker, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPurse_DR"), MouthPucker, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_press_UL"), MouthClose, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_press_UR"), MouthClose, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_press_DL"), MouthClose, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_press_DR"), MouthClose, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPress_L"), MouthPressLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lipsPress_R"), MouthPressRight, Scale);

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_cornerPull_left"), MouthSmileLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_cornerPull_right"), MouthSmileRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_cornerDepress_L"), MouthFrownLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_cornerDepress_R"), MouthFrownRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_dimple_left"), MouthDimpleLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_dimple_right"), MouthDimpleRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_stretch_left"), MouthStretchLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_stretch_right"), MouthStretchRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lowerLipDepress_left"), MouthLowerDownLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_lowerLipDepress_right"), MouthLowerDownRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_upperLipRaise_left"), MouthUpperUpLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__mouth_upperLipRaise_right"), MouthUpperUpRight, Scale);

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_blink_L"), EyeBlinkLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_blink_R"), EyeBlinkRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_blink_L"), EyeBlinkLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_blink_R"), EyeBlinkRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_widen_L"), EyeWideLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_widen_R"), EyeWideRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_widen_L"), EyeWideLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_widen_R"), EyeWideRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_squintInner_L"), EyeSquintLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_squintInner_R"), EyeSquintRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_squintInner_L"), EyeSquintLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("cartilage_lod0_mesh__eye_squintInner_R"), EyeSquintRight, Scale);

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_down_L"), BrowDownLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_down_R"), BrowDownRight, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_raiseIn_L"), BrowInnerUp, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_raiseIn_R"), BrowInnerUp, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_raiseOuter_left"), BrowOuterUpLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__brow_raiseOuter_right"), BrowOuterUpRight, Scale);

        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__cheek_blow_left"), CheekPuff, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__cheek_blow_right"), CheekPuff, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_cheekRaise_L"), CheekSquintLeft, Scale);
        SetMorphTargetScaled(Mesh, TEXT("head_lod0_mesh__eye_cheekRaise_R"), CheekSquintRight, Scale);
    }
}

void UOpenAIJackComponent::ApplyACEDirectMorphBridge()
{
    ResolveACEDirectMorphBridge();
    if (!IsValid(ACECurveSource) || !IsValid(ACEFaceMesh))
    {
        return;
    }

    TArray<float> Weights;
    ACECurveSource->GetCurveOutputsInterp(Weights);
    if (Weights.IsEmpty())
    {
        if (bACEDirectMorphsActive)
        {
            ClearACEDirectMorphBridge();
        }
        return;
    }

    ApplyACEToMetaHumanMorphs(
        ACEFaceMesh,
        Weights,
        FMath::Max(0.0f, ACEDirectMorphScale)
    );
    bACEDirectMorphsActive = true;
}

void UOpenAIJackComponent::ClearACEDirectMorphBridge()
{
    if (!IsValid(ACEFaceMesh))
    {
        return;
    }

    static const TCHAR* MorphsToClear[] = {
        TEXT("head_lod0_mesh__jaw_open"),
        TEXT("head_lod0_mesh__jaw_left"),
        TEXT("head_lod0_mesh__jaw_right"),
        TEXT("head_lod0_mesh__jaw_fwd"),
        TEXT("head_lod0_mesh__mouth_funnel_UL"),
        TEXT("head_lod0_mesh__mouth_funnel_UR"),
        TEXT("head_lod0_mesh__mouth_funnel_DL"),
        TEXT("head_lod0_mesh__mouth_funnel_DR"),
        TEXT("head_lod0_mesh__mouth_lipsPurse_UL"),
        TEXT("head_lod0_mesh__mouth_lipsPurse_UR"),
        TEXT("head_lod0_mesh__mouth_lipsPurse_DL"),
        TEXT("head_lod0_mesh__mouth_lipsPurse_DR"),
        TEXT("head_lod0_mesh__mouth_press_UL"),
        TEXT("head_lod0_mesh__mouth_press_UR"),
        TEXT("head_lod0_mesh__mouth_press_DL"),
        TEXT("head_lod0_mesh__mouth_press_DR"),
        TEXT("head_lod0_mesh__mouth_lipsPress_L"),
        TEXT("head_lod0_mesh__mouth_lipsPress_R"),
        TEXT("head_lod0_mesh__mouth_cornerPull_left"),
        TEXT("head_lod0_mesh__mouth_cornerPull_right"),
        TEXT("head_lod0_mesh__mouth_cornerDepress_L"),
        TEXT("head_lod0_mesh__mouth_cornerDepress_R"),
        TEXT("head_lod0_mesh__mouth_dimple_left"),
        TEXT("head_lod0_mesh__mouth_dimple_right"),
        TEXT("head_lod0_mesh__mouth_stretch_left"),
        TEXT("head_lod0_mesh__mouth_stretch_right"),
        TEXT("head_lod0_mesh__mouth_lowerLipDepress_left"),
        TEXT("head_lod0_mesh__mouth_lowerLipDepress_right"),
        TEXT("head_lod0_mesh__mouth_upperLipRaise_left"),
        TEXT("head_lod0_mesh__mouth_upperLipRaise_right"),
        TEXT("head_lod0_mesh__eye_blink_L"),
        TEXT("head_lod0_mesh__eye_blink_R"),
        TEXT("cartilage_lod0_mesh__eye_blink_L"),
        TEXT("cartilage_lod0_mesh__eye_blink_R"),
        TEXT("head_lod0_mesh__eye_widen_L"),
        TEXT("head_lod0_mesh__eye_widen_R"),
        TEXT("cartilage_lod0_mesh__eye_widen_L"),
        TEXT("cartilage_lod0_mesh__eye_widen_R"),
        TEXT("head_lod0_mesh__eye_squintInner_L"),
        TEXT("head_lod0_mesh__eye_squintInner_R"),
        TEXT("cartilage_lod0_mesh__eye_squintInner_L"),
        TEXT("cartilage_lod0_mesh__eye_squintInner_R"),
        TEXT("head_lod0_mesh__brow_down_L"),
        TEXT("head_lod0_mesh__brow_down_R"),
        TEXT("head_lod0_mesh__brow_raiseIn_L"),
        TEXT("head_lod0_mesh__brow_raiseIn_R"),
        TEXT("head_lod0_mesh__brow_raiseOuter_left"),
        TEXT("head_lod0_mesh__brow_raiseOuter_right"),
        TEXT("head_lod0_mesh__cheek_blow_left"),
        TEXT("head_lod0_mesh__cheek_blow_right"),
        TEXT("head_lod0_mesh__eye_cheekRaise_L"),
        TEXT("head_lod0_mesh__eye_cheekRaise_R"),
    };

    for (const TCHAR* MorphName : MorphsToClear)
    {
        ACEFaceMesh->SetMorphTarget(FName(MorphName), 0.0f);
    }
    bACEDirectMorphsActive = false;
}

void UOpenAIJackComponent::UpdateKeyboardPushToTalk(float DeltaTime)
{
    (void)DeltaTime;

    if (!bEnableKeyboardPushToTalk)
    {
        bKeyboardPushToTalkWasDown = false;
        bKeyboardPushToTalkStopPending = false;
        return;
    }

    UWorld* World = GetWorld();
    APlayerController* PlayerController =
        World ? World->GetFirstPlayerController() : nullptr;
    if (!PlayerController)
    {
        return;
    }

    const bool bIsDown =
        PlayerController->IsInputKeyDown(KeyboardPushToTalkKey);
    const double NowSeconds =
        World ? static_cast<double>(World->GetTimeSeconds()) : 0.0;

    if (bIsDown && !bKeyboardPushToTalkWasDown)
    {
        bKeyboardPushToTalkStopPending = false;
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_SPEECH_INPUT push_to_talk_down key=%s backend=%s"),
            *KeyboardPushToTalkKey.ToString(),
            bEnableHttpSTT ? TEXT("http") : TEXT("windows_sapi")
        );
        UE_LOG(
            LogTemp,
            Verbose,
            TEXT("JACK_WINDOWS_STT push_to_talk_down key=%s"),
            *KeyboardPushToTalkKey.ToString()
        );
        if (bEnableHttpSTT)
        {
            StartHttpSTT();
        }
        else
        {
            StartWindowsSTT();
        }
    }
    else if (!bIsDown && bKeyboardPushToTalkWasDown)
    {
        const float GraceSeconds = FMath::Max(
            0.0f,
            KeyboardPushToTalkReleaseGraceSeconds
        );
        bKeyboardPushToTalkStopPending = true;
        KeyboardPushToTalkStopAtTimeSeconds =
            NowSeconds + static_cast<double>(GraceSeconds);
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_SPEECH_INPUT push_to_talk_up grace=%.2f backend=%s"),
            GraceSeconds,
            bEnableHttpSTT ? TEXT("http") : TEXT("windows_sapi")
        );
        UE_LOG(
            LogTemp,
            Verbose,
            TEXT("JACK_WINDOWS_STT push_to_talk_up grace=%.2f"),
            GraceSeconds
        );
    }
    else if (bIsDown)
    {
        bKeyboardPushToTalkStopPending = false;
    }

    bKeyboardPushToTalkWasDown = bIsDown;

    if (bKeyboardPushToTalkStopPending &&
        NowSeconds >= KeyboardPushToTalkStopAtTimeSeconds)
    {
        bKeyboardPushToTalkStopPending = false;
        if (bEnableHttpSTT)
        {
            StopHttpSTT();
        }
        else if (bWindowsSTTListening)
        {
            StopWindowsSTT();
        }
    }
}

bool UOpenAIJackComponent::StartHttpSTT()
{
    if (HttpSTTStartUrl.TrimStartAndEnd().IsEmpty())
    {
        Fail(TEXT("HTTP STT start URL is empty."));
        return false;
    }
    if (bHttpSTTListening)
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_HTTP_STT already_listening"));
        return true;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("language"), HttpSTTLanguage);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const int32 RequestGeneration = ++HttpSTTRequestGeneration;
    bHttpSTTListening = true;
    bHttpSTTRequestInFlight = true;

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(HttpSTTStartUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [this, RequestGeneration](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (RequestGeneration != HttpSTTRequestGeneration)
            {
                return;
            }

            bHttpSTTRequestInFlight = false;
            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded ||
                !Response.IsValid() ||
                ResponseCode < 200 ||
                ResponseCode >= 300)
            {
                bHttpSTTListening = false;
                Fail(FString::Printf(
                    TEXT("HTTP STT start failed code=%d body=%s"),
                    ResponseCode,
                    Response.IsValid()
                        ? *Response->GetContentAsString()
                        : TEXT("<no response>")
                ));
                return;
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_HTTP_STT started url=%s language=%s"),
                *HttpSTTStartUrl,
                *HttpSTTLanguage
            );
        }
    );

    if (!Request->ProcessRequest())
    {
        bHttpSTTListening = false;
        bHttpSTTRequestInFlight = false;
        Fail(TEXT("HTTP STT start request could not be processed."));
        return false;
    }

    return true;
}

void UOpenAIJackComponent::StopHttpSTT()
{
    if (!bHttpSTTListening && !bHttpSTTRequestInFlight)
    {
        UE_LOG(LogTemp, Display, TEXT("JACK_HTTP_STT not_listening"));
        return;
    }
    if (HttpSTTStopUrl.TrimStartAndEnd().IsEmpty())
    {
        bHttpSTTListening = false;
        bHttpSTTRequestInFlight = false;
        Fail(TEXT("HTTP STT stop URL is empty."));
        return;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("language"), HttpSTTLanguage);

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const int32 RequestGeneration = ++HttpSTTRequestGeneration;
    bHttpSTTListening = false;
    bHttpSTTRequestInFlight = true;

    const double StopStartSeconds = FPlatformTime::Seconds();
    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(HttpSTTStopUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [this, RequestGeneration, StopStartSeconds](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (RequestGeneration != HttpSTTRequestGeneration)
            {
                return;
            }

            bHttpSTTRequestInFlight = false;
            const double StopMs =
                (FPlatformTime::Seconds() - StopStartSeconds) * 1000.0;
            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded ||
                !Response.IsValid() ||
                ResponseCode < 200 ||
                ResponseCode >= 300)
            {
                Fail(FString::Printf(
                    TEXT("HTTP STT stop failed code=%d body=%s"),
                    ResponseCode,
                    Response.IsValid()
                        ? *Response->GetContentAsString()
                        : TEXT("<no response>")
                ));
                return;
            }

            FString Text;
            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            if (FJsonSerializer::Deserialize(Reader, Root) &&
                Root.IsValid())
            {
                Root->TryGetStringField(TEXT("text"), Text);
            }

            Text = Text.TrimStartAndEnd();
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_HTTP_STT stopped stt_ms=%.1f chars=%d text=%s"),
                StopMs,
                Text.Len(),
                *Text
            );

            HandleSpeechInputText(Text, TEXT("http_whisper"));
        }
    );

    if (!Request->ProcessRequest())
    {
        bHttpSTTRequestInFlight = false;
        Fail(TEXT("HTTP STT stop request could not be processed."));
    }
}

void UOpenAIJackComponent::HandleSpeechInputText(
    const FString& Text,
    const TCHAR* Source
)
{
    const FString TrimmedText = Text.TrimStartAndEnd();
    if (TrimmedText.Len() < FMath::Max(1, WindowsSTTMinCharacters))
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_SPEECH_INPUT ignored_short source=%s chars=%d text=%s"),
            Source,
            TrimmedText.Len(),
            *TrimmedText
        );
        return;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_SPEECH_INPUT recognized source=%s text=%s"),
        Source,
        *TrimmedText
    );
    OnRecognizedSpeechText.Broadcast(TrimmedText);

    if (bBusy)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_SPEECH_INPUT ignored_busy source=%s text=%s"),
            Source,
            *TrimmedText
        );
        return;
    }

    SendPlayerText(TrimmedText);
}

void UOpenAIJackComponent::PollWindowsSTT()
{
#if PLATFORM_WINDOWS
    if (!bWindowsSTTListening || !WindowsSTTContext)
    {
        return;
    }

    ISpRecoContext* Context =
        static_cast<ISpRecoContext*>(WindowsSTTContext);
    while (bWindowsSTTListening)
    {
        SPEVENT Event = {};
        ULONG EventCount = 0;
        const HRESULT Result = Context->GetEvents(1, &Event, &EventCount);
        if (FAILED(Result) || EventCount == 0)
        {
            break;
        }

        FString RecognizedText;
        if (Event.eEventId == SPEI_SOUND_START)
        {
            UE_LOG(LogTemp, Display, TEXT("JACK_WINDOWS_STT sound_start"));
        }
        else if (Event.eEventId == SPEI_SOUND_END)
        {
            UE_LOG(LogTemp, Display, TEXT("JACK_WINDOWS_STT sound_end"));
        }
        else if (Event.eEventId == SPEI_HYPOTHESIS)
        {
            const FString HypothesisText = GetSapiRecoText(Event.lParam);
            if (!HypothesisText.IsEmpty() &&
                HypothesisText != LastWindowsSTTHypothesis)
            {
                LastWindowsSTTHypothesis = HypothesisText;
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_WINDOWS_STT hypothesis=%s"),
                    *HypothesisText
                );
            }
        }
        else if (Event.eEventId == SPEI_FALSE_RECOGNITION)
        {
            UE_LOG(LogTemp, Display, TEXT("JACK_WINDOWS_STT false_recognition"));
        }
        else if (Event.eEventId == SPEI_RECOGNITION)
        {
            RecognizedText = GetSapiRecoText(Event.lParam);
        }

        ClearSapiEvent(Event);

        if (!RecognizedText.IsEmpty() &&
            RecognizedText.Len() >= FMath::Max(1, WindowsSTTMinCharacters))
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_WINDOWS_STT recognized=%s"),
                *RecognizedText
            );

            if (bStopWindowsSTTAfterRecognizedSpeech)
            {
                StopWindowsSTT();
            }

            HandleSpeechInputText(RecognizedText, TEXT("windows_sapi"));
            return;
        }
    }
#endif
}

void UOpenAIJackComponent::CleanupWindowsSTT()
{
#if PLATFORM_WINDOWS
    ISpRecoGrammar* Grammar =
        static_cast<ISpRecoGrammar*>(WindowsSTTGrammar);
    ISpRecoContext* Context =
        static_cast<ISpRecoContext*>(WindowsSTTContext);
    ISpRecognizer* Recognizer =
        static_cast<ISpRecognizer*>(WindowsSTTRecognizer);
    ISpAudio* AudioInput =
        static_cast<ISpAudio*>(WindowsSTTAudioInput);

    if (Grammar)
    {
        Grammar->SetDictationState(SPRS_INACTIVE);
        Grammar->Release();
    }
    if (Context)
    {
        Context->Release();
    }
    if (Recognizer)
    {
        Recognizer->SetRecoState(SPRST_INACTIVE);
        Recognizer->Release();
    }
    if (AudioInput)
    {
        AudioInput->Release();
    }
    if (bWindowsSTTShouldUninitializeCOM)
    {
        CoUninitialize();
    }
#endif

    WindowsSTTGrammar = nullptr;
    WindowsSTTContext = nullptr;
    WindowsSTTRecognizer = nullptr;
    WindowsSTTAudioInput = nullptr;
    bWindowsSTTListening = false;
    bWindowsSTTShouldUninitializeCOM = false;
    LastWindowsSTTHypothesis.Reset();
}

bool UOpenAIJackComponent::AnimateWavFileWithACE(const FString& WavFilePath)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || WavFilePath.IsEmpty())
    {
        return false;
    }

    const bool bStarted = StartACEWavFileAsync(WavFilePath, false);

    if (bStarted)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACE_A2F_TEST started=1 path=%s provider=%s"),
            *WavFilePath,
            *ACEAudio2FaceProviderName.ToString()
        );
    }
    else
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACE_A2F_TEST started=0 path=%s provider=%s"),
            *WavFilePath,
            *ACEAudio2FaceProviderName.ToString()
        );
    }
    return bStarted;
}

void UOpenAIJackComponent::PlayWavBytes(const TArray<uint8>& WavBytes)
{
    int32 SampleRate = 0;
    int32 Channels = 0;
    TArray<uint8> Pcm;
    if (!ParseWav(WavBytes, SampleRate, Channels, Pcm))
    {
        Fail(TEXT("OpenAI returned an unsupported WAV format."));
        return;
    }

    ProceduralSound = NewObject<USoundWaveProcedural>(this);
    ProceduralSound->SetSampleRate(SampleRate);
    ProceduralSound->NumChannels = Channels;
    ProceduralSound->Duration = INDEFINITELY_LOOPING_DURATION;
    ProceduralSound->SoundGroup = SOUNDGROUP_Voice;
    ProceduralSound->QueueAudio(Pcm.GetData(), Pcm.Num());

    AudioComponent->SetSound(ProceduralSound);
    AudioComponent->Play();
}

void UOpenAIJackComponent::Fail(const FString& Message)
{
    ResetSpeechQueue();
    bBusy = false;
    UE_LOG(LogTemp, Error, TEXT("OPENAI_JACK_ERROR %s"), *Message);
    OnError.Broadcast(Message);
}
