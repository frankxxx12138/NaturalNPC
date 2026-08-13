#if WITH_DEV_AUTOMATION_TESTS

#include "OpenAIJackComponent.h"
#include "OpenAINPCConversationSubsystem.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAINPCNearbyConversationMemoryTest,
    "NaturalNPC.OpenAINPC.Conversation.NearbyMemory",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAINPCNearbyConversationSynchronizationTest,
    "NaturalNPC.OpenAINPC.Conversation.NearbySynchronization",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FOpenAINPCNearbyConversationMemoryTest::RunTest(
    const FString& Parameters
)
{
    UOpenAIJackComponent* Oliver = NewObject<UOpenAIJackComponent>();
    Oliver->NPCID = TEXT("Oliver");
    Oliver->bEnableSessionMemoryFile = false;
    Oliver->AutonomousListeningRadius = 1500.0f;
    Oliver->NearbyConversationContextRadius = 500.0f;

    TestEqual(
        TEXT("Nearby context limits autonomous listening to five metres"),
        Oliver->GetEffectiveConversationListeningRadius(),
        500.0f
    );
    TestTrue(
        TEXT("A listener on the five metre boundary hears the exchange"),
        UOpenAINPCConversationSubsystem::
            IsWithinConversationContextRadius(
                FVector::ZeroVector,
                FVector(500.0f, 0.0f, 500.0f),
                500.0f
            )
    );
    TestFalse(
        TEXT("A listener outside five metres does not hear the exchange"),
        UOpenAINPCConversationSubsystem::
            IsWithinConversationContextRadius(
                FVector::ZeroVector,
                FVector(500.1f, 0.0f, 0.0f),
                500.0f
            )
    );

    Oliver->RememberConversationExchange(
        TEXT("Jack"),
        TEXT("What do you recommend tonight?"),
        TEXT("Try an Old Fashioned.")
    );
    const FString HeardContext =
        Oliver->BuildSessionConversationContext(true);
    TestTrue(
        TEXT("The listener remembers who the player addressed"),
        HeardContext.Contains(
            TEXT("Player to Jack: What do you recommend tonight?")
        )
    );
    TestTrue(
        TEXT("The listener preserves the other NPC's speaker identity"),
        HeardContext.Contains(TEXT("Jack: Try an Old Fashioned."))
    );

    Oliver->RememberConversationExchange(
        TEXT("Oliver"),
        TEXT("How are you?"),
        TEXT("Doing well, thanks.")
    );
    const FString NearbyOnlyContext =
        Oliver->BuildSessionConversationContext(false);
    TestFalse(
        TEXT("HTTP nearby context does not duplicate Oliver's own turn"),
        NearbyOnlyContext.Contains(TEXT("Doing well, thanks."))
    );
    TestTrue(
        TEXT("HTTP nearby context retains Jack's overheard turn"),
        NearbyOnlyContext.Contains(TEXT("Try an Old Fashioned."))
    );

    Oliver->RememberConversationExchange(
        TEXT("Jack"),
        TEXT("What about food?"),
        TEXT("The stew is reliable."),
        TEXT("Oliver"),
        TEXT("I agree with Jack.")
    );
    const FString GroupContext =
        Oliver->BuildSessionConversationContext(true);
    TestTrue(
        TEXT("A secondary contribution is retained in group context"),
        GroupContext.Contains(TEXT("Oliver: I agree with Jack."))
    );

    return true;
}

bool FOpenAINPCNearbyConversationSynchronizationTest::RunTest(
    const FString& Parameters
)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("Transient test world is created"), World))
    {
        return false;
    }

    AActor* JackActor = World->SpawnActor<AActor>();
    AActor* OliverActor = World->SpawnActor<AActor>();
    USceneComponent* JackRoot = NewObject<USceneComponent>(JackActor);
    USceneComponent* OliverRoot = NewObject<USceneComponent>(OliverActor);
    JackActor->AddInstanceComponent(JackRoot);
    OliverActor->AddInstanceComponent(OliverRoot);
    JackActor->SetRootComponent(JackRoot);
    OliverActor->SetRootComponent(OliverRoot);
    JackRoot->RegisterComponent();
    OliverRoot->RegisterComponent();
    JackActor->SetActorLocation(FVector::ZeroVector);
    OliverActor->SetActorLocation(FVector(400.0f, 0.0f, 0.0f));

    UOpenAIJackComponent* Jack =
        NewObject<UOpenAIJackComponent>(JackActor);
    UOpenAIJackComponent* Oliver =
        NewObject<UOpenAIJackComponent>(OliverActor);
    JackActor->AddInstanceComponent(Jack);
    OliverActor->AddInstanceComponent(Oliver);
    Jack->NPCID = TEXT("Jack");
    Oliver->NPCID = TEXT("Oliver");
    Jack->bEnableSessionMemoryFile = false;
    Oliver->bEnableSessionMemoryFile = false;
    Jack->NearbyConversationContextRadius = 500.0f;
    Oliver->NearbyConversationContextRadius = 500.0f;

    UOpenAINPCConversationSubsystem* Conversation =
        World->GetSubsystem<UOpenAINPCConversationSubsystem>();
    if (!TestNotNull(TEXT("Conversation subsystem is created"), Conversation))
    {
        World->DestroyWorld(false);
        return false;
    }

    Conversation->RegisterNPC(Jack);
    Conversation->RegisterNPC(Oliver);
    Conversation->NotifyPlayerTextSubmitted(
        Jack,
        TEXT("What do you recommend?")
    );
    Conversation->NotifyNPCReplyReady(
        Jack,
        TEXT("Try an Old Fashioned.")
    );
    TestTrue(
        TEXT("Oliver receives Jack's exchange at four metres"),
        Oliver->BuildSessionConversationContext(true).Contains(
            TEXT("Jack: Try an Old Fashioned.")
        )
    );

    OliverActor->SetActorLocation(FVector(600.0f, 0.0f, 0.0f));
    Conversation->NotifyPlayerTextSubmitted(
        Jack,
        TEXT("What food is good?")
    );
    Conversation->NotifyNPCReplyReady(
        Jack,
        TEXT("Try the stew.")
    );
    TestFalse(
        TEXT("Oliver does not receive Jack's exchange at six metres"),
        Oliver->BuildSessionConversationContext(true).Contains(
            TEXT("Jack: Try the stew.")
        )
    );

    Conversation->UnregisterNPC(Oliver);
    Conversation->UnregisterNPC(Jack);
    World->DestroyWorld(false);
    return true;
}

#endif
