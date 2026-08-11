#if WITH_DEV_AUTOMATION_TESTS

#include "ConversationGovernanceComponent.h"
#include "ConversationListenerComponent.h"
#include "ConversationResponseQueue.h"
#include "ConversationTurnManager.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

namespace
{
FCandidateNPCResponse MakeCandidate(
    FName NPCID,
    float Priority,
    double CreatedTime
)
{
    FCandidateNPCResponse Response;
    Response.ResponseID = FGuid::NewGuid();
    Response.ConversationID = TEXT("TestConversation");
    Response.NPCID = NPCID;
    Response.ResponseText = FString::Printf(
        TEXT("Candidate from %s"),
        *NPCID.ToString()
    );
    Response.ResponseType =
        EConversationResponseType::RelevantContribution;
    Response.PriorityScore = Priority;
    Response.CreatedTime = CreatedTime;
    return Response;
}

AActor* MakeTransientActorAt(const FVector& Location)
{
    AActor* Actor = NewObject<AActor>();
    USceneComponent* Root = NewObject<USceneComponent>(Actor);
    Actor->AddInstanceComponent(Root);
    Actor->SetRootComponent(Root);
    Root->SetWorldLocation(Location);
    return Actor;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationGovernanceQueueTest,
    "NaturalNPC.ConversationGovernance.Phase1.Queue",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FConversationGovernanceQueueTest::RunTest(const FString& Parameters)
{
    UConversationResponseQueue* Queue =
        NewObject<UConversationResponseQueue>();
    Queue->Initialize(3);

    FCandidateNPCResponse Low = MakeCandidate(TEXT("NPC_A"), 0.2f, 1.0);
    FCandidateNPCResponse High = MakeCandidate(TEXT("NPC_B"), 0.9f, 2.0);
    FCandidateNPCResponse Mid = MakeCandidate(TEXT("NPC_A"), 0.5f, 3.0);

    FString FailureReason;
    TestTrue(TEXT("Low priority candidate enqueues"),
        Queue->Enqueue(Low, 2, FailureReason));
    TestTrue(TEXT("High priority candidate enqueues"),
        Queue->Enqueue(High, 2, FailureReason));
    TestTrue(TEXT("Second NPC_A candidate reaches its allowed limit"),
        Queue->Enqueue(Mid, 2, FailureReason));

    const TArray<FCandidateNPCResponse> Ordered =
        Queue->GetQueuedResponses();
    TestEqual(TEXT("Queue remains bounded"), Ordered.Num(), 3);
    TestEqual(
        TEXT("Highest priority is first"),
        Ordered[0].ResponseID,
        High.ResponseID
    );
    TestEqual(
        TEXT("Middle priority is second"),
        Ordered[1].ResponseID,
        Mid.ResponseID
    );

    FCandidateNPCResponse Duplicate = High;
    TestFalse(
        TEXT("Duplicate response ID is rejected"),
        Queue->Enqueue(Duplicate, 3, FailureReason)
    );
    TestEqual(
        TEXT("Total queue limit reason is not used before ID deduplication"),
        FailureReason,
        FString(TEXT("DuplicateResponseID"))
    );

    TestTrue(
        TEXT("Response can be reprioritized"),
        Queue->ReprioritizeResponse(Low.ResponseID, 1.0f)
    );
    TestEqual(
        TEXT("Reprioritized response moves to front"),
        Queue->GetQueuedResponses()[0].ResponseID,
        Low.ResponseID
    );

    TestEqual(
        TEXT("Remove all responses for an NPC"),
        Queue->RemoveAllFromNPC(TEXT("NPC_A")),
        2
    );
    TestEqual(TEXT("One response remains"), Queue->Num(), 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationGovernanceTurnTest,
    "NaturalNPC.ConversationGovernance.Phase1.TurnOwnership",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FConversationGovernanceTurnTest::RunTest(const FString& Parameters)
{
    UConversationTurnManager* TurnManager =
        NewObject<UConversationTurnManager>();
    AActor* PrimaryNPC = NewObject<AActor>();
    AActor* SecondaryNPC = NewObject<AActor>();

    TurnManager->Initialize(TEXT("TestConversation"), PrimaryNPC);
    TestTrue(
        TEXT("Turn starts idle"),
        TurnManager->IsMainTurnAvailable()
    );
    TestTrue(
        TEXT("Primary NPC acquires main turn"),
        TurnManager->NotifyNPCSpeechStarted(PrimaryNPC)
    );
    TestEqual(
        TEXT("Primary NPC owns the turn"),
        TurnManager->GetCurrentTurnOwner(),
        PrimaryNPC
    );
    TestEqual(
        TEXT("Primary state is reported"),
        TurnManager->GetTurnState(),
        EConversationTurnState::PrimaryNPCSpeaking
    );
    TestFalse(
        TEXT("Secondary NPC cannot overlap main turn"),
        TurnManager->NotifyNPCSpeechStarted(SecondaryNPC)
    );
    TestFalse(
        TEXT("Mismatched completion is ignored"),
        TurnManager->NotifyNPCSpeechEnded(SecondaryNPC)
    );
    TestTrue(
        TEXT("Primary NPC releases main turn"),
        TurnManager->NotifyNPCSpeechEnded(PrimaryNPC)
    );
    TestFalse(
        TEXT("Duplicate completion is ignored"),
        TurnManager->NotifyNPCSpeechEnded(PrimaryNPC)
    );
    TestTrue(
        TEXT("Secondary NPC can acquire the released turn"),
        TurnManager->NotifyNPCSpeechStarted(SecondaryNPC)
    );
    TestEqual(
        TEXT("Secondary state is reported"),
        TurnManager->GetTurnState(),
        EConversationTurnState::SecondaryNPCSpeaking
    );
    TestTrue(
        TEXT("Secondary releases its turn"),
        TurnManager->NotifyNPCSpeechEnded(SecondaryNPC)
    );
    TestTrue(
        TEXT("A side comment can start during an idle micro-pause"),
        TurnManager->BeginSideComment(SecondaryNPC)
    );
    TestNull(
        TEXT("Side comment does not own the main turn"),
        TurnManager->GetCurrentTurnOwner()
    );
    TestEqual(
        TEXT("Side comment uses transition-pause state"),
        TurnManager->GetTurnState(),
        EConversationTurnState::TransitionPause
    );
    TestTrue(
        TEXT("Side comment completes independently"),
        TurnManager->EndSideComment(SecondaryNPC)
    );
    TestTrue(
        TEXT("Primary can begin another turn"),
        TurnManager->NotifyNPCSpeechStarted(PrimaryNPC)
    );
    TestTrue(
        TEXT("Emergency can forcibly replace the current speaker"),
        TurnManager->BeginEmergencyInterrupt(SecondaryNPC)
    );
    TestEqual(
        TEXT("Emergency speaker owns the turn"),
        TurnManager->GetCurrentTurnOwner(),
        SecondaryNPC
    );
    TestEqual(
        TEXT("Emergency state is reported"),
        TurnManager->GetTurnState(),
        EConversationTurnState::EmergencyInterrupt
    );
    TestTrue(
        TEXT("Emergency speaker releases the turn"),
        TurnManager->NotifyNPCSpeechEnded(SecondaryNPC)
    );
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationGovernancePermissionTest,
    "NaturalNPC.ConversationGovernance.Phase2.PermissionFlow",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FConversationGovernancePermissionTest::RunTest(
    const FString& Parameters
)
{
    AActor* Coordinator = MakeTransientActorAt(FVector::ZeroVector);
    AActor* PrimaryNPC = MakeTransientActorAt(FVector::ZeroVector);
    AActor* ListenerActor =
        MakeTransientActorAt(FVector(100.0, 0.0, 0.0));

    UConversationGovernanceComponent* Governance =
        NewObject<UConversationGovernanceComponent>(Coordinator);
    Coordinator->AddInstanceComponent(Governance);
    Governance->bEnableDebugLogging = false;

    UConversationListenerComponent* Listener =
        NewObject<UConversationListenerComponent>(ListenerActor);
    ListenerActor->AddInstanceComponent(Listener);
    Listener->NPCID = TEXT("InformedListener");
    Listener->ListeningRadius = 500.0f;
    Listener->bPermissionRequestsEnabled = true;

    TestTrue(
        TEXT("Permission listener registers"),
        Governance->RegisterListener(Listener)
    );
    TestTrue(
        TEXT("Permission conversation starts"),
        Governance->StartConversation(TEXT("PermissionTest"), PrimaryNPC)
    );

    FCandidateNPCResponse Candidate;
    Candidate.NPCActor = ListenerActor;
    Candidate.ResponseText = TEXT("The lab result changed rapidly.");
    Candidate.ResponseType =
        EConversationResponseType::RelevantContribution;
    Candidate.PermissionRequestText =
        TEXT("I have additional information. May I add it?");
    FGuid ResponseID;
    TestTrue(
        TEXT("Relevant contribution enters the queue"),
        Governance->SubmitCandidateResponseData(Candidate, ResponseID)
    );
    TestTrue(
        TEXT("Permission request enters pending state"),
        Governance->RequestSpeakingPermission(ResponseID)
    );
    TestEqual(
        TEXT("Turn manager reports permission pending"),
        Governance->GetTurnState(),
        EConversationTurnState::PermissionPending
    );

    FCandidateNPCResponse Approved;
    TestTrue(
        TEXT("Permission can be granted"),
        Governance->GrantPermission(ResponseID, Approved)
    );
    TestEqual(
        TEXT("Approved response is returned"),
        Approved.ResponseText,
        Candidate.ResponseText
    );
    TestEqual(
        TEXT("Grant returns the conversation to idle"),
        Governance->GetTurnState(),
        EConversationTurnState::Idle
    );
    Governance->EndConversation();
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationGovernanceManualSubmissionTest,
    "NaturalNPC.ConversationGovernance.Phase1.ManualSubmission",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FConversationGovernanceManualSubmissionTest::RunTest(
    const FString& Parameters
)
{
    AActor* Coordinator = MakeTransientActorAt(FVector::ZeroVector);
    AActor* PrimaryNPC = MakeTransientActorAt(FVector::ZeroVector);
    AActor* ListenerActor = MakeTransientActorAt(FVector(100.0, 0.0, 0.0));

    UConversationGovernanceComponent* Governance =
        NewObject<UConversationGovernanceComponent>(Coordinator);
    Coordinator->AddInstanceComponent(Governance);
    Governance->bEnableDebugLogging = false;

    UConversationListenerComponent* Listener =
        NewObject<UConversationListenerComponent>(ListenerActor);
    ListenerActor->AddInstanceComponent(Listener);
    Listener->NPCID = TEXT("ListenerNPC");
    Listener->ListeningRadius = 500.0f;
    Listener->MaximumQueuedResponses = 1;

    TestTrue(
        TEXT("Listener registers before a conversation starts"),
        Governance->RegisterListener(Listener)
    );
    TestTrue(
        TEXT("Conversation starts with a valid primary NPC"),
        Governance->StartConversation(TEXT("ManualTest"), PrimaryNPC)
    );
    TestTrue(
        TEXT("Nearby registered listener becomes eligible"),
        Governance->IsListenerEligible(Listener)
    );

    FGuid ResponseID;
    TestTrue(
        TEXT("Manual candidate is accepted and queued"),
        Governance->SubmitCandidateResponse(
            ListenerActor,
            TEXT("I can add something relevant."),
            EConversationResponseType::RelevantContribution,
            0.75f,
            ResponseID
        )
    );
    TestTrue(TEXT("Manual candidate receives a valid ID"), ResponseID.IsValid());
    TestEqual(
        TEXT("Manual candidate appears in the queue"),
        Governance->GetQueuedResponses().Num(),
        1
    );

    ListenerActor->GetRootComponent()->SetWorldLocation(
        FVector(1000.0, 0.0, 0.0)
    );
    Governance->RefreshListenerEligibility();
    TestFalse(
        TEXT("Listener outside its radius is no longer eligible"),
        Governance->IsListenerEligible(Listener)
    );
    TestEqual(
        TEXT("Out-of-range listener responses are removed"),
        Governance->GetQueuedResponses().Num(),
        0
    );

    Governance->EndConversation();
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationGovernanceThreeParticipantTest,
    "NaturalNPC.ConversationGovernance.Phase1.ThreeParticipantFlow",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FConversationGovernanceThreeParticipantTest::RunTest(
    const FString& Parameters
)
{
    AActor* Player = MakeTransientActorAt(FVector::ZeroVector);
    AActor* Human1 = MakeTransientActorAt(FVector(0.0, 100.0, 0.0));
    AActor* Human2 = MakeTransientActorAt(FVector(0.0, 200.0, 0.0));

    UConversationGovernanceComponent* Governance =
        NewObject<UConversationGovernanceComponent>(Player);
    Player->AddInstanceComponent(Governance);
    Governance->bEnableDebugLogging = false;

    UConversationListenerComponent* Human2Listener =
        NewObject<UConversationListenerComponent>(Human2);
    Human2->AddInstanceComponent(Human2Listener);
    Human2Listener->NPCID = TEXT("Human2");
    Human2Listener->ListeningRadius = 1000.0f;

    TestTrue(
        TEXT("Human2 registers as the secondary listener"),
        Governance->RegisterListener(Human2Listener)
    );
    TestTrue(
        TEXT("Human1 starts as the primary NPC"),
        Governance->StartConversation(TEXT("ThreeParticipant"), Human1)
    );

    TestTrue(
        TEXT("Player acquires the first turn"),
        Governance->NotifyPlayerSpeechStarted(Player)
    );
    TestFalse(
        TEXT("Human1 cannot talk over the player"),
        Governance->NotifyNPCSpeechStarted(Human1)
    );
    TestTrue(
        TEXT("Player releases the turn"),
        Governance->NotifyPlayerSpeechEnded(Player)
    );
    TestTrue(
        TEXT("Human1 acquires the primary NPC turn"),
        Governance->NotifyNPCSpeechStarted(Human1)
    );

    FGuid Human2ResponseID;
    TestTrue(
        TEXT("Human2 can submit a response while listening"),
        Governance->SubmitCandidateResponse(
            Human2,
            TEXT("I have something useful to add."),
            EConversationResponseType::RelevantContribution,
            0.8f,
            Human2ResponseID
        )
    );
    TestEqual(
        TEXT("Human1 keeps ownership while Human2 waits"),
        Governance->GetCurrentTurnOwner(),
        Human1
    );
    TestEqual(
        TEXT("Human2 candidate waits in the queue"),
        Governance->GetQueuedResponses().Num(),
        1
    );
    TestFalse(
        TEXT("Human2 cannot overlap Human1's main turn"),
        Governance->NotifyNPCSpeechStarted(Human2)
    );

    TestTrue(
        TEXT("Human1 releases its turn"),
        Governance->NotifyNPCSpeechEnded(Human1)
    );

    FCandidateNPCResponse ApprovedHuman2Response;
    TestTrue(
        TEXT("Human2 response becomes the next queued candidate"),
        Governance->GetResponseQueue()->PopNextResponse(
            ApprovedHuman2Response
        )
    );
    TestEqual(
        TEXT("The next response belongs to Human2"),
        ApprovedHuman2Response.NPCID,
        FName(TEXT("Human2"))
    );
    TestTrue(
        TEXT("Human2 acquires the released turn"),
        Governance->NotifyNPCSpeechStarted(Human2)
    );
    TestEqual(
        TEXT("Human2 owns the main turn"),
        Governance->GetCurrentTurnOwner(),
        Human2
    );
    TestTrue(
        TEXT("Human2 releases the main turn"),
        Governance->NotifyNPCSpeechEnded(Human2)
    );

    Governance->EndConversation();
    return true;
}

#endif
