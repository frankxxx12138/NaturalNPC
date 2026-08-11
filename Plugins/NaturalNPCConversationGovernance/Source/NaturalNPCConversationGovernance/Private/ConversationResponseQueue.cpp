#include "ConversationResponseQueue.h"

void UConversationResponseQueue::Initialize(int32 InMaximumTotalEntries)
{
    MaximumTotalEntries = FMath::Max(1, InMaximumTotalEntries);
    Responses.Reset();
}

bool UConversationResponseQueue::Enqueue(
    const FCandidateNPCResponse& Response,
    int32 MaximumEntriesForNPC,
    FString& OutFailureReason
)
{
    OutFailureReason.Reset();

    if (!Response.ResponseID.IsValid())
    {
        OutFailureReason = TEXT("InvalidResponseID");
        return false;
    }

    if (Response.ConversationID.IsNone() || Response.NPCID.IsNone())
    {
        OutFailureReason = TEXT("MissingIdentity");
        return false;
    }

    if (Response.ResponseText.TrimStartAndEnd().IsEmpty())
    {
        OutFailureReason = TEXT("EmptyResponseText");
        return false;
    }

    FCandidateNPCResponse Existing;
    if (GetResponseByID(Response.ResponseID, Existing))
    {
        OutFailureReason = TEXT("DuplicateResponseID");
        return false;
    }

    if (Responses.Num() >= MaximumTotalEntries)
    {
        OutFailureReason = TEXT("TotalQueueLimitReached");
        return false;
    }

    const int32 SafeNPCMaximum = FMath::Max(1, MaximumEntriesForNPC);
    if (CountResponsesFromNPC(Response.NPCID) >= SafeNPCMaximum)
    {
        OutFailureReason = TEXT("NPCQueueLimitReached");
        return false;
    }

    Responses.Add(Response);
    SortByPriority();
    return true;
}

bool UConversationResponseQueue::RemoveByResponseID(FGuid ResponseID)
{
    const int32 Removed = Responses.RemoveAll(
        [ResponseID](const FCandidateNPCResponse& Response)
        {
            return Response.ResponseID == ResponseID;
        }
    );
    return Removed > 0;
}

int32 UConversationResponseQueue::RemoveAllFromNPC(FName NPCID)
{
    return Responses.RemoveAll(
        [NPCID](const FCandidateNPCResponse& Response)
        {
            return Response.NPCID == NPCID;
        }
    );
}

int32 UConversationResponseQueue::RemoveAllFromConversation(
    FName ConversationID
)
{
    return Responses.RemoveAll(
        [ConversationID](const FCandidateNPCResponse& Response)
        {
            return Response.ConversationID == ConversationID;
        }
    );
}

bool UConversationResponseQueue::ReprioritizeResponse(
    FGuid ResponseID,
    float NewPriorityScore
)
{
    for (FCandidateNPCResponse& Response : Responses)
    {
        if (Response.ResponseID == ResponseID)
        {
            Response.PriorityScore = NewPriorityScore;
            SortByPriority();
            return true;
        }
    }

    return false;
}

bool UConversationResponseQueue::PopNextResponse(
    FCandidateNPCResponse& OutResponse
)
{
    if (Responses.IsEmpty())
    {
        return false;
    }

    OutResponse = Responses[0];
    Responses.RemoveAt(0);
    return true;
}

bool UConversationResponseQueue::PeekNextResponse(
    FCandidateNPCResponse& OutResponse
) const
{
    if (Responses.IsEmpty())
    {
        return false;
    }

    OutResponse = Responses[0];
    return true;
}

bool UConversationResponseQueue::GetResponseByID(
    FGuid ResponseID,
    FCandidateNPCResponse& OutResponse
) const
{
    const FCandidateNPCResponse* Found = Responses.FindByPredicate(
        [ResponseID](const FCandidateNPCResponse& Response)
        {
            return Response.ResponseID == ResponseID;
        }
    );

    if (Found == nullptr)
    {
        return false;
    }

    OutResponse = *Found;
    return true;
}

bool UConversationResponseQueue::HasResponseFromNPC(FName NPCID) const
{
    return Responses.ContainsByPredicate(
        [NPCID](const FCandidateNPCResponse& Response)
        {
            return Response.NPCID == NPCID;
        }
    );
}

int32 UConversationResponseQueue::CountResponsesFromNPC(FName NPCID) const
{
    int32 Count = 0;
    for (const FCandidateNPCResponse& Response : Responses)
    {
        if (Response.NPCID == NPCID)
        {
            ++Count;
        }
    }
    return Count;
}

void UConversationResponseQueue::Reset()
{
    Responses.Reset();
}

void UConversationResponseQueue::SortByPriority()
{
    Responses.StableSort(
        [](const FCandidateNPCResponse& Left,
           const FCandidateNPCResponse& Right)
        {
            if (!FMath::IsNearlyEqual(
                    Left.PriorityScore,
                    Right.PriorityScore))
            {
                return Left.PriorityScore > Right.PriorityScore;
            }

            return Left.CreatedTime < Right.CreatedTime;
        }
    );
}
