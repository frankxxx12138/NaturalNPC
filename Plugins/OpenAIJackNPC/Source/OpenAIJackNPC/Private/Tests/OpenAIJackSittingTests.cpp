#if WITH_DEV_AUTOMATION_TESTS

#include "OpenAIJackComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAIJackSittingLanguageTest,
    "NaturalNPC.OpenAINPC.Actions.SittingLanguage",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FOpenAIJackSittingLanguageTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("Transient test world is created"), World))
    {
        return false;
    }

    AActor* Actor = World->SpawnActor<AActor>();
    UOpenAIJackComponent* Component =
        NewObject<UOpenAIJackComponent>(Actor);
    Actor->AddInstanceComponent(Component);
    Component->bUseActionAnimationOverride = false;

    FString Reply;
    TestTrue(
        TEXT("Sit down is handled locally"),
        Component->TryExecuteMovementCommand(TEXT("sit down"), Reply)
    );
    TestFalse(
        TEXT("NPC does not sit in empty space when no seat exists"),
        Component->IsSitting()
    );
    TestTrue(
        TEXT("The failure reply explains that a real seat is required"),
        Reply.Contains(TEXT("chair")) || Reply.Contains(TEXT("seat"))
    );

    Reply.Reset();
    TestTrue(
        TEXT("Stand up is handled locally"),
        Component->TryExecuteMovementCommand(TEXT("stand up"), Reply)
    );
    TestFalse(TEXT("NPC remains standing"), Component->IsSitting());

    Reply.Reset();
    TestTrue(
        TEXT("Come and sit is recognized before the generic come command"),
        Component->TryExecuteMovementCommand(TEXT("come and sit"), Reply)
    );
    TestTrue(
        TEXT("Missing-player failure belongs to the sit-here route"),
        Reply.Contains(TEXT("locate"))
    );

    Reply.Reset();
    TestTrue(
        TEXT("Sit near me is handled as a local seated movement command"),
        Component->TryExecuteMovementCommand(TEXT("sit near me"), Reply)
    );
    TestTrue(
        TEXT("Sit near me searches relative to the player"),
        Reply.Contains(TEXT("locate"))
    );

    Reply.Reset();
    TestTrue(
        TEXT("Chinese sit-near-me wording uses the same local route"),
        Component->TryExecuteMovementCommand(TEXT("\u5750\u6211\u65c1\u8fb9"), Reply)
    );
    TestTrue(
        TEXT("Chinese sit-near-me searches relative to the player"),
        Reply.Contains(TEXT("locate"))
    );

    Reply.Reset();
    TestFalse(
        TEXT("Sit is not matched inside an unrelated word"),
        Component->TryExecuteMovementCommand(
            TEXT("Tell me about this situation"),
            Reply
        )
    );

    World->DestroyWorld(false);
    return true;
}

#endif
