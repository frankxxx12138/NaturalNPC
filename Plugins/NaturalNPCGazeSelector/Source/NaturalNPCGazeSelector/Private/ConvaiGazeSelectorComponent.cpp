#include "ConvaiGazeSelectorComponent.h"

#include "ConvaiChatbotComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UConvaiGazeSelectorComponent::UConvaiGazeSelectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UConvaiGazeSelectorComponent::BeginPlay()
{
    Super::BeginPlay();
    TimeUntilNextSelection = 0.0f;
}

void UConvaiGazeSelectorComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeUntilNextSelection -= DeltaTime;
    if (TimeUntilNextSelection > 0.0f)
    {
        return;
    }

    TimeUntilNextSelection = FMath::Max(SelectionInterval, 0.02f);
    UpdateGazeSelection();
}

void UConvaiGazeSelectorComponent::UpdateGazeSelection()
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!IsValid(Owner) || !IsValid(World))
    {
        return;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!IsValid(PlayerController) || PlayerController->GetPawn() != Owner)
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

    UConvaiChatbotComponent* BestChatbot =
        FindBestVisibleChatbot(ViewLocation, ViewRotation.Vector());
    if (!IsValid(BestChatbot) || BestChatbot == SelectedChatbot)
    {
        CandidateChatbot = nullptr;
        CandidateGazeStartTime = 0.0f;
        return;
    }

    const float CurrentTime = World->GetTimeSeconds();
    if (BestChatbot != CandidateChatbot)
    {
        CandidateChatbot = BestChatbot;
        CandidateGazeStartTime = CurrentTime;
        return;
    }

    if (CurrentTime - CandidateGazeStartTime < GazeHoldDuration ||
        CurrentTime - LastSwitchTime < SwitchCooldown)
    {
        return;
    }

    if (bBlockSwitchDuringConversation &&
        IsValid(SelectedChatbot) &&
        SelectedChatbot->IsInConversation())
    {
        return;
    }

    SwitchToChatbot(BestChatbot);
}

UConvaiChatbotComponent* UConvaiGazeSelectorComponent::FindBestVisibleChatbot(
    const FVector& ViewLocation,
    const FVector& ViewDirection
) const
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return nullptr;
    }

    const float MinimumDot =
        FMath::Cos(FMath::DegreesToRadians(SelectionHalfAngleDegrees));
    float BestDot = MinimumDot;
    float BestDistanceSquared = TNumericLimits<float>::Max();
    UConvaiChatbotComponent* BestChatbot = nullptr;

    for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
    {
        AActor* Actor = *ActorIterator;
        if (!IsValid(Actor) || Actor == GetOwner())
        {
            continue;
        }

        TArray<UConvaiChatbotComponent*> Components;
        Actor->GetComponents(Components);
        for (UConvaiChatbotComponent* Component : Components)
        {
            if (!IsValid(Component))
            {
                continue;
            }

            const FVector TargetLocation = Actor->GetActorLocation() + FVector(0, 0, 90);
            const FVector ToTarget = TargetLocation - ViewLocation;
            const float DistanceSquared = ToTarget.SizeSquared();
            if (DistanceSquared > FMath::Square(MaxSelectionDistance) ||
                DistanceSquared < UE_SMALL_NUMBER)
            {
                continue;
            }

            const float Dot = FVector::DotProduct(
                ViewDirection,
                ToTarget.GetSafeNormal()
            );
            if (Dot < BestDot ||
                (FMath::IsNearlyEqual(Dot, BestDot) &&
                 DistanceSquared >= BestDistanceSquared))
            {
                continue;
            }

            if (bRequireLineOfSight && !HasLineOfSight(ViewLocation, Component))
            {
                continue;
            }

            BestDot = Dot;
            BestDistanceSquared = DistanceSquared;
            BestChatbot = Component;
        }
    }

    return BestChatbot;
}

bool UConvaiGazeSelectorComponent::HasLineOfSight(
    const FVector& ViewLocation,
    const UConvaiChatbotComponent* Chatbot
) const
{
    const AActor* TargetActor = Chatbot->GetOwner();
    UWorld* World = GetWorld();
    if (!IsValid(TargetActor) || !IsValid(World))
    {
        return false;
    }

    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(ConvaiGazeLineOfSight),
        false,
        GetOwner()
    );
    FHitResult Hit;
    const FVector TargetLocation = TargetActor->GetActorLocation() + FVector(0, 0, 90);
    if (!World->LineTraceSingleByChannel(
            Hit,
            ViewLocation,
            TargetLocation,
            ECC_Visibility,
            QueryParams))
    {
        return true;
    }

    const AActor* HitActor = Hit.GetActor();
    return HitActor == TargetActor ||
        (IsValid(HitActor) && HitActor->IsAttachedTo(TargetActor));
}

void UConvaiGazeSelectorComponent::SwitchToChatbot(
    UConvaiChatbotComponent* Chatbot
)
{
    if (!IsValid(Chatbot))
    {
        return;
    }

    if (IsValid(SelectedChatbot))
    {
        SelectedChatbot->StopSession();
    }
    else
    {
        UWorld* World = GetWorld();
        if (IsValid(World))
        {
            for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
            {
                TArray<UConvaiChatbotComponent*> Components;
                ActorIterator->GetComponents(Components);
                for (UConvaiChatbotComponent* Component : Components)
                {
                    if (IsValid(Component) && Component != Chatbot)
                    {
                        Component->StopSession();
                    }
                }
            }
        }
    }

    Chatbot->StartSession();
    SelectedChatbot = Chatbot;
    CandidateChatbot = nullptr;
    CandidateGazeStartTime = 0.0f;
    LastSwitchTime = GetWorld()->GetTimeSeconds();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("NATURALNPC_GAZE_SELECTED actor=%s name=%s character_id=%s"),
        *Chatbot->GetOwner()->GetName(),
        *Chatbot->GetConversationalName(),
        *Chatbot->CharacterID
    );
}
