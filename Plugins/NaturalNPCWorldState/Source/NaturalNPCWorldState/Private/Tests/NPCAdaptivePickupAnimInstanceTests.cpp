#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "NPCAdaptivePickupAnimInstance.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateObjectComponent.h"
#include "NPCWorldStateSubsystem.h"

namespace
{
class FAdaptivePickupTestWorld
{
public:
    FAdaptivePickupTestWorld()
    {
        const FName WorldName = MakeUniqueObjectName(
            nullptr,
            UWorld::StaticClass(),
            TEXT("AdaptivePickupTestWorld"),
            EUniqueObjectNameOptions::GloballyUnique
        );
        FWorldContext& Context = GEngine->CreateNewWorldContext(
            EWorldType::Game
        );
        World = UWorld::CreateWorld(
            EWorldType::Game,
            false,
            WorldName,
            GetTransientPackage()
        );
        if (World)
        {
            World->AddToRoot();
            Context.SetCurrentWorld(World);
            World->InitializeActorsForPlay(FURL());
        }
    }

    ~FAdaptivePickupTestWorld()
    {
        if (!World)
        {
            return;
        }
        GEngine->ShutdownWorldNetDriver(World);
        World->DestroyWorld(true);
        World->SetPhysicsScene(nullptr);
        GEngine->DestroyWorldContext(World);
        World->RemoveFromRoot();
    }

    UWorld* Get() const { return World; }

private:
    TObjectPtr<UWorld> World = nullptr;
};

void EvaluateAtTime(
    USkeletalMeshComponent* Mesh,
    UAnimSingleNodeInstance* Instance,
    const float Time
)
{
    Instance->SetPosition(Time, false);
    Mesh->TickAnimation(0.0f, false);
    Mesh->RefreshBoneTransforms();
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNPCAdaptivePickupHeightTest,
    "NaturalNPC.WorldState.AdaptivePickup.HeightAndFootPlanting",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FNPCAdaptivePickupHeightTest::RunTest(const FString& Parameters)
{
    USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Body/"
            "SKM_Human2_BodyMesh.SKM_Human2_BodyMesh")
    );
    UAnimSequence* PickupAnimation = LoadObject<UAnimSequence>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/"
            "Human2_PickingUp_Small.Human2_PickingUp_Small")
    );
    TestNotNull(TEXT("Human2 body mesh is available"), SkeletalMesh);
    TestNotNull(TEXT("Human2 pickup animation is available"), PickupAnimation);
    if (!SkeletalMesh || !PickupAnimation)
    {
        return false;
    }

    FAdaptivePickupTestWorld TestWorld;
    if (!TestNotNull(TEXT("A test world can be created"), TestWorld.Get()))
    {
        return false;
    }

    AActor* Actor = TestWorld.Get()->SpawnActor<AActor>();
    USkeletalMeshComponent* Mesh = NewObject<USkeletalMeshComponent>(Actor);
    Actor->SetRootComponent(Mesh);
    Actor->AddInstanceComponent(Mesh);
    Mesh->SetSkeletalMeshAsset(SkeletalMesh);
    Mesh->RegisterComponent();

    Mesh->PlayAnimation(PickupAnimation, false);
    UAnimSingleNodeInstance* BaseInstance = Mesh->GetSingleNodeInstance();
    if (!TestNotNull(TEXT("The base sequence instance is created"), BaseInstance))
    {
        return false;
    }

    float ContactNormalizedTime = 0.55f;
    float LowestHandHeight = TNumericLimits<float>::Max();
    for (int32 SampleIndex = 4; SampleIndex <= 38; ++SampleIndex)
    {
        const float SampleNormalizedTime = SampleIndex / 40.0f;
        EvaluateAtTime(
            Mesh,
            BaseInstance,
            PickupAnimation->GetPlayLength() * SampleNormalizedTime
        );
        const float SampleHeight = Mesh->GetBoneLocation(
            TEXT("hand_r"),
            EBoneSpaces::ComponentSpace
        ).Z;
        if (SampleHeight < LowestHandHeight)
        {
            LowestHandHeight = SampleHeight;
            ContactNormalizedTime = SampleNormalizedTime;
        }
    }
    const float ContactTime = PickupAnimation->GetPlayLength() *
        ContactNormalizedTime;
    EvaluateAtTime(Mesh, BaseInstance, ContactTime);
    const FVector BaseHand = Mesh->GetBoneLocation(
        TEXT("hand_r"),
        EBoneSpaces::ComponentSpace
    );
    const FVector BaseFootR = Mesh->GetBoneLocation(
        TEXT("foot_r"),
        EBoneSpaces::ComponentSpace
    );
    const FVector BaseFootL = Mesh->GetBoneLocation(
        TEXT("foot_l"),
        EBoneSpaces::ComponentSpace
    );
    const FVector BaseChest = Mesh->GetBoneLocation(
        TEXT("spine_03"),
        EBoneSpaces::ComponentSpace
    );
    AddInfo(FString::Printf(
        TEXT("Sampled pickup contact=%.3f hand=%s chest=%s feet=(%.1f, %.1f)"),
        ContactNormalizedTime,
        *BaseHand.ToCompactString(),
        *BaseChest.ToCompactString(),
        BaseFootR.Z,
        BaseFootL.Z
    ));

    FVector RaisedTarget = BaseHand;
    RaisedTarget.Z = FMath::Min(BaseHand.Z + 55.0f, BaseChest.Z - 5.0f);
    if (!TestTrue(
            TEXT("The fixture has room for a meaningful raised target"),
            RaisedTarget.Z >= BaseHand.Z + 20.0f))
    {
        return false;
    }

    Mesh->SetAnimInstanceClass(
        UNPCAdaptivePickupAnimInstance::StaticClass()
    );
    UNPCAdaptivePickupAnimInstance* AdaptiveInstance =
        Cast<UNPCAdaptivePickupAnimInstance>(Mesh->GetAnimInstance());
    if (!TestNotNull(
            TEXT("The adaptive pickup instance is created"),
            AdaptiveInstance))
    {
        return false;
    }

    AdaptiveInstance->ConfigurePickup(
        Mesh->GetComponentTransform().TransformPosition(RaisedTarget),
        ContactNormalizedTime,
        false,
        true,
        6.0f,
        0.22f,
        0.30f,
        40.0f,
        1.08f
    );
    AdaptiveInstance->SetAnimationAsset(PickupAnimation, false, 1.0f);
    AdaptiveInstance->SetPlaying(true);
    EvaluateAtTime(Mesh, AdaptiveInstance, ContactTime);

    const FVector AdaptiveHand = Mesh->GetBoneLocation(
        TEXT("hand_r"),
        EBoneSpaces::ComponentSpace
    );
    const FVector AdaptiveFootR = Mesh->GetBoneLocation(
        TEXT("foot_r"),
        EBoneSpaces::ComponentSpace
    );
    const FVector AdaptiveFootL = Mesh->GetBoneLocation(
        TEXT("foot_l"),
        EBoneSpaces::ComponentSpace
    );

    TestTrue(
        TEXT("The hand follows the raised pickup target"),
        FVector::Dist(AdaptiveHand, RaisedTarget) <= 12.0f
    );
    TestTrue(
        TEXT("The right foot remains planted"),
        FVector::Dist(AdaptiveFootR, BaseFootR) <= 3.0f
    );
    TestTrue(
        TEXT("The left foot remains planted"),
        FVector::Dist(AdaptiveFootL, BaseFootL) <= 3.0f
    );
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNPCTablePickupArmOnlyIKTest,
    "NaturalNPC.WorldState.AdaptivePickup.TableArmOnlyPreservesTorso",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FNPCTablePickupArmOnlyIKTest::RunTest(const FString& Parameters)
{
    USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Body/"
            "SKM_Human2_BodyMesh.SKM_Human2_BodyMesh")
    );
    UAnimSequence* TableAnimation = LoadObject<UAnimSequence>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/"
            "Human2_Pickup_Table.Human2_Pickup_Table")
    );
    if (!TestNotNull(TEXT("Human2 body mesh is available"), SkeletalMesh) ||
        !TestNotNull(TEXT("Table pickup animation is available"),
            TableAnimation))
    {
        return false;
    }

    FAdaptivePickupTestWorld TestWorld;
    AActor* Actor = TestWorld.Get()->SpawnActor<AActor>();
    USkeletalMeshComponent* Mesh = NewObject<USkeletalMeshComponent>(Actor);
    Actor->SetRootComponent(Mesh);
    Actor->AddInstanceComponent(Mesh);
    Mesh->SetSkeletalMeshAsset(SkeletalMesh);
    Mesh->RegisterComponent();

    constexpr float ContactNormalizedTime = 0.41f;
    const float ContactTime = TableAnimation->GetPlayLength() *
        ContactNormalizedTime;
    Mesh->PlayAnimation(TableAnimation, false);
    UAnimSingleNodeInstance* BaseInstance = Mesh->GetSingleNodeInstance();
    if (!TestNotNull(TEXT("The base sequence instance is created"),
            BaseInstance))
    {
        return false;
    }
    EvaluateAtTime(Mesh, BaseInstance, ContactTime);

    const FVector BasePelvis = Mesh->GetBoneLocation(
        TEXT("pelvis"), EBoneSpaces::ComponentSpace
    );
    const FVector BaseSpine = Mesh->GetBoneLocation(
        TEXT("spine_03"), EBoneSpaces::ComponentSpace
    );
    const FVector BaseHead = Mesh->GetBoneLocation(
        TEXT("head"), EBoneSpaces::ComponentSpace
    );
    const FVector BaseFootR = Mesh->GetBoneLocation(
        TEXT("foot_r"), EBoneSpaces::ComponentSpace
    );
    const FVector BaseFootL = Mesh->GetBoneLocation(
        TEXT("foot_l"), EBoneSpaces::ComponentSpace
    );
    const FVector BaseHand = Mesh->GetBoneLocation(
        TEXT("hand_r"), EBoneSpaces::ComponentSpace
    );
    const FVector ArmTarget = BaseHand + FVector(0.0f, 18.0f, 8.0f);

    Mesh->SetAnimInstanceClass(
        UNPCAdaptivePickupAnimInstance::StaticClass()
    );
    UNPCAdaptivePickupAnimInstance* AdaptiveInstance =
        Cast<UNPCAdaptivePickupAnimInstance>(Mesh->GetAnimInstance());
    if (!TestNotNull(TEXT("The adaptive instance is created"),
            AdaptiveInstance))
    {
        return false;
    }
    AdaptiveInstance->ConfigurePickup(
        Mesh->GetComponentTransform().TransformPosition(ArmTarget),
        ContactNormalizedTime,
        false,
        false,
        6.0f,
        0.22f,
        0.30f,
        40.0f,
        1.08f
    );
    AdaptiveInstance->SetAnimationAsset(TableAnimation, false, 1.0f);
    AdaptiveInstance->SetPlaying(true);
    EvaluateAtTime(Mesh, AdaptiveInstance, ContactTime);

    TestTrue(
        TEXT("Arm-only IK moves the hand toward the table target"),
        FVector::Dist(
            Mesh->GetBoneLocation(TEXT("hand_r"),
                EBoneSpaces::ComponentSpace),
            ArmTarget
        ) <= 12.0f
    );
    const auto TestBoneUnchanged = [this, Mesh](
        const TCHAR* Label,
        const FName Bone,
        const FVector& Expected
    )
    {
        return TestTrue(
            Label,
            FVector::Dist(
                Mesh->GetBoneLocation(Bone, EBoneSpaces::ComponentSpace),
                Expected
            ) <= 0.5f
        );
    };
    TestBoneUnchanged(TEXT("Arm-only IK preserves the pelvis"),
        TEXT("pelvis"), BasePelvis);
    TestBoneUnchanged(TEXT("Arm-only IK preserves the spine"),
        TEXT("spine_03"), BaseSpine);
    TestBoneUnchanged(TEXT("Arm-only IK preserves the head"),
        TEXT("head"), BaseHead);
    TestBoneUnchanged(TEXT("Arm-only IK preserves the right foot"),
        TEXT("foot_r"), BaseFootR);
    TestBoneUnchanged(TEXT("Arm-only IK preserves the left foot"),
        TEXT("foot_l"), BaseFootL);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNPCCompoundWorldActionSequenceTest,
    "NaturalNPC.WorldState.Actions.CompoundDropPickupSequence",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FNPCCompoundWorldActionSequenceTest::RunTest(
    const FString& Parameters
)
{
    USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Body/"
            "SKM_Human2_BodyMesh.SKM_Human2_BodyMesh")
    );
    if (!TestNotNull(TEXT("Human2 body mesh is available"), SkeletalMesh))
    {
        return false;
    }

    FAdaptivePickupTestWorld TestWorld;
    UWorld* World = TestWorld.Get();
    if (!TestNotNull(TEXT("A test world can be created"), World))
    {
        return false;
    }

    AActor* NPC = World->SpawnActor<AActor>();
    USkeletalMeshComponent* Body = NewObject<USkeletalMeshComponent>(
        NPC,
        TEXT("Body")
    );
    NPC->SetRootComponent(Body);
    NPC->AddInstanceComponent(Body);
    Body->SetSkeletalMeshAsset(SkeletalMesh);
    Body->RegisterComponent();

    UNPCWorldStateAgentComponent* Agent =
        NewObject<UNPCWorldStateAgentComponent>(NPC);
    NPC->AddInstanceComponent(Agent);
    Agent->bAutoRefresh = false;
    Agent->bApproachPickupTargets = false;
    Agent->DefaultPickupAnimation.Reset();
    Agent->GroundPickupAnimation.Reset();
    Agent->TablePickupAnimation.Reset();
    Agent->DefaultDropAnimation.Reset();
    Agent->ItemAnimationProfiles.Reset();
    Agent->RegisterComponent();

    const auto SpawnWorldObject = [World](
        const FName ObjectId,
        const FName Category,
        const FString& Alias
    )
    {
        AActor* ObjectActor = World->SpawnActor<AActor>();
        USceneComponent* Root = NewObject<USceneComponent>(ObjectActor);
        ObjectActor->SetRootComponent(Root);
        ObjectActor->AddInstanceComponent(Root);
        Root->RegisterComponent();

        UNPCWorldStateObjectComponent* ObjectComponent =
            NewObject<UNPCWorldStateObjectComponent>(ObjectActor);
        ObjectActor->AddInstanceComponent(ObjectComponent);
        ObjectComponent->ObjectId = ObjectId;
        ObjectComponent->DisplayName = FText::FromString(Alias);
        ObjectComponent->Category = Category;
        ObjectComponent->Aliases = {Alias};
        ObjectComponent->bAutoInferVRTemplateActions = false;

        FNPCWorldActionDefinition Pickup;
        Pickup.ActionId = TEXT("pickup");
        Pickup.DisplayName = FText::FromString(TEXT("Pick up"));
        Pickup.MaxDistance = 1000.0f;
        FNPCWorldActionDefinition Drop;
        Drop.ActionId = TEXT("drop");
        Drop.DisplayName = FText::FromString(TEXT("Drop"));
        Drop.MaxDistance = 0.0f;
        ObjectComponent->Actions = {Pickup, Drop};
        ObjectComponent->RegisterComponent();
        return ObjectComponent;
    };

    UNPCWorldStateObjectComponent* Pistol = SpawnWorldObject(
        TEXT("Pistol_Test"),
        TEXT("pistol"),
        TEXT("pistol")
    );
    UNPCWorldStateObjectComponent* Ball = SpawnWorldObject(
        TEXT("Ball_Test"),
        TEXT("ball"),
        TEXT("ball")
    );
    UNPCWorldStateSubsystem* Subsystem = World->GetSubsystem<
        UNPCWorldStateSubsystem
    >();
    if (!TestNotNull(TEXT("World-state subsystem is available"), Subsystem))
    {
        return false;
    }
    Subsystem->RegisterWorldObject(Pistol);
    Subsystem->RegisterWorldObject(Ball);
    Agent->RefreshWorldState();

    FNPCWorldActionResult InitialPickup;
    TestTrue(
        TEXT("The NPC can start by holding the pistol"),
        Agent->ExecuteWorldAction(
            Pistol->GetResolvedObjectId(),
            TEXT("pickup"),
            FString(),
            InitialPickup
        )
    );
    TestEqual(
        TEXT("The pistol is initially held"),
        Agent->GetHeldActor(),
        Pistol->GetOwner()
    );

    FString Reply;
    FString RemainingCommand;
    TestTrue(
        TEXT("The compound command is handled locally"),
        Agent->TryExecuteNaturalLanguageActionDetailed(
            TEXT("put down the pistol and pick up the ball then follow me"),
            Reply,
            RemainingCommand
        )
    );
    TestTrue(TEXT("The compound sequence succeeds"),
        Agent->WasLastNaturalLanguageActionSequenceSuccessful());
    TestFalse(TEXT("The sequence is no longer active"),
        Agent->IsNaturalLanguageActionSequenceInProgress());
    TestEqual(
        TEXT("The ball replaces the pistol in the NPC's hand"),
        Agent->GetHeldActor(),
        Ball->GetOwner()
    );
    TestEqual(
        TEXT("The movement clause is preserved for deferred execution"),
        RemainingCommand,
        FString(TEXT("follow me"))
    );
    TestTrue(TEXT("A successful sequence needs no error reply"), Reply.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNPCPickupAnimationVariantTest,
    "NaturalNPC.WorldState.AdaptivePickup.SelectsGroundAndTableAnimations",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter
)

bool FNPCPickupAnimationVariantTest::RunTest(const FString& Parameters)
{
    USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Body/"
            "SKM_Human2_BodyMesh.SKM_Human2_BodyMesh")
    );
    UAnimSequence* GroundAnimation = LoadObject<UAnimSequence>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/"
            "Human2_Pickup_Floor.Human2_Pickup_Floor")
    );
    UAnimSequence* TableAnimation = LoadObject<UAnimSequence>(
        nullptr,
        TEXT("/Game/MetaHumans/Human2/Animations/Actions/"
            "Human2_Pickup_Table.Human2_Pickup_Table")
    );
    if (!TestNotNull(TEXT("Human2 body mesh is available"), SkeletalMesh) ||
        !TestNotNull(TEXT("Ground pickup animation is available"),
            GroundAnimation) ||
        !TestNotNull(TEXT("Table pickup animation is available"),
            TableAnimation))
    {
        return false;
    }

    FAdaptivePickupTestWorld TestWorld;
    UWorld* World = TestWorld.Get();
    if (!TestNotNull(TEXT("A test world can be created"), World))
    {
        return false;
    }

    AActor* NPC = World->SpawnActor<AActor>();
    USkeletalMeshComponent* Body = NewObject<USkeletalMeshComponent>(
        NPC,
        TEXT("Body")
    );
    NPC->SetRootComponent(Body);
    NPC->AddInstanceComponent(Body);
    Body->SetSkeletalMeshAsset(SkeletalMesh);
    Body->RegisterComponent();

    UNPCWorldStateAgentComponent* Agent =
        NewObject<UNPCWorldStateAgentComponent>(NPC);
    NPC->AddInstanceComponent(Agent);
    Agent->bAutoRefresh = false;
    Agent->GroundPickupAnimation = GroundAnimation;
    Agent->TablePickupAnimation = TableAnimation;
    Agent->TablePickupMinimumHeight = 55.0f;
    Agent->RegisterComponent();

    FVector BoundsOrigin;
    FVector BoundsExtent;
    NPC->GetActorBounds(false, BoundsOrigin, BoundsExtent);
    const float GroundZ = BoundsOrigin.Z - BoundsExtent.Z;
    const auto SpawnTarget = [World](const FVector& Location)
    {
        AActor* Target = World->SpawnActor<AActor>();
        USceneComponent* Root = NewObject<USceneComponent>(Target);
        Target->SetRootComponent(Root);
        Target->AddInstanceComponent(Root);
        Root->RegisterComponent();
        Target->SetActorLocation(Location);
        return Target;
    };

    AActor* GroundTarget = SpawnTarget(
        FVector(80.0f, 0.0f, GroundZ + 20.0f)
    );
    AActor* TableTarget = SpawnTarget(
        FVector(80.0f, 0.0f, GroundZ + 90.0f)
    );
    TestEqual(
        TEXT("A low object selects the ground pickup"),
        Agent->GetPickupAnimationForTarget(GroundTarget),
        GroundAnimation
    );
    TestEqual(
        TEXT("A raised object selects the table pickup"),
        Agent->GetPickupAnimationForTarget(TableTarget),
        TableAnimation
    );
    return true;
}

#endif
