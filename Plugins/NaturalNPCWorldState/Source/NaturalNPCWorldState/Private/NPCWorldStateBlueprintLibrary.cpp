#include "NPCWorldStateBlueprintLibrary.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateSubsystem.h"

UNPCWorldStateSubsystem*
UNPCWorldStateBlueprintLibrary::GetNPCWorldStateSubsystem(
    const UObject* WorldContextObject
)
{
    const UWorld* World = WorldContextObject
        ? WorldContextObject->GetWorld()
        : nullptr;
    return World
        ? const_cast<UWorld*>(World)->GetSubsystem<UNPCWorldStateSubsystem>()
        : nullptr;
}

UNPCWorldStateAgentComponent*
UNPCWorldStateBlueprintLibrary::EnsureNPCWorldStateAgent(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return nullptr;
    }
    if (UNPCWorldStateAgentComponent* Existing =
        Actor->FindComponentByClass<UNPCWorldStateAgentComponent>())
    {
        return Existing;
    }

    UNPCWorldStateAgentComponent* Component =
        NewObject<UNPCWorldStateAgentComponent>(
            Actor,
            UNPCWorldStateAgentComponent::StaticClass(),
            TEXT("NPCWorldStateAgent")
        );
    Actor->AddInstanceComponent(Component);
    Component->RegisterComponent();
    return Component;
}
