#include "OpenAIJackComponent.h"

#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Dom/JsonObject.h"
#include "Async/Async.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Misc/CommandLine.h"
#include "Misc/Paths.h"
#include "Misc/Parse.h"
#include "Misc/ScopeLock.h"
#include "Containers/StringConv.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "InputAction.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Sound/SoundWaveProcedural.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimSequence.h"
#include "Algo/Reverse.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "OpenAINPCConversationSubsystem.h"
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
    TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<UOpenAIJackComponent>>
        GNaturalNPCSeatReservations;
    TWeakObjectPtr<UOpenAIJackComponent> GRealtimePushToTalkOwner;

    bool TryParseLLMProvider(
        const FString& Value,
        EJackLLMProvider& OutProvider
    )
    {
        const FString Normalized = Value.TrimStartAndEnd();
        if (Normalized.Equals(TEXT("openai"), ESearchCase::IgnoreCase) ||
            Normalized.Equals(TEXT("api"), ESearchCase::IgnoreCase) ||
            Normalized.Equals(TEXT("cloud"), ESearchCase::IgnoreCase))
        {
            OutProvider = EJackLLMProvider::OpenAIAPI;
            return true;
        }
        if (Normalized.Equals(TEXT("ollama"), ESearchCase::IgnoreCase) ||
            Normalized.Equals(TEXT("local"), ESearchCase::IgnoreCase))
        {
            OutProvider = EJackLLMProvider::OllamaLocal;
            return true;
        }
        return false;
    }

    bool IsRealtimePushToTalkOwner(const UOpenAIJackComponent* Component)
    {
        return IsValid(Component) &&
            GRealtimePushToTalkOwner.Get() == Component;
    }

    UOpenAIJackComponent* GetRealtimePushToTalkOwner()
    {
        return GRealtimePushToTalkOwner.Get();
    }

    bool TryClaimRealtimePushToTalk(UOpenAIJackComponent* Component)
    {
        if (!IsValid(Component))
        {
            return false;
        }
        if (!GRealtimePushToTalkOwner.IsValid())
        {
            GRealtimePushToTalkOwner = Component;
            return true;
        }
        return IsRealtimePushToTalkOwner(Component);
    }

    void ReleaseRealtimePushToTalk(UOpenAIJackComponent* Component)
    {
        if (IsRealtimePushToTalkOwner(Component))
        {
            GRealtimePushToTalkOwner.Reset();
        }
    }

    constexpr float SitIdlePelvisX = 2.996f;
    constexpr float SitIdlePelvisY = 7.531f;
    constexpr float SitIdlePelvisZ = 51.976f;

    bool IsNaturalNPCSeat(const AActor* Actor)
    {
        if (!IsValid(Actor))
        {
            return false;
        }

        if (Actor->ActorHasTag(TEXT("NPCSeat")))
        {
            return true;
        }

        const FString ActorName = Actor->GetName();
        return ActorName.Contains(TEXT("Chair"), ESearchCase::IgnoreCase) ||
            ActorName.Contains(TEXT("BarStool"), ESearchCase::IgnoreCase) ||
            ActorName.Contains(TEXT("Stool"), ESearchCase::IgnoreCase);
    }

    bool IsBarStoolSeat(const AActor* Actor)
    {
        if (!IsValid(Actor))
        {
            return false;
        }
        return Actor->ActorHasTag(TEXT("NPCSeat.BarStool")) ||
            Actor->GetName().Contains(
                TEXT("BarStool"),
                ESearchCase::IgnoreCase
            ) ||
            Actor->GetName().Contains(TEXT("Stool"), ESearchCase::IgnoreCase);
    }

    bool IsSeatReservedByAnother(
        const AActor* Seat,
        const UOpenAIJackComponent* Requester
    )
    {
        for (auto Iterator = GNaturalNPCSeatReservations.CreateIterator();
             Iterator;
             ++Iterator)
        {
            if (!Iterator.Key().IsValid() || !Iterator.Value().IsValid())
            {
                Iterator.RemoveCurrent();
                continue;
            }
            if (Iterator.Key().Get() == Seat)
            {
                return Iterator.Value().Get() != Requester;
            }
        }
        return false;
    }

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

    FString ExtractOpenAIChatReply(const TSharedPtr<FJsonObject>& Root)
    {
        const TArray<TSharedPtr<FJsonValue>>* Choices = nullptr;
        if (!Root->TryGetArrayField(TEXT("choices"), Choices) ||
            Choices->IsEmpty())
        {
            return FString();
        }

        const TSharedPtr<FJsonObject>* Choice = nullptr;
        if (!(*Choices)[0]->TryGetObject(Choice) || !Choice->IsValid())
        {
            return FString();
        }

        const TSharedPtr<FJsonObject>* Message = nullptr;
        if (!(*Choice)->TryGetObjectField(TEXT("message"), Message) ||
            !Message->IsValid())
        {
            return FString();
        }

        FString Content;
        (*Message)->TryGetStringField(TEXT("content"), Content);
        return Content.TrimStartAndEnd();
    }

    FString ExtractOpenAIChatDelta(const TSharedPtr<FJsonObject>& Root)
    {
        const TArray<TSharedPtr<FJsonValue>>* Choices = nullptr;
        if (!Root->TryGetArrayField(TEXT("choices"), Choices) ||
            Choices->IsEmpty())
        {
            return FString();
        }

        const TSharedPtr<FJsonObject>* Choice = nullptr;
        if (!(*Choices)[0]->TryGetObject(Choice) || !Choice->IsValid())
        {
            return FString();
        }

        const TSharedPtr<FJsonObject>* Delta = nullptr;
        if (!(*Choice)->TryGetObjectField(TEXT("delta"), Delta) ||
            !Delta->IsValid())
        {
            return FString();
        }

        FString Content;
        (*Delta)->TryGetStringField(TEXT("content"), Content);
        return Content;
    }

    void ExtractOpenAIUsage(
        const TSharedPtr<FJsonObject>& Root,
        double& OutPromptTokenCount,
        double& OutOutputTokenCount
    )
    {
        const TSharedPtr<FJsonObject>* Usage = nullptr;
        if (!Root->TryGetObjectField(TEXT("usage"), Usage) ||
            !Usage->IsValid())
        {
            return;
        }

        (*Usage)->TryGetNumberField(
            TEXT("prompt_tokens"),
            OutPromptTokenCount
        );
        (*Usage)->TryGetNumberField(
            TEXT("completion_tokens"),
            OutOutputTokenCount
        );
    }

    FString ExtractSseData(const FString& Line)
    {
        FString Payload = Line.TrimStartAndEnd();
        if (Payload.StartsWith(TEXT("data:")))
        {
            Payload.RightChopInline(5, EAllowShrinking::No);
            Payload.TrimStartAndEndInline();
        }
        return Payload;
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

    struct FSharedACEProviderState
    {
        int32 OwnerCount = 0;
        int32 PersistentOwnerCount = 0;
        int32 ActiveSessionCount = 0;
        uint64 Generation = 0;
        bool bResourcesMayBeAllocated = false;
        bool bPreallocationRequested = false;
        bool bWarmupRequested = false;
    };

    FCriticalSection GSharedACEProviderMutex;
    TMap<FName, FSharedACEProviderState> GSharedACEProviderStates;

    void ReleaseSharedACEProviderIfIdle(
        FName ProviderName,
        uint64 ExpectedGeneration
    )
    {
        bool bShouldFree = false;
        {
            FScopeLock Lock(&GSharedACEProviderMutex);
            FSharedACEProviderState* State =
                GSharedACEProviderStates.Find(ProviderName);
            if (State &&
                State->Generation == ExpectedGeneration &&
                State->ActiveSessionCount == 0 &&
                State->PersistentOwnerCount == 0 &&
                State->bResourcesMayBeAllocated)
            {
                State->bResourcesMayBeAllocated = false;
                State->bPreallocationRequested = false;
                State->bWarmupRequested = false;
                ++State->Generation;
                bShouldFree = true;
            }
        }

        if (bShouldFree)
        {
            UACEBlueprintLibrary::FreeA2F3DResources(ProviderName);
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_ACE_A2F_RESOURCES released provider=%s"),
                *ProviderName.ToString()
            );
        }
    }

    bool ContainsAnyPhrase(
        const FString& Text,
        std::initializer_list<const TCHAR*> Phrases
    )
    {
        for (const TCHAR* Phrase : Phrases)
        {
            if (Text.Contains(Phrase, ESearchCase::IgnoreCase))
            {
                return true;
            }
        }
        return false;
    }

    bool ContainsAnyWholeWord(
        const FString& Text,
        std::initializer_list<const TCHAR*> Words
    )
    {
        FString Normalized = TEXT(" ") + Text.ToLower() + TEXT(" ");
        Normalized = Normalized
            .Replace(TEXT(","), TEXT(" "))
            .Replace(TEXT("."), TEXT(" "))
            .Replace(TEXT("?"), TEXT(" "))
            .Replace(TEXT("!"), TEXT(" "))
            .Replace(TEXT(":"), TEXT(" "))
            .Replace(TEXT(";"), TEXT(" "));
        while (Normalized.Contains(TEXT("  ")))
        {
            Normalized.ReplaceInline(TEXT("  "), TEXT(" "));
        }

        for (const TCHAR* Word : Words)
        {
            if (Normalized.Contains(
                    TEXT(" ") + FString(Word).ToLower() + TEXT(" ")))
            {
                return true;
            }
        }
        return false;
    }

    bool ParseMovementDistanceCm(
        const FString& Text,
        float StepLengthCentimeters,
        float& OutDistanceCm
    )
    {
        float NumericValue = 0.0f;
        bool bFoundNumber = false;
        int32 NumberStart = INDEX_NONE;
        for (int32 Index = 0; Index < Text.Len(); ++Index)
        {
            if (FChar::IsDigit(Text[Index]))
            {
                NumberStart = Index;
                break;
            }
        }
        if (NumberStart != INDEX_NONE)
        {
            int32 NumberEnd = NumberStart;
            bool bSeenDecimal = false;
            while (NumberEnd < Text.Len())
            {
                const TCHAR Character = Text[NumberEnd];
                if (FChar::IsDigit(Character))
                {
                    ++NumberEnd;
                    continue;
                }
                if (Character == TEXT('.') && !bSeenDecimal)
                {
                    bSeenDecimal = true;
                    ++NumberEnd;
                    continue;
                }
                break;
            }
            NumericValue = FCString::Atof(
                *Text.Mid(NumberStart, NumberEnd - NumberStart)
            );
            bFoundNumber = NumericValue > 0.0f;
        }

        if (!bFoundNumber)
        {
            struct FNumberWord
            {
                const TCHAR* Word;
                float Value;
            };
            static const FNumberWord NumberWords[] = {
                {TEXT("twenty"), 20.0f}, {TEXT("nineteen"), 19.0f},
                {TEXT("eighteen"), 18.0f}, {TEXT("seventeen"), 17.0f},
                {TEXT("sixteen"), 16.0f}, {TEXT("fifteen"), 15.0f},
                {TEXT("fourteen"), 14.0f}, {TEXT("thirteen"), 13.0f},
                {TEXT("twelve"), 12.0f}, {TEXT("eleven"), 11.0f},
                {TEXT("ten"), 10.0f}, {TEXT("nine"), 9.0f},
                {TEXT("eight"), 8.0f}, {TEXT("seven"), 7.0f},
                {TEXT("six"), 6.0f}, {TEXT("five"), 5.0f},
                {TEXT("four"), 4.0f}, {TEXT("three"), 3.0f},
                {TEXT("two"), 2.0f}, {TEXT("one"), 1.0f},
                {TEXT("\u4e8c\u5341"), 20.0f},
                {TEXT("\u5341\u4e5d"), 19.0f},
                {TEXT("\u5341\u516b"), 18.0f},
                {TEXT("\u5341\u4e03"), 17.0f},
                {TEXT("\u5341\u516d"), 16.0f},
                {TEXT("\u5341\u4e94"), 15.0f},
                {TEXT("\u5341\u56db"), 14.0f},
                {TEXT("\u5341\u4e09"), 13.0f},
                {TEXT("\u5341\u4e8c"), 12.0f},
                {TEXT("\u5341\u4e00"), 11.0f},
                {TEXT("\u5341"), 10.0f},
                {TEXT("\u4e5d"), 9.0f}, {TEXT("\u516b"), 8.0f},
                {TEXT("\u4e03"), 7.0f}, {TEXT("\u516d"), 6.0f},
                {TEXT("\u4e94"), 5.0f}, {TEXT("\u56db"), 4.0f},
                {TEXT("\u4e09"), 3.0f}, {TEXT("\u4e24"), 2.0f},
                {TEXT("\u4e8c"), 2.0f}, {TEXT("\u4e00"), 1.0f}
            };

            FString WordText = TEXT(" ") + Text.ToLower() + TEXT(" ");
            WordText = WordText
                .Replace(TEXT(","), TEXT(" "))
                .Replace(TEXT("."), TEXT(" "))
                .Replace(TEXT("?"), TEXT(" "))
                .Replace(TEXT("!"), TEXT(" "));
            for (const FNumberWord& NumberWord : NumberWords)
            {
                const FString Word = NumberWord.Word;
                const bool bEnglishWord = Word[0] < 128;
                const bool bMatches = bEnglishWord
                    ? WordText.Contains(TEXT(" ") + Word + TEXT(" "))
                    : Text.Contains(Word);
                if (bMatches)
                {
                    NumericValue = NumberWord.Value;
                    bFoundNumber = true;
                    break;
                }
            }
        }

        if (!bFoundNumber)
        {
            return false;
        }

        const FString LowerText = Text.ToLower();
        if (ContainsAnyPhrase(LowerText, {
                TEXT("centimeter"), TEXT("centimetre"), TEXT(" cm"),
                TEXT("\u5398\u7c73")}))
        {
            OutDistanceCm = NumericValue;
        }
        else if (ContainsAnyPhrase(LowerText, {
                TEXT("meter"), TEXT("metre"), TEXT(" meters"),
                TEXT(" metres"), TEXT("\u7c73")}))
        {
            OutDistanceCm = NumericValue * 100.0f;
        }
        else if (ContainsAnyPhrase(LowerText, {
                TEXT("foot"), TEXT("feet"), TEXT(" ft"),
                TEXT("\u82f1\u5c3a")}))
        {
            OutDistanceCm = NumericValue * 30.48f;
        }
        else
        {
            OutDistanceCm = NumericValue * FMath::Max(
                20.0f,
                StepLengthCentimeters
            );
        }

        OutDistanceCm = FMath::Clamp(OutDistanceCm, 20.0f, 3000.0f);
        return true;
    }
}

UOpenAIJackComponent::UOpenAIJackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    VRPushToTalkAction = TSoftObjectPtr<UInputAction>(FSoftObjectPath(
        TEXT("/Game/XRFramework/Input/Actions/IA_PushToTalk.IA_PushToTalk")
    ));
}

FName UOpenAIJackComponent::GetResolvedNPCID() const
{
    if (!NPCID.IsNone())
    {
        return NPCID;
    }

    const FString SpeakerName = SubtitleSpeakerName.TrimStartAndEnd();
    if (!SpeakerName.IsEmpty())
    {
        return FName(*SpeakerName);
    }

    const AActor* Owner = GetOwner();
    return IsValid(Owner) ? Owner->GetFName() : NAME_None;
}

FString UOpenAIJackComponent::GetEffectiveModelKeepAlive() const
{
    const FString ConfiguredValue = bUseMemoryOptimizedModelLifetime
        ? ModelIdleKeepAlive.TrimStartAndEnd()
        : KeepAlive.TrimStartAndEnd();
    return ConfiguredValue.IsEmpty() ? TEXT("60s") : ConfiguredValue;
}

EJackLLMProvider UOpenAIJackComponent::GetEffectiveLLMProvider() const
{
    FString Override;
    if (FParse::Value(
            FCommandLine::Get(),
            TEXT("NaturalNPCLLMProvider="),
            Override))
    {
        EJackLLMProvider ParsedProvider;
        if (TryParseLLMProvider(Override, ParsedProvider))
        {
            return ParsedProvider;
        }
    }

    Override = FPlatformMisc::GetEnvironmentVariable(
        TEXT("NATURALNPC_LLM_PROVIDER")
    );
    EJackLLMProvider ParsedProvider;
    return TryParseLLMProvider(Override, ParsedProvider)
        ? ParsedProvider
        : LLMProvider;
}

void UOpenAIJackComponent::SetLLMProvider(EJackLLMProvider Provider)
{
    LLMProvider = Provider;
    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_LLM_BACKEND provider=%s actor=%s"),
        Provider == EJackLLMProvider::OpenAIAPI
            ? TEXT("openai")
            : TEXT("ollama"),
        *GetNameSafe(GetOwner())
    );

    if (GetEffectiveLLMProvider() == EJackLLMProvider::OpenAIAPI &&
        bUnloadLocalModelsWhenUsingOpenAI)
    {
        UnloadLocalModelsFromMemory();
    }
}

void UOpenAIJackComponent::BeginPlay()
{
    Super::BeginPlay();

    // Keeps headless automation away from ACE's render/audio worker without changing gameplay defaults.
    if (FParse::Param(FCommandLine::Get(), TEXT("NaturalNPCDisableACE")))
    {
        bEnableACEAudio2Face = false;
        UE_LOG(LogTemp, Log, TEXT("OpenAI NPC: ACE Audio2Face disabled by -NaturalNPCDisableACE."));
    }

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
        BindACEPlaybackDelegates();
        RegisterSharedACEProvider();
    }

    InitializeSessionMemory();
    const bool bUseOpenAI =
        GetEffectiveLLMProvider() == EJackLLMProvider::OpenAIAPI;
    if (bUseOpenAI && bUnloadLocalModelsWhenUsingOpenAI)
    {
        UnloadLocalModelsFromMemory();
    }
    if (bWarmEmbeddingModelOnBeginPlay &&
        (!bUseOpenAI || bUseLocalMemoryEmbeddingsInOpenAIMode))
    {
        WarmEmbeddingModel();
    }

    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>())
        {
            ConversationSubsystem->RegisterNPC(this);
        }
    }
}

void UOpenAIJackComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>())
        {
            ConversationSubsystem->UnregisterNPC(this);
        }
    }

    ++HttpSTTRequestGeneration;
    ++RealtimeVoiceRequestGeneration;
    bHttpSTTListening = false;
    bHttpSTTRequestInFlight = false;
    bRealtimeVoiceListening = false;
    bRealtimeVoiceRequestInFlight = false;
    ReleaseRealtimePushToTalk(this);
    CleanupWindowsSTT();
    PendingPostWorldActionCommand.Reset();
    CancelSittingImmediately();
    StopFollowingPlayer();
    InterruptConversationOutput();
    UnregisterSharedACEProvider();
    ResetSpeechQueue();
    HideScreenSubtitle();
    for (const FString& InterruptedPath : InterruptedACEDeletePaths)
    {
        if (!InterruptedPath.IsEmpty())
        {
            IFileManager::Get().Delete(*InterruptedPath);
        }
    }
    InterruptedACEDeletePaths.Reset();
    InterruptedACEActions.Reset();
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
    UpdateActionGrounding(DeltaTime);
    TryRunPendingPostWorldActionCommand();
}

FString UOpenAIJackComponent::GetApiKey() const
{
    return FPlatformMisc::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"));
}

FString UOpenAIJackComponent::GetLLMApiKey() const
{
    const FString EnvironmentVariable =
        OpenAIApiKeyEnvironmentVariable.TrimStartAndEnd();
    return EnvironmentVariable.IsEmpty()
        ? FString()
        : FPlatformMisc::GetEnvironmentVariable(*EnvironmentVariable);
}

FString UOpenAIJackComponent::GetResolvedRealtimeVoice() const
{
    const FString ConfiguredVoice =
        OpenAIRealtimeVoice.TrimStartAndEnd();
    if (!ConfiguredVoice.IsEmpty())
    {
        return ConfiguredVoice;
    }

    return GetResolvedNPCID().ToString().Contains(
        TEXT("Jack"),
        ESearchCase::IgnoreCase
    )
        ? TEXT("cedar")
        : TEXT("marin");
}

FString UOpenAIJackComponent::GetResolvedRealtimeInstructions() const
{
    FString Instructions = CharacterInstructions;
    if (GetResolvedNPCID().ToString().Contains(
            TEXT("Jack"),
            ESearchCase::IgnoreCase))
    {
        Instructions +=
            TEXT(" Speak with a warm, mature, clearly masculine bartender ")
            TEXT("delivery. Keep it natural and unforced.");
    }
    const FString SessionContext = BuildSessionConversationContext(true);
    if (!SessionContext.IsEmpty())
    {
        Instructions += TEXT("\n\n") + SessionContext;
    }
    return Instructions;
}

float UOpenAIJackComponent::GetEffectiveConversationListeningRadius() const
{
    const float ListeningRadius = FMath::Max(
        0.0f,
        AutonomousListeningRadius
    );
    if (!bEnableNearbyConversationContext)
    {
        return ListeningRadius;
    }
    return FMath::Min(
        ListeningRadius,
        FMath::Max(0.0f, NearbyConversationContextRadius)
    );
}

FString UOpenAIJackComponent::BuildSessionConversationContext(
    bool bIncludeOwnPrimaryTurns
) const
{
    if (SessionConversationExchanges.IsEmpty())
    {
        return FString();
    }

    const FName ThisNPCID = GetResolvedNPCID();
    const int32 MaximumContextTurns = FMath::Max(1, MaxConversationTurns);
    const int32 FirstIndex = FMath::Max(
        0,
        SessionConversationExchanges.Num() - MaximumContextTurns
    );
    FString Context = TEXT(
        "Session conversation memory (oldest to newest). Lines spoken by "
        "another NPC are things you heard, not things you said. Preserve "
        "speaker ownership and use this memory naturally:\n"
    );
    int32 IncludedExchangeCount = 0;

    for (int32 Index = FirstIndex;
         Index < SessionConversationExchanges.Num();
         ++Index)
    {
        const FSessionConversationExchange& Exchange =
            SessionConversationExchanges[Index];
        const bool bOwnPrimaryTurn = Exchange.PrimaryNPCID == ThisNPCID;
        bool bIncludedAnyLine = false;

        if (bIncludeOwnPrimaryTurns || !bOwnPrimaryTurn)
        {
            Context += FString::Printf(
                TEXT("Player to %s: %s\n%s: %s\n"),
                *Exchange.PrimaryNPCID.ToString(),
                *Exchange.PlayerText,
                *Exchange.PrimaryNPCID.ToString(),
                *Exchange.PrimaryReply
            );
            bIncludedAnyLine = true;
        }
        if (!Exchange.SecondaryNPCID.IsNone() &&
            !Exchange.SecondaryReply.IsEmpty())
        {
            Context += FString::Printf(
                TEXT("%s: %s\n"),
                *Exchange.SecondaryNPCID.ToString(),
                *Exchange.SecondaryReply
            );
            bIncludedAnyLine = true;
        }
        if (bIncludedAnyLine)
        {
            Context += TEXT("\n");
            ++IncludedExchangeCount;
        }
    }

    return IncludedExchangeCount > 0 ? Context.TrimEnd() : FString();
}

bool UOpenAIJackComponent::ShouldUseOpenAIRealtimeVoice() const
{
    return GetEffectiveLLMProvider() == EJackLLMProvider::OpenAIAPI &&
        bEnableOpenAIRealtimeVoice;
}

void UOpenAIJackComponent::UnloadLocalModelsFromMemory()
{
    FString UnloadUrl = OllamaChatUrl.TrimStartAndEnd();
    const int32 ApiPathIndex = UnloadUrl.Find(
        TEXT("/api/"),
        ESearchCase::IgnoreCase,
        ESearchDir::FromEnd
    );
    if (ApiPathIndex != INDEX_NONE)
    {
        UnloadUrl = UnloadUrl.Left(ApiPathIndex) + TEXT("/api/generate");
    }
    if (UnloadUrl.IsEmpty())
    {
        return;
    }

    TSet<FString> ModelsToUnload;
    if (!Model.TrimStartAndEnd().IsEmpty())
    {
        ModelsToUnload.Add(Model.TrimStartAndEnd());
    }
    if (!EmbeddingModel.TrimStartAndEnd().IsEmpty())
    {
        ModelsToUnload.Add(EmbeddingModel.TrimStartAndEnd());
    }

    for (const FString& ModelName : ModelsToUnload)
    {
        TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
        Body->SetStringField(TEXT("model"), ModelName);
        Body->SetNumberField(TEXT("keep_alive"), 0);

        FString Json;
        const TSharedRef<TJsonWriter<>> Writer =
            TJsonWriterFactory<>::Create(&Json);
        FJsonSerializer::Serialize(Body, Writer);

        const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
            FHttpModule::Get().CreateRequest();
        Request->SetURL(UnloadUrl);
        Request->SetVerb(TEXT("POST"));
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        Request->SetTimeout(5.0f);
        Request->SetContentAsString(Json);
        Request->OnProcessRequestComplete().BindLambda(
            [ModelName](
                FHttpRequestPtr,
                FHttpResponsePtr Response,
                bool bSucceeded
            )
            {
                if (bSucceeded && Response.IsValid() &&
                    Response->GetResponseCode() >= 200 &&
                    Response->GetResponseCode() < 300)
                {
                    UE_LOG(
                        LogTemp,
                        Display,
                        TEXT("JACK_LLM_BACKEND unloaded_local_model=%s"),
                        *ModelName
                    );
                }
            }
        );
        Request->ProcessRequest();
    }
}

void UOpenAIJackComponent::SendPlayerText(const FString& PlayerText)
{
    const FString TrimmedPlayerText = PlayerText.TrimStartAndEnd();
    if (TrimmedPlayerText.IsEmpty())
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>();
            IsValid(ConversationSubsystem))
        {
            if (ConversationSubsystem->HandlePermissionPlayerText(
                    TrimmedPlayerText))
            {
                return;
            }
            if (!ConversationSubsystem->CanAcceptPlayerText(this))
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT(
                        "NATURALNPC_AUTONOMY event=player_input_deferred "
                        "target=%s reason=SecondaryNPCSpeaking"
                    ),
                    *GetResolvedNPCID().ToString()
                );
                return;
            }
        }
    }

    FString ActionReply;
    if (TryExecutePlayerAction(
            TrimmedPlayerText,
            ActionReply,
            true))
    {
        return;
    }

    if (bBusy)
    {
        return;
    }

    ResetSpeechQueue();
    bBusy = true;
    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>())
        {
            ConversationSubsystem->NotifyPlayerTextSubmitted(
                this,
                TrimmedPlayerText
            );
        }
    }
    if (bEnableQueuedSpeech &&
        bEnableHttpTTS &&
        bEnableInstantAcknowledgement)
    {
        ScheduleInstantAcknowledgement();
    }

    if (GetEffectiveLLMProvider() == EJackLLMProvider::OpenAIAPI &&
        !bUseLocalMemoryEmbeddingsInOpenAIMode)
    {
        RequestResponse(TrimmedPlayerText, {});
    }
    else
    {
        RequestRelevantMemory(TrimmedPlayerText);
    }
}

bool UOpenAIJackComponent::TryExecuteRecognizedPlayerAction(
    const FString& PlayerText,
    FString& OutReply
)
{
    return TryExecutePlayerAction(PlayerText, OutReply, false);
}

bool UOpenAIJackComponent::TryExecutePlayerAction(
    const FString& PlayerText,
    FString& OutReply,
    const bool bSpeakReply
)
{
    const FString TrimmedPlayerText = PlayerText.TrimStartAndEnd();
    OutReply.Reset();
    if (TrimmedPlayerText.IsEmpty())
    {
        return false;
    }

    EnsureWorldStateAgent();
    FString RemainingCommand;
    if (bEnableWorldStateNaturalLanguageActions &&
        IsValid(WorldStateAgent) &&
        WorldStateAgent->TryExecuteNaturalLanguageActionDetailed(
            TrimmedPlayerText,
            OutReply,
            RemainingCommand
        ))
    {
        if (!RemainingCommand.IsEmpty())
        {
            if (WorldStateAgent->
                    IsNaturalLanguageActionSequenceInProgress())
            {
                PendingPostWorldActionCommand = RemainingCommand;
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_WORLD_ACTION deferred_command text=%s"),
                    *PendingPostWorldActionCommand
                );
            }
            else if (WorldStateAgent->
                WasLastNaturalLanguageActionSequenceSuccessful())
            {
                FString RemainingReply;
                if (TryHandleNaturalLanguageAction(
                        RemainingCommand,
                        RemainingReply) &&
                    bSpeakReply && !bBusy)
                {
                    SpeakLocalActionReply(
                        TrimmedPlayerText,
                        RemainingReply
                    );
                }
            }
        }
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_WORLD_ACTION handled text=%s reply=%s"),
            *TrimmedPlayerText,
            *OutReply
        );
        if (bSpeakReply && !bBusy)
        {
            SpeakLocalActionReply(TrimmedPlayerText, OutReply);
        }
        return true;
    }

    if (!TryHandleNaturalLanguageAction(TrimmedPlayerText, OutReply))
    {
        return false;
    }

    if (bSpeakReply && !bBusy)
    {
        SpeakLocalActionReply(TrimmedPlayerText, OutReply);
    }
    else if (bSpeakReply)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACTION handled_while_busy text=%s"),
            *TrimmedPlayerText
        );
    }
    return true;
}

void UOpenAIJackComponent::TryRunPendingPostWorldActionCommand()
{
    if (PendingPostWorldActionCommand.IsEmpty() ||
        !IsValid(WorldStateAgent) ||
        WorldStateAgent->IsNaturalLanguageActionSequenceInProgress())
    {
        return;
    }

    const FString Command = MoveTemp(PendingPostWorldActionCommand);
    PendingPostWorldActionCommand.Reset();
    if (!WorldStateAgent->
        WasLastNaturalLanguageActionSequenceSuccessful())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_WORLD_ACTION deferred_command_skipped text=%s "
                "reason=world_sequence_failed"),
            *Command
        );
        return;
    }

    FString Reply;
    const bool bHandled = TryHandleNaturalLanguageAction(Command, Reply);
    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_WORLD_ACTION deferred_command_completed text=%s "
            "handled=%d reply=%s"),
        *Command,
        bHandled ? 1 : 0,
        *Reply
    );
    if (bHandled && !bBusy)
    {
        SpeakLocalActionReply(Command, Reply);
    }
}

bool UOpenAIJackComponent::SpeakGovernedText(
    const FString& ResponseText
)
{
    const FString TrimmedReply = ResponseText.TrimStartAndEnd();
    if (TrimmedReply.IsEmpty() || bBusy)
    {
        return false;
    }

    ResetSpeechQueue();
    bBusy = true;
    PublishReplyText(TrimmedReply);

    if (ShouldUseOpenAIRealtimeVoice())
    {
        RequestRealtimeSpeech(TrimmedReply);
    }
    else if (bEnableQueuedSpeech && bEnableHttpTTS)
    {
        bFinalSpeechQueuedForCurrentTurn = true;
        for (const FString& Segment : SplitReplyIntoSpeechSegments(
            TrimmedReply))
        {
            EnqueueSpeechText(Segment);
        }
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
            Fail(TEXT(
                "OPENAI_API_KEY is required when OpenAI TTS is enabled."
            ));
            return false;
        }
        RequestSpeech(TrimmedReply);
    }
    else
    {
        bBusy = false;
    }

    return true;
}

bool UOpenAIJackComponent::IsConversationOutputActive() const
{
    return bBusy ||
        bACEPlaybackInFlight ||
        IsValid(ActiveACEAction) ||
        (IsValid(AudioComponent) && AudioComponent->IsPlaying());
}

bool UOpenAIJackComponent::InterruptConversationOutput()
{
    const bool bWasActive = IsConversationOutputActive();
    ResetSpeechQueue();

    if (IsValid(AudioComponent))
    {
        AudioComponent->Stop();
        AudioComponent->SetSound(nullptr);
    }
    if (AActor* Owner = GetOwner())
    {
        UACEBlueprintLibrary::StopCharacter(Owner);
    }
    if (IsValid(ActiveACEAction))
    {
        ActiveACEAction->AudioSendCompleted.RemoveDynamic(
            this,
            &UOpenAIJackComponent::HandleACEAudioSendCompleted
        );
        InterruptedACEActions.Add(ActiveACEAction);
        ActiveACEAction = nullptr;
    }
    MarkSharedACESessionFinished();
    if (!PendingACEDeletePath.IsEmpty())
    {
        InterruptedACEDeletePaths.Add(PendingACEDeletePath);
        PendingACEDeletePath.Reset();
    }

    bACEPlaybackInFlight = false;
    bACEWarmupInFlight = false;
    bCurrentACEFromSpeechQueue = false;
    bBusy = false;
    HideScreenSubtitle();

    if (bWasActive)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("NATURALNPC_AUTONOMY event=speech_interrupted npc=%s"),
            *GetResolvedNPCID().ToString()
        );
    }
    return bWasActive;
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
    SessionConversationExchanges.Reset();

    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
    UE_LOG(LogTemp, Display, TEXT("JACK_LOCAL_AI_MEMORY cleared session=true"));
}

void UOpenAIJackComponent::StartFollowingPlayer(bool bRun)
{
    if (bIsSitting || bSitDownInProgress || bStandUpInProgress)
    {
        PendingAfterStandAction = bRun
            ? EPendingAfterStandAction::FollowRun
            : EPendingAfterStandAction::FollowWalk;
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACTION queued_after_stand action=%s"),
            bRun ? TEXT("follow_run") : TEXT("follow_walk")
        );
        BeginStandUpTransition();
        return;
    }

    StartFollowingPlayerImmediate(bRun);
}

void UOpenAIJackComponent::StartFollowingPlayerImmediate(bool bRun)
{
    if (!bIsSitting && !bSitDownInProgress && !bStandUpInProgress)
    {
        ReleaseReservedSeat();
    }
    EnsureWorldStateAgent();
    if (IsValid(WorldStateAgent))
    {
        WorldStateAgent->SuspendHeldIdleAnimation();
    }
    FollowTargetActor = ResolveFollowTarget();
    bFollowingPlayer = true;
    bFollowUsingRun = bRun;
    bMovingToCommandLocation = false;
    bSitAfterCommandMove = false;
    ClearActionNavigationPath();
    ActionNavigationRepathElapsedSeconds = FMath::Max(
        0.05f,
        ActionNavigationRepathIntervalSeconds
    );
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
    if (!bFollowingPlayer && !bMovingToCommandLocation)
    {
        return;
    }

    const bool bCancelledSeatApproach =
        bMovingToCommandLocation && bSitAfterCommandMove;
    bFollowingPlayer = false;
    bFollowUsingRun = false;
    bMovingToCommandLocation = false;
    bCommandMoveUseRun = false;
    bFacePlayerWhenCommandMoveFinishes = false;
    bSitAfterCommandMove = false;
    FollowTargetActor.Reset();
    ClearActionNavigationPath();
    if (bCancelledSeatApproach)
    {
        ReleaseReservedSeat();
    }
    if (!bIsSitting && !bSitDownInProgress && !bStandUpInProgress)
    {
        RestoreActionAnimation();
        if (IsValid(WorldStateAgent))
        {
            WorldStateAgent->ResumeHeldIdleAnimation();
        }
    }

    UE_LOG(LogTemp, Display, TEXT("JACK_ACTION follow stopped"));
}

void UOpenAIJackComponent::PlayJumpAction()
{
    if (bIsSitting || bSitDownInProgress || bStandUpInProgress)
    {
        PendingAfterStandAction = EPendingAfterStandAction::Jump;
        BeginStandUpTransition();
        return;
    }

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

void UOpenAIJackComponent::PlaySitDownAction()
{
    if (bIsSitting || bSitDownInProgress)
    {
        return;
    }

    if (ReservedSeatActor.IsValid())
    {
        BeginSitAnimationAtReservedSeat();
        return;
    }

    const AActor* Owner = GetOwner();
    FString Failure;
    if (!IsValid(Owner) ||
        !StartSittingAtBestSeat(Owner->GetActorLocation(), Failure))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("JACK_ACTION sit_failed reason=%s"),
            Failure.IsEmpty() ? TEXT("NoSeat") : *Failure
        );
    }
}

bool UOpenAIJackComponent::StartSittingAtBestSeat(
    const FVector& ReferenceLocation,
    FString& OutFailure
)
{
    OutFailure.Reset();
    if (bIsSitting || bSitDownInProgress)
    {
        return true;
    }

    AActor* Seat = nullptr;
    FVector SeatCenter = FVector::ZeroVector;
    FVector SeatForward = FVector::ForwardVector;
    FVector ApproachLocation = FVector::ZeroVector;
    float SeatSurfaceZ = 0.0f;
    if (!FindBestAvailableSeat(
            ReferenceLocation,
            Seat,
            SeatCenter,
            SeatForward,
            SeatSurfaceZ,
            ApproachLocation))
    {
        OutFailure = TEXT("I can't find an available chair or bar stool nearby.");
        return false;
    }

    ReleaseReservedSeat();
    if (!ReserveSeat(Seat))
    {
        OutFailure = TEXT("That seat is already occupied.");
        return false;
    }

    ReservedSeatCenter = SeatCenter;
    ReservedSeatForward = SeatForward;
    ReservedSeatSurfaceZ = SeatSurfaceZ;
    ReservedSeatApproachLocation = ApproachLocation;

    if (!StartMoveToLocation(
            ApproachLocation,
            20.0f,
            false,
            false,
            OutFailure,
            true))
    {
        ReleaseReservedSeat();
        return false;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION seat_reserved seat=%s center=%s surface_z=%.1f "
            "approach=%s"),
        *Seat->GetName(),
        *SeatCenter.ToCompactString(),
        SeatSurfaceZ,
        *ApproachLocation.ToCompactString()
    );
    return true;
}

bool UOpenAIJackComponent::FindBestAvailableSeat(
    const FVector& ReferenceLocation,
    AActor*& OutSeat,
    FVector& OutSeatCenter,
    FVector& OutSeatForward,
    float& OutSeatSurfaceZ,
    FVector& OutApproachLocation
) const
{
    OutSeat = nullptr;
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!IsValid(Owner) || !World)
    {
        return false;
    }

    const float MaximumDistance = FMath::Max(100.0f, SeatSearchRadius);
    struct FSeatCandidate
    {
        TWeakObjectPtr<AActor> Seat;
        FVector Center = FVector::ZeroVector;
        FVector Forward = FVector::ForwardVector;
        FVector Approach = FVector::ZeroVector;
        float SurfaceZ = 0.0f;
        float Score = 0.0f;
    };
    TArray<FSeatCandidate> Candidates;
    for (TActorIterator<AActor> Iterator(World); Iterator; ++Iterator)
    {
        AActor* Candidate = *Iterator;
        if (!IsNaturalNPCSeat(Candidate) ||
            IsSeatReservedByAnother(Candidate, this))
        {
            continue;
        }

        FVector BoundsOrigin;
        FVector BoundsExtent;
        Candidate->GetActorBounds(false, BoundsOrigin, BoundsExtent);
        const float ReferenceDistance = FVector::Dist2D(
            ReferenceLocation,
            BoundsOrigin
        );
        if (ReferenceDistance > MaximumDistance)
        {
            continue;
        }

        FVector SeatForward = Candidate->GetActorForwardVector();
        if (IsBarStoolSeat(Candidate))
        {
            // Approach a bar stool from the player's side, then face the bar.
            SeatForward = BoundsOrigin - ReferenceLocation;
        }
        SeatForward.Z = 0.0f;
        if (!SeatForward.Normalize())
        {
            SeatForward = FVector::ForwardVector;
        }

        float SeatSurfaceZ = BoundsOrigin.Z + BoundsExtent.Z;
        const FVector TraceStart(
            BoundsOrigin.X,
            BoundsOrigin.Y,
            BoundsOrigin.Z + BoundsExtent.Z + 20.0f
        );
        const FVector TraceEnd(
            BoundsOrigin.X,
            BoundsOrigin.Y,
            BoundsOrigin.Z - BoundsExtent.Z - 20.0f
        );
        bool bFoundSeatSurface = false;
        FCollisionQueryParams TraceParams(
            SCENE_QUERY_STAT(NaturalNPCSeatSurface),
            true
        );
        TArray<UPrimitiveComponent*> PrimitiveComponents;
        Candidate->GetComponents(PrimitiveComponents);
        for (UPrimitiveComponent* Primitive : PrimitiveComponents)
        {
            if (!IsValid(Primitive) || !Primitive->IsCollisionEnabled())
            {
                continue;
            }
            FHitResult SurfaceHit;
            if (Primitive->LineTraceComponent(
                    SurfaceHit,
                    TraceStart,
                    TraceEnd,
                    TraceParams))
            {
                SeatSurfaceZ = bFoundSeatSurface
                    ? FMath::Max(SeatSurfaceZ, SurfaceHit.ImpactPoint.Z)
                    : SurfaceHit.ImpactPoint.Z;
                bFoundSeatSurface = true;
            }
        }

        FVector ApproachLocation =
            BoundsOrigin - SeatForward * FMath::Max(35.0f, SeatApproachDistance);
        ApproachLocation.Z = Owner->GetActorLocation().Z;
        const float OwnerDistance = FVector::Dist2D(
            Owner->GetActorLocation(),
            ApproachLocation
        );
        const float Score = ReferenceDistance + OwnerDistance * 0.35f;
        FSeatCandidate& SeatCandidate = Candidates.AddDefaulted_GetRef();
        SeatCandidate.Seat = Candidate;
        SeatCandidate.Center = BoundsOrigin;
        SeatCandidate.Forward = SeatForward;
        SeatCandidate.Approach = ApproachLocation;
        SeatCandidate.SurfaceZ = SeatSurfaceZ;
        SeatCandidate.Score = Score;
    }

    Candidates.Sort([](
        const FSeatCandidate& Left,
        const FSeatCandidate& Right)
    {
        return Left.Score < Right.Score;
    });

    UNavigationSystemV1* NavigationSystem =
        FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavigationSystem)
    {
        return false;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const FVector ProjectionExtent(160.0f, 160.0f, 250.0f);
    for (const FSeatCandidate& Candidate : Candidates)
    {
        AActor* CandidateSeat = Candidate.Seat.Get();
        if (!IsValid(CandidateSeat) ||
            IsSeatReservedByAnother(CandidateSeat, this))
        {
            continue;
        }

        bool bReachable = FVector::Dist2D(
            OwnerLocation,
            Candidate.Approach
        ) <= 20.0f;
        AActor* BlockingActor = nullptr;
        int32 ExpandedNodes = 0;
        if (!bReachable)
        {
            FNavLocation ProjectedApproach;
            if (!NavigationSystem->ProjectPointToNavigation(
                    Candidate.Approach,
                    ProjectedApproach,
                    ProjectionExtent))
            {
                UE_LOG(
                    LogTemp,
                    Verbose,
                    TEXT("JACK_ACTION seat_candidate_skipped seat=%s "
                        "reason=ProjectionFailed"),
                    *CandidateSeat->GetName()
                );
                continue;
            }

            UNavigationPath* Path =
                UNavigationSystemV1::FindPathToLocationSynchronously(
                    World,
                    OwnerLocation,
                    ProjectedApproach.Location,
                    nullptr
                );
            if (!Path || !Path->IsValid() || Path->IsPartial() ||
                Path->PathPoints.Num() < 2)
            {
                UE_LOG(
                    LogTemp,
                    Verbose,
                    TEXT("JACK_ACTION seat_candidate_skipped seat=%s "
                        "reason=NoNavPath"),
                    *CandidateSeat->GetName()
                );
                continue;
            }

            bool bPhysicalPathBlocked = false;
            for (int32 PointIndex = 1;
                 PointIndex < Path->PathPoints.Num();
                 ++PointIndex)
            {
                FHitResult SegmentHit;
                if (IsActionNavigationSegmentBlocked(
                        Path->PathPoints[PointIndex - 1],
                        Path->PathPoints[PointIndex],
                        SegmentHit,
                        CandidateSeat))
                {
                    bPhysicalPathBlocked = true;
                    BlockingActor = SegmentHit.GetActor();
                    break;
                }
            }

            if (!bPhysicalPathBlocked)
            {
                bReachable = true;
            }
            else
            {
                TArray<FVector> CollisionAwarePath;
                bReachable = BuildCollisionAwareNavigationPath(
                    OwnerLocation,
                    ProjectedApproach.Location,
                    CollisionAwarePath,
                    ExpandedNodes,
                    CandidateSeat
                );
            }
        }

        if (!bReachable)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_ACTION seat_candidate_skipped seat=%s "
                    "reason=PhysicallyBlocked blocker=%s expanded=%d"),
                *CandidateSeat->GetName(),
                IsValid(BlockingActor)
                    ? *BlockingActor->GetName()
                    : TEXT("unknown"),
                ExpandedNodes
            );
            continue;
        }

        OutSeat = CandidateSeat;
        OutSeatCenter = Candidate.Center;
        OutSeatForward = Candidate.Forward;
        OutSeatSurfaceZ = Candidate.SurfaceZ;
        OutApproachLocation = Candidate.Approach;
        return true;
    }

    return false;
}

bool UOpenAIJackComponent::ReserveSeat(AActor* Seat)
{
    if (!IsValid(Seat) || IsSeatReservedByAnother(Seat, this))
    {
        return false;
    }
    GNaturalNPCSeatReservations.Add(Seat, this);
    ReservedSeatActor = Seat;
    return true;
}

void UOpenAIJackComponent::ReleaseReservedSeat()
{
    AActor* Seat = ReservedSeatActor.Get();
    if (IsValid(Seat))
    {
        if (const TWeakObjectPtr<UOpenAIJackComponent>* Reservation =
                GNaturalNPCSeatReservations.Find(Seat);
            Reservation && Reservation->Get() == this)
        {
            GNaturalNPCSeatReservations.Remove(Seat);
        }
    }
    ReservedSeatActor.Reset();
    ReservedSeatCenter = FVector::ZeroVector;
    ReservedSeatForward = FVector::ForwardVector;
    ReservedSeatApproachLocation = FVector::ZeroVector;
    ReservedSeatSurfaceZ = 0.0f;
    bSeatBodyAlignmentActive = false;
}

void UOpenAIJackComponent::PrepareSeatBodyAlignment()
{
    AActor* Owner = GetOwner();
    USkeletalMeshComponent* Body = ResolveActionBodyMesh();
    if (!IsValid(Owner) || !IsValid(Body) || !ReservedSeatActor.IsValid())
    {
        return;
    }

    const FVector TargetPelvisWorld(
        ReservedSeatCenter.X,
        ReservedSeatCenter.Y,
        ReservedSeatSurfaceZ + SeatPelvisAboveSurface
    );
    const FVector TargetPelvisLocal =
        Owner->GetActorTransform().InverseTransformPosition(TargetPelvisWorld);
    ActionBodyTargetRelativeLocation = ActionBodyBaseRelativeLocation +
        TargetPelvisLocal - FVector(
            SitIdlePelvisX,
            SitIdlePelvisY,
            SitIdlePelvisZ
        );
    bSeatBodyAlignmentActive = true;
}

void UOpenAIJackComponent::BeginSitAnimationAtReservedSeat()
{
    if (bIsSitting || bSitDownInProgress)
    {
        return;
    }
    if (bMovingToCommandLocation)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (!ReservedSeatActor.IsValid() || !IsValid(Owner))
    {
        return;
    }

    if (bFollowingPlayer)
    {
        StopFollowingPlayer();
    }
    if (bStandUpInProgress)
    {
        CancelSittingImmediately();
    }

    PendingAfterStandAction = EPendingAfterStandAction::None;
    bSitAfterCommandMove = false;
    bIsSitting = false;
    bSitDownInProgress = true;
    bStandUpInProgress = false;
    const FRotator CurrentRotation = Owner->GetActorRotation();
    const FRotator SeatRotation = ReservedSeatForward.Rotation();
    Owner->SetActorRotation(FRotator(
        CurrentRotation.Pitch,
        SeatRotation.Yaw + ActionFacingYawOffsetDegrees,
        CurrentRotation.Roll
    ));
    PrepareSeatBodyAlignment();
    if (IsValid(WorldStateAgent))
    {
        WorldStateAgent->SuspendHeldIdleAnimation();
    }

    UAnimSequence* Sequence =
        ResolveActionAnimation(EActionAnimationState::SitDown);
    if (!bUseActionAnimationOverride || !Sequence)
    {
        bSitDownInProgress = false;
        bIsSitting = true;
        PlayActionAnimation(EActionAnimationState::SitIdle, true);
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACTION sit completed_without_transition")
        );
        return;
    }

    const float StartTime = FMath::Clamp(
        SitDownStartTimeSeconds,
        0.0f,
        FMath::Max(0.0f, Sequence->GetPlayLength() - 0.01f)
    );
    const float EndTime = FMath::Clamp(
        SitDownEndTimeSeconds,
        StartTime + 0.05f,
        Sequence->GetPlayLength()
    );
    const float PlayRate = FMath::Max(0.1f, SitTransitionPlayRate);
    PlayActionAnimation(
        EActionAnimationState::SitDown,
        false,
        StartTime,
        PlayRate
    );

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SitTransitionTimerHandle);
        World->GetTimerManager().SetTimer(
            SitTransitionTimerHandle,
            this,
            &UOpenAIJackComponent::FinishSitDownTransition,
            FMath::Max(0.05f, (EndTime - StartTime) / PlayRate),
            false
        );
    }
    else
    {
        FinishSitDownTransition();
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION sit started start=%.2f end=%.2f rate=%.2f"),
        StartTime,
        EndTime,
        PlayRate
    );
}

void UOpenAIJackComponent::FinishSitDownTransition()
{
    if (!bSitDownInProgress || bStandUpInProgress)
    {
        return;
    }

    bSitDownInProgress = false;
    bIsSitting = true;
    PlayActionAnimation(EActionAnimationState::SitIdle, true);
    if (USkeletalMeshComponent* Body = ResolveActionBodyMesh())
    {
        Body->SetRelativeLocation(ActionBodyTargetRelativeLocation);
    }
    UE_LOG(LogTemp, Display, TEXT("JACK_ACTION sit completed"));
}

void UOpenAIJackComponent::PlayStandUpAction()
{
    PendingAfterStandAction = EPendingAfterStandAction::None;
    BeginStandUpTransition();
}

void UOpenAIJackComponent::BeginStandUpTransition()
{
    if (bStandUpInProgress)
    {
        return;
    }
    if (!bIsSitting && !bSitDownInProgress)
    {
        RunPendingAfterStandAction();
        return;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SitTransitionTimerHandle);
    }
    bIsSitting = false;
    bSitDownInProgress = false;
    bStandUpInProgress = true;
    bSitAfterCommandMove = false;
    if (bSeatBodyAlignmentActive)
    {
        ActionBodyTargetRelativeLocation = ActionBodyBaseRelativeLocation;
    }

    UAnimSequence* Sequence =
        ResolveActionAnimation(EActionAnimationState::StandUp);
    if (!bUseActionAnimationOverride || !Sequence)
    {
        FinishStandUpTransition();
        return;
    }

    const float EndTime = FMath::Clamp(
        StandUpEndTimeSeconds,
        0.05f,
        Sequence->GetPlayLength()
    );
    const float PlayRate = FMath::Max(0.1f, SitTransitionPlayRate);
    PlayActionAnimation(
        EActionAnimationState::StandUp,
        false,
        0.0f,
        PlayRate
    );

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            SitTransitionTimerHandle,
            this,
            &UOpenAIJackComponent::FinishStandUpTransition,
            EndTime / PlayRate,
            false
        );
    }
    else
    {
        FinishStandUpTransition();
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION stand_up started end=%.2f rate=%.2f"),
        EndTime,
        PlayRate
    );
}

void UOpenAIJackComponent::FinishStandUpTransition()
{
    if (!bStandUpInProgress)
    {
        return;
    }

    const bool bPlayedStandUpAnimation =
        CurrentActionAnimationState == EActionAnimationState::StandUp;
    bStandUpInProgress = false;
    if (bPlayedStandUpAnimation && StandUpForwardAdjustment > 0.0f)
    {
        if (AActor* Owner = GetOwner())
        {
            const FVector WorldOffset =
                Owner->GetActorTransform().TransformVectorNoScale(
                    FVector(0.0f, StandUpForwardAdjustment, 0.0f)
                );
            Owner->SetActorLocation(
                Owner->GetActorLocation() + WorldOffset,
                true
            );
        }
    }

    RestoreActionAnimation();
    ReleaseReservedSeat();
    if (IsValid(WorldStateAgent))
    {
        WorldStateAgent->ResumeHeldIdleAnimation();
    }
    UE_LOG(LogTemp, Display, TEXT("JACK_ACTION stand_up completed"));
    RunPendingAfterStandAction();
}

void UOpenAIJackComponent::RunPendingAfterStandAction()
{
    const EPendingAfterStandAction PendingAction = PendingAfterStandAction;
    PendingAfterStandAction = EPendingAfterStandAction::None;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION run_after_stand action=%d"),
        static_cast<int32>(PendingAction)
    );

    switch (PendingAction)
    {
    case EPendingAfterStandAction::FollowWalk:
        StartFollowingPlayerImmediate(false);
        break;
    case EPendingAfterStandAction::FollowRun:
        StartFollowingPlayerImmediate(true);
        break;
    case EPendingAfterStandAction::MoveToLocation:
        {
            FString Failure;
            if (!StartMoveToLocation(
                    PendingStandMoveDestination,
                    PendingStandMoveStopDistance,
                    bPendingStandMoveUseRun,
                    bPendingStandMoveFacePlayer,
                    Failure,
                    bPendingStandMoveSitAtDestination))
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_ACTION post_stand_move_failed reason=%s"),
                    *Failure
                );
            }
        }
        break;
    case EPendingAfterStandAction::Jump:
        PlayJumpAction();
        break;
    default:
        break;
    }
}

void UOpenAIJackComponent::CancelSittingImmediately()
{
    const bool bHadSittingState =
        bIsSitting || bSitDownInProgress || bStandUpInProgress;
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SitTransitionTimerHandle);
    }
    bIsSitting = false;
    bSitDownInProgress = false;
    bStandUpInProgress = false;
    bSitAfterCommandMove = false;
    PendingAfterStandAction = EPendingAfterStandAction::None;
    if (bHadSittingState)
    {
        RestoreActionAnimation();
        if (IsValid(WorldStateAgent))
        {
            WorldStateAgent->ResumeHeldIdleAnimation();
        }
    }
    else
    {
        ResetActionBodyOffset();
    }
    ReleaseReservedSeat();
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

    const bool bStandUpIntent = ContainsAnyPhrase(LowerText, {
        TEXT("stand up"), TEXT("get up"), TEXT("rise to your feet"),
        TEXT("get back on your feet"), TEXT("get on your feet")}) ||
        ContainsAnyWholeWord(LowerText, {
            TEXT("stand"), TEXT("standa"), TEXT("stando")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u7ad9\u8d77\u6765"), TEXT("\u8d77\u8eab"),
            TEXT("\u8d77\u6765")});
    const bool bMovementAfterStandIntent = ContainsAnyPhrase(LowerText, {
        TEXT("follow me"), TEXT("come with me"),
        TEXT("come to me"), TEXT("come here"),
        TEXT("come over here"), TEXT("walk to me"),
        TEXT("in front of me"), TEXT("infront of me")});
    if (bStandUpIntent && !bMovementAfterStandIntent)
    {
        if (bIsSitting || bSitDownInProgress || bStandUpInProgress)
        {
            PlayStandUpAction();
            OutReply = TEXT("All right, I'm getting up.");
        }
        else
        {
            StopFollowingPlayer();
            OutReply = TEXT("I'm already standing.");
        }
        return true;
    }

    const FString TrimmedLowerText = LowerText.TrimStartAndEnd();
    const bool bSitIntent =
        TrimmedLowerText == TEXT("sit") ||
        TrimmedLowerText == TEXT("sit.") ||
        ContainsAnyPhrase(LowerText, {
            TEXT("sit down"), TEXT("sit here"), TEXT("please sit"),
            TEXT("can you sit"), TEXT("could you sit"),
            TEXT("come and sit"), TEXT("come sit"),
            TEXT("sit near me"), TEXT("sit nearby"),
            TEXT("sit next to me"), TEXT("sit beside me"),
            TEXT("sit by me"), TEXT("come sit near me"),
            TEXT("come and sit near me"),
            TEXT("take a seat"), TEXT("have a seat"),
            TEXT("be seated")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u5750\u4e0b"), TEXT("\u5750\u8fd9\u91cc"),
            TEXT("\u5750\u5728\u8fd9\u91cc"), TEXT("\u8fc7\u6765\u5750"),
            TEXT("\u6765\u8fd9\u91cc\u5750"), TEXT("\u5750\u6211\u65c1\u8fb9"),
            TEXT("\u5750\u5230\u6211\u65c1\u8fb9"), TEXT("\u5728\u6211\u65c1\u8fb9\u5750\u4e0b"),
            TEXT("\u5750\u6211\u9644\u8fd1"), TEXT("\u8fc7\u6765\u5750\u6211\u65c1\u8fb9")});
    if (bSitIntent)
    {
        const bool bSitHereIntent = ContainsAnyPhrase(LowerText, {
            TEXT("sit here"), TEXT("come and sit"), TEXT("come sit"),
            TEXT("come over and sit"), TEXT("sit near me"),
            TEXT("sit nearby"), TEXT("sit next to me"),
            TEXT("sit beside me"), TEXT("sit by me"),
            TEXT("come sit near me"), TEXT("come and sit near me")}) ||
            ContainsAnyPhrase(PlayerText, {
                TEXT("\u5750\u8fd9\u91cc"), TEXT("\u5750\u5728\u8fd9\u91cc"),
                TEXT("\u8fc7\u6765\u5750"), TEXT("\u6765\u8fd9\u91cc\u5750"),
                TEXT("\u5750\u6211\u65c1\u8fb9"), TEXT("\u5750\u5230\u6211\u65c1\u8fb9"),
                TEXT("\u5728\u6211\u65c1\u8fb9\u5750\u4e0b"), TEXT("\u5750\u6211\u9644\u8fd1"),
                TEXT("\u8fc7\u6765\u5750\u6211\u65c1\u8fb9")});
        if (bSitHereIntent)
        {
            AActor* PlayerActor = ResolveFollowTarget();
            if (!IsValid(PlayerActor))
            {
                OutReply = TEXT("I can't locate you right now.");
                return true;
            }

            FString Failure;
            if (!StartSittingAtBestSeat(
                    PlayerActor->GetActorLocation(),
                    Failure))
            {
                OutReply = Failure.IsEmpty()
                    ? TEXT("I can't find an available seat nearby.")
                    : Failure;
            }
            else
            {
                OutReply = TEXT("I'll take a seat near you.");
            }
        }
        else
        {
            const bool bWasAlreadySitting =
                bIsSitting || bSitDownInProgress;
            if (bWasAlreadySitting)
            {
                OutReply = TEXT("I'm already sitting.");
            }
            else
            {
                AActor* Owner = GetOwner();
                FString Failure;
                if (!IsValid(Owner) ||
                    !StartSittingAtBestSeat(
                        Owner->GetActorLocation(),
                        Failure))
                {
                    OutReply = Failure.IsEmpty()
                        ? TEXT("I can't find an available seat nearby.")
                        : Failure;
                }
                else
                {
                    OutReply = TEXT("All right, I'll take a seat.");
                }
            }
        }
        return true;
    }

    const bool bCorrectedStopIntent = ContainsAnyPhrase(LowerText, {
        TEXT("stop following"), TEXT("don't follow"),
        TEXT("dont follow"), TEXT("stay there"),
        TEXT("wait there"), TEXT("stop there")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u505c\u6b62\u8ddf\u968f"),
            TEXT("\u522b\u8ddf"), TEXT("\u4e0d\u8981\u8ddf"),
            TEXT("\u539f\u5730"), TEXT("\u7ad9\u4f4f"),
            TEXT("\u505c\u4e0b")});
    if (bCorrectedStopIntent ||
        ((bFollowingPlayer || bMovingToCommandLocation) &&
         LowerText == TEXT("stop")))
    {
        StopFollowingPlayer();
        OutReply = TEXT("I'll stay here.");
        return true;
    }

    const bool bCommandRunIntent = ContainsAnyPhrase(LowerText, {
        TEXT("run"), TEXT("hurry"), TEXT("faster"),
        TEXT("quick"), TEXT("catch up")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u8dd1"), TEXT("\u5feb\u70b9"),
            TEXT("\u5feb\u4e9b")});
    const bool bComeToPlayerIntent = ContainsAnyPhrase(LowerText, {
        TEXT("come to me"), TEXT("come over here"),
        TEXT("come here"), TEXT("walk to me"),
        TEXT("stand in front of me"), TEXT("come in front of me"),
        TEXT("stand infront of me"), TEXT("come infront of me")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u5230\u6211\u9762\u524d"),
            TEXT("\u6765\u6211\u9762\u524d"),
            TEXT("\u8d70\u5230\u6211\u9762\u524d"),
            TEXT("\u5230\u6211\u8fd9\u91cc"),
            TEXT("\u8fc7\u6765")});
    if (bComeToPlayerIntent)
    {
        AActor* PlayerActor = ResolveFollowTarget();
        if (!IsValid(PlayerActor))
        {
            OutReply = TEXT("I can't locate you right now.");
            return true;
        }

        FVector Destination = PlayerActor->GetActorLocation();
        const bool bInFront =
            LowerText.Contains(TEXT("in front of me")) ||
            LowerText.Contains(TEXT("infront of me")) ||
            PlayerText.Contains(TEXT("\u9762\u524d"));
        float StopDistance = FMath::Max(60.0f, FollowStopDistance);
        if (bInFront)
        {
            FVector ViewForward = PlayerActor->GetActorForwardVector();
            if (APlayerController* PlayerController = GetWorld()
                    ? GetWorld()->GetFirstPlayerController()
                    : nullptr)
            {
                FVector ViewLocation;
                FRotator ViewRotation;
                PlayerController->GetPlayerViewPoint(
                    ViewLocation,
                    ViewRotation
                );
                ViewForward = ViewRotation.Vector().GetSafeNormal2D();
            }
            Destination += ViewForward * FMath::Max(
                120.0f,
                FollowStopDistance
            );
            StopDistance = 35.0f;
        }

        FString Failure;
        if (!StartMoveToLocation(
                Destination,
                StopDistance,
                bCommandRunIntent,
                true,
                Failure))
        {
            OutReply = Failure.IsEmpty()
                ? TEXT("I can't find a route there.")
                : Failure;
        }
        else
        {
            OutReply = TEXT("I'm coming over.");
        }
        return true;
    }

    const bool bLeftIntent =
        LowerText.Contains(TEXT("left")) ||
        PlayerText.Contains(TEXT("\u5de6"));
    const bool bRightIntent =
        LowerText.Contains(TEXT("right")) ||
        PlayerText.Contains(TEXT("\u53f3"));
    const bool bBackwardIntent = ContainsAnyPhrase(LowerText, {
        TEXT("backward"), TEXT("backwards"), TEXT("step back"),
        TEXT("move back"), TEXT("walk back")}) ||
        PlayerText.Contains(TEXT("\u540e"));
    const bool bForwardIntent =
        LowerText.Contains(TEXT("forward")) ||
        PlayerText.Contains(TEXT("\u524d"));
    const bool bTurnIntent = ContainsAnyPhrase(LowerText, {
        TEXT("turn"), TEXT("rotate")}) ||
        PlayerText.Contains(TEXT("\u8f6c"));
    const bool bMoveVerb = ContainsAnyPhrase(LowerText, {
        TEXT("walk"), TEXT("move"), TEXT("go "),
        TEXT("advance"), TEXT("step")}) ||
        ContainsAnyPhrase(PlayerText, {
            TEXT("\u8d70"), TEXT("\u524d\u8fdb"),
            TEXT("\u79fb\u52a8")});

    float MovementDistanceCm = 0.0f;
    const bool bHasMovementDistance = ParseMovementDistanceCm(
        PlayerText,
        ActionStepLengthCentimeters,
        MovementDistanceCm
    );
    const bool bRelativeMoveIntent =
        (bLeftIntent || bRightIntent || bBackwardIntent || bForwardIntent) &&
        (bMoveVerb || bHasMovementDistance);
    if (bRelativeMoveIntent)
    {
        AActor* Owner = GetOwner();
        if (!IsValid(Owner))
        {
            OutReply = TEXT("I can't move right now.");
            return true;
        }

        if (!bHasMovementDistance)
        {
            MovementDistanceCm = ActionStepLengthCentimeters * 2.0f;
        }
        const float VisualYaw =
            Owner->GetActorRotation().Yaw - ActionFacingYawOffsetDegrees;
        FVector MoveDirection = FRotator(0.0f, VisualYaw, 0.0f).Vector();
        if (bLeftIntent)
        {
            MoveDirection = MoveDirection.RotateAngleAxis(
                -90.0f,
                FVector::UpVector
            );
        }
        else if (bRightIntent)
        {
            MoveDirection = MoveDirection.RotateAngleAxis(
                90.0f,
                FVector::UpVector
            );
        }
        else if (bBackwardIntent)
        {
            MoveDirection *= -1.0f;
        }

        const FVector Destination =
            Owner->GetActorLocation() +
            MoveDirection.GetSafeNormal2D() * MovementDistanceCm;
        FString Failure;
        if (!StartMoveToLocation(
                Destination,
                25.0f,
                bCommandRunIntent,
                false,
                Failure))
        {
            OutReply = Failure.IsEmpty()
                ? TEXT("I can't find a route that way.")
                : Failure;
        }
        else
        {
            OutReply = TEXT("Okay.");
        }
        return true;
    }

    if (bTurnIntent && (bLeftIntent || bRightIntent))
    {
        if (AActor* Owner = GetOwner())
        {
            FRotator Rotation = Owner->GetActorRotation();
            Rotation.Yaw += bLeftIntent ? -90.0f : 90.0f;
            Owner->SetActorRotation(Rotation);
        }
        OutReply = TEXT("Okay.");
        return true;
    }

    const bool bCorrectedChineseFollow = ContainsAnyPhrase(PlayerText, {
        TEXT("\u8ddf\u7740\u6211"),
        TEXT("\u8ddf\u968f\u6211"),
        TEXT("\u8ddf\u6211\u8d70"),
        TEXT("\u8ddf\u4e0a")});
    if (bCorrectedChineseFollow)
    {
        StartFollowingPlayer(bCommandRunIntent);
        OutReply = bCommandRunIntent
            ? TEXT("I'll keep up with you.")
            : TEXT("I'll follow you.");
        return true;
    }

    if (PlayerText.Contains(TEXT("\u8df3")))
    {
        PlayJumpAction();
        OutReply = TEXT("Sure.");
        return true;
    }
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

bool UOpenAIJackComponent::TryExecuteMovementCommand(
    const FString& PlayerText,
    FString& OutReply
)
{
    return TryHandleNaturalLanguageAction(PlayerText, OutReply);
}

void UOpenAIJackComponent::ClearActionNavigationPath()
{
    ActionNavigationPathPoints.Reset();
    ActionNavigationPathPointIndex = 0;
    ActionNavigationRepathElapsedSeconds = 0.0f;
    ActionNavigationStallElapsedSeconds = 0.0f;
    ActionNavigationFailureCount = 0;
    ActionNavigationGoal = FVector::ZeroVector;
    ActionPersistentAvoidanceDirection = FVector::ZeroVector;
    ActionPersistentAvoidanceDistanceRemaining = 0.0f;
    ActionPersistentBlockingActor.Reset();
    bActionActorGroundOffsetInitialized = false;
    ActionActorGroundOffsetZ = 0.0f;
}

bool UOpenAIJackComponent::IsActionNavigationLocationBlocked(
    const FVector& NavigationLocation,
    const AActor* AdditionalIgnoredActor
) const
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !IsValid(Owner))
    {
        return true;
    }

    const float Radius = FMath::Max(
        10.0f,
        ActionCollisionRadius +
            FMath::Max(0.0f, ActionNavigationClearancePadding)
    );
    const float HalfHeight = FMath::Max(
        Radius + 1.0f,
        ActionCollisionHalfHeight
    );
    const FVector CapsuleCenter(
        NavigationLocation.X,
        NavigationLocation.Y,
        NavigationLocation.Z + HalfHeight + 2.0f
    );

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCActionPathLocation),
        false,
        Owner
    );
    if (IsValid(WorldStateAgent) &&
        IsValid(WorldStateAgent->GetHeldActor()))
    {
        QueryParams.AddIgnoredActor(WorldStateAgent->GetHeldActor());
    }
    if (ReservedSeatActor.IsValid())
    {
        QueryParams.AddIgnoredActor(ReservedSeatActor.Get());
    }
    if (IsValid(AdditionalIgnoredActor))
    {
        QueryParams.AddIgnoredActor(AdditionalIgnoredActor);
    }

    TArray<FOverlapResult> Overlaps;
    if (!World->OverlapMultiByObjectType(
            Overlaps,
            CapsuleCenter,
            FQuat::Identity,
            ObjectQuery,
            FCollisionShape::MakeCapsule(Radius, HalfHeight),
            QueryParams))
    {
        return false;
    }
    for (const FOverlapResult& Overlap : Overlaps)
    {
        if (Overlap.bBlockingHit)
        {
            return true;
        }
    }
    return false;
}

bool UOpenAIJackComponent::IsActionNavigationSegmentBlocked(
    const FVector& StartNavigationLocation,
    const FVector& EndNavigationLocation,
    FHitResult& OutHit,
    const AActor* AdditionalIgnoredActor
) const
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !IsValid(Owner))
    {
        OutHit = FHitResult();
        return true;
    }

    const float Radius = FMath::Max(
        10.0f,
        ActionCollisionRadius +
            FMath::Max(0.0f, ActionNavigationClearancePadding)
    );
    const float HalfHeight = FMath::Max(
        Radius + 1.0f,
        ActionCollisionHalfHeight
    );
    // Keep the capsule bottom above the higher endpoint. This preserves
    // static wall/bar blocking without treating a stair riser or floor edge
    // below the feet as a wall.
    const float SweepGroundZ = FMath::Max(
        StartNavigationLocation.Z,
        EndNavigationLocation.Z
    );
    const FVector CapsuleStart(
        StartNavigationLocation.X,
        StartNavigationLocation.Y,
        SweepGroundZ + HalfHeight + 2.0f
    );
    const FVector CapsuleEnd(
        EndNavigationLocation.X,
        EndNavigationLocation.Y,
        SweepGroundZ + HalfHeight + 2.0f
    );

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCActionPathSegment),
        false,
        Owner
    );
    if (IsValid(WorldStateAgent) &&
        IsValid(WorldStateAgent->GetHeldActor()))
    {
        QueryParams.AddIgnoredActor(WorldStateAgent->GetHeldActor());
    }
    if (ReservedSeatActor.IsValid())
    {
        QueryParams.AddIgnoredActor(ReservedSeatActor.Get());
    }
    if (IsValid(AdditionalIgnoredActor))
    {
        QueryParams.AddIgnoredActor(AdditionalIgnoredActor);
    }

    if (CapsuleStart.Equals(CapsuleEnd, 0.1f))
    {
        OutHit = FHitResult();
        return IsActionNavigationLocationBlocked(
            StartNavigationLocation,
            AdditionalIgnoredActor
        );
    }

    FVector NavigationHitLocation;
    if (UNavigationSystemV1::NavigationRaycast(
            World,
            StartNavigationLocation,
            EndNavigationLocation,
            NavigationHitLocation))
    {
        OutHit = FHitResult();
        OutHit.bBlockingHit = true;
        OutHit.Location = NavigationHitLocation;
        OutHit.ImpactPoint = NavigationHitLocation;
        return true;
    }

    return World->SweepSingleByObjectType(
        OutHit,
        CapsuleStart,
        CapsuleEnd,
        FQuat::Identity,
        ObjectQuery,
        FCollisionShape::MakeCapsule(Radius, HalfHeight),
        QueryParams
    );
}

bool UOpenAIJackComponent::BuildCollisionAwareNavigationPath(
    const FVector& StartLocation,
    const FVector& GoalLocation,
    TArray<FVector>& OutPathPoints,
    int32& OutExpandedNodes,
    const AActor* AdditionalIgnoredActor
) const
{
    OutPathPoints.Reset();
    OutExpandedNodes = 0;

    UWorld* World = GetWorld();
    UNavigationSystemV1* NavigationSystem = World
        ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)
        : nullptr;
    if (!World || !NavigationSystem)
    {
        return false;
    }

    const float GridSize = FMath::Clamp(
        ActionFallbackGridSize,
        30.0f,
        120.0f
    );
    const FVector ProjectionExtent(
        GridSize * 0.48f,
        GridSize * 0.48f,
        250.0f
    );
    FNavLocation StartNavLocation;
    FNavLocation GoalNavLocation;
    if (!NavigationSystem->ProjectPointToNavigation(
            StartLocation,
            StartNavLocation,
            FVector(160.0f, 160.0f, 250.0f)) ||
        !NavigationSystem->ProjectPointToNavigation(
            GoalLocation,
            GoalNavLocation,
            FVector(160.0f, 160.0f, 250.0f)))
    {
        return false;
    }

    struct FActionGridOpenNode
    {
        FIntPoint Cell = FIntPoint::ZeroValue;
        float Score = 0.0f;

        bool operator<(const FActionGridOpenNode& Other) const
        {
            return Score < Other.Score;
        }
    };

    const FVector GridOrigin = StartNavLocation.Location;
    const FVector GoalDelta = GoalNavLocation.Location - GridOrigin;
    const FIntPoint GoalCell(
        FMath::RoundToInt(GoalDelta.X / GridSize),
        FMath::RoundToInt(GoalDelta.Y / GridSize)
    );
    const int32 MarginCells = FMath::CeilToInt(
        FMath::Clamp(
            ActionFallbackSearchMargin,
            200.0f,
            2000.0f
        ) / GridSize
    );
    const int32 MinCellX = FMath::Min(0, GoalCell.X) - MarginCells;
    const int32 MaxCellX = FMath::Max(0, GoalCell.X) + MarginCells;
    const int32 MinCellY = FMath::Min(0, GoalCell.Y) - MarginCells;
    const int32 MaxCellY = FMath::Max(0, GoalCell.Y) + MarginCells;

    TMap<FIntPoint, FVector> ValidCellLocations;
    TSet<FIntPoint> InvalidCells;
    auto ResolveCellLocation = [
        this,
        NavigationSystem,
        GridOrigin,
        GoalNavLocation,
        GridSize,
        ProjectionExtent,
        AdditionalIgnoredActor,
        &ValidCellLocations,
        &InvalidCells
    ](const FIntPoint& Cell, FVector& OutLocation) -> bool
    {
        if (const FVector* Existing = ValidCellLocations.Find(Cell))
        {
            OutLocation = *Existing;
            return true;
        }
        if (InvalidCells.Contains(Cell))
        {
            return false;
        }

        FVector Candidate(
            GridOrigin.X + static_cast<float>(Cell.X) * GridSize,
            GridOrigin.Y + static_cast<float>(Cell.Y) * GridSize,
            GridOrigin.Z
        );
        const float DistanceFromStart = FVector::Dist2D(
            Candidate,
            GridOrigin
        );
        const float DistanceFromGoal = FVector::Dist2D(
            Candidate,
            GoalNavLocation.Location
        );
        const float HeightAlpha = DistanceFromStart + DistanceFromGoal > 1.0f
            ? DistanceFromStart /
                (DistanceFromStart + DistanceFromGoal)
            : 0.0f;
        Candidate.Z = FMath::Lerp(
            GridOrigin.Z,
            GoalNavLocation.Location.Z,
            HeightAlpha
        );

        FNavLocation Projected;
        if (!NavigationSystem->ProjectPointToNavigation(
                Candidate,
                Projected,
                ProjectionExtent) ||
            FVector::Dist2D(Candidate, Projected.Location) >
                GridSize * 0.8f ||
            IsActionNavigationLocationBlocked(
                Projected.Location,
                AdditionalIgnoredActor))
        {
            InvalidCells.Add(Cell);
            return false;
        }

        OutLocation = Projected.Location;
        ValidCellLocations.Add(Cell, OutLocation);
        return true;
    };

    const FIntPoint StartCell = FIntPoint::ZeroValue;
    FVector ResolvedStart;
    if (!ResolveCellLocation(StartCell, ResolvedStart))
    {
        return false;
    }

    auto Heuristic = [GoalCell](const FIntPoint& Cell) -> float
    {
        const float DeltaX = static_cast<float>(GoalCell.X - Cell.X);
        const float DeltaY = static_cast<float>(GoalCell.Y - Cell.Y);
        return FMath::Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
    };

    TArray<FActionGridOpenNode> OpenHeap;
    TMap<FIntPoint, float> ScoresFromStart;
    TMap<FIntPoint, FIntPoint> CameFrom;
    TSet<FIntPoint> ClosedCells;
    ScoresFromStart.Add(StartCell, 0.0f);
    OpenHeap.HeapPush({StartCell, Heuristic(StartCell)});

    static const FIntPoint Neighbors[] = {
        FIntPoint(1, 0), FIntPoint(-1, 0),
        FIntPoint(0, 1), FIntPoint(0, -1),
        FIntPoint(1, 1), FIntPoint(1, -1),
        FIntPoint(-1, 1), FIntPoint(-1, -1)
    };

    FIntPoint ReachedCell = StartCell;
    bool bReachedGoal = false;
    const int32 MaxExpandedNodes = FMath::Clamp(
        ActionFallbackMaxExpandedNodes,
        500,
        20000
    );
    while (!OpenHeap.IsEmpty() && OutExpandedNodes < MaxExpandedNodes)
    {
        FActionGridOpenNode OpenNode;
        OpenHeap.HeapPop(OpenNode);
        if (ClosedCells.Contains(OpenNode.Cell))
        {
            continue;
        }

        const float* CurrentScorePtr = ScoresFromStart.Find(OpenNode.Cell);
        if (!CurrentScorePtr)
        {
            continue;
        }
        ClosedCells.Add(OpenNode.Cell);
        ++OutExpandedNodes;

        FVector CurrentLocation;
        if (!ResolveCellLocation(OpenNode.Cell, CurrentLocation))
        {
            continue;
        }

        FHitResult GoalHit;
        if (FVector::Dist2D(
                CurrentLocation,
                GoalNavLocation.Location) <= GridSize * 1.25f &&
            !IsActionNavigationSegmentBlocked(
                CurrentLocation,
                GoalNavLocation.Location,
                GoalHit,
                AdditionalIgnoredActor))
        {
            ReachedCell = OpenNode.Cell;
            bReachedGoal = true;
            break;
        }

        for (const FIntPoint& NeighborOffset : Neighbors)
        {
            const FIntPoint Neighbor = OpenNode.Cell + NeighborOffset;
            if (Neighbor.X < MinCellX || Neighbor.X > MaxCellX ||
                Neighbor.Y < MinCellY || Neighbor.Y > MaxCellY ||
                ClosedCells.Contains(Neighbor))
            {
                continue;
            }

            FVector NeighborLocation;
            if (!ResolveCellLocation(Neighbor, NeighborLocation))
            {
                continue;
            }
            const float NavigationHeightDelta =
                NeighborLocation.Z - CurrentLocation.Z;
            if (NavigationHeightDelta >
                    FMath::Max(10.0f, ActionMaximumGroundStepUp) ||
                NavigationHeightDelta <
                    -FMath::Max(20.0f, ActionMaximumGroundStepDown))
            {
                continue;
            }

            FHitResult EdgeHit;
            if (IsActionNavigationSegmentBlocked(
                    CurrentLocation,
                    NeighborLocation,
                    EdgeHit,
                    AdditionalIgnoredActor))
            {
                continue;
            }

            const bool bDiagonal = NeighborOffset.X != 0 &&
                NeighborOffset.Y != 0;
            const float TentativeScore = *CurrentScorePtr +
                (bDiagonal ? 1.41421356f : 1.0f);
            const float* ExistingScore = ScoresFromStart.Find(Neighbor);
            if (ExistingScore && TentativeScore >= *ExistingScore)
            {
                continue;
            }

            CameFrom.Add(Neighbor, OpenNode.Cell);
            ScoresFromStart.Add(Neighbor, TentativeScore);
            OpenHeap.HeapPush({
                Neighbor,
                TentativeScore + Heuristic(Neighbor)
            });
        }
    }

    if (!bReachedGoal)
    {
        return false;
    }

    TArray<FIntPoint> CellPath;
    CellPath.Add(ReachedCell);
    while (CellPath.Last() != StartCell)
    {
        const FIntPoint* Parent = CameFrom.Find(CellPath.Last());
        if (!Parent)
        {
            return false;
        }
        CellPath.Add(*Parent);
    }
    Algo::Reverse(CellPath);

    TArray<FVector> RawPath;
    RawPath.Reserve(CellPath.Num() + 1);
    for (const FIntPoint& Cell : CellPath)
    {
        FVector CellLocation;
        if (ResolveCellLocation(Cell, CellLocation))
        {
            RawPath.Add(CellLocation);
        }
    }
    if (RawPath.IsEmpty() ||
        !RawPath.Last().Equals(GoalNavLocation.Location, 1.0f))
    {
        RawPath.Add(GoalNavLocation.Location);
    }

    OutPathPoints.Reserve(RawPath.Num());
    for (const FVector& Point : RawPath)
    {
        if (OutPathPoints.Num() < 2)
        {
            OutPathPoints.Add(Point);
            continue;
        }

        const FVector PreviousDirection =
            (OutPathPoints.Last() -
             OutPathPoints[OutPathPoints.Num() - 2]).GetSafeNormal2D();
        const FVector NewDirection =
            (Point - OutPathPoints.Last()).GetSafeNormal2D();
        FHitResult CombinedHit;
        if (FVector::DotProduct(PreviousDirection, NewDirection) > 0.999f &&
            !IsActionNavigationSegmentBlocked(
                OutPathPoints[OutPathPoints.Num() - 2],
                Point,
                CombinedHit,
                AdditionalIgnoredActor))
        {
            OutPathPoints.Last() = Point;
        }
        else
        {
            OutPathPoints.Add(Point);
        }
    }

    return OutPathPoints.Num() >= 2;
}

bool UOpenAIJackComponent::RebuildActionNavigationPath(
    const FVector& DesiredGoal,
    float StopDistance
)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    UNavigationSystemV1* NavigationSystem = World
        ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)
        : nullptr;
    if (!IsValid(Owner) || !NavigationSystem)
    {
        return false;
    }

    const FVector OwnerLocation = Owner->GetActorLocation();
    const float CandidateRadius = StopDistance > 60.0f
        ? StopDistance * 0.75f
        : 0.0f;
    const int32 CandidateCount = CandidateRadius > 0.0f ? 12 : 1;
    FVector PreferredDirection = OwnerLocation - DesiredGoal;
    PreferredDirection.Z = 0.0f;
    if (!PreferredDirection.Normalize())
    {
        PreferredDirection = FVector::ForwardVector;
    }

    float BestPathLength = TNumericLimits<float>::Max();
    TArray<FVector> BestPoints;
    const FVector ProjectionExtent(160.0f, 160.0f, 250.0f);
    for (int32 CandidateIndex = 0;
         CandidateIndex < CandidateCount;
         ++CandidateIndex)
    {
        FVector CandidateLocation = DesiredGoal;
        if (CandidateRadius > 0.0f)
        {
            const float Angle =
                360.0f * static_cast<float>(CandidateIndex) /
                static_cast<float>(CandidateCount);
            CandidateLocation += PreferredDirection.RotateAngleAxis(
                Angle,
                FVector::UpVector
            ) * CandidateRadius;
        }

        FNavLocation Projected;
        if (!NavigationSystem->ProjectPointToNavigation(
                CandidateLocation,
                Projected,
                ProjectionExtent))
        {
            continue;
        }

        UNavigationPath* Path =
            UNavigationSystemV1::FindPathToLocationSynchronously(
                World,
                OwnerLocation,
                Projected.Location,
                nullptr
            );
        if (!Path || !Path->IsValid() || Path->IsPartial() ||
            Path->PathPoints.Num() < 2)
        {
            continue;
        }

        float PathLength = 0.0f;
        for (int32 PointIndex = 1;
             PointIndex < Path->PathPoints.Num();
             ++PointIndex)
        {
            PathLength += FVector::Dist2D(
                Path->PathPoints[PointIndex - 1],
                Path->PathPoints[PointIndex]
            );
        }
        if (PathLength < BestPathLength)
        {
            BestPathLength = PathLength;
            BestPoints = Path->PathPoints;
        }
    }

    if (BestPoints.IsEmpty())
    {
        return false;
    }

    bool bNeedsCollisionFallback = false;
    AActor* BlockingActor = nullptr;
    for (int32 PointIndex = 1;
         PointIndex < BestPoints.Num();
         ++PointIndex)
    {
        FHitResult SegmentHit;
        if (IsActionNavigationSegmentBlocked(
                BestPoints[PointIndex - 1],
                BestPoints[PointIndex],
                SegmentHit))
        {
            bNeedsCollisionFallback = true;
            BlockingActor = SegmentHit.GetActor();
            break;
        }
    }

    int32 ExpandedFallbackNodes = 0;
    if (bNeedsCollisionFallback)
    {
        TArray<FVector> CollisionAwarePoints;
        if (!BuildCollisionAwareNavigationPath(
                OwnerLocation,
                BestPoints.Last(),
                CollisionAwarePoints,
                ExpandedFallbackNodes))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("JACK_ACTION nav_fallback_failed npc=%s blocker=%s "
                    "expanded=%d goal=%s"),
                *Owner->GetName(),
                IsValid(BlockingActor)
                    ? *BlockingActor->GetName()
                    : TEXT("unknown"),
                ExpandedFallbackNodes,
                *DesiredGoal.ToCompactString()
            );
            return false;
        }
        BestPoints = MoveTemp(CollisionAwarePoints);
        BestPathLength = 0.0f;
        for (int32 PointIndex = 1;
             PointIndex < BestPoints.Num();
             ++PointIndex)
        {
            BestPathLength += FVector::Dist2D(
                BestPoints[PointIndex - 1],
                BestPoints[PointIndex]
            );
        }
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_ACTION nav_fallback npc=%s blocker=%s "
                "points=%d expanded=%d length=%.1f"),
            *Owner->GetName(),
            IsValid(BlockingActor)
                ? *BlockingActor->GetName()
                : TEXT("unknown"),
            BestPoints.Num(),
            ExpandedFallbackNodes,
            BestPathLength
        );
    }

    ActionNavigationPathPoints = MoveTemp(BestPoints);
    ActionNavigationPathPointIndex = 0;
    ActionNavigationGoal = DesiredGoal;
    ActionNavigationRepathElapsedSeconds = 0.0f;
    ActionNavigationStallElapsedSeconds = 0.0f;

    UE_LOG(
        LogTemp,
        Verbose,
        TEXT("JACK_ACTION nav_path npc=%s points=%d length=%.1f goal=%s"),
        *Owner->GetName(),
        ActionNavigationPathPoints.Num(),
        BestPathLength,
        *DesiredGoal.ToCompactString()
    );
    return true;
}

bool UOpenAIJackComponent::StartMoveToLocation(
    const FVector& Destination,
    float StopDistance,
    bool bRun,
    bool bFacePlayerAtDestination,
    FString& OutFailure,
    bool bSitAtDestination
)
{
    OutFailure.Reset();
    if (!bSitAtDestination &&
        !bIsSitting && !bSitDownInProgress && !bStandUpInProgress)
    {
        ReleaseReservedSeat();
    }
    if (bIsSitting || bSitDownInProgress || bStandUpInProgress)
    {
        PendingAfterStandAction = EPendingAfterStandAction::MoveToLocation;
        PendingStandMoveDestination = Destination;
        PendingStandMoveStopDistance = StopDistance;
        bPendingStandMoveUseRun = bRun;
        bPendingStandMoveFacePlayer = bFacePlayerAtDestination;
        bPendingStandMoveSitAtDestination = bSitAtDestination;
        UE_LOG(
            LogTemp,
            Display,
            TEXT(
                "JACK_ACTION queued_after_stand action=move_to "
                "destination=%s run=%d sit=%d"
            ),
            *Destination.ToCompactString(),
            bRun ? 1 : 0,
            bSitAtDestination ? 1 : 0
        );
        BeginStandUpTransition();
        return true;
    }

    EnsureWorldStateAgent();
    if (IsValid(WorldStateAgent))
    {
        WorldStateAgent->SuspendHeldIdleAnimation();
    }

    bFollowingPlayer = false;
    bFollowUsingRun = false;
    FollowTargetActor.Reset();
    bMovingToCommandLocation = true;
    bCommandMoveUseRun = bRun;
    bFacePlayerWhenCommandMoveFinishes = bFacePlayerAtDestination;
    bSitAfterCommandMove = bSitAtDestination;
    CommandMoveDestination = Destination;
    CommandMoveStopDistance = FMath::Max(10.0f, StopDistance);
    ClearActionNavigationPath();

    if (!RebuildActionNavigationPath(
            CommandMoveDestination,
            CommandMoveStopDistance))
    {
        bMovingToCommandLocation = false;
        bSitAfterCommandMove = false;
        if (IsValid(WorldStateAgent))
        {
            WorldStateAgent->ResumeHeldIdleAnimation();
        }
        OutFailure = TEXT("I can't find a navigable route there.");
        return false;
    }

    CurrentActionAnimationState = EActionAnimationState::None;
    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION move_to started run=%d destination=%s stop=%.1f"),
        bRun ? 1 : 0,
        *Destination.ToCompactString(),
        CommandMoveStopDistance
    );
    return true;
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

    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>())
        {
            ConversationSubsystem->NotifyPlayerTextSubmitted(
                this,
                PlayerText
            );
        }
    }

    ResetSpeechQueue();
    bBusy = true;
    AddConversationTurn(PlayerText, TrimmedReply);
    PublishReplyText(TrimmedReply);

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
    if ((!bFollowingPlayer && !bMovingToCommandLocation) ||
        DeltaTime <= 0.0f)
    {
        return;
    }

    {
        AActor* Owner = GetOwner();
        if (!IsValid(Owner))
        {
            return;
        }

        AActor* Target = nullptr;
        FVector DesiredGoal = CommandMoveDestination;
        float StopDistance = FMath::Max(
            10.0f,
            CommandMoveStopDistance
        );
        bool bRequestedRun = bCommandMoveUseRun;
        if (bFollowingPlayer)
        {
            Target = FollowTargetActor.Get();
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
            DesiredGoal = Target->GetActorLocation();
            StopDistance = FMath::Max(40.0f, FollowStopDistance);
            bRequestedRun = bFollowUsingRun;
        }

        const FVector OwnerLocation = Owner->GetActorLocation();
        FVector ToGoal = DesiredGoal - OwnerLocation;
        ToGoal.Z = 0.0f;
        const float DistanceToGoal = ToGoal.Size();
        const float ArrivalDistance = StopDistance +
            (bFollowingPlayer &&
             CurrentActionAnimationState == EActionAnimationState::Idle
                ? FMath::Max(0.0f, FollowResumeDistanceBuffer)
                : 0.0f);
        if (DistanceToGoal <= ArrivalDistance)
        {
            const bool bCompletedCommandMove = bMovingToCommandLocation;
            const bool bShouldSit =
                bCompletedCommandMove && bSitAfterCommandMove;
            if (!bActionJumpInProgress && !bShouldSit)
            {
                PlayActionAnimation(EActionAnimationState::Idle, true);
            }

            FVector FacingVector = ToGoal;
            if (bCompletedCommandMove)
            {
                const bool bFacePlayer =
                    bFacePlayerWhenCommandMoveFinishes;
                bMovingToCommandLocation = false;
                bCommandMoveUseRun = false;
                bFacePlayerWhenCommandMoveFinishes = false;
                bSitAfterCommandMove = false;
                ClearActionNavigationPath();
                if (!bShouldSit)
                {
                    RestoreActionAnimation();
                    if (IsValid(WorldStateAgent))
                    {
                        WorldStateAgent->ResumeHeldIdleAnimation();
                    }
                }

                if (bFacePlayer)
                {
                    Target = ResolveFollowTarget();
                    if (IsValid(Target))
                    {
                        FacingVector = Target->GetActorLocation() -
                            Owner->GetActorLocation();
                        FacingVector.Z = 0.0f;
                    }
                }
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_ACTION move_to completed sit=%d"),
                    bShouldSit ? 1 : 0
                );
            }

            if (bShouldSit && ReservedSeatActor.IsValid())
            {
                const FRotator CurrentRotation = Owner->GetActorRotation();
                const FRotator SeatRotation = ReservedSeatForward.Rotation();
                Owner->SetActorRotation(FRotator(
                    CurrentRotation.Pitch,
                    SeatRotation.Yaw + ActionFacingYawOffsetDegrees,
                    CurrentRotation.Roll
                ));
            }
            else if (!FacingVector.IsNearlyZero())
            {
                const FRotator DesiredRotation =
                    FacingVector.GetSafeNormal2D().Rotation();
                const FRotator CurrentRotation = Owner->GetActorRotation();
                const FRotator TargetRotation(
                    CurrentRotation.Pitch,
                    DesiredRotation.Yaw + ActionFacingYawOffsetDegrees,
                    CurrentRotation.Roll
                );
                Owner->SetActorRotation(
                    bShouldSit
                        ? TargetRotation
                        : FMath::RInterpTo(
                            CurrentRotation,
                            TargetRotation,
                            DeltaTime,
                            FMath::Max(0.1f, FollowRotationInterpSpeed)
                        )
                );
            }
            if (bShouldSit)
            {
                BeginSitAnimationAtReservedSeat();
            }
            return;
        }

        ActionNavigationRepathElapsedSeconds += DeltaTime;
        const float RepathInterval = FMath::Max(
            0.05f,
            ActionNavigationRepathIntervalSeconds
        );
        const bool bGoalMoved = FVector::Dist2D(
            DesiredGoal,
            ActionNavigationGoal
        ) > FMath::Max(50.0f, StopDistance * 0.35f);
        if ((ActionNavigationPathPoints.IsEmpty() || bGoalMoved) &&
            ActionNavigationRepathElapsedSeconds >= RepathInterval)
        {
            if (!RebuildActionNavigationPath(DesiredGoal, StopDistance))
            {
                ++ActionNavigationFailureCount;
                ActionNavigationRepathElapsedSeconds = 0.0f;
            }
            else
            {
                ActionNavigationFailureCount = 0;
            }
        }

        while (ActionNavigationPathPointIndex <
                ActionNavigationPathPoints.Num() &&
            FVector::Dist2D(
                OwnerLocation,
                ActionNavigationPathPoints[ActionNavigationPathPointIndex]
            ) <= 20.0f)
        {
            ++ActionNavigationPathPointIndex;
        }
        while (ActionNavigationPathPointIndex + 1 <
               ActionNavigationPathPoints.Num())
        {
            const FVector ToLaterPoint =
                ActionNavigationPathPoints[
                    ActionNavigationPathPointIndex + 1
                ] - OwnerLocation;
            FHitResult LookAheadHit;
            if (IsActionCapsuleMoveBlocked(
                    OwnerLocation,
                    ToLaterPoint,
                    LookAheadHit))
            {
                break;
            }
            ++ActionNavigationPathPointIndex;
        }
        if (ActionNavigationPathPointIndex >=
            ActionNavigationPathPoints.Num())
        {
            if (ActionNavigationRepathElapsedSeconds >= RepathInterval)
            {
                if (!RebuildActionNavigationPath(
                        DesiredGoal,
                        StopDistance))
                {
                    ++ActionNavigationFailureCount;
                    ActionNavigationRepathElapsedSeconds = 0.0f;
                }
            }
            if (ActionNavigationFailureCount >= 3 &&
                bMovingToCommandLocation)
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_ACTION move_to failed reason=NoPath")
                );
                StopFollowingPlayer();
                return;
            }
            if (!bActionJumpInProgress)
            {
                PlayActionAnimation(EActionAnimationState::Idle, true);
            }
            return;
        }

        FVector ToPathPoint =
            ActionNavigationPathPoints[ActionNavigationPathPointIndex] -
            OwnerLocation;
        ToPathPoint.Z = 0.0f;
        const float DistanceToPathPoint = ToPathPoint.Size();
        if (DistanceToPathPoint <= UE_SMALL_NUMBER)
        {
            ++ActionNavigationPathPointIndex;
            return;
        }

        const FVector MoveDirection =
            ToPathPoint.GetSafeNormal2D();
        const bool bHoldingObject =
            IsValid(WorldStateAgent) &&
            IsValid(WorldStateAgent->GetHeldActor());
        const bool bShouldRun = !bHoldingObject &&
            (bRequestedRun ||
             DistanceToGoal >= FMath::Max(
                StopDistance,
                FollowRunDistance
             ));
        const float Speed = bShouldRun
            ? FMath::Max(20.0f, FollowRunSpeed)
            : FMath::Max(20.0f, FollowWalkSpeed);
        const float MoveDistance = FMath::Min(
            DistanceToPathPoint,
            Speed * DeltaTime
        );
        const FVector DesiredDelta = MoveDirection * MoveDistance;
        FVector NewLocation = OwnerLocation;
        FHitResult SweepHit;
        FindCollisionSafeMove(
            OwnerLocation,
            DesiredDelta,
            MoveDirection,
            NewLocation,
            SweepHit
        );
        if (!bActionJumpInProgress &&
            !UpdateActorGroundHeight(NewLocation, DeltaTime))
        {
            NewLocation = OwnerLocation;
        }
        Owner->SetActorLocation(NewLocation, false);
        const float ActualMoveDistance = FVector::Dist2D(
            OwnerLocation,
            Owner->GetActorLocation()
        );
        if (ActualMoveDistance <
            FMath::Min(1.0f, MoveDistance * 0.2f))
        {
            ActionNavigationStallElapsedSeconds += DeltaTime;
        }
        else
        {
            ActionNavigationStallElapsedSeconds = 0.0f;
            ActionNavigationFailureCount = 0;
        }

        if (ActionNavigationStallElapsedSeconds >= FMath::Max(
                0.05f,
                ActionNavigationStallSeconds
            ) &&
            ActionNavigationRepathElapsedSeconds >= RepathInterval)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_ACTION obstacle_stall npc=%s blocker=%s "
                    "location=%s path_index=%d/%d"),
                *Owner->GetName(),
                ActionPersistentBlockingActor.IsValid()
                    ? *ActionPersistentBlockingActor->GetName()
                    : TEXT("unknown"),
                *OwnerLocation.ToCompactString(),
                ActionNavigationPathPointIndex,
                ActionNavigationPathPoints.Num()
            );
            if (!RebuildActionNavigationPath(
                    DesiredGoal,
                    StopDistance))
            {
                ++ActionNavigationFailureCount;
                ActionNavigationRepathElapsedSeconds = 0.0f;
                ActionNavigationStallElapsedSeconds = 0.0f;
            }
            if (ActionNavigationFailureCount >= 3 &&
                bMovingToCommandLocation)
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("JACK_ACTION move_to failed reason=Blocked")
                );
                StopFollowingPlayer();
                return;
            }
        }

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
        return;
    }
}

bool UOpenAIJackComponent::FindActionGroundHeight(
    const FVector& Location,
    float& OutGroundZ
) const
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !IsValid(Owner))
    {
        return false;
    }

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCGrounding),
        false,
        Owner
    );
    if (ReservedSeatActor.IsValid())
    {
        QueryParams.AddIgnoredActor(ReservedSeatActor.Get());
    }
    if (IsValid(WorldStateAgent) &&
        IsValid(WorldStateAgent->GetHeldActor()))
    {
        QueryParams.AddIgnoredActor(WorldStateAgent->GetHeldActor());
    }
    FHitResult Hit;
    const float TraceAbove = FMath::Max(
        60.0f,
        ActionMaximumGroundStepUp + 30.0f
    );
    const float TraceBelow = FMath::Max(
        300.0f,
        ActionMaximumGroundStepDown + 100.0f
    );
    const FVector Start(Location.X, Location.Y, Location.Z + TraceAbove);
    const FVector End(Location.X, Location.Y, Location.Z - TraceBelow);
    if (!World->LineTraceSingleByObjectType(
            Hit,
            Start,
            End,
            ObjectQuery,
            QueryParams))
    {
        return false;
    }

    OutGroundZ = Hit.ImpactPoint.Z;
    return true;
}

bool UOpenAIJackComponent::UpdateActorGroundHeight(
    FVector& InOutLocation,
    float DeltaTime
)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner) || DeltaTime <= 0.0f)
    {
        return false;
    }
    if (bActionJumpInProgress)
    {
        return true;
    }

    const FVector CurrentLocation = Owner->GetActorLocation();
    if (!bActionActorGroundOffsetInitialized)
    {
        float CurrentGroundZ = 0.0f;
        if (!FindActionGroundHeight(CurrentLocation, CurrentGroundZ))
        {
            return false;
        }
        ActionActorGroundOffsetZ = CurrentLocation.Z - CurrentGroundZ;
        bActionActorGroundOffsetInitialized = true;
    }

    FVector GroundProbeLocation = InOutLocation;
    GroundProbeLocation.Z = CurrentLocation.Z;
    float TargetGroundZ = 0.0f;
    if (!FindActionGroundHeight(GroundProbeLocation, TargetGroundZ))
    {
        return false;
    }

    const float CurrentGroundZ =
        CurrentLocation.Z - ActionActorGroundOffsetZ;
    const float GroundDelta = TargetGroundZ - CurrentGroundZ;
    if (GroundDelta > FMath::Max(10.0f, ActionMaximumGroundStepUp) ||
        GroundDelta < -FMath::Max(20.0f, ActionMaximumGroundStepDown))
    {
        return false;
    }

    const float TargetActorZ =
        TargetGroundZ + ActionActorGroundOffsetZ;
    InOutLocation.Z = FMath::FInterpTo(
        CurrentLocation.Z,
        TargetActorZ,
        DeltaTime,
        FMath::Max(1.0f, ActionActorGroundFollowInterpSpeed)
    );
    return true;
}

bool UOpenAIJackComponent::IsActionCapsuleMoveBlocked(
    const FVector& StartLocation,
    const FVector& Delta,
    FHitResult& OutHit
) const
{
    UWorld* World = GetWorld();
    AActor* Owner = GetOwner();
    if (!World || !IsValid(Owner) || Delta.IsNearlyZero())
    {
        OutHit = FHitResult();
        return false;
    }

    UNavigationSystemV1* NavigationSystem =
        FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (NavigationSystem)
    {
        const float ProjectionRadius = FMath::Max(
            50.0f,
            ActionCollisionRadius + 20.0f
        );
        const FVector ProjectionExtent(
            ProjectionRadius,
            ProjectionRadius,
            FMath::Max(
                120.0f,
                ActionMaximumGroundStepUp +
                    ActionMaximumGroundStepDown
            )
        );
        FNavLocation StartNavigationLocation;
        FNavLocation EndNavigationLocation;
        const FVector EndLocation = StartLocation + Delta;
        const float ProjectionTolerance = FMath::Clamp(
            ActionCollisionRadius * 0.35f,
            6.0f,
            15.0f
        );
        if (NavigationSystem->ProjectPointToNavigation(
                StartLocation,
                StartNavigationLocation,
                ProjectionExtent) &&
            NavigationSystem->ProjectPointToNavigation(
                EndLocation,
                EndNavigationLocation,
                ProjectionExtent) &&
            FVector::Dist2D(
                StartLocation,
                StartNavigationLocation.Location) <=
                ProjectionTolerance &&
            FVector::Dist2D(
                EndLocation,
                EndNavigationLocation.Location) <=
                ProjectionTolerance)
        {
            return IsActionNavigationSegmentBlocked(
                StartNavigationLocation.Location,
                EndNavigationLocation.Location,
                OutHit
            );
        }
    }

    const float Radius = FMath::Max(10.0f, ActionCollisionRadius);
    const float HalfHeight = FMath::Max(
        Radius + 1.0f,
        ActionCollisionHalfHeight
    );
    float StartGroundZ = StartLocation.Z;
    FindActionGroundHeight(StartLocation, StartGroundZ);
    float EndGroundZ = StartGroundZ;
    const FVector EndLocation = StartLocation + Delta;
    if (!FindActionGroundHeight(EndLocation, EndGroundZ))
    {
        EndGroundZ = StartGroundZ;
    }
    const float GroundDelta = EndGroundZ - StartGroundZ;
    if (!IsGroundStepTraversable(
            GroundDelta,
            ActionMaximumGroundStepUp,
            ActionMaximumGroundStepDown))
    {
        OutHit = FHitResult();
        OutHit.bBlockingHit = true;
        OutHit.Location = EndLocation;
        OutHit.ImpactPoint = EndLocation;
        return true;
    }
    const float SweepGroundZ = FMath::Max(StartGroundZ, EndGroundZ);
    const FVector CapsuleStart(
        StartLocation.X,
        StartLocation.Y,
        SweepGroundZ + HalfHeight + 2.0f
    );
    const FVector CapsuleEnd = CapsuleStart +
        FVector(Delta.X, Delta.Y, 0.0f);

    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(NaturalNPCActionMovement),
        false,
        Owner
    );
    if (IsValid(WorldStateAgent) &&
        IsValid(WorldStateAgent->GetHeldActor()))
    {
        QueryParams.AddIgnoredActor(WorldStateAgent->GetHeldActor());
    }
    if (ReservedSeatActor.IsValid())
    {
        QueryParams.AddIgnoredActor(ReservedSeatActor.Get());
    }
    return World->SweepSingleByObjectType(
        OutHit,
        CapsuleStart,
        CapsuleEnd,
        FQuat::Identity,
        ObjectQuery,
        FCollisionShape::MakeCapsule(Radius, HalfHeight),
        QueryParams
    );
}

bool UOpenAIJackComponent::IsGroundStepTraversable(
    const float GroundDelta,
    const float MaximumStepUp,
    const float MaximumStepDown
)
{
    return GroundDelta <= FMath::Max(10.0f, MaximumStepUp) &&
        GroundDelta >= -FMath::Max(20.0f, MaximumStepDown);
}

bool UOpenAIJackComponent::FindCollisionSafeMove(
    const FVector& StartLocation,
    const FVector& DesiredDelta,
    const FVector& GoalDirection,
    FVector& OutLocation,
    FHitResult& OutHit
)
{
    OutLocation = StartLocation;
    OutHit = FHitResult();
    if (DesiredDelta.IsNearlyZero())
    {
        return true;
    }

    const float MoveDistance = DesiredDelta.Size2D();
    if (ActionPersistentAvoidanceDistanceRemaining > 0.0f &&
        !ActionPersistentAvoidanceDirection.IsNearlyZero())
    {
        const FVector PersistentDelta =
            ActionPersistentAvoidanceDirection.GetSafeNormal2D() *
            MoveDistance;
        FHitResult PersistentHit;
        if (!IsActionCapsuleMoveBlocked(
                StartLocation,
                PersistentDelta,
                PersistentHit))
        {
            OutLocation = StartLocation + PersistentDelta;
            ActionPersistentAvoidanceDistanceRemaining = FMath::Max(
                0.0f,
                ActionPersistentAvoidanceDistanceRemaining - MoveDistance
            );
            return true;
        }
        ActionPersistentAvoidanceDistanceRemaining = 0.0f;
    }

    FHitResult DesiredHit;
    if (!IsActionCapsuleMoveBlocked(
            StartLocation,
            DesiredDelta,
            DesiredHit))
    {
        OutLocation = StartLocation + DesiredDelta;
        ActionPersistentAvoidanceDirection = FVector::ZeroVector;
        ActionPersistentBlockingActor.Reset();
        return true;
    }
    OutHit = DesiredHit;

    FVector WallNormal = DesiredHit.ImpactNormal.GetSafeNormal2D();
    if (WallNormal.IsNearlyZero() &&
        DesiredHit.ImpactNormal.Z > 0.65f)
    {
        OutHit = FHitResult();
        OutLocation = StartLocation + DesiredDelta;
        return true;
    }
    if (WallNormal.IsNearlyZero())
    {
        WallNormal = -DesiredDelta.GetSafeNormal2D();
    }

    if (!DesiredHit.bStartPenetrating && DesiredHit.Time > 0.1f)
    {
        const float SafeFraction = FMath::Clamp(
            DesiredHit.Time - 0.05f,
            0.0f,
            1.0f
        );
        const FVector SafeDelta = DesiredDelta * SafeFraction;
        if (SafeDelta.Size2D() > 0.5f)
        {
            OutLocation = StartLocation + SafeDelta;
            ActionPersistentBlockingActor = DesiredHit.GetActor();
            return true;
        }
    }

    const FVector DesiredDirection = DesiredDelta.GetSafeNormal2D();
    const FVector EffectiveGoalDirection = GoalDirection.IsNearlyZero()
        ? DesiredDirection
        : GoalDirection.GetSafeNormal2D();
    const FVector TangentA(-WallNormal.Y, WallNormal.X, 0.0f);
    const FVector TangentB = -TangentA;
    FVector PreferredTangent = TangentA;
    if (ActionPersistentBlockingActor.Get() == DesiredHit.GetActor() &&
        !ActionPersistentAvoidanceDirection.IsNearlyZero())
    {
        PreferredTangent =
            FVector::DotProduct(
                TangentA,
                ActionPersistentAvoidanceDirection
            ) >= FVector::DotProduct(
                TangentB,
                ActionPersistentAvoidanceDirection
            )
                ? TangentA
                : TangentB;
    }
    else if (FVector::DotProduct(TangentB, EffectiveGoalDirection) >
             FVector::DotProduct(TangentA, EffectiveGoalDirection))
    {
        PreferredTangent = TangentB;
    }

    const FVector PreferredEscapeDirection =
        (PreferredTangent + WallNormal * 0.4f).GetSafeNormal2D();
    TArray<FVector, TInlineAllocator<8>> CandidateDeltas;
    CandidateDeltas.Add(PreferredEscapeDirection * MoveDistance);
    CandidateDeltas.Add(PreferredTangent * MoveDistance);
    CandidateDeltas.Add(
        (-PreferredTangent + WallNormal * 0.4f).GetSafeNormal2D() *
        MoveDistance
    );
    CandidateDeltas.Add(WallNormal * MoveDistance);

    FVector SlideDelta = FVector::VectorPlaneProject(
        DesiredDelta,
        DesiredHit.ImpactNormal
    );
    SlideDelta.Z = 0.0f;
    if (!SlideDelta.IsNearlyZero())
    {
        CandidateDeltas.Add(
            SlideDelta.GetSafeNormal2D() * DesiredDelta.Size2D()
        );
    }

    const float AvoidanceAngle = FMath::Clamp(
        ActionAvoidanceAngleDegrees,
        10.0f,
        90.0f
    );
    CandidateDeltas.Add(
        DesiredDirection.RotateAngleAxis(AvoidanceAngle, FVector::UpVector) *
        DesiredDelta.Size2D()
    );
    CandidateDeltas.Add(
        DesiredDirection.RotateAngleAxis(-AvoidanceAngle, FVector::UpVector) *
        DesiredDelta.Size2D()
    );

    float BestScore = -TNumericLimits<float>::Max();
    FVector BestDelta = FVector::ZeroVector;
    for (const FVector& CandidateDelta : CandidateDeltas)
    {
        FHitResult CandidateHit;
        if (IsActionCapsuleMoveBlocked(
                StartLocation,
                CandidateDelta,
                CandidateHit))
        {
            continue;
        }
        float Score = FVector::DotProduct(
            CandidateDelta.GetSafeNormal2D(),
            EffectiveGoalDirection
        );
        Score += 0.25f * FVector::DotProduct(
            CandidateDelta.GetSafeNormal2D(),
            PreferredEscapeDirection
        );
        Score += 0.1f * FVector::DotProduct(
            CandidateDelta.GetSafeNormal2D(),
            WallNormal
        );
        if (Score > BestScore)
        {
            BestScore = Score;
            BestDelta = CandidateDelta;
        }
    }

    if (BestDelta.IsNearlyZero())
    {
        if (DesiredHit.bStartPenetrating && !WallNormal.IsNearlyZero())
        {
            OutLocation = StartLocation + WallNormal *
                FMath::Min(MoveDistance, 4.0f);
            ActionPersistentAvoidanceDirection = PreferredEscapeDirection;
            ActionPersistentAvoidanceDistanceRemaining = 60.0f;
            ActionPersistentBlockingActor = DesiredHit.GetActor();
            return true;
        }
        return false;
    }

    OutLocation = StartLocation + BestDelta;
    ActionPersistentAvoidanceDirection = BestDelta.GetSafeNormal2D();
    ActionPersistentAvoidanceDistanceRemaining = 60.0f;
    ActionPersistentBlockingActor = DesiredHit.GetActor();
    return true;
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

USkeletalMeshComponent* UOpenAIJackComponent::ResolveActionBodyMesh()
{
    if (ActionBodyMesh.IsValid())
    {
        return ActionBodyMesh.Get();
    }

    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return nullptr;
    }

    USkeletalMeshComponent* BestMesh = nullptr;
    int32 BestScore = TNumericLimits<int32>::Lowest();
    TArray<USkeletalMeshComponent*> SkeletalMeshes;
    Owner->GetComponents(SkeletalMeshes);
    for (USkeletalMeshComponent* Mesh : SkeletalMeshes)
    {
        if (!ShouldUseMeshForActionAnimation(Mesh))
        {
            continue;
        }

        const FString MeshName = Mesh->GetName();
        int32 Score = MeshName.Equals(TEXT("Body"), ESearchCase::IgnoreCase)
            ? 100
            : 0;
        if (MeshName.Contains(TEXT("Body"), ESearchCase::IgnoreCase))
        {
            Score += 40;
        }
        if (Mesh->GetAttachParent() == Owner->GetRootComponent())
        {
            Score += 20;
        }
        if (Score > BestScore)
        {
            BestScore = Score;
            BestMesh = Mesh;
        }
    }

    if (IsValid(BestMesh))
    {
        ActionBodyMesh = BestMesh;
        ActionBodyBaseRelativeLocation = BestMesh->GetRelativeLocation();
        ActionBodyTargetRelativeLocation = ActionBodyBaseRelativeLocation;
        bActionBodyBaseTransformSaved = true;
    }
    return BestMesh;
}

void UOpenAIJackComponent::ResetActionBodyOffset()
{
    if (bActionBodyBaseTransformSaved && ActionBodyMesh.IsValid())
    {
        ActionBodyMesh->SetRelativeLocation(ActionBodyBaseRelativeLocation);
    }
    ActionBodyMesh.Reset();
    bActionBodyBaseTransformSaved = false;
    bSeatBodyAlignmentActive = false;
    ActionBodyBaseRelativeLocation = FVector::ZeroVector;
    ActionBodyTargetRelativeLocation = FVector::ZeroVector;
}

void UOpenAIJackComponent::UpdateActionGrounding(float DeltaTime)
{
    if (DeltaTime <= 0.0f)
    {
        return;
    }

    USkeletalMeshComponent* Body = ResolveActionBodyMesh();
    if (!IsValid(Body))
    {
        return;
    }

    if (bSeatBodyAlignmentActive)
    {
        const FVector DesiredLocation = bStandUpInProgress
            ? ActionBodyBaseRelativeLocation
            : ActionBodyTargetRelativeLocation;
        Body->SetRelativeLocation(FMath::VInterpTo(
            Body->GetRelativeLocation(),
            DesiredLocation,
            DeltaTime,
            FMath::Max(1.0f, SeatAlignmentInterpSpeed)
        ));
        return;
    }

    if (!bEnableActionFootGrounding ||
        Body->GetAnimationMode() != EAnimationMode::AnimationSingleNode)
    {
        if (bActionBodyBaseTransformSaved &&
            !Body->GetRelativeLocation().Equals(
                ActionBodyBaseRelativeLocation,
                0.05f))
        {
            Body->SetRelativeLocation(FMath::VInterpTo(
                Body->GetRelativeLocation(),
                ActionBodyBaseRelativeLocation,
                DeltaTime,
                FMath::Max(1.0f, ActionGroundingInterpSpeed)
            ));
        }
        return;
    }

    float GroundZ = 0.0f;
    if (!FindActionGroundHeight(GetOwner()->GetActorLocation(), GroundZ))
    {
        return;
    }

    const FName LeftGroundBone = Body->DoesSocketExist(TEXT("ball_l"))
        ? FName(TEXT("ball_l"))
        : FName(TEXT("foot_l"));
    const FName RightGroundBone = Body->DoesSocketExist(TEXT("ball_r"))
        ? FName(TEXT("ball_r"))
        : FName(TEXT("foot_r"));
    if (!Body->DoesSocketExist(LeftGroundBone) ||
        !Body->DoesSocketExist(RightGroundBone))
    {
        return;
    }

    const float LowestFootZ = FMath::Min(
        Body->GetSocketLocation(LeftGroundBone).Z,
        Body->GetSocketLocation(RightGroundBone).Z
    );
    const float Correction =
        GroundZ + FMath::Max(0.0f, ActionFootGroundClearance) - LowestFootZ;
    FVector DesiredLocation = ActionBodyBaseRelativeLocation;
    DesiredLocation.Z = FMath::Clamp(
        Body->GetRelativeLocation().Z + Correction,
        ActionBodyBaseRelativeLocation.Z -
            FMath::Max(0.0f, ActionGroundingMaxDrop),
        ActionBodyBaseRelativeLocation.Z +
            FMath::Max(20.0f, ActionGroundingMaxLift)
    );
    ActionBodyTargetRelativeLocation = DesiredLocation;
    Body->SetRelativeLocation(FMath::VInterpTo(
        Body->GetRelativeLocation(),
        DesiredLocation,
        DeltaTime,
        FMath::Max(1.0f, ActionGroundingInterpSpeed)
    ));
}

void UOpenAIJackComponent::PlayActionAnimation(
    EActionAnimationState State,
    bool bLooping,
    float StartPositionSeconds,
    float PlayRate
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
            if (UAnimSingleNodeInstance* SingleNodeInstance =
                    Mesh->GetSingleNodeInstance())
            {
                SingleNodeInstance->SetPosition(
                    FMath::Clamp(
                        StartPositionSeconds,
                        0.0f,
                        FMath::Max(
                            0.0f,
                            Animation->GetPlayLength() - 0.001f
                        )
                    ),
                    false
                );
                SingleNodeInstance->SetPlayRate(FMath::Max(0.1f, PlayRate));
            }
        }
    }
    CurrentActionAnimationState = State;

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACTION animation state=%d loop=%d asset=%s meshes=%d "
            "start=%.2f rate=%.2f"),
        static_cast<int32>(State),
        bLooping ? 1 : 0,
        *Animation->GetPathName(),
        Meshes.Num(),
        StartPositionSeconds,
        PlayRate
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
    ResetActionBodyOffset();
}

UAnimSequence* UOpenAIJackComponent::ResolveActionAnimation(
    EActionAnimationState State
) const
{
    switch (State)
    {
    case EActionAnimationState::Idle:
        if (IsValid(WorldStateAgent))
        {
            if (UAnimSequence* HeldIdle =
                WorldStateAgent->GetCurrentHeldIdleAnimation())
            {
                return HeldIdle;
            }
        }
        return IdleAnimation.LoadSynchronous();
    case EActionAnimationState::Walk:
    case EActionAnimationState::Run:
        if (IsValid(WorldStateAgent))
        {
            if (UAnimSequence* HeldWalk =
                WorldStateAgent->GetHeldWalkAnimation())
            {
                return HeldWalk;
            }
        }
        return State == EActionAnimationState::Run
            ? RunAnimation.LoadSynchronous()
            : WalkAnimation.LoadSynchronous();
    case EActionAnimationState::Jump:
        return JumpAnimation.LoadSynchronous();
    case EActionAnimationState::SitDown:
        return SitDownAnimation.LoadSynchronous();
    case EActionAnimationState::SitIdle:
        return SitIdleAnimation.LoadSynchronous();
    case EActionAnimationState::StandUp:
        return StandUpAnimation.LoadSynchronous();
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
    Body->SetStringField(
        TEXT("keep_alive"),
        GetEffectiveModelKeepAlive()
    );

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
    const bool bUseOpenAI =
        GetEffectiveLLMProvider() == EJackLLMProvider::OpenAIAPI;
    const bool bUseStreamingResponse =
        bEnableStreamingResponses &&
        bEnableQueuedSpeech &&
        bEnableHttpTTS;
    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(
        TEXT("model"),
        bUseOpenAI ? OpenAIChatModel : Model
    );
    Body->SetBoolField(TEXT("stream"), bUseStreamingResponse);
    if (!bUseOpenAI)
    {
        Body->SetBoolField(TEXT("think"), false);
        Body->SetStringField(
            TEXT("keep_alive"),
            GetEffectiveModelKeepAlive()
        );
    }

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedRef<FJsonObject> SystemMessage = MakeShared<FJsonObject>();
    SystemMessage->SetStringField(TEXT("role"), TEXT("system"));
    SystemMessage->SetStringField(
        TEXT("content"),
        CharacterInstructions +
        TEXT(" Speak only dialogue addressed to the player. Never read out ")
        TEXT("world-state metadata, action status, stage directions, or ")
        TEXT("narration such as what you are currently holding or doing, ")
        TEXT("unless the player explicitly asks about it.")
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

    const FString NearbyConversationContext =
        BuildSessionConversationContext(false);
    if (!NearbyConversationContext.IsEmpty())
    {
        TSharedRef<FJsonObject> ContextMessage = MakeShared<FJsonObject>();
        ContextMessage->SetStringField(TEXT("role"), TEXT("system"));
        ContextMessage->SetStringField(
            TEXT("content"),
            NearbyConversationContext
        );
        Messages.Add(MakeShared<FJsonValueObject>(ContextMessage));
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

    if (bUseOpenAI)
    {
        if (MaxOutputTokens > 0)
        {
            Body->SetNumberField(TEXT("max_completion_tokens"), MaxOutputTokens);
        }
        if (bUseStreamingResponse)
        {
            TSharedRef<FJsonObject> StreamOptions = MakeShared<FJsonObject>();
            StreamOptions->SetBoolField(TEXT("include_usage"), true);
            Body->SetObjectField(TEXT("stream_options"), StreamOptions);
        }
    }
    else
    {
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
    }

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(bUseOpenAI ? OpenAIChatUrl : OllamaChatUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    if (bUseOpenAI)
    {
        const FString ApiKey = GetLLMApiKey();
        if (ApiKey.IsEmpty())
        {
            Fail(FString::Printf(
                TEXT("%s is required for OpenAI LLM chat."),
                *OpenAIApiKeyEnvironmentVariable
            ));
            return;
        }
        Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + ApiKey);
    }
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
                    bUseOpenAI,
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
                                const FString Payload = bUseOpenAI
                                    ? ExtractSseData(Line)
                                    : Line;
                                if (bUseOpenAI && Payload == TEXT("[DONE]"))
                                {
                                    StreamState->bSawDone = true;
                                    LineBreakIndex = FindLineBreakIndex(
                                        StreamState->PendingBytes
                                    );
                                    continue;
                                }

                                TSharedPtr<FJsonObject> Root;
                                const TSharedRef<TJsonReader<>> Reader =
                                    TJsonReaderFactory<>::Create(Payload);
                                if (FJsonSerializer::Deserialize(Reader, Root) &&
                                    Root.IsValid())
                                {
                                    FString Delta;
                                    if (bUseOpenAI)
                                    {
                                        Delta = ExtractOpenAIChatDelta(Root);
                                        ExtractOpenAIUsage(
                                            Root,
                                            StreamState->PromptTokenCount,
                                            StreamState->OutputTokenCount
                                        );
                                    }
                                    else
                                    {
                                        const TSharedPtr<FJsonObject>* Message =
                                            nullptr;
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
                                                StreamState
                                                    ->PromptDurationNanoseconds
                                            );
                                            Root->TryGetNumberField(
                                                TEXT("eval_duration"),
                                                StreamState
                                                    ->EvalDurationNanoseconds
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
        [
            this,
            PlayerText,
            ChatStartSeconds,
            bUseStreamingResponse,
            bUseOpenAI,
            StreamState
        ](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            if (!bSucceeded || !Response.IsValid() ||
                Response->GetResponseCode() < 200 ||
                Response->GetResponseCode() >= 300)
            {
                const FString ErrorMessage = Response.IsValid()
                    ? Response->GetContentAsString()
                    : bUseOpenAI
                        ? FString(TEXT("OpenAI API request failed."))
                        : FString(
                            TEXT("Ollama request failed. Is Ollama running?")
                        );
                Fail(ErrorMessage);
                return;
            }

            if (bUseStreamingResponse)
            {
                if (!StreamState.IsValid())
                {
                    Fail(bUseOpenAI
                        ? TEXT("OpenAI streaming state was not initialized.")
                        : TEXT("Ollama streaming state was not initialized."));
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
                    Fail(bUseOpenAI
                        ? TEXT("OpenAI returned no streamed reply text.")
                        : TEXT("Ollama returned no streamed reply text."));
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
                PublishReplyText(Reply);

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
                Fail(bUseOpenAI
                    ? TEXT("Could not parse the OpenAI response.")
                    : TEXT("Could not parse the Ollama response."));
                return;
            }

            const FString Reply = bUseOpenAI
                ? ExtractOpenAIChatReply(Root)
                : ExtractOllamaReply(Root);
            if (Reply.IsEmpty())
            {
                Fail(bUseOpenAI
                    ? TEXT("OpenAI returned no reply text.")
                    : TEXT("Ollama returned no reply text."));
                return;
            }

            double PromptDurationNanoseconds = 0.0;
            double EvalDurationNanoseconds = 0.0;
            double PromptTokenCount = 0.0;
            double OutputTokenCount = 0.0;
            if (bUseOpenAI)
            {
                ExtractOpenAIUsage(
                    Root,
                    PromptTokenCount,
                    OutputTokenCount
                );
            }
            else
            {
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
            }
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
            PublishReplyText(Reply);
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
        Fail(bUseOpenAI
            ? TEXT("Could not start the OpenAI HTTP request.")
            : TEXT("Could not start the Ollama HTTP request."));
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
    RecordSessionConversationExchange(
        GetResolvedNPCID(),
        PlayerText,
        ReplyText,
        NAME_None,
        FString(),
        false
    );
    const int32 NewTurnIndex = MemoryTurns.Num() - 1;

    if (bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
    RequestTurnEmbedding(NewTurnIndex);
}

void UOpenAIJackComponent::RememberConversationExchange(
    FName PrimaryNPCID,
    const FString& PlayerText,
    const FString& PrimaryReply,
    FName SecondaryNPCID,
    const FString& SecondaryReply
)
{
    RecordSessionConversationExchange(
        PrimaryNPCID,
        PlayerText,
        PrimaryReply,
        SecondaryNPCID,
        SecondaryReply,
        true
    );
}

void UOpenAIJackComponent::RecordSessionConversationExchange(
    FName PrimaryNPCID,
    const FString& PlayerText,
    const FString& PrimaryReply,
    FName SecondaryNPCID,
    const FString& SecondaryReply,
    bool bSaveAfterRecording
)
{
    const FString TrimmedPlayerText = PlayerText.TrimStartAndEnd();
    const FString TrimmedPrimaryReply = PrimaryReply.TrimStartAndEnd();
    const FString TrimmedSecondaryReply = SecondaryReply.TrimStartAndEnd();
    if (PrimaryNPCID.IsNone() || TrimmedPlayerText.IsEmpty() ||
        TrimmedPrimaryReply.IsEmpty())
    {
        return;
    }

    FSessionConversationExchange* Existing = nullptr;
    if (!SessionConversationExchanges.IsEmpty())
    {
        FSessionConversationExchange& Last =
            SessionConversationExchanges.Last();
        if (Last.PrimaryNPCID == PrimaryNPCID &&
            Last.PlayerText == TrimmedPlayerText &&
            Last.PrimaryReply == TrimmedPrimaryReply)
        {
            Existing = &Last;
        }
    }

    if (Existing)
    {
        if (!SecondaryNPCID.IsNone() && !TrimmedSecondaryReply.IsEmpty())
        {
            Existing->SecondaryNPCID = SecondaryNPCID;
            Existing->SecondaryReply = TrimmedSecondaryReply;
        }
    }
    else
    {
        FSessionConversationExchange& Exchange =
            SessionConversationExchanges.AddDefaulted_GetRef();
        Exchange.PrimaryNPCID = PrimaryNPCID;
        Exchange.PlayerText = TrimmedPlayerText;
        Exchange.PrimaryReply = TrimmedPrimaryReply;
        Exchange.SecondaryNPCID = SecondaryNPCID;
        Exchange.SecondaryReply = TrimmedSecondaryReply;
    }

    const int32 MaximumStoredTurns =
        FMath::Max(1, MaxStoredConversationTurns);
    if (SessionConversationExchanges.Num() > MaximumStoredTurns)
    {
        SessionConversationExchanges.RemoveAt(
            0,
            SessionConversationExchanges.Num() - MaximumStoredTurns
        );
    }
    if (bSaveAfterRecording && bEnableSessionMemoryFile)
    {
        SaveMemory();
    }
}

void UOpenAIJackComponent::PublishReplyText(const FString& ReplyText)
{
    const FString TrimmedReply = ReplyText.TrimStartAndEnd();
    if (TrimmedReply.IsEmpty())
    {
        return;
    }

    OnReplyText.Broadcast(TrimmedReply);
    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>())
        {
            ConversationSubsystem->NotifyNPCReplyReady(
                this,
                TrimmedReply
            );
        }
    }
}

void UOpenAIJackComponent::InitializeSessionMemory()
{
    ConversationHistory.Reset();
    PersistentHistory.Reset();
    MemoryTurns.Reset();
    SessionConversationExchanges.Reset();

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
    Root->SetNumberField(TEXT("version"), 2);
    Root->SetStringField(
        TEXT("npc"),
        GetResolvedNPCID().ToString()
    );

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

    TArray<TSharedPtr<FJsonValue>> SessionContext;
    for (const FSessionConversationExchange& Exchange :
         SessionConversationExchanges)
    {
        TSharedRef<FJsonObject> JsonExchange = MakeShared<FJsonObject>();
        JsonExchange->SetStringField(
            TEXT("primary_npc"),
            Exchange.PrimaryNPCID.ToString()
        );
        JsonExchange->SetStringField(TEXT("player"), Exchange.PlayerText);
        JsonExchange->SetStringField(
            TEXT("primary_reply"),
            Exchange.PrimaryReply
        );
        if (!Exchange.SecondaryNPCID.IsNone() &&
            !Exchange.SecondaryReply.IsEmpty())
        {
            JsonExchange->SetStringField(
                TEXT("secondary_npc"),
                Exchange.SecondaryNPCID.ToString()
            );
            JsonExchange->SetStringField(
                TEXT("secondary_reply"),
                Exchange.SecondaryReply
            );
        }
        SessionContext.Add(MakeShared<FJsonValueObject>(JsonExchange));
    }
    Root->SetArrayField(TEXT("session_context"), SessionContext);

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
    Body->SetStringField(
        TEXT("keep_alive"),
        GetEffectiveModelKeepAlive()
    );

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
    Body->SetStringField(
        TEXT("keep_alive"),
        GetEffectiveModelKeepAlive()
    );

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

void UOpenAIJackComponent::RegisterSharedACEProvider()
{
    if (bSharedACEProviderRegistered || !bEnableACEAudio2Face)
    {
        return;
    }

    bool bShouldPreallocate = false;
    bool bShouldWarmUp = false;
    {
        FScopeLock Lock(&GSharedACEProviderMutex);
        FSharedACEProviderState& State =
            GSharedACEProviderStates.FindOrAdd(ACEAudio2FaceProviderName);
        ++State.OwnerCount;
        if (!bReleaseACEResourcesWhenIdle)
        {
            ++State.PersistentOwnerCount;
            if (bPreAllocateACEAudio2FaceResources &&
                !State.bPreallocationRequested)
            {
                State.bPreallocationRequested = true;
                State.bResourcesMayBeAllocated = true;
                bShouldPreallocate = true;
            }
            if (bWarmUpACEAudio2FaceOnBeginPlay &&
                !State.bWarmupRequested)
            {
                State.bWarmupRequested = true;
                bShouldWarmUp = true;
            }
        }
        ++State.Generation;
    }
    bSharedACEProviderRegistered = true;

    if (bShouldPreallocate)
    {
        UACEBlueprintLibrary::AllocateA2F3DResources(
            ACEAudio2FaceProviderName
        );
    }
    if (bShouldWarmUp)
    {
        ScheduleACEWarmup();
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("JACK_ACE_A2F_RESOURCES mode=%s provider=%s"),
        bReleaseACEResourcesWhenIdle ? TEXT("idle_release") : TEXT("shared_warm"),
        *ACEAudio2FaceProviderName.ToString()
    );
}

void UOpenAIJackComponent::UnregisterSharedACEProvider()
{
    MarkSharedACESessionFinished();
    if (!bSharedACEProviderRegistered)
    {
        return;
    }

    bool bShouldFree = false;
    {
        FScopeLock Lock(&GSharedACEProviderMutex);
        FSharedACEProviderState* State =
            GSharedACEProviderStates.Find(ACEAudio2FaceProviderName);
        if (State)
        {
            State->OwnerCount = FMath::Max(0, State->OwnerCount - 1);
            if (!bReleaseACEResourcesWhenIdle)
            {
                State->PersistentOwnerCount = FMath::Max(
                    0,
                    State->PersistentOwnerCount - 1
                );
            }
            ++State->Generation;
            if (State->OwnerCount == 0 &&
                State->ActiveSessionCount == 0 &&
                State->bResourcesMayBeAllocated)
            {
                State->bResourcesMayBeAllocated = false;
                State->bPreallocationRequested = false;
                State->bWarmupRequested = false;
                bShouldFree = true;
            }
        }
    }
    bSharedACEProviderRegistered = false;

    if (bShouldFree)
    {
        UACEBlueprintLibrary::FreeA2F3DResources(
            ACEAudio2FaceProviderName
        );
    }
}

void UOpenAIJackComponent::MarkSharedACESessionStarted()
{
    if (bSharedACESessionCounted)
    {
        return;
    }
    if (!bSharedACEProviderRegistered)
    {
        RegisterSharedACEProvider();
    }

    {
        FScopeLock Lock(&GSharedACEProviderMutex);
        FSharedACEProviderState& State =
            GSharedACEProviderStates.FindOrAdd(ACEAudio2FaceProviderName);
        ++State.ActiveSessionCount;
        ++State.Generation;
        State.bResourcesMayBeAllocated = true;
    }
    bSharedACESessionCounted = true;
}

void UOpenAIJackComponent::MarkSharedACESessionFinished()
{
    if (!bSharedACESessionCounted)
    {
        return;
    }
    bSharedACESessionCounted = false;

    uint64 ReleaseGeneration = 0;
    bool bScheduleRelease = false;
    {
        FScopeLock Lock(&GSharedACEProviderMutex);
        FSharedACEProviderState* State =
            GSharedACEProviderStates.Find(ACEAudio2FaceProviderName);
        if (State)
        {
            State->ActiveSessionCount = FMath::Max(
                0,
                State->ActiveSessionCount - 1
            );
            ReleaseGeneration = ++State->Generation;
            bScheduleRelease =
                bReleaseACEResourcesWhenIdle &&
                State->ActiveSessionCount == 0 &&
                State->PersistentOwnerCount == 0 &&
                State->bResourcesMayBeAllocated;
        }
    }

    if (!bScheduleRelease)
    {
        return;
    }

    const FName ProviderName = ACEAudio2FaceProviderName;
    const float DelaySeconds = FMath::Clamp(
        ACEIdleReleaseSeconds,
        5.0f,
        600.0f
    );
    if (UWorld* World = GetWorld())
    {
        FTimerHandle ReleaseTimerHandle;
        World->GetTimerManager().SetTimer(
            ReleaseTimerHandle,
            FTimerDelegate::CreateLambda(
                [ProviderName, ReleaseGeneration]()
                {
                    ReleaseSharedACEProviderIfIdle(
                        ProviderName,
                        ReleaseGeneration
                    );
                }
            ),
            DelaySeconds,
            false
        );
    }
    else
    {
        ReleaseSharedACEProviderIfIdle(
            ProviderName,
            ReleaseGeneration
        );
    }
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

    if (bEnableACEDirectMorphBridge && bACEDirectMorphsActive)
    {
        ClearACEDirectMorphBridge();
    }
    ACECurveSource = nullptr;
    ACEFaceMesh = nullptr;
    ResolveACEDirectMorphBridge();
    if (IsValid(ACECurveSource))
    {
        ACECurveSource->Activate(true);
        ACECurveSource->SetComponentTickEnabled(true);
    }
    if (IsValid(ACEFaceMesh))
    {
        ACEFaceMesh->SetComponentTickEnabled(true);
    }
    UE_LOG(
        LogTemp,
        Display,
        TEXT(
            "JACK_ACE_BINDING actor=%s curve_source=%s active=%d tick=%d "
            "face_mesh=%s face_anim_class=%s face_tick=%d direct_morph=%d"
        ),
        *GetNameSafe(Owner),
        *GetNameSafe(ACECurveSource),
        IsValid(ACECurveSource) && ACECurveSource->IsActive() ? 1 : 0,
        IsValid(ACECurveSource) && ACECurveSource->IsComponentTickEnabled()
            ? 1
            : 0,
        *GetNameSafe(ACEFaceMesh),
        IsValid(ACEFaceMesh)
            ? *GetNameSafe(ACEFaceMesh->GetAnimClass())
            : TEXT("None"),
        IsValid(ACEFaceMesh) && ACEFaceMesh->IsComponentTickEnabled()
            ? 1
            : 0,
        bEnableACEDirectMorphBridge ? 1 : 0
    );

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
    MarkSharedACESessionStarted();
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
    MarkSharedACESessionFinished();
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

    if (!bACEDirectMorphsActive)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT(
                "JACK_ACE_MORPHS_ACTIVE actor=%s face_mesh=%s "
                "weights=%d jaw_open=%.3f mouth_close=%.3f"
            ),
            *GetNameSafe(GetOwner()),
            *GetNameSafe(ACEFaceMesh),
            Weights.Num(),
            GetACECurveWeight(Weights, TEXT("JawOpen")),
            GetACECurveWeight(Weights, TEXT("MouthClose"))
        );
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

    if (!bEnableKeyboardPushToTalk && !bEnableVRControllerPushToTalk)
    {
        bKeyboardPushToTalkWasDown = false;
        bKeyboardPushToTalkStopPending = false;
        return;
    }

    UWorld* World = GetWorld();
    if (UOpenAINPCConversationSubsystem* ConversationSubsystem = World
            ? World->GetSubsystem<UOpenAINPCConversationSubsystem>()
            : nullptr;
        IsValid(ConversationSubsystem) &&
        !ConversationSubsystem->IsSpeechInputCoordinator(this))
    {
        bKeyboardPushToTalkWasDown = false;
        bKeyboardPushToTalkStopPending = false;
        return;
    }

    APlayerController* PlayerController =
        World ? World->GetFirstPlayerController() : nullptr;
    if (!PlayerController)
    {
        return;
    }

    const bool bKeyboardIsDown = bEnableKeyboardPushToTalk &&
        PlayerController->IsInputKeyDown(KeyboardPushToTalkKey);
    bool bVRControllerIsDown = false;
    if (bEnableVRControllerPushToTalk)
    {
        const UInputAction* PushToTalkAction = VRPushToTalkAction.Get();
        if (!PushToTalkAction && !VRPushToTalkAction.IsNull())
        {
            PushToTalkAction = VRPushToTalkAction.LoadSynchronous();
        }

        if (const UEnhancedPlayerInput* EnhancedPlayerInput =
                Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
            EnhancedPlayerInput && PushToTalkAction)
        {
            bVRControllerIsDown = EnhancedPlayerInput
                ->GetActionValue(PushToTalkAction)
                .Get<bool>();
        }

        // Some XR runtimes still publish controller keys directly.
        bVRControllerIsDown = bVRControllerIsDown ||
            PlayerController->IsInputKeyDown(
                EKeys::OculusTouch_Right_A_Click) ||
            PlayerController->IsInputKeyDown(
                EKeys::Gamepad_FaceButton_Bottom);
    }
    const bool bIsDown = bKeyboardIsDown || bVRControllerIsDown;
    const bool bUseRealtimeVoice = ShouldUseOpenAIRealtimeVoice();
    const TCHAR* InputSource = bVRControllerIsDown
        ? TEXT("right_controller_a")
        : TEXT("keyboard_t");
    const double NowSeconds =
        World ? static_cast<double>(World->GetTimeSeconds()) : 0.0;

    if (bIsDown && !bKeyboardPushToTalkWasDown)
    {
        bKeyboardPushToTalkStopPending = false;
        UOpenAIJackComponent* RealtimeTarget = this;
        FString RealtimeTargetReason = TEXT("InputCoordinator");
        if (bUseRealtimeVoice && World)
        {
            if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                    World->GetSubsystem<
                        UOpenAINPCConversationSubsystem>())
            {
                if (UOpenAIJackComponent* ResolvedTarget =
                        ConversationSubsystem->ResolveRecognizedPlayerTarget(
                            this,
                            FString(),
                            RealtimeTargetReason
                        );
                    IsValid(ResolvedTarget) &&
                    ResolvedTarget->ShouldUseOpenAIRealtimeVoice())
                {
                    RealtimeTarget = ResolvedTarget;
                }
            }
        }
        if (bUseRealtimeVoice &&
            !TryClaimRealtimePushToTalk(RealtimeTarget))
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_SPEECH_INPUT skipped reason=realtime_owned actor=%s"),
                *GetNameSafe(GetOwner())
            );
            bKeyboardPushToTalkWasDown = bIsDown;
            return;
        }
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_SPEECH_INPUT push_to_talk_down source=%s key=%s backend=%s"),
            InputSource,
            bVRControllerIsDown
                ? TEXT("IA_PushToTalk")
                : *KeyboardPushToTalkKey.ToString(),
            bUseRealtimeVoice
                ? TEXT("openai_realtime")
                : (bEnableHttpSTT ? TEXT("http") : TEXT("windows_sapi"))
        );
        UE_LOG(
            LogTemp,
            Verbose,
            TEXT("JACK_WINDOWS_STT push_to_talk_down key=%s"),
            *KeyboardPushToTalkKey.ToString()
        );
        if (bUseRealtimeVoice)
        {
            UE_LOG(
                LogTemp,
                Display,
                TEXT(
                    "JACK_REALTIME target_selected npc=%s reason=%s "
                    "actor=%s"
                ),
                *RealtimeTarget->GetResolvedNPCID().ToString(),
                *RealtimeTargetReason,
                *GetNameSafe(RealtimeTarget->GetOwner())
            );
            if (!RealtimeTarget->StartRealtimeVoice())
            {
                ReleaseRealtimePushToTalk(RealtimeTarget);
            }
        }
        else if (bEnableHttpSTT)
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
        const UOpenAIJackComponent* RealtimeOwner =
            GetRealtimePushToTalkOwner();
        const bool bRealtimeActive = IsValid(RealtimeOwner) &&
            (RealtimeOwner->bRealtimeVoiceListening ||
             RealtimeOwner->bRealtimeVoiceRequestInFlight);
        UE_LOG(
            LogTemp,
            Display,
            TEXT("JACK_SPEECH_INPUT push_to_talk_up grace=%.2f backend=%s"),
            GraceSeconds,
            bRealtimeActive
                ? TEXT("openai_realtime")
                : (bEnableHttpSTT ? TEXT("http") : TEXT("windows_sapi"))
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
        UOpenAIJackComponent* RealtimeOwner =
            GetRealtimePushToTalkOwner();
        if (IsValid(RealtimeOwner) &&
            (RealtimeOwner->bRealtimeVoiceListening ||
             RealtimeOwner->bRealtimeVoiceRequestInFlight))
        {
            RealtimeOwner->StopRealtimeVoice();
        }
        else if (bEnableHttpSTT)
        {
            StopHttpSTT();
        }
        else if (bWindowsSTTListening)
        {
            StopWindowsSTT();
        }
    }
}

bool UOpenAIJackComponent::StartRealtimeVoice()
{
    if (!ShouldUseOpenAIRealtimeVoice())
    {
        ReleaseRealtimePushToTalk(this);
        return false;
    }
    if (RealtimeStartUrl.TrimStartAndEnd().IsEmpty())
    {
        ReleaseRealtimePushToTalk(this);
        Fail(TEXT("OpenAI Realtime start URL is empty."));
        return false;
    }
    if (bRealtimeVoiceListening)
    {
        return true;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(
        TEXT("instructions"),
        GetResolvedRealtimeInstructions() +
        TEXT(" Speak only dialogue addressed to the player. Do not read ")
        TEXT("metadata, stage directions, or system behavior aloud.")
    );
    Body->SetStringField(TEXT("npc_id"), GetResolvedNPCID().ToString());
    Body->SetStringField(TEXT("memory_file"), MemoryFileName);
    Body->SetStringField(TEXT("voice"), GetResolvedRealtimeVoice());

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const int32 RequestGeneration = ++RealtimeVoiceRequestGeneration;
    bRealtimeVoiceListening = true;
    bRealtimeVoiceRequestInFlight = true;
    const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(RealtimeStartUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [WeakThis, RequestGeneration](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            UOpenAIJackComponent* StrongThis = WeakThis.Get();
            if (!IsValid(StrongThis) ||
                RequestGeneration !=
                    StrongThis->RealtimeVoiceRequestGeneration)
            {
                return;
            }

            StrongThis->bRealtimeVoiceRequestInFlight = false;
            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded || !Response.IsValid() ||
                ResponseCode < 200 || ResponseCode >= 300)
            {
                StrongThis->bRealtimeVoiceListening = false;
                ReleaseRealtimePushToTalk(StrongThis);
                StrongThis->Fail(FString::Printf(
                    TEXT("OpenAI Realtime start failed code=%d body=%s"),
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
                TEXT("JACK_REALTIME started actor=%s npc=%s voice=%s url=%s"),
                *GetNameSafe(StrongThis->GetOwner()),
                *StrongThis->GetResolvedNPCID().ToString(),
                *StrongThis->GetResolvedRealtimeVoice(),
                *StrongThis->RealtimeStartUrl
            );
        }
    );

    if (!Request->ProcessRequest())
    {
        bRealtimeVoiceListening = false;
        bRealtimeVoiceRequestInFlight = false;
        ReleaseRealtimePushToTalk(this);
        Fail(TEXT("OpenAI Realtime start request could not be processed."));
        return false;
    }
    return true;
}

void UOpenAIJackComponent::RequestRealtimeSpeech(const FString& Text)
{
    const FString TrimmedText = Text.TrimStartAndEnd();
    if (TrimmedText.IsEmpty())
    {
        bBusy = false;
        return;
    }
    if (RealtimeSayUrl.TrimStartAndEnd().IsEmpty())
    {
        Fail(TEXT("OpenAI Realtime say URL is empty."));
        return;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("text"), TrimmedText);
    Body->SetStringField(
        TEXT("instructions"),
        GetResolvedRealtimeInstructions()
    );
    Body->SetStringField(TEXT("npc_id"), GetResolvedNPCID().ToString());
    Body->SetStringField(TEXT("voice"), GetResolvedRealtimeVoice());

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const double RequestStartSeconds = FPlatformTime::Seconds();
    const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);
    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(RealtimeSayUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [WeakThis, TrimmedText, RequestStartSeconds](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            UOpenAIJackComponent* StrongThis = WeakThis.Get();
            if (!IsValid(StrongThis))
            {
                return;
            }

            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded || !Response.IsValid() ||
                ResponseCode < 200 || ResponseCode >= 300)
            {
                StrongThis->Fail(FString::Printf(
                    TEXT("OpenAI Realtime say failed code=%d body=%s"),
                    ResponseCode,
                    Response.IsValid()
                        ? *Response->GetContentAsString()
                        : TEXT("<no response>")
                ));
                return;
            }

            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            bool bOk = false;
            if (!FJsonSerializer::Deserialize(Reader, Root) ||
                !Root.IsValid() ||
                !Root->TryGetBoolField(TEXT("ok"), bOk) ||
                !bOk)
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime say returned invalid JSON.")
                );
                return;
            }

            FString AudioBase64;
            Root->TryGetStringField(TEXT("audio_wav_base64"), AudioBase64);
            TArray<uint8> WavBytes;
            if (AudioBase64.IsEmpty() ||
                !FBase64::Decode(AudioBase64, WavBytes))
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime say returned no valid WAV audio.")
                );
                return;
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT(
                    "JACK_REALTIME_SAY completed npc=%s ms=%.1f "
                    "chars=%d audio_bytes=%d"
                ),
                *StrongThis->GetResolvedNPCID().ToString(),
                (FPlatformTime::Seconds() - RequestStartSeconds) * 1000.0,
                TrimmedText.Len(),
                WavBytes.Num()
            );
            StrongThis->ShowScreenSubtitle(
                TrimmedText,
                EstimateWavDurationSeconds(WavBytes)
            );
            if (StrongThis->bEnableACEAudio2Face &&
                StrongThis->TryPlayWavWithACE(WavBytes))
            {
                return;
            }
            StrongThis->PlayWavBytes(WavBytes);
            StrongThis->bBusy = false;
        }
    );

    if (!Request->ProcessRequest())
    {
        Fail(TEXT("OpenAI Realtime say request could not be processed."));
    }
}

void UOpenAIJackComponent::RequestRealtimeTextResponse(
    const FString& PlayerText
)
{
    const FString TrimmedPlayerText = PlayerText.TrimStartAndEnd();
    if (TrimmedPlayerText.IsEmpty())
    {
        bBusy = false;
        return;
    }
    if (RealtimeRespondUrl.TrimStartAndEnd().IsEmpty())
    {
        Fail(TEXT("OpenAI Realtime respond URL is empty."));
        return;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("text"), TrimmedPlayerText);
    Body->SetStringField(
        TEXT("instructions"),
        GetResolvedRealtimeInstructions()
    );
    Body->SetStringField(TEXT("npc_id"), GetResolvedNPCID().ToString());
    Body->SetStringField(TEXT("voice"), GetResolvedRealtimeVoice());

    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const double RequestStartSeconds = FPlatformTime::Seconds();
    const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);
    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(RealtimeRespondUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [WeakThis, TrimmedPlayerText, RequestStartSeconds](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            UOpenAIJackComponent* StrongThis = WeakThis.Get();
            if (!IsValid(StrongThis))
            {
                return;
            }

            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded || !Response.IsValid() ||
                ResponseCode < 200 || ResponseCode >= 300)
            {
                StrongThis->Fail(FString::Printf(
                    TEXT("OpenAI Realtime respond failed code=%d body=%s"),
                    ResponseCode,
                    Response.IsValid()
                        ? *Response->GetContentAsString()
                        : TEXT("<no response>")
                ));
                return;
            }

            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            bool bOk = false;
            if (!FJsonSerializer::Deserialize(Reader, Root) ||
                !Root.IsValid() ||
                !Root->TryGetBoolField(TEXT("ok"), bOk) ||
                !bOk)
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime respond returned invalid JSON.")
                );
                return;
            }

            FString ReplyText;
            FString AudioBase64;
            Root->TryGetStringField(TEXT("reply_text"), ReplyText);
            Root->TryGetStringField(TEXT("audio_wav_base64"), AudioBase64);
            ReplyText.TrimStartAndEndInline();

            TArray<uint8> WavBytes;
            if (ReplyText.IsEmpty() || AudioBase64.IsEmpty() ||
                !FBase64::Decode(AudioBase64, WavBytes))
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime respond returned no reply audio.")
                );
                return;
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT(
                    "JACK_REALTIME_RESPOND completed npc=%s voice=%s "
                    "ms=%.1f reply_chars=%d audio_bytes=%d"
                ),
                *StrongThis->GetResolvedNPCID().ToString(),
                *StrongThis->GetResolvedRealtimeVoice(),
                (FPlatformTime::Seconds() - RequestStartSeconds) * 1000.0,
                ReplyText.Len(),
                WavBytes.Num()
            );
            StrongThis->PublishReplyText(ReplyText);
            StrongThis->AddConversationTurn(
                TrimmedPlayerText,
                ReplyText
            );
            StrongThis->ShowScreenSubtitle(
                ReplyText,
                EstimateWavDurationSeconds(WavBytes)
            );
            if (StrongThis->bEnableACEAudio2Face &&
                StrongThis->TryPlayWavWithACE(WavBytes))
            {
                return;
            }
            StrongThis->PlayWavBytes(WavBytes);
            StrongThis->bBusy = false;
        }
    );

    if (!Request->ProcessRequest())
    {
        Fail(TEXT("OpenAI Realtime respond request could not be processed."));
    }
}

void UOpenAIJackComponent::StopRealtimeVoice()
{
    if (!bRealtimeVoiceListening && !bRealtimeVoiceRequestInFlight)
    {
        ReleaseRealtimePushToTalk(this);
        return;
    }
    if (RealtimeStopUrl.TrimStartAndEnd().IsEmpty())
    {
        bRealtimeVoiceListening = false;
        bRealtimeVoiceRequestInFlight = false;
        ReleaseRealtimePushToTalk(this);
        Fail(TEXT("OpenAI Realtime stop URL is empty."));
        return;
    }

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    FString Json;
    const TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Body, Writer);

    const int32 RequestGeneration = ++RealtimeVoiceRequestGeneration;
    bRealtimeVoiceListening = false;
    bRealtimeVoiceRequestInFlight = true;
    bBusy = true;

    const double StopStartSeconds = FPlatformTime::Seconds();
    const TWeakObjectPtr<UOpenAIJackComponent> WeakThis(this);
    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();
    Request->SetURL(RealtimeStopUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetTimeout(RequestTimeoutSeconds);
    Request->SetContentAsString(Json);
    Request->OnProcessRequestComplete().BindLambda(
        [WeakThis, RequestGeneration, StopStartSeconds](
            FHttpRequestPtr,
            FHttpResponsePtr Response,
            bool bSucceeded
        )
        {
            UOpenAIJackComponent* StrongThis = WeakThis.Get();
            if (!IsValid(StrongThis) ||
                RequestGeneration !=
                    StrongThis->RealtimeVoiceRequestGeneration)
            {
                return;
            }

            ReleaseRealtimePushToTalk(StrongThis);
            StrongThis->bRealtimeVoiceRequestInFlight = false;
            const int32 ResponseCode =
                Response.IsValid() ? Response->GetResponseCode() : 0;
            if (!bSucceeded || !Response.IsValid() ||
                ResponseCode < 200 || ResponseCode >= 300)
            {
                StrongThis->Fail(FString::Printf(
                    TEXT("OpenAI Realtime stop failed code=%d body=%s"),
                    ResponseCode,
                    Response.IsValid()
                        ? *Response->GetContentAsString()
                        : TEXT("<no response>")
                ));
                return;
            }

            TSharedPtr<FJsonObject> Root;
            const TSharedRef<TJsonReader<>> Reader =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());
            if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime bridge returned invalid JSON.")
                );
                return;
            }

            bool bOk = false;
            bool bSkipped = false;
            Root->TryGetBoolField(TEXT("ok"), bOk);
            Root->TryGetBoolField(TEXT("skipped"), bSkipped);
            if (!bOk && bSkipped)
            {
                FString Reason;
                Root->TryGetStringField(TEXT("reason"), Reason);
                StrongThis->bBusy = false;
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("JACK_REALTIME skipped reason=%s"),
                    *Reason
                );
                return;
            }
            if (!bOk)
            {
                FString Error;
                Root->TryGetStringField(TEXT("error"), Error);
                StrongThis->Fail(Error.IsEmpty()
                    ? TEXT("OpenAI Realtime bridge request failed.")
                    : Error);
                return;
            }

            FString UserText;
            FString ReplyText;
            FString AudioBase64;
            double RealtimeMs = 0.0;
            Root->TryGetStringField(TEXT("user_text"), UserText);
            Root->TryGetStringField(TEXT("reply_text"), ReplyText);
            Root->TryGetStringField(TEXT("audio_wav_base64"), AudioBase64);
            Root->TryGetNumberField(TEXT("realtime_ms"), RealtimeMs);
            UserText.TrimStartAndEndInline();
            ReplyText.TrimStartAndEndInline();

            TArray<uint8> WavBytes;
            if (AudioBase64.IsEmpty() ||
                !FBase64::Decode(AudioBase64, WavBytes))
            {
                StrongThis->Fail(
                    TEXT("OpenAI Realtime bridge returned no valid WAV audio.")
                );
                return;
            }

            UE_LOG(
                LogTemp,
                Display,
                TEXT("JACK_REALTIME stopped total_ms=%.1f realtime_ms=%.1f user_chars=%d reply_chars=%d audio_bytes=%d"),
                (FPlatformTime::Seconds() - StopStartSeconds) * 1000.0,
                RealtimeMs,
                UserText.Len(),
                ReplyText.Len(),
                WavBytes.Num()
            );

            UOpenAIJackComponent* ResponseTarget = StrongThis;
            UOpenAINPCConversationSubsystem* ConversationSubsystem = nullptr;
            FString TargetReason = TEXT("RealtimeSessionOwner");
            if (UWorld* World = StrongThis->GetWorld())
            {
                ConversationSubsystem = World->GetSubsystem<
                    UOpenAINPCConversationSubsystem>();
            }

            if (!UserText.IsEmpty())
            {
                if (IsValid(ConversationSubsystem))
                {
                    if (UOpenAIJackComponent* ResolvedTarget =
                            ConversationSubsystem->
                                ResolveRecognizedPlayerTarget(
                                    StrongThis,
                                    UserText,
                                    TargetReason
                                ))
                    {
                        ResponseTarget = ResolvedTarget;
                    }
                }

                if (ResponseTarget != StrongThis)
                {
                    StrongThis->bBusy = false;
                    if (ResponseTarget->IsConversationOutputActive())
                    {
                        ResponseTarget->InterruptConversationOutput();
                    }
                    ResponseTarget->bBusy = true;
                }

                ResponseTarget->OnRecognizedSpeechText.Broadcast(UserText);
                if (IsValid(ConversationSubsystem))
                {
                    ConversationSubsystem->NotifyPlayerTextSubmitted(
                        ResponseTarget,
                        UserText
                    );
                }

                FString ActionReply;
                const bool bActionHandled = ResponseTarget->
                    TryExecuteRecognizedPlayerAction(
                        UserText,
                        ActionReply
                    );
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT(
                        "JACK_REALTIME response_target=%s reason=%s "
                        "session_owner=%s action_handled=%d "
                        "text=%s reply=%s"
                    ),
                    *ResponseTarget->GetResolvedNPCID().ToString(),
                    *TargetReason,
                    *StrongThis->GetResolvedNPCID().ToString(),
                    bActionHandled ? 1 : 0,
                    *UserText,
                    *ActionReply
                );
            }
            if (ResponseTarget != StrongThis && !UserText.IsEmpty())
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT(
                        "JACK_REALTIME reroute_regenerate from=%s to=%s "
                        "voice=%s"
                    ),
                    *StrongThis->GetResolvedNPCID().ToString(),
                    *ResponseTarget->GetResolvedNPCID().ToString(),
                    *ResponseTarget->GetResolvedRealtimeVoice()
                );
                ResponseTarget->RequestRealtimeTextResponse(UserText);
                return;
            }
            if (!ReplyText.IsEmpty())
            {
                ResponseTarget->PublishReplyText(ReplyText);
                ResponseTarget->AddConversationTurn(
                    UserText.IsEmpty() ? TEXT("<voice input>") : UserText,
                    ReplyText
                );
                ResponseTarget->ShowScreenSubtitle(
                    ReplyText,
                    EstimateWavDurationSeconds(WavBytes)
                );
            }

            if (ResponseTarget->bEnableACEAudio2Face &&
                ResponseTarget->TryPlayWavWithACE(WavBytes))
            {
                return;
            }
            ResponseTarget->PlayWavBytes(WavBytes);
            ResponseTarget->bBusy = false;
            StrongThis->bBusy = false;
        }
    );

    if (!Request->ProcessRequest())
    {
        bRealtimeVoiceRequestInFlight = false;
        bBusy = false;
        ReleaseRealtimePushToTalk(this);
        Fail(TEXT("OpenAI Realtime stop request could not be processed."));
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

    if (UWorld* World = GetWorld())
    {
        if (UOpenAINPCConversationSubsystem* ConversationSubsystem =
                World->GetSubsystem<UOpenAINPCConversationSubsystem>();
            IsValid(ConversationSubsystem) &&
            ConversationSubsystem->RouteRecognizedPlayerText(
                this,
                TrimmedText
            ))
        {
            return;
        }
    }

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
