#include "ConversationListenerComponent.h"

#include "GameFramework/Actor.h"

UConversationListenerComponent::UConversationListenerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FName UConversationListenerComponent::GetResolvedNPCID() const
{
    if (!NPCID.IsNone())
    {
        return NPCID;
    }

    const AActor* Owner = GetOwner();
    return IsValid(Owner) ? Owner->GetFName() : NAME_None;
}

FText UConversationListenerComponent::GetResolvedDisplayName() const
{
    if (!DisplayName.IsEmpty())
    {
        return DisplayName;
    }

    const AActor* Owner = GetOwner();
    return IsValid(Owner)
        ? FText::FromString(Owner->GetActorNameOrLabel())
        : FText::GetEmpty();
}

bool UConversationListenerComponent::CanHearActor(
    const AActor* SoundSource
) const
{
    const AActor* Owner = GetOwner();
    if (!bListenerEnabled ||
        !IsValid(Owner) ||
        !IsValid(SoundSource) ||
        Owner == SoundSource ||
        ListeningRadius < 0.0f)
    {
        return false;
    }

    return FVector::DistSquared(
            Owner->GetActorLocation(),
            SoundSource->GetActorLocation()
        ) <= FMath::Square(ListeningRadius);
}

float UConversationListenerComponent::GetDistanceToActor(
    const AActor* OtherActor
) const
{
    const AActor* Owner = GetOwner();
    if (!IsValid(Owner) || !IsValid(OtherActor))
    {
        return -1.0f;
    }

    return FVector::Distance(
        Owner->GetActorLocation(),
        OtherActor->GetActorLocation()
    );
}
