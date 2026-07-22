#include "NPCWorldStateObjectComponent.h"

#include "GameFramework/Actor.h"
#include "NPCWorldStateSubsystem.h"

UNPCWorldStateObjectComponent::UNPCWorldStateObjectComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNPCWorldStateObjectComponent::BeginPlay()
{
    Super::BeginPlay();
    if (UWorld* World = GetWorld())
    {
        if (UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            Subsystem->RegisterWorldObject(this);
        }
    }
}

void UNPCWorldStateObjectComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    if (UWorld* World = GetWorld())
    {
        if (UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            Subsystem->UnregisterWorldObject(this);
        }
    }
    Super::EndPlay(EndPlayReason);
}

FName UNPCWorldStateObjectComponent::GetResolvedObjectId() const
{
    if (!RuntimeObjectId.IsNone())
    {
        return RuntimeObjectId;
    }
    if (!ObjectId.IsNone())
    {
        return ObjectId;
    }
    return GetOwner() ? GetOwner()->GetFName() : NAME_None;
}

FText UNPCWorldStateObjectComponent::GetResolvedDisplayName() const
{
    if (!DisplayName.IsEmpty())
    {
        return DisplayName;
    }
    if (const AActor* Owner = GetOwner())
    {
#if WITH_EDITOR
        const FString Label = Owner->GetActorLabel();
        if (!Label.IsEmpty())
        {
            return FText::FromString(Label);
        }
#endif
        return FText::FromString(Owner->GetName());
    }
    return FText::GetEmpty();
}

bool UNPCWorldStateObjectComponent::HasAction(
    const TArray<FNPCWorldActionDefinition>& Source,
    FName ActionId
) const
{
    return Source.ContainsByPredicate(
        [ActionId](const FNPCWorldActionDefinition& Action)
        {
            return Action.ActionId == ActionId;
        }
    );
}

FNPCWorldActionDefinition UNPCWorldStateObjectComponent::MakeInferredAction(
    FName ActionId
) const
{
    FNPCWorldActionDefinition Action;
    Action.ActionId = ActionId;
    if (ActionId == TEXT("inspect"))
    {
        Action.DisplayName = FText::FromString(TEXT("Inspect"));
        Action.Description = TEXT("Inspect the object and its current state.");
        Action.MaxDistance = 0.0f;
        Action.UtteranceHints = {TEXT("inspect"), TEXT("look at"), TEXT("check")};
    }
    else if (ActionId == TEXT("pickup"))
    {
        Action.DisplayName = FText::FromString(TEXT("Pick up"));
        Action.Description = TEXT("Pick up and hold the object.");
        Action.UtteranceHints = {TEXT("pick up"), TEXT("grab"), TEXT("take")};
        Action.EffectTriggerNormalizedTime = 0.62f;
    }
    else if (ActionId == TEXT("drop"))
    {
        Action.DisplayName = FText::FromString(TEXT("Drop"));
        Action.Description = TEXT("Put down the held object.");
        Action.MaxDistance = 0.0f;
        Action.UtteranceHints = {TEXT("drop"), TEXT("put down"), TEXT("release")};
        Action.EffectTriggerNormalizedTime = 0.38f;
        Action.bPlayAnimationInReverse = true;
    }
    else if (ActionId == TEXT("throw"))
    {
        Action.DisplayName = FText::FromString(TEXT("Throw"));
        Action.Description = TEXT("Throw the held object forward.");
        Action.MaxDistance = 0.0f;
        Action.UtteranceHints = {TEXT("throw"), TEXT("toss")};
        Action.EffectTriggerNormalizedTime = 0.7f;
    }
    return Action;
}

TArray<FNPCWorldActionDefinition>
UNPCWorldStateObjectComponent::GetAvailableActions(AActor* Observer) const
{
    TArray<FNPCWorldActionDefinition> Result = Actions;
    if (!HasAction(Result, TEXT("inspect")))
    {
        Result.Add(MakeInferredAction(TEXT("inspect")));
    }

    if (bAutoInferVRTemplateActions && HasVRGrabComponent())
    {
        for (const FName ActionId : {
            FName(TEXT("pickup")),
            FName(TEXT("drop")),
            FName(TEXT("throw"))})
        {
            if (!HasAction(Result, ActionId))
            {
                Result.Add(MakeInferredAction(ActionId));
            }
        }
    }

    const AActor* Owner = GetOwner();
    const AActor* Holder = Owner ? Owner->GetAttachParentActor() : nullptr;
    Result.RemoveAll(
        [Holder, Observer](const FNPCWorldActionDefinition& Action)
        {
            if (Action.ActionId == TEXT("pickup"))
            {
                return Holder != nullptr;
            }
            if (Action.ActionId == TEXT("drop") ||
                Action.ActionId == TEXT("throw"))
            {
                return Holder == nullptr || Holder != Observer;
            }
            return false;
        }
    );
    return Result;
}

void UNPCWorldStateObjectComponent::SetWorldStateValue(
    FName Key,
    const FString& Value
)
{
    if (!Key.IsNone())
    {
        State.Add(Key, Value);
        NotifyStateChanged();
    }
}

void UNPCWorldStateObjectComponent::RemoveWorldStateValue(FName Key)
{
    if (State.Remove(Key) > 0)
    {
        NotifyStateChanged();
    }
}

bool UNPCWorldStateObjectComponent::HasVRGrabComponent() const
{
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return false;
    }

    TArray<UActorComponent*> Components;
    Owner->GetComponents(Components);
    return Components.ContainsByPredicate(
        [](const UActorComponent* Component)
        {
            return Component &&
                Component->GetClass()->GetName().Contains(TEXT("GrabComponent"));
        }
    );
}

void UNPCWorldStateObjectComponent::SetRuntimeObjectId(FName Value)
{
    RuntimeObjectId = Value;
}

void UNPCWorldStateObjectComponent::NotifyStateChanged()
{
    if (UWorld* World = GetWorld())
    {
        if (UNPCWorldStateSubsystem* Subsystem =
            World->GetSubsystem<UNPCWorldStateSubsystem>())
        {
            Subsystem->NotifyWorldObjectChanged(this);
        }
    }
}
