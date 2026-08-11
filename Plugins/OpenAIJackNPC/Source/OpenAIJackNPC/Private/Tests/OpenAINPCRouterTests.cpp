#if WITH_DEV_AUTOMATION_TESTS

#include "OpenAIJackComponent.h"
#include "OpenAINPCRouter.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAINPCRouterIdentityTest,
    "NaturalNPC.OpenAINPC.Routing.UniqueIdentity",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAINPCRouterDirectAddressTest,
    "NaturalNPC.OpenAINPC.Routing.DirectAddress",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FOpenAINPCRouterDirectAddressTest::RunTest(const FString& Parameters)
{
    TestTrue(
        TEXT("Prefix vocative addresses Jack"),
        OpenAINPCRouter::IsDirectAddress(TEXT("Jack, come here."), TEXT("Jack"))
    );
    TestTrue(
        TEXT("Greeting addresses Oliver"),
        OpenAINPCRouter::IsDirectAddress(
            TEXT("Hey Oliver can you help?"),
            TEXT("Oliver")
        )
    );
    TestTrue(
        TEXT("Suffix vocative addresses Jack"),
        OpenAINPCRouter::IsDirectAddress(
            TEXT("Can you come here, Jack?"),
            TEXT("Jack")
        )
    );
    TestFalse(
        TEXT("A reported state is a mention rather than an address"),
        OpenAINPCRouter::IsDirectAddress(
            TEXT("Jack is holding the pistol."),
            TEXT("Jack")
        )
    );
    TestFalse(
        TEXT("An indirect-object name is not an address"),
        OpenAINPCRouter::IsDirectAddress(
            TEXT("Oliver, tell Jack to follow me."),
            TEXT("Jack")
        )
    );
    TestTrue(
        TEXT("The leading name remains the direct addressee"),
        OpenAINPCRouter::IsDirectAddress(
            TEXT("Oliver, tell Jack to follow me."),
            TEXT("Oliver")
        )
    );
    TestFalse(
        TEXT("Name matching respects word boundaries"),
        OpenAINPCRouter::ContainsNameMention(TEXT("My jacket is here."), TEXT("Jack"))
    );
    return true;
}

bool FOpenAINPCRouterIdentityTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("Transient test world is created"), World))
    {
        return false;
    }

    FActorSpawnParameters OliverSpawnParameters;
    OliverSpawnParameters.Name = TEXT("Human1_Oliver");
    AActor* OliverActor =
        World->SpawnActor<AActor>(OliverSpawnParameters);
    UOpenAIJackComponent* Oliver =
        NewObject<UOpenAIJackComponent>(OliverActor);
    OliverActor->AddInstanceComponent(Oliver);
    Oliver->SubtitleSpeakerName = TEXT("Oliver");

    FActorSpawnParameters JackSpawnParameters;
    JackSpawnParameters.Name = TEXT("Human2_Jack");
    AActor* JackActor =
        World->SpawnActor<AActor>(JackSpawnParameters);
    UOpenAIJackComponent* Jack =
        NewObject<UOpenAIJackComponent>(JackActor);
    JackActor->AddInstanceComponent(Jack);
    Jack->SubtitleSpeakerName = TEXT("Jack");

    TestEqual(
        TEXT("Oliver falls back to the configured speaker name"),
        Oliver->GetResolvedNPCID(),
        FName(TEXT("Oliver"))
    );
    TestEqual(
        TEXT("Jack falls back to the configured speaker name"),
        Jack->GetResolvedNPCID(),
        FName(TEXT("Jack"))
    );

    int32 MatchCount = 0;
    TestEqual(
        TEXT("Jack routing selects Human2 rather than the first component"),
        OpenAINPCRouter::FindUniqueNPC(
            World,
            TEXT("Jack"),
            MatchCount
        ),
        Jack
    );
    TestEqual(TEXT("Exactly one Jack is present"), MatchCount, 1);

    TestEqual(
        TEXT("Oliver routing selects Human1"),
        OpenAINPCRouter::FindUniqueNPC(
            World,
            TEXT("Oliver"),
            MatchCount
        ),
        Oliver
    );
    TestEqual(TEXT("Exactly one Oliver is present"), MatchCount, 1);

    Oliver->NPCID = TEXT("Jack");
    TestNull(
        TEXT("Duplicate Jack IDs fail instead of choosing arbitrarily"),
        OpenAINPCRouter::FindUniqueNPC(
            World,
            TEXT("Jack"),
            MatchCount
        )
    );
    TestEqual(TEXT("Duplicate Jack IDs are reported"), MatchCount, 2);

    World->DestroyWorld(false);
    return true;
}

#endif
