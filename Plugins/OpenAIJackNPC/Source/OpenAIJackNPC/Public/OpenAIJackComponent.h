#pragma once

#include "Components/ActorComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "UObject/SoftObjectPtr.h"
#include "OpenAIJackComponent.generated.h"

class UAudioComponent;
class UAsyncActionAnimateCharacter;
class UACEAudioCurveSourceComponent;
class UAnimSequence;
class AActor;
class USkeletalMeshComponent;
class USoundWaveProcedural;
class UNPCWorldStateAgentComponent;
class SOverlay;
class STextBlock;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Identity")
    FName NPCID = NAME_None;

    UFUNCTION(BlueprintPure, Category = "Local AI|Identity")
    FName GetResolvedNPCID() const;

    UFUNCTION(BlueprintPure, Category = "Local AI|Memory Optimization")
    FString GetEffectiveModelKeepAlive() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Autonomous Conversation")
    bool bEnableAutonomousListening = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Autonomous Conversation",
        meta = (ClampMin = "0.0", Units = "cm"))
    float AutonomousListeningRadius = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Autonomous Conversation",
        meta = (ClampMin = "40", ClampMax = "500"))
    int32 MaximumAutonomousResponseCharacters = 240;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Autonomous Conversation",
        meta = (MultiLine = true))
    FString AutonomousKnowledgeSummary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Autonomous Conversation",
        meta = (MultiLine = true))
    FString AutonomousInterestSummary;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Memory Optimization")
    bool bReleaseACEResourcesWhenIdle = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Memory Optimization",
        meta = (ClampMin = "5.0", ClampMax = "600.0", Units = "s"))
    float ACEIdleReleaseSeconds = 60.0f;

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
    FString HttpTTSVoice =
        TEXT("Saved/VoicePrompts/Human2_Male_Reference.wav");

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Memory Optimization")
    bool bUseMemoryOptimizedModelLifetime = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Memory Optimization")
    FString ModelIdleKeepAlive = TEXT("60s");

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Local AI|Memory Optimization")
    bool bWarmEmbeddingModelOnBeginPlay = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString SpeechModel = TEXT("gpt-4o-mini-tts");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack")
    FString Voice = TEXT("onyx");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles")
    bool bEnableScreenSubtitles = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles")
    bool bShowSubtitleSpeakerName = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles")
    FString SubtitleSpeakerName = TEXT("Jack");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles",
        meta = (ClampMin = "1.0", ClampMax = "30.0"))
    float SubtitleDisplaySeconds = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles",
        meta = (ClampMin = "12", ClampMax = "48"))
    int32 SubtitleFontSize = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Subtitles",
        meta = (ClampMin = "0.0", ClampMax = "240.0"))
    float SubtitleBottomPadding = 96.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    bool bEnableNaturalLanguageActions = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack World State")
    bool bEnableWorldState = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack World State")
    bool bEnableWorldStateNaturalLanguageActions = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack World State",
        meta = (ClampMin = "0.0"))
    float WorldStatePerceptionRadius = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    bool bUseActionAnimationOverride = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    FString ActionBodyMeshNameHint = TEXT("Body");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "20.0", ClampMax = "1000.0"))
    float FollowWalkSpeed = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "20.0", ClampMax = "1500.0"))
    float FollowRunSpeed = 360.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "40.0", ClampMax = "1000.0"))
    float FollowStopDistance = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "60.0", ClampMax = "1400.0"))
    float FollowRunDistance = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.1", ClampMax = "30.0"))
    float FollowRotationInterpSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.0", ClampMax = "200.0", Units = "cm"))
    float FollowResumeDistanceBuffer = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Collision",
        meta = (ClampMin = "10.0", ClampMax = "100.0", Units = "cm"))
    float ActionCollisionRadius = 24.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Collision",
        meta = (ClampMin = "40.0", ClampMax = "160.0", Units = "cm"))
    float ActionCollisionHalfHeight = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Collision",
        meta = (ClampMin = "10.0", ClampMax = "90.0", Units = "deg"))
    float ActionAvoidanceAngleDegrees = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Navigation",
        meta = (ClampMin = "30.0", ClampMax = "120.0", Units = "cm"))
    float ActionFallbackGridSize = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Navigation",
        meta = (ClampMin = "0.0", ClampMax = "40.0", Units = "cm"))
    float ActionNavigationClearancePadding = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Navigation",
        meta = (ClampMin = "200.0", ClampMax = "2000.0", Units = "cm"))
    float ActionFallbackSearchMargin = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Navigation",
        meta = (ClampMin = "500", ClampMax = "20000"))
    int32 ActionFallbackMaxExpandedNodes = 6000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding")
    bool bEnableActionFootGrounding = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "0.0", ClampMax = "10.0", Units = "cm"))
    float ActionFootGroundClearance = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "1.0", ClampMax = "40.0"))
    float ActionGroundingInterpSpeed = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "20.0", ClampMax = "150.0", Units = "cm"))
    float ActionGroundingMaxLift = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "0.0", ClampMax = "100.0", Units = "cm"))
    float ActionGroundingMaxDrop = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "1.0", ClampMax = "40.0"))
    float ActionActorGroundFollowInterpSpeed = 16.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "10.0", ClampMax = "100.0", Units = "cm"))
    float ActionMaximumGroundStepUp = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Grounding",
        meta = (ClampMin = "20.0", ClampMax = "200.0", Units = "cm"))
    float ActionMaximumGroundStepDown = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.05", ClampMax = "2.0", Units = "s"))
    float ActionNavigationRepathIntervalSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.05", ClampMax = "2.0", Units = "s"))
    float ActionNavigationStallSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "20.0", ClampMax = "150.0", Units = "cm"))
    float ActionStepLengthCentimeters = 70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "-180.0", ClampMax = "180.0"))
    float ActionFacingYawOffsetDegrees = -90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.0", ClampMax = "300.0"))
    float ActionJumpHeight = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions",
        meta = (ClampMin = "0.2", ClampMax = "3.0"))
    float ActionJumpDurationSeconds = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    TSoftObjectPtr<UAnimSequence> IdleAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Locomotion/"
                "Human2_MM_Idle.Human2_MM_Idle"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    TSoftObjectPtr<UAnimSequence> WalkAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Locomotion/"
                "Human2_MF_Unarmed_Walk_Fwd.Human2_MF_Unarmed_Walk_Fwd"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    TSoftObjectPtr<UAnimSequence> RunAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Locomotion/"
                "Human2_MF_Unarmed_Jog_Fwd.Human2_MF_Unarmed_Jog_Fwd"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions")
    TSoftObjectPtr<UAnimSequence> JumpAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Locomotion/"
                "Human2_MM_Jump.Human2_MM_Jump"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting")
    TSoftObjectPtr<UAnimSequence> SitDownAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Actions/"
                "Human2_SitDown.Human2_SitDown"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting")
    TSoftObjectPtr<UAnimSequence> SitIdleAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Actions/"
                "Human2_SitIdle.Human2_SitIdle"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting")
    TSoftObjectPtr<UAnimSequence> StandUpAnimation =
        TSoftObjectPtr<UAnimSequence>(
            FSoftObjectPath(TEXT(
                "/Game/MetaHumans/Human2/Animations/Actions/"
                "Human2_StandUp.Human2_StandUp"
            ))
        );

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "40.0", ClampMax = "400.0", Units = "cm"))
    float SitApproachStopDistance = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "100.0", ClampMax = "3000.0", Units = "cm"))
    float SeatSearchRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "35.0", ClampMax = "150.0", Units = "cm"))
    float SeatApproachDistance = 70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.0", ClampMax = "35.0", Units = "cm"))
    float SeatPelvisAboveSurface = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "1.0", ClampMax = "30.0"))
    float SeatAlignmentInterpSpeed = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.0", ClampMax = "30.0", Units = "s"))
    float SitDownStartTimeSeconds = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.0", ClampMax = "30.0", Units = "s"))
    float SitDownEndTimeSeconds = 4.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.0", ClampMax = "30.0", Units = "s"))
    float StandUpEndTimeSeconds = 2.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float SitTransitionPlayRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Local AI|Jack Actions|Sitting",
        meta = (ClampMin = "0.0", ClampMax = "200.0", Units = "cm"))
    float StandUpForwardAdjustment = 59.0f;

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

    UFUNCTION(BlueprintCallable, Category = "Local AI|Autonomous Conversation")
    bool SpeakGovernedText(const FString& ResponseText);

    UFUNCTION(BlueprintCallable, Category = "Local AI|Autonomous Conversation")
    bool InterruptConversationOutput();

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

    UFUNCTION(BlueprintPure, Category = "Local AI|Autonomous Conversation")
    bool IsConversationOutputActive() const;

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    void StartFollowingPlayer(bool bRun);

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    void StopFollowingPlayer();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    void PlayJumpAction();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    void PlaySitDownAction();

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    void PlayStandUpAction();

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Actions")
    bool IsSitting() const
    {
        return bIsSitting || bSitDownInProgress;
    }

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack Actions")
    bool TryExecuteMovementCommand(
        const FString& PlayerText,
        FString& OutReply
    );

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Actions")
    bool IsFollowingPlayer() const { return bFollowingPlayer; }

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Actions")
    bool IsExecutingMoveCommand() const
    {
        return bMovingToCommandLocation;
    }

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack Actions")
    FVector GetMoveCommandDestination() const
    {
        return CommandMoveDestination;
    }

    UFUNCTION(BlueprintPure, Category = "Local AI|Jack World State")
    FString GetWorldStateJson() const;

    UFUNCTION(BlueprintCallable, Category = "Local AI|Jack World State")
    bool ExecuteWorldAction(
        FName ObjectId,
        FName ActionId,
        const FString& Parameters,
        FString& OutMessage
    );

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

    enum class EActionAnimationState : uint8
    {
        None,
        Idle,
        Walk,
        Run,
        Jump,
        SitDown,
        SitIdle,
        StandUp
    };

    enum class EPendingAfterStandAction : uint8
    {
        None,
        FollowWalk,
        FollowRun,
        MoveToLocation,
        Jump
    };

    struct FActionAnimationMeshState
    {
        TWeakObjectPtr<USkeletalMeshComponent> Mesh;
        EAnimationMode::Type SavedAnimationMode =
            EAnimationMode::AnimationBlueprint;
        UClass* SavedAnimClass = nullptr;
    };

    FString GetApiKey() const;
    bool TryHandleNaturalLanguageAction(
        const FString& PlayerText,
        FString& OutReply
    );
    void TryRunPendingPostWorldActionCommand();
    void SpeakLocalActionReply(
        const FString& PlayerText,
        const FString& ReplyText
    );
    void UpdateActionMovement(float DeltaTime);
    void UpdateActionGrounding(float DeltaTime);
    void StartFollowingPlayerImmediate(bool bRun);
    bool StartSittingAtBestSeat(
        const FVector& ReferenceLocation,
        FString& OutFailure
    );
    bool FindBestAvailableSeat(
        const FVector& ReferenceLocation,
        AActor*& OutSeat,
        FVector& OutSeatCenter,
        FVector& OutSeatForward,
        float& OutSeatSurfaceZ,
        FVector& OutApproachLocation
    ) const;
    bool ReserveSeat(AActor* Seat);
    void ReleaseReservedSeat();
    void BeginSitAnimationAtReservedSeat();
    void PrepareSeatBodyAlignment();
    USkeletalMeshComponent* ResolveActionBodyMesh();
    void ResetActionBodyOffset();
    bool FindActionGroundHeight(const FVector& Location, float& OutGroundZ) const;
    bool FindCollisionSafeMove(
        const FVector& StartLocation,
        const FVector& DesiredDelta,
        const FVector& GoalDirection,
        FVector& OutLocation,
        FHitResult& OutHit
    );
    bool IsActionCapsuleMoveBlocked(
        const FVector& StartLocation,
        const FVector& Delta,
        FHitResult& OutHit
    ) const;
    bool IsActionNavigationSegmentBlocked(
        const FVector& StartNavigationLocation,
        const FVector& EndNavigationLocation,
        FHitResult& OutHit
    ) const;
    bool IsActionNavigationLocationBlocked(
        const FVector& NavigationLocation
    ) const;
    bool BuildCollisionAwareNavigationPath(
        const FVector& StartLocation,
        const FVector& GoalLocation,
        TArray<FVector>& OutPathPoints,
        int32& OutExpandedNodes
    ) const;
    bool UpdateActorGroundHeight(
        FVector& InOutLocation,
        float DeltaTime
    );
    bool StartMoveToLocation(
        const FVector& Destination,
        float StopDistance,
        bool bRun,
        bool bFacePlayerAtDestination,
        FString& OutFailure,
        bool bSitAtDestination = false
    );
    bool RebuildActionNavigationPath(
        const FVector& DesiredGoal,
        float StopDistance
    );
    void ClearActionNavigationPath();
    void UpdateActionJump(float DeltaTime);
    void BeginStandUpTransition();
    void FinishSitDownTransition();
    void FinishStandUpTransition();
    void RunPendingAfterStandAction();
    void CancelSittingImmediately();
    void EnsureWorldStateAgent();
    AActor* ResolveFollowTarget() const;
    TArray<USkeletalMeshComponent*> ResolveActionAnimationMeshes();
    bool ShouldUseMeshForActionAnimation(
        const USkeletalMeshComponent* Mesh
    ) const;
    void PlayActionAnimation(
        EActionAnimationState State,
        bool bLooping,
        float StartPositionSeconds = 0.0f,
        float PlayRate = 1.0f
    );
    void RestoreActionAnimation();
    UAnimSequence* ResolveActionAnimation(EActionAnimationState State) const;
    void RequestRelevantMemory(const FString& PlayerText);
    void RequestResponse(
        const FString& PlayerText,
        const TArray<FConversationMessage>& RelevantPastMessages
    );
    void AddConversationTurn(
        const FString& PlayerText,
        const FString& ReplyText
    );
    void PublishReplyText(const FString& ReplyText);
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
    void RegisterSharedACEProvider();
    void UnregisterSharedACEProvider();
    void MarkSharedACESessionStarted();
    void MarkSharedACESessionFinished();
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
    void EnsureScreenSubtitleWidget();
    void ShowScreenSubtitle(
        const FString& Text,
        float AudioDurationSeconds = 0.0f
    );
    void HideScreenSubtitle();
    float GetSubtitleDisplayDuration(
        const FString& Text,
        float AudioDurationSeconds
    ) const;

    void PlayWavBytes(const TArray<uint8>& WavBytes);
    void Fail(const FString& Message);

    UPROPERTY(Transient)
    TObjectPtr<UAudioComponent> AudioComponent;

    FString PendingPostWorldActionCommand;

    UPROPERTY(Transient)
    TObjectPtr<USoundWaveProcedural> ProceduralSound;

    UPROPERTY(Transient)
    TObjectPtr<UAsyncActionAnimateCharacter> ActiveACEAction;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UAsyncActionAnimateCharacter>> InterruptedACEActions;

    UPROPERTY(Transient)
    TObjectPtr<UACEAudioCurveSourceComponent> ACECurveSource;

    UPROPERTY(Transient)
    TObjectPtr<UNPCWorldStateAgentComponent> WorldStateAgent;

    UPROPERTY(Transient)
    TObjectPtr<USkeletalMeshComponent> ACEFaceMesh;

    FString PendingACEDeletePath;
    TArray<FString> InterruptedACEDeletePaths;
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
    bool bSharedACEProviderRegistered = false;
    bool bSharedACESessionCounted = false;
    bool bSubtitleWidgetAdded = false;
    bool bFollowingPlayer = false;
    bool bFollowUsingRun = false;
    bool bMovingToCommandLocation = false;
    bool bCommandMoveUseRun = false;
    bool bFacePlayerWhenCommandMoveFinishes = false;
    bool bSitAfterCommandMove = false;
    bool bIsSitting = false;
    bool bSitDownInProgress = false;
    bool bStandUpInProgress = false;
    bool bActionJumpInProgress = false;
    bool bActionAnimationStateSaved = false;
    bool bActionBodyBaseTransformSaved = false;
    bool bSeatBodyAlignmentActive = false;
    float ActionJumpElapsedSeconds = 0.0f;
    float ActionJumpActiveDurationSeconds = 0.0f;
    float ActionJumpBaseZ = 0.0f;
    TWeakObjectPtr<AActor> FollowTargetActor;
    FVector CommandMoveDestination = FVector::ZeroVector;
    float CommandMoveStopDistance = 35.0f;
    EPendingAfterStandAction PendingAfterStandAction =
        EPendingAfterStandAction::None;
    FVector PendingStandMoveDestination = FVector::ZeroVector;
    float PendingStandMoveStopDistance = 35.0f;
    bool bPendingStandMoveUseRun = false;
    bool bPendingStandMoveFacePlayer = false;
    bool bPendingStandMoveSitAtDestination = false;
    TArray<FVector> ActionNavigationPathPoints;
    int32 ActionNavigationPathPointIndex = 0;
    float ActionNavigationRepathElapsedSeconds = 0.0f;
    float ActionNavigationStallElapsedSeconds = 0.0f;
    int32 ActionNavigationFailureCount = 0;
    FVector ActionNavigationGoal = FVector::ZeroVector;
    FVector ActionPersistentAvoidanceDirection = FVector::ZeroVector;
    float ActionPersistentAvoidanceDistanceRemaining = 0.0f;
    TWeakObjectPtr<AActor> ActionPersistentBlockingActor;
    bool bActionActorGroundOffsetInitialized = false;
    float ActionActorGroundOffsetZ = 0.0f;
    TWeakObjectPtr<AActor> ReservedSeatActor;
    FVector ReservedSeatCenter = FVector::ZeroVector;
    FVector ReservedSeatForward = FVector::ForwardVector;
    FVector ReservedSeatApproachLocation = FVector::ZeroVector;
    float ReservedSeatSurfaceZ = 0.0f;
    TWeakObjectPtr<USkeletalMeshComponent> ActionBodyMesh;
    FVector ActionBodyBaseRelativeLocation = FVector::ZeroVector;
    FVector ActionBodyTargetRelativeLocation = FVector::ZeroVector;
    TArray<TWeakObjectPtr<USkeletalMeshComponent>> ActionAnimationMeshes;
    TArray<FActionAnimationMeshState> SavedActionAnimationMeshStates;
    EActionAnimationState CurrentActionAnimationState =
        EActionAnimationState::None;
    FString PendingQueuedACESubtitleText;
    float PendingQueuedACESubtitleDurationSeconds = 0.0f;
    TSharedPtr<SOverlay> SubtitleRootWidget;
    TSharedPtr<STextBlock> SubtitleTextBlock;
    FTimerHandle InstantAcknowledgementTimerHandle;
    FTimerHandle ACEWarmupTimerHandle;
    FTimerHandle SubtitleTimerHandle;
    FTimerHandle ActionAnimationTimerHandle;
    FTimerHandle SitTransitionTimerHandle;
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
