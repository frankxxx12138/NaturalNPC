#include "OpenAINPCRouter.h"

#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "OpenAIJackComponent.h"

namespace
{
    bool IsNameCharacter(TCHAR Character)
    {
        return FChar::IsAlnum(Character) || Character == TEXT('_');
    }

    bool HasNameAt(const FString& Text, const FString& Name, int32 Index)
    {
        if (Name.IsEmpty() || Index < 0 || Index + Name.Len() > Text.Len() ||
            !Text.Mid(Index, Name.Len()).Equals(Name, ESearchCase::IgnoreCase))
        {
            return false;
        }

        const bool bLeftBoundary = Index == 0 ||
            !IsNameCharacter(Text[Index - 1]);
        const int32 EndIndex = Index + Name.Len();
        const bool bRightBoundary = EndIndex == Text.Len() ||
            !IsNameCharacter(Text[EndIndex]);
        return bLeftBoundary && bRightBoundary;
    }

    FString TrimLeadingAddressPunctuation(const FString& Text)
    {
        int32 Index = 0;
        while (Index < Text.Len())
        {
            const TCHAR Character = Text[Index];
            if (!FChar::IsWhitespace(Character) &&
                Character != TEXT(',') && Character != TEXT(':') &&
                Character != TEXT(';') && Character != TEXT('!') &&
                Character != TEXT('?') && Character != TEXT('\uFF0C') &&
                Character != TEXT('\uFF1A'))
            {
                break;
            }
            ++Index;
        }
        return Text.Mid(Index);
    }

    bool StartsWithAny(
        const FString& Text,
        std::initializer_list<const TCHAR*> Prefixes
    )
    {
        for (const TCHAR* Prefix : Prefixes)
        {
            if (Text.StartsWith(Prefix, ESearchCase::IgnoreCase))
            {
                return true;
            }
        }
        return false;
    }
}

bool OpenAINPCRouter::ContainsNameMention(
    const FString& PlayerText,
    const FString& CandidateName
)
{
    const FString Text = PlayerText.TrimStartAndEnd();
    const FString Name = CandidateName.TrimStartAndEnd();
    if (Text.IsEmpty() || Name.IsEmpty())
    {
        return false;
    }

    int32 SearchIndex = 0;
    while (SearchIndex < Text.Len())
    {
        const int32 MatchIndex = Text.Find(
            Name,
            ESearchCase::IgnoreCase,
            ESearchDir::FromStart,
            SearchIndex
        );
        if (MatchIndex == INDEX_NONE)
        {
            return false;
        }
        if (HasNameAt(Text, Name, MatchIndex))
        {
            return true;
        }
        SearchIndex = MatchIndex + FMath::Max(1, Name.Len());
    }
    return false;
}

bool OpenAINPCRouter::IsDirectAddress(
    const FString& PlayerText,
    const FString& CandidateName
)
{
    FString Text = PlayerText.TrimStartAndEnd();
    const FString Name = CandidateName.TrimStartAndEnd();
    if (Text.IsEmpty() || Name.IsEmpty())
    {
        return false;
    }

    bool bGreetingPrefix = false;
    for (const TCHAR* Greeting : {
        TEXT("hey "), TEXT("hi "), TEXT("hello "),
        TEXT("excuse me ")})
    {
        if (Text.StartsWith(Greeting, ESearchCase::IgnoreCase))
        {
            Text = Text.Mid(FCString::Strlen(Greeting)).TrimStart();
            bGreetingPrefix = true;
            break;
        }
    }

    if (HasNameAt(Text, Name, 0))
    {
        FString Remainder = Text.Mid(Name.Len());
        if (Remainder.TrimStartAndEnd().IsEmpty() || bGreetingPrefix)
        {
            return true;
        }

        const TCHAR FirstCharacter = Remainder[0];
        if (FirstCharacter == TEXT(',') || FirstCharacter == TEXT(':') ||
            FirstCharacter == TEXT('!') || FirstCharacter == TEXT('?') ||
            FirstCharacter == TEXT('\uFF0C') ||
            FirstCharacter == TEXT('\uFF1A'))
        {
            return true;
        }

        Remainder = TrimLeadingAddressPunctuation(Remainder);
        if (StartsWithAny(Remainder, {
            TEXT("can "), TEXT("could "), TEXT("would "),
            TEXT("will "), TEXT("please "), TEXT("come "),
            TEXT("go "), TEXT("walk "), TEXT("run "),
            TEXT("turn "), TEXT("pick "), TEXT("grab "),
            TEXT("give "), TEXT("drop "), TEXT("tell me "),
            TEXT("what "), TEXT("where "), TEXT("when "),
            TEXT("why "), TEXT("how "), TEXT("do you "),
            TEXT("are you "), TEXT("have you ")}))
        {
            return true;
        }
    }

    const int32 NameIndex = Text.Find(
        Name,
        ESearchCase::IgnoreCase,
        ESearchDir::FromEnd
    );
    if (!HasNameAt(Text, Name, NameIndex))
    {
        return false;
    }

    const FString AfterName = Text.Mid(NameIndex + Name.Len())
        .TrimStartAndEnd();
    if (!AfterName.IsEmpty() && AfterName != TEXT("?") &&
        AfterName != TEXT("!") && AfterName != TEXT(".") &&
        AfterName != TEXT(","))
    {
        return false;
    }

    const FString BeforeName = Text.Left(NameIndex).TrimEnd();
    const bool bSeparatedAsVocative = !BeforeName.IsEmpty() &&
        (BeforeName.EndsWith(TEXT(",")) ||
         BeforeName.EndsWith(TEXT(":")) ||
         BeforeName.EndsWith(TEXT("\uFF0C")) ||
         BeforeName.EndsWith(TEXT("\uFF1A")));
    const bool bSecondPersonCommand = StartsWithAny(BeforeName, {
        TEXT("can you "), TEXT("could you "), TEXT("would you "),
        TEXT("will you "), TEXT("please "), TEXT("come "),
        TEXT("go "), TEXT("walk "), TEXT("run "),
        TEXT("turn "), TEXT("pick "), TEXT("grab "),
        TEXT("give "), TEXT("tell me ")});
    return bSeparatedAsVocative || bSecondPersonCommand;
}

UOpenAIJackComponent* OpenAINPCRouter::FindUniqueNPC(
    UWorld* World,
    FName NPCID,
    int32& OutMatchCount
)
{
    OutMatchCount = 0;
    UOpenAIJackComponent* Match = nullptr;

    if (!IsValid(World) || NPCID.IsNone())
    {
        return nullptr;
    }

    for (TActorIterator<AActor> ActorIterator(World);
         ActorIterator;
         ++ActorIterator)
    {
        TArray<UOpenAIJackComponent*> Components;
        ActorIterator->GetComponents(Components);
        for (UOpenAIJackComponent* Component : Components)
        {
            if (!IsValid(Component) ||
                Component->GetResolvedNPCID() != NPCID)
            {
                continue;
            }

            ++OutMatchCount;
            Match = Component;
        }
    }

    return OutMatchCount == 1 ? Match : nullptr;
}

FString OpenAINPCRouter::DescribeAvailableNPCs(UWorld* World)
{
    if (!IsValid(World))
    {
        return TEXT("None");
    }

    TArray<FString> Descriptions;
    for (TActorIterator<AActor> ActorIterator(World);
         ActorIterator;
         ++ActorIterator)
    {
        TArray<UOpenAIJackComponent*> Components;
        ActorIterator->GetComponents(Components);
        for (const UOpenAIJackComponent* Component : Components)
        {
            if (!IsValid(Component))
            {
                continue;
            }

            Descriptions.Add(FString::Printf(
                TEXT("%s@%s"),
                *Component->GetResolvedNPCID().ToString(),
                *ActorIterator->GetName()
            ));
        }
    }

    Descriptions.Sort();
    return Descriptions.IsEmpty()
        ? TEXT("None")
        : FString::Join(Descriptions, TEXT(","));
}
