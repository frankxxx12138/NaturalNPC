#pragma once

#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "OpenAIJackComponent.generated.h"

class UAudioComponent;
class UAsyncActionAnimateCharacter;
class UACEAudioCurveSourceComponent;
class USkeletalMeshComponent;
class USoundWaveProcedural;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FJackTextEvent,
    const FString&,
    Text
);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FJackErrorEvent,
    const FString&,
    Error
);

UCLASS(ClassGroup = (OpenAI), meta = (BlueprintSpawnableComponent))
class OPENAIJACKNPC_API UOpenAIJackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOpenAIJackComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString Model = TEXT("gemma4:e4b");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString OllamaChatUrl = TEXT("http://127.0.0.1:11435/api/chat");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    bool bEnableOpenAITTS = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableWindowsTTS = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableHttpTTS = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableACEAudio2Face = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FName ACEAudio2FaceProviderName = TEXT("LocalA2F-Mark");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableACEDirectMorphBridge = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float ACEDirectMorphScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bPreAllocateACEAudio2FaceResources = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bWarmUpACEAudio2FaceOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float ACEWarmupDelaySeconds = 0.75f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.05", ClampMax = "2.0"))
    float ACEWarmupDurationSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.02", ClampMax = "0.5"))
    float ACERealtimeInitialChunkSeconds = 0.03f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.01", ClampMax = "0.5"))
    float ACEAudioBufferSeconds = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FString WindowsVoiceName = TEXT("Microsoft Huihui Desktop");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "-10", ClampMax = "10"))
    int32 WindowsVoiceRate = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0", ClampMax = "100"))
    int32 WindowsVoiceVolume = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FString HttpTTSUrl = TEXT("http://127.0.0.1:8020/tts");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FString HttpTTSVoice = TEXT("Mandarin-CN.Male-1");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FString HttpTTSLanguageCode = TEXT("en-US");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "8000", ClampMax = "48000"))
    int32 HttpTTSSampleRateHz = 44100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableQueuedSpeech = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableInstantAcknowledgement = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    FString InstantAcknowledgementText = TEXT("Sure.");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.0", ClampMax = "3.0"))
    float InstantAcknowledgementDelaySeconds = 0.75f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "8", ClampMax = "120"))
    int32 MaxSpeechSegmentCharacters = 48;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "1", ClampMax = "6"))
    int32 FirstSpeechSegmentSentences = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice")
    bool bEnableFirstSpeechSegmentSoftBoundary = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "16", ClampMax = "120"))
    int32 FirstSpeechSegmentSoftMinCharacters = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "1", ClampMax = "6"))
    int32 MaxSentencesPerSpeechSegment = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float SpeechSegmentPaddingSeconds = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Voice",
        meta = (ClampMin = "1", ClampMax = "4"))
    int32 MaxConcurrentSpeechRequests = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (ClampMin = "1.0"))
    float RequestTimeoutSeconds = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    bool bEnableStreamingResponses = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (ClampMin = "512", ClampMax = "32768"))
    int32 ContextLength = 4096;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (ClampMin = "0", ClampMax = "512"))
    int32 MaxOutputTokens = 64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (ClampMin = "0"))
    int32 GpuLayers = 999;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString KeepAlive = TEXT("10m");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (ClampMin = "1", ClampMax = "20"))
    int32 MaxConversationTurns = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory")
    bool bEnableSessionMemoryFile = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory",
        meta = (ClampMin = "10", ClampMax = "1000"))
    int32 MaxStoredConversationTurns = 200;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory",
        meta = (ClampMin = "0", ClampMax = "10"))
    int32 MaxRelevantPastTurns = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory")
    FString MemoryFileName = TEXT("JackMemory.json");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory")
    FString EmbeddingModel = TEXT("embeddinggemma");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Memory")
    FString OllamaEmbedUrl = TEXT("http://127.0.0.1:11435/api/embed");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString SpeechModel = TEXT("gpt-4o-mini-tts");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString Voice = TEXT("onyx");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack",
        meta = (MultiLine = true))
    FString CharacterInstructions =
        TEXT("You are Jack Thompson, a grounded and observant bartender. ")
        TEXT("Always reply in natural English unless the player explicitly ")
        TEXT("asks you to speak another language. For voice replies, usually ")
        TEXT("answer in one or two natural sentences. If the player asks for ")
        TEXT("recommendations, comparisons, or explanations, you may answer in ")
        TEXT("two or three concise sentences. Stay in character and never ")
        TEXT("mention being an AI or language model.");

    UPROPERTY(BlueprintAssignable, Category = "Local AI|Jack")
    FJackTextEvent OnReplyText;

    UPROPERTY(BlueprintAssignable, Category = "Local AI|Jack Speech Input")
    FJackTextEvent OnRecognizedSpeechText;

    UPROPERTY(BlueprintAssignable, Category = "Local AI|Jack")
    FJackErrorEvent OnError;

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack")
    void SendPlayerText(const FString& PlayerText);

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack")
    void ClearConversation();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Voice")
    bool AnimateWavFileWithACE(const FString& WavFilePath);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input")
    bool bStopWindowsSTTAfterRecognizedSpeech = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input")
    bool bEnableKeyboardPushToTalk = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input")
    bool bEnableHttpSTT = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (EditCondition = "bEnableHttpSTT"))
    FString HttpSTTStartUrl = TEXT("http://127.0.0.1:8030/stt/start");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (EditCondition = "bEnableHttpSTT"))
    FString HttpSTTStopUrl = TEXT("http://127.0.0.1:8030/stt/stop");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (EditCondition = "bEnableHttpSTT"))
    FString HttpSTTLanguage = TEXT("auto");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (EditCondition = "bEnableKeyboardPushToTalk"))
    FKey KeyboardPushToTalkKey = EKeys::T;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (ClampMin = "0.0", ClampMax = "2.0", EditCondition = "bEnableKeyboardPushToTalk"))
    float KeyboardPushToTalkReleaseGraceSeconds = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Speech Input",
        meta = (ClampMin = "1", ClampMax = "120"))
    int32 WindowsSTTMinCharacters = 2;

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Speech Input")
    bool StartWindowsSTT();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Speech Input")
    void StopWindowsSTT();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Speech Input")
    bool ToggleWindowsSTT();

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Speech Input")
    bool IsWindowsSTTListening() const { return bWindowsSTTListening; }

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Memory")
    bool SaveMemory();

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Memory")
    FString GetMemoryFilePath() const;

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack")
    int32 GetConversationTurnCount() const;

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack")
    bool IsBusy() const { return bBusy; }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:
    struct FConversationMessage
    {
        FString Role;
        FString Content;
    };

    struct FMemoryTurn
    {
        FString PlayerText;
        FString ReplyText;
        TArray<float> Embedding;
    };

    struct FSpeechQueueItem
    {
        int32 Id = 0;
        FString Text;
        TArray<uint8> WavBytes;
        float DurationSeconds = 0.0f;
        bool bTTSRequested = false;
        bool bTTSReady = false;
        bool bTTSFailed = false;
    };

    FString GetApiKey() const;
    void RequestRelevantMemory(const FString& PlayerText);
    void RequestResponse(
        const FString& PlayerText,
        const TArray<FConversationMessage>& RelevantPastMessages
    );
    void AddConversationTurn(
        const FString& PlayerText,
        const FString& ReplyText
    );
    void RequestTurnEmbedding(int32 TurnIndex);
    void WarmEmbeddingModel();
    void InitializeSessionMemory();
    TArray<FConversationMessage> FindRelevantPastMessages(
        const TArray<float>& QueryEmbedding
    ) const;
    void RequestSpeech(const FString& ReplyText);
    void RequestHttpSpeech(const FString& ReplyText);
    void RequestHttpSpeechInternal(
        const FString& ReplyText,
        bool bFromSpeechQueue,
        int32 SpeechQueueItemId = INDEX_NONE,
        int32 QueueGeneration = 0
    );
    void RequestQueuedHttpSpeech(
        int32 SpeechQueueItemId,
        const FString& ReplyText
    );
    void RequestWindowsSpeech(const FString& ReplyText);
    void ResetSpeechQueue();
    void EnqueueSpeechText(const FString& Text);
    void StartSpeechPrefetches();
    FSpeechQueueItem* FindSpeechQueueItem(int32 SpeechQueueItemId);
    void PumpSpeechQueue();
    void CompleteQueuedSpeechSegment();
    void ScheduleQueuedSpeechCompletion(float DelaySeconds);
    void TryCompleteQueuedACESpeechSegment();
    void ScheduleQueuedACEFallbackCompletion(float DelaySeconds);
    void BindACEPlaybackDelegates();
    void ScheduleACEWarmup();
    void TryStartACEWarmup();
    void ScheduleInstantAcknowledgement();
    void TryEnqueueInstantAcknowledgement();
    TArray<FString> SplitReplyIntoSpeechSegments(const FString& ReplyText) const;
    bool TryPlayWavWithACE(const TArray<uint8>& WavBytes);
    bool StartACEWavFileAsync(const FString& WavFilePath, bool bDeleteWhenComplete);

    UFUNCTION()
    void HandleACEAudioSendCompleted(bool bSuccess);

    UFUNCTION()
    void HandleACEAnimationStarted();

    UFUNCTION()
    void HandleACEAnimationEnded();

    void ResolveACEDirectMorphBridge();
    void ApplyACEDirectMorphBridge();
    void ClearACEDirectMorphBridge();
    void UpdateKeyboardPushToTalk(float DeltaTime);
    bool StartHttpSTT();
    void StopHttpSTT();
    void PollWindowsSTT();
    void CleanupWindowsSTT();
    void HandleSpeechInputText(const FString& Text, const TCHAR* Source);

    void PlayWavBytes(const TArray<uint8>& WavBytes);
    void Fail(const FString& Message);

    UPROPERTY(Transient)
    TObjectPtr<UAudioComponent> AudioComponent;

    UPROPERTY(Transient)
    TObjectPtr<USoundWaveProcedural> ProceduralSound;

    UPROPERTY(Transient)
    TObjectPtr<UAsyncActionAnimateCharacter> ActiveACEAction;

    UPROPERTY(Transient)
    TObjectPtr<UACEAudioCurveSourceComponent> ACECurveSource;

    UPROPERTY(Transient)
    TObjectPtr<USkeletalMeshComponent> ACEFaceMesh;

    FString PendingACEDeletePath;
    bool bACEDirectMorphsActive = false;
    TArray<FSpeechQueueItem> SpeechQueue;
    FTimerHandle SpeechQueueTimerHandle;
    float QueuedSpeechDurationSeconds = 0.0f;
    int32 SpeechRequestsInFlight = 0;
    int32 NextSpeechQueueItemId = 1;
    int32 SpeechQueueGeneration = 0;
    bool bACEPlaybackInFlight = false;
    bool bFinalReplyReadyForCurrentTurn = false;
    bool bCurrentACEFromSpeechQueue = false;
    bool bQueuedACESendCompleted = false;
    bool bQueuedACEPlaybackStarted = false;
    bool bQueuedACEPlaybackEnded = false;
    bool bFinalSpeechQueuedForCurrentTurn = false;
    bool bACEWarmupInFlight = false;
    FTimerHandle InstantAcknowledgementTimerHandle;
    FTimerHandle ACEWarmupTimerHandle;
    void* WindowsSTTRecognizer = nullptr;
    void* WindowsSTTAudioInput = nullptr;
    void* WindowsSTTContext = nullptr;
    void* WindowsSTTGrammar = nullptr;
    bool bWindowsSTTListening = false;
    bool bWindowsSTTShouldUninitializeCOM = false;
    bool bKeyboardPushToTalkWasDown = false;
    bool bKeyboardPushToTalkStopPending = false;
    double KeyboardPushToTalkStopAtTimeSeconds = 0.0;
    FString LastWindowsSTTHypothesis;
    bool bHttpSTTListening = false;
    bool bHttpSTTRequestInFlight = false;
    int32 HttpSTTRequestGeneration = 0;

    TArray<FConversationMessage> ConversationHistory;
    TArray<FConversationMessage> PersistentHistory;
    TArray<FMemoryTurn> MemoryTurns;
    bool bBusy = false;
};
