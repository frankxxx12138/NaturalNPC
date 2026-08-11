#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "OpenAINPCAutonomy.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FOpenAINPCAutonomyDecisionParsingTest,
    "NaturalNPC.OpenAINPC.Autonomy.CandidateDecisionParsing",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FOpenAINPCAutonomyDecisionParsingTest::RunTest(
    const FString& Parameters
)
{
    FOpenAINPCAutonomousDecision Decision;
    FString Failure;

    TestTrue(
        TEXT("A JSON candidate is accepted"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "```json\n{\"should_respond\":true,"
                "\"type\":\"RelevantContribution\","
                "\"text\":\"I think Jack is overlooking the weather.\","
                "\"permission_request\":\"May I add something important?\","
                "\"priority\":0.82,\"relevance\":0.9,\"urgency\":0.2,"
                "\"topic_affinity\":0.8,\"conversation_impact\":0.7,"
                "\"confidence\":0.9,\"has_unique_information\":true,"
                "\"topic_transition_ready\":false,"
                "\"emergency_event_id\":\"\","
                "\"reason\":\"Useful correction\"}\n```"
            ),
            240,
            Decision,
            Failure
        )
    );
    TestTrue(TEXT("The candidate wants to respond"), Decision.bShouldRespond);
    TestEqual(
        TEXT("The response type is retained"),
        Decision.ResponseType,
        EConversationResponseType::RelevantContribution
    );
    TestEqual(
        TEXT("The candidate text is retained"),
        Decision.ResponseText,
        FString(TEXT("I think Jack is overlooking the weather."))
    );
    TestTrue(
        TEXT("Priority is parsed"),
        FMath::IsNearlyEqual(Decision.PriorityScore, 0.82f)
    );
    FOpenAINPCAutonomyValidationContext ValidationContext;
    TestTrue(
        TEXT("Relevant information difference passes category validation"),
        OpenAINPCAutonomy::ValidateDecision(
            Decision,
            ValidationContext,
            Failure
        )
    );

    TestTrue(
        TEXT("A silence decision is accepted"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "{\"should_respond\":false,\"type\":\"Silence\","
                "\"text\":\"\",\"priority\":0.0,"
                "\"reason\":\"Nothing useful to add\"}"
            ),
            240,
            Decision,
            Failure
        )
    );
    TestFalse(
        TEXT("Silence does not request speech"),
        Decision.bShouldRespond
    );
    TestEqual(
        TEXT("Silence remains silence"),
        Decision.ResponseType,
        EConversationResponseType::Silence
    );

    TestFalse(
        TEXT("Unexpected response types are rejected"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "{\"should_respond\":true,\"type\":\"Monologue\","
                "\"text\":\"Let me take over.\",\"priority\":1.0}"
            ),
            240,
            Decision,
            Failure
        )
    );
    TestEqual(
        TEXT("The validation failure is explicit"),
        Failure,
        FString(TEXT("UnexpectedResponseType"))
    );

    TestFalse(
        TEXT("Overlong autonomous speech is rejected"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "{\"should_respond\":true,\"type\":\"SideComment\","
                "\"text\":\"This response is deliberately too long for "
                "the configured test limit.\",\"priority\":0.5}"
            ),
            40,
            Decision,
            Failure
        )
    );
    TestEqual(
        TEXT("The length failure is explicit"),
        Failure,
        FString(TEXT("ResponseTooLong"))
    );

    TestTrue(
        TEXT("A verified emergency candidate parses"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "{\"should_respond\":true,\"type\":\"Emergency\","
                "\"text\":\"Fire! Everyone get outside now!\","
                "\"permission_request\":\"\",\"priority\":1.0,"
                "\"relevance\":1.0,\"urgency\":1.0,"
                "\"topic_affinity\":1.0,\"conversation_impact\":1.0,"
                "\"confidence\":1.0,\"has_unique_information\":true,"
                "\"topic_transition_ready\":true,"
                "\"emergency_event_id\":\"Fire_01\"}"
            ),
            240,
            Decision,
            Failure
        )
    );
    TestFalse(
        TEXT("An emergency cannot be accepted without a world event"),
        OpenAINPCAutonomy::ValidateDecision(
            Decision,
            ValidationContext,
            Failure
        )
    );
    TestEqual(
        TEXT("Unverified emergency failure is explicit"),
        Failure,
        FString(TEXT("UnverifiedEmergency"))
    );
    ValidationContext.bVerifiedEmergencyActive = true;
    ValidationContext.VerifiedEmergencyEventID = TEXT("Fire_01");
    TestTrue(
        TEXT("Matching verified emergency passes"),
        OpenAINPCAutonomy::ValidateDecision(
            Decision,
            ValidationContext,
            Failure
        )
    );

    TestTrue(
        TEXT("A short low-impact side comment parses"),
        OpenAINPCAutonomy::ParseDecision(
            TEXT(
                "{\"should_respond\":true,\"type\":\"SideComment\","
                "\"text\":\"That is amazing!\",\"permission_request\":\"\","
                "\"priority\":0.4,\"relevance\":0.7,\"urgency\":0.0,"
                "\"topic_affinity\":0.8,\"conversation_impact\":0.1,"
                "\"confidence\":0.9,\"has_unique_information\":false,"
                "\"topic_transition_ready\":false,"
                "\"emergency_event_id\":\"\"}"
            ),
            240,
            Decision,
            Failure
        )
    );
    TestTrue(
        TEXT("Short low-impact side comment passes"),
        OpenAINPCAutonomy::ValidateDecision(
            Decision,
            ValidationContext,
            Failure
        )
    );

    return true;
}

#endif
