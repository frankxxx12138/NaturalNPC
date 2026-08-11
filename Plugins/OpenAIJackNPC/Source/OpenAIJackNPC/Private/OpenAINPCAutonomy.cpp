#include "OpenAINPCAutonomy.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
    FString ExtractJsonObject(const FString& Payload)
    {
        FString Trimmed = Payload.TrimStartAndEnd();
        const int32 FirstBrace = Trimmed.Find(TEXT("{"));
        const int32 LastBrace = Trimmed.Find(
            TEXT("}"),
            ESearchCase::CaseSensitive,
            ESearchDir::FromEnd
        );
        if (FirstBrace == INDEX_NONE ||
            LastBrace == INDEX_NONE ||
            LastBrace < FirstBrace)
        {
            return FString();
        }

        return Trimmed.Mid(FirstBrace, LastBrace - FirstBrace + 1);
    }

    bool ParseResponseType(
        const FString& TypeText,
        EConversationResponseType& OutType
    )
    {
        if (TypeText.Equals(
                TEXT("RelevantContribution"),
                ESearchCase::IgnoreCase))
        {
            OutType = EConversationResponseType::RelevantContribution;
            return true;
        }
        if (TypeText.Equals(
                TEXT("SideComment"),
                ESearchCase::IgnoreCase))
        {
            OutType = EConversationResponseType::SideComment;
            return true;
        }
        if (TypeText.Equals(
                TEXT("AssociativeTopic"),
                ESearchCase::IgnoreCase))
        {
            OutType = EConversationResponseType::AssociativeTopic;
            return true;
        }
        if (TypeText.Equals(
                TEXT("Emergency"),
                ESearchCase::IgnoreCase))
        {
            OutType = EConversationResponseType::Emergency;
            return true;
        }
        if (TypeText.Equals(
                TEXT("Silence"),
                ESearchCase::IgnoreCase))
        {
            OutType = EConversationResponseType::Silence;
            return true;
        }
        return false;
    }

    bool ParseReactionType(
        const FString& TypeText,
        ENonVerbalReactionType& OutType
    )
    {
        const UEnum* Enum = StaticEnum<ENonVerbalReactionType>();
        if (!IsValid(Enum))
        {
            return false;
        }

        for (int32 Index = 0; Index < Enum->NumEnums(); ++Index)
        {
            if (Enum->GetNameStringByIndex(Index).Equals(
                    TypeText,
                    ESearchCase::IgnoreCase))
            {
                OutType = static_cast<ENonVerbalReactionType>(
                    Enum->GetValueByIndex(Index)
                );
                return true;
            }
        }
        return false;
    }

    bool ReadUnitScore(
        const TSharedPtr<FJsonObject>& Root,
        const TCHAR* FieldName,
        float& OutValue,
        FString& OutFailureReason
    )
    {
        double Value = 0.0;
        if (!Root->TryGetNumberField(FieldName, Value))
        {
            OutFailureReason = FString::Printf(
                TEXT("Missing%s"),
                FieldName
            );
            return false;
        }
        if (Value < 0.0 || Value > 1.0)
        {
            OutFailureReason = FString::Printf(
                TEXT("OutOfRange%s"),
                FieldName
            );
            return false;
        }

        OutValue = static_cast<float>(Value);
        return true;
    }

    int32 CountSentenceTerminators(const FString& Text)
    {
        int32 Count = 0;
        for (const TCHAR Character : Text)
        {
            if (Character == TEXT('.') ||
                Character == TEXT('!') ||
                Character == TEXT('?'))
            {
                ++Count;
            }
        }
        return Count;
    }

    int32 CountWords(const FString& Text)
    {
        TArray<FString> Words;
        Text.ParseIntoArrayWS(Words);
        return Words.Num();
    }
}

bool OpenAINPCAutonomy::ParseDecision(
    const FString& Payload,
    int32 MaximumResponseCharacters,
    FOpenAINPCAutonomousDecision& OutDecision,
    FString& OutFailureReason
)
{
    OutDecision = FOpenAINPCAutonomousDecision();
    OutFailureReason.Reset();

    const FString Json = ExtractJsonObject(Payload);
    if (Json.IsEmpty())
    {
        OutFailureReason = TEXT("MissingJsonObject");
        return false;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(Json);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
    {
        OutFailureReason = TEXT("MalformedJson");
        return false;
    }

    bool bShouldRespond = false;
    if (!Root->TryGetBoolField(TEXT("should_respond"), bShouldRespond))
    {
        OutFailureReason = TEXT("MissingShouldRespond");
        return false;
    }

    FString TypeText;
    Root->TryGetStringField(TEXT("type"), TypeText);
    EConversationResponseType ResponseType =
        EConversationResponseType::Silence;
    if (!TypeText.IsEmpty() &&
        !ParseResponseType(TypeText, ResponseType))
    {
        OutFailureReason = TEXT("UnexpectedResponseType");
        return false;
    }

    FString ResponseText;
    Root->TryGetStringField(TEXT("text"), ResponseText);
    ResponseText = ResponseText.TrimStartAndEnd();

    if (!bShouldRespond || ResponseType == EConversationResponseType::Silence)
    {
        OutDecision.bShouldRespond = false;
        OutDecision.ResponseType = EConversationResponseType::Silence;
        FString ReactionText;
        if (Root->TryGetStringField(
                TEXT("non_verbal_reaction"),
                ReactionText) &&
            !ReactionText.IsEmpty() &&
            !ParseReactionType(
                ReactionText,
                OutDecision.NonVerbalReaction))
        {
            OutFailureReason = TEXT("UnexpectedNonVerbalReaction");
            return false;
        }
        Root->TryGetStringField(TEXT("reason"), OutDecision.Reason);
        return true;
    }

    if (ResponseType == EConversationResponseType::Invalid)
    {
        OutFailureReason = TEXT("InvalidResponseType");
        return false;
    }
    if (ResponseText.IsEmpty())
    {
        OutFailureReason = TEXT("EmptyResponseText");
        return false;
    }

    const int32 SafeMaximum = FMath::Clamp(
        MaximumResponseCharacters,
        40,
        500
    );
    if (ResponseText.Len() > SafeMaximum)
    {
        OutFailureReason = TEXT("ResponseTooLong");
        return false;
    }

    if (!ReadUnitScore(
            Root,
            TEXT("priority"),
            OutDecision.PriorityScore,
            OutFailureReason) ||
        !ReadUnitScore(
            Root,
            TEXT("relevance"),
            OutDecision.RelevanceScore,
            OutFailureReason) ||
        !ReadUnitScore(
            Root,
            TEXT("urgency"),
            OutDecision.UrgencyScore,
            OutFailureReason) ||
        !ReadUnitScore(
            Root,
            TEXT("topic_affinity"),
            OutDecision.TopicAffinityScore,
            OutFailureReason) ||
        !ReadUnitScore(
            Root,
            TEXT("conversation_impact"),
            OutDecision.ConversationImpactScore,
            OutFailureReason) ||
        !ReadUnitScore(
            Root,
            TEXT("confidence"),
            OutDecision.ConfidenceScore,
            OutFailureReason))
    {
        return false;
    }

    if (!Root->TryGetBoolField(
            TEXT("has_unique_information"),
            OutDecision.bHasUniqueInformation) ||
        !Root->TryGetBoolField(
            TEXT("topic_transition_ready"),
            OutDecision.bTopicTransitionReady))
    {
        OutFailureReason = TEXT("MissingCategoryEvidence");
        return false;
    }

    FString EmergencyEventID;
    Root->TryGetStringField(
        TEXT("emergency_event_id"),
        EmergencyEventID
    );
    OutDecision.EmergencyEventID =
        FName(*EmergencyEventID.TrimStartAndEnd());
    Root->TryGetStringField(
        TEXT("permission_request"),
        OutDecision.PermissionRequestText
    );
    OutDecision.PermissionRequestText =
        OutDecision.PermissionRequestText.TrimStartAndEnd();

    OutDecision.bShouldRespond = true;
    OutDecision.ResponseType = ResponseType;
    OutDecision.ResponseText = ResponseText;
    Root->TryGetStringField(TEXT("reason"), OutDecision.Reason);
    return true;
}

bool OpenAINPCAutonomy::ValidateDecision(
    const FOpenAINPCAutonomousDecision& Decision,
    const FOpenAINPCAutonomyValidationContext& Context,
    FString& OutFailureReason
)
{
    OutFailureReason.Reset();
    if (!Decision.bShouldRespond ||
        Decision.ResponseType == EConversationResponseType::Silence)
    {
        return true;
    }

    switch (Decision.ResponseType)
    {
    case EConversationResponseType::Emergency:
        if (!Context.bVerifiedEmergencyActive ||
            Context.VerifiedEmergencyEventID.IsNone() ||
            Decision.EmergencyEventID !=
                Context.VerifiedEmergencyEventID ||
            Decision.UrgencyScore < 0.8f)
        {
            OutFailureReason = TEXT("UnverifiedEmergency");
            return false;
        }
        return true;

    case EConversationResponseType::RelevantContribution:
        if (!Decision.bHasUniqueInformation ||
            Decision.RelevanceScore < 0.6f)
        {
            OutFailureReason = TEXT("MissingUsefulInformationDifference");
            return false;
        }
        if (Decision.PermissionRequestText.IsEmpty() ||
            Decision.PermissionRequestText.Len() > 160)
        {
            OutFailureReason = TEXT("InvalidPermissionRequest");
            return false;
        }
        return true;

    case EConversationResponseType::SideComment:
        if (Decision.ConversationImpactScore > 0.35f ||
            Decision.ResponseText.Len() > 120 ||
            CountSentenceTerminators(Decision.ResponseText) > 1 ||
            (Decision.ResponseText.Contains(TEXT("?")) &&
             CountWords(Decision.ResponseText) > 8))
        {
            OutFailureReason = TEXT("InvalidSideComment");
            return false;
        }
        return true;

    case EConversationResponseType::AssociativeTopic:
        if (Decision.TopicAffinityScore < 0.6f ||
            !Decision.bTopicTransitionReady)
        {
            OutFailureReason = TEXT("WeakOrUntimelyTopicAssociation");
            return false;
        }
        return true;

    default:
        OutFailureReason = TEXT("InvalidResponseType");
        return false;
    }
}
