#include "NPCWorldStateSubsystem.h"

#include "Components/PrimitiveComponent.h"
#include "Dom/JsonObject.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateObjectComponent.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

namespace
{
    FString BoolText(bool bValue)
    {
        return bValue ? TEXT("true") : TEXT("false");
    }

    FString InferCategory(const AActor* Actor)
    {
        if (!Actor)
        {
            return TEXT("object");
        }
        const FString Search = (
            Actor->GetName() + TEXT(" ") + Actor->GetClass()->GetName()
        ).ToLower();
        if (Search.Contains(TEXT("pistol")) || Search.Contains(TEXT("gun")))
        {
            return TEXT("pistol");
        }
        if (Search.Contains(TEXT("ball")))
        {
            return TEXT("ball");
        }
        if (Search.Contains(TEXT("firelog")) || Search.Contains(TEXT("log")))
        {
            return TEXT("log");
        }
        if (Search.Contains(TEXT("cube")))
        {
            return TEXT("cube");
        }
        return TEXT("object");
    }
}

void UNPCWorldStateSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    RefreshAutoDiscovery();
}

FName UNPCWorldStateSubsystem::MakeUniqueObjectId(FName RequestedId) const
{
    if (!Registry.Contains(RequestedId))
    {
        return RequestedId;
    }
    const FString Base = RequestedId.ToString();
    for (int32 Suffix = 2; Suffix < 100000; ++Suffix)
    {
        const FName Candidate(*FString::Printf(TEXT("%s_%d"), *Base, Suffix));
        if (!Registry.Contains(Candidate))
        {
            return Candidate;
        }
    }
    return FName(*FString::Printf(TEXT("%s_%u"), *Base, GetUniqueID()));
}

void UNPCWorldStateSubsystem::RegisterWorldObject(
    UNPCWorldStateObjectComponent* Component
)
{
    if (!IsValid(Component) || !IsValid(Component->GetOwner()))
    {
        return;
    }

    for (const TPair<FName, TWeakObjectPtr<UNPCWorldStateObjectComponent>>& Pair
        : Registry)
    {
        if (Pair.Value.Get() == Component)
        {
            return;
        }
    }

    FName ObjectId = Component->GetResolvedObjectId();
    if (ObjectId.IsNone())
    {
        ObjectId = Component->GetOwner()->GetFName();
    }
    ObjectId = MakeUniqueObjectId(ObjectId);
    Component->SetRuntimeObjectId(ObjectId);
    Registry.Add(ObjectId, Component);
    OnWorldObjectAdded.Broadcast(ObjectId, Component->GetOwner());
}

void UNPCWorldStateSubsystem::UnregisterWorldObject(
    UNPCWorldStateObjectComponent* Component
)
{
    if (!Component)
    {
        return;
    }
    const FName ObjectId = Component->GetResolvedObjectId();
    Registry.Remove(ObjectId);
    OnWorldObjectRemoved.Broadcast(ObjectId, Component->GetOwner());
}

void UNPCWorldStateSubsystem::NotifyWorldObjectChanged(
    UNPCWorldStateObjectComponent* Component
)
{
    if (IsValid(Component))
    {
        OnWorldObjectChanged.Broadcast(
            Component->GetResolvedObjectId(),
            Component->GetOwner()
        );
    }
}

bool UNPCWorldStateSubsystem::ShouldAutoDiscoverActor(const AActor* Actor) const
{
    if (!IsValid(Actor) || Actor->IsA<APawn>())
    {
        return false;
    }
    if (Actor->ActorHasTag(TEXT("NPCWorldObject")))
    {
        return true;
    }

    TArray<UActorComponent*> Components;
    Actor->GetComponents(Components);
    return Components.ContainsByPredicate(
        [](const UActorComponent* Component)
        {
            return Component &&
                Component->GetClass()->GetName().Contains(TEXT("GrabComponent"));
        }
    );
}

bool UNPCWorldStateSubsystem::ShouldAutoAttachAgent(const AActor* Actor) const
{
    if (!IsValid(Actor) ||
        Actor->FindComponentByClass<UNPCWorldStateAgentComponent>())
    {
        return false;
    }
    if (Actor->ActorHasTag(TEXT("NPCWorldStateAgent")))
    {
        return true;
    }

    TArray<UActorComponent*> Components;
    Actor->GetComponents(Components);
    return Components.ContainsByPredicate(
        [](const UActorComponent* Component)
        {
            if (!Component)
            {
                return false;
            }
            const FString ClassName = Component->GetClass()->GetName();
            return ClassName.Contains(TEXT("OpenAIJackComponent")) ||
                ClassName.Contains(TEXT("ConvaiChatbotComponent"));
        }
    );
}

void UNPCWorldStateSubsystem::ConfigureRuntimeComponent(
    AActor* Actor,
    UNPCWorldStateObjectComponent* Component
) const
{
    if (!Actor || !Component)
    {
        return;
    }
#if WITH_EDITOR
    Component->DisplayName = FText::FromString(Actor->GetActorLabel());
#else
    Component->DisplayName = FText::FromString(Actor->GetName());
#endif
    const FString InferredCategory = InferCategory(Actor);
    Component->Category = FName(*InferredCategory);
    Component->Description = FString::Printf(
        TEXT("A %s in the current world."),
        *InferredCategory
    );
    Component->Aliases.AddUnique(InferredCategory);
    if (InferredCategory == TEXT("pistol"))
    {
        Component->Aliases.Append({
            TEXT("gun"),
            TEXT("\u624b\u67aa"),
            TEXT("\u67aa")
        });
    }
    else if (InferredCategory == TEXT("ball"))
    {
        Component->Aliases.Add(TEXT("\u7403"));
    }
}

void UNPCWorldStateSubsystem::RefreshAutoDiscovery()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (ShouldAutoAttachAgent(Actor))
        {
            UNPCWorldStateAgentComponent* Agent =
                NewObject<UNPCWorldStateAgentComponent>(
                    Actor,
                    UNPCWorldStateAgentComponent::StaticClass(),
                    TEXT("NPCWorldStateAgent")
                );
            Actor->AddInstanceComponent(Agent);
            Agent->RegisterComponent();
        }
        if (!ShouldAutoDiscoverActor(Actor) ||
            Actor->FindComponentByClass<UNPCWorldStateObjectComponent>())
        {
            continue;
        }

        UNPCWorldStateObjectComponent* Component =
            NewObject<UNPCWorldStateObjectComponent>(
                Actor,
                UNPCWorldStateObjectComponent::StaticClass(),
                TEXT("NPCWorldStateObject")
            );
        ConfigureRuntimeComponent(Actor, Component);
        Actor->AddInstanceComponent(Component);
        Component->RegisterComponent();
        RegisterWorldObject(Component);
    }
}

UNPCWorldStateObjectComponent* UNPCWorldStateSubsystem::FindWorldObject(
    FName ObjectId
) const
{
    if (const TWeakObjectPtr<UNPCWorldStateObjectComponent>* Found =
        Registry.Find(ObjectId))
    {
        return Found->Get();
    }
    return nullptr;
}

TArray<FNPCWorldObjectState> UNPCWorldStateSubsystem::GetWorldState(
    AActor* Observer,
    float Radius
) const
{
    TArray<FNPCWorldObjectState> Result;
    const FVector ObserverLocation = Observer
        ? Observer->GetActorLocation()
        : FVector::ZeroVector;

    for (const TPair<FName, TWeakObjectPtr<UNPCWorldStateObjectComponent>>& Pair
        : Registry)
    {
        UNPCWorldStateObjectComponent* Component = Pair.Value.Get();
        AActor* Actor = Component ? Component->GetOwner() : nullptr;
        if (!IsValid(Component) || !IsValid(Actor) ||
            !Component->bVisibleToNPCs)
        {
            continue;
        }

        FVector Origin = Actor->GetActorLocation();
        FVector Extent = FVector::ZeroVector;
        Actor->GetActorBounds(false, Origin, Extent);
        const float Distance = Observer
            ? FVector::Distance(ObserverLocation, Origin)
            : 0.0f;
        if (Observer && Radius > 0.0f && Distance > Radius)
        {
            continue;
        }

        FNPCWorldObjectState Snapshot;
        Snapshot.ObjectId = Pair.Key;
        Snapshot.DisplayName = Component->GetResolvedDisplayName();
        Snapshot.Category = Component->Category;
        Snapshot.Description = Component->Description;
        Snapshot.Actor = Actor;
        Snapshot.Location = Origin;
        Snapshot.Distance = Distance;
        Snapshot.State = Component->State;

        const AActor* Holder = Actor->GetAttachParentActor();
        Snapshot.State.Add(TEXT("is_held"), BoolText(Holder != nullptr));
        Snapshot.State.Add(
            TEXT("held_by"),
            Holder ? Holder->GetName() : FString()
        );

        bool bSimulatingPhysics = false;
        TArray<UPrimitiveComponent*> PrimitiveComponents;
        Actor->GetComponents(PrimitiveComponents);
        for (const UPrimitiveComponent* Primitive : PrimitiveComponents)
        {
            bSimulatingPhysics = bSimulatingPhysics ||
                (Primitive && Primitive->IsSimulatingPhysics());
        }
        Snapshot.State.Add(
            TEXT("simulating_physics"),
            BoolText(bSimulatingPhysics)
        );
        Snapshot.AvailableActions = Component->GetAvailableActions(Observer);
        Result.Add(MoveTemp(Snapshot));
    }

    Result.Sort(
        [](const FNPCWorldObjectState& Left, const FNPCWorldObjectState& Right)
        {
            if (!FMath::IsNearlyEqual(Left.Distance, Right.Distance))
            {
                return Left.Distance < Right.Distance;
            }
            return Left.ObjectId.LexicalLess(Right.ObjectId);
        }
    );
    return Result;
}

FString UNPCWorldStateSubsystem::BuildWorldStateJson(
    AActor* Observer,
    float Radius
) const
{
    TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
    TArray<TSharedPtr<FJsonValue>> Objects;
    for (const FNPCWorldObjectState& Snapshot : GetWorldState(Observer, Radius))
    {
        TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
        Object->SetStringField(TEXT("id"), Snapshot.ObjectId.ToString());
        Object->SetStringField(
            TEXT("name"),
            Snapshot.DisplayName.ToString()
        );
        Object->SetStringField(TEXT("category"), Snapshot.Category.ToString());
        Object->SetStringField(TEXT("description"), Snapshot.Description);
        Object->SetNumberField(TEXT("distance_cm"), Snapshot.Distance);

        TSharedRef<FJsonObject> Location = MakeShared<FJsonObject>();
        Location->SetNumberField(TEXT("x"), Snapshot.Location.X);
        Location->SetNumberField(TEXT("y"), Snapshot.Location.Y);
        Location->SetNumberField(TEXT("z"), Snapshot.Location.Z);
        Object->SetObjectField(TEXT("location"), Location);

        TSharedRef<FJsonObject> State = MakeShared<FJsonObject>();
        for (const TPair<FName, FString>& Pair : Snapshot.State)
        {
            State->SetStringField(Pair.Key.ToString(), Pair.Value);
        }
        Object->SetObjectField(TEXT("state"), State);

        TArray<TSharedPtr<FJsonValue>> Actions;
        for (const FNPCWorldActionDefinition& Definition
            : Snapshot.AvailableActions)
        {
            TSharedRef<FJsonObject> Action = MakeShared<FJsonObject>();
            Action->SetStringField(
                TEXT("id"),
                Definition.ActionId.ToString()
            );
            Action->SetStringField(
                TEXT("name"),
                Definition.DisplayName.ToString()
            );
            Action->SetStringField(TEXT("description"), Definition.Description);
            Actions.Add(MakeShared<FJsonValueObject>(Action));
        }
        Object->SetArrayField(TEXT("actions"), Actions);
        Objects.Add(MakeShared<FJsonValueObject>(Object));
    }
    Root->SetArrayField(TEXT("objects"), Objects);

    FString Json;
    const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer =
        TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(
            &Json
        );
    FJsonSerializer::Serialize(Root, Writer);
    return Json;
}

FString UNPCWorldStateSubsystem::BuildWorldStateText(
    AActor* Observer,
    float Radius
) const
{
    FString Text = TEXT("Current world objects:\n");
    const TArray<FNPCWorldObjectState> Snapshots =
        GetWorldState(Observer, Radius);
    if (Snapshots.IsEmpty())
    {
        return Text + TEXT("- none\n");
    }

    for (const FNPCWorldObjectState& Snapshot : Snapshots)
    {
        TArray<FString> ActionIds;
        for (const FNPCWorldActionDefinition& Action : Snapshot.AvailableActions)
        {
            ActionIds.Add(Action.ActionId.ToString());
        }
        Text += FString::Printf(
            TEXT("- %s (id=%s, type=%s, distance=%.0fcm, actions=%s)\n"),
            *Snapshot.DisplayName.ToString(),
            *Snapshot.ObjectId.ToString(),
            *Snapshot.Category.ToString(),
            Snapshot.Distance,
            *FString::Join(ActionIds, TEXT(","))
        );
    }
    return Text;
}
