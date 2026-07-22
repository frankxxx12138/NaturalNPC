import unreal


PREFIX = "NATURALNPC_VRTEST_PICKUP_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def unpack(outcome):
    if isinstance(outcome, tuple):
        return bool(outcome[0]), outcome[1] if len(outcome) > 1 else None
    return bool(outcome), None


unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
world = unreal.EditorLevelLibrary.get_editor_world()
subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()

actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
npc = next(
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_class().get_name() == "BP_Human2_C"
)
agent = npc.get_component_by_class(unreal.NPCWorldStateAgentComponent)
jack = npc.get_component_by_class(unreal.OpenAIJackComponent)
if agent is None or jack is None:
    raise RuntimeError("VRTest Human2 is missing Jack or world-state agent")
if not jack.get_editor_property("enable_world_state_natural_language_actions"):
    raise RuntimeError("World-state natural-language actions are disabled")

states = subsystem.get_world_state(npc, 2500.0)
pistols = [state for state in states if str(state.category) == "pistol"]
if len(pistols) != 2:
    raise RuntimeError(f"Expected two pistols in VRTest, found {len(pistols)}")

pickup = unreal.NPCWorldActionDefinition()
pickup.set_editor_property("action_id", unreal.Name("pickup"))
pickup.set_editor_property("max_distance", 320.0)
pickup.set_editor_property("effect_trigger_normalized_time", 0.0)
for pistol in pistols:
    component = pistol.actor.get_component_by_class(
        unreal.NPCWorldStateObjectComponent
    )
    component.set_editor_property("actions", [pickup])

right = npc.get_actor_right_vector()
origin = npc.get_actor_location()
expected = min(
    pistols,
    key=lambda state: (
        (state.location.x - origin.x) * right.x
        + (state.location.y - origin.y) * right.y
        + (state.location.z - origin.z) * right.z
    ),
)

mention_handled, _ = unpack(
    agent.try_execute_natural_language_action(
        "I mean the pistol on your left"
    )
)
if mention_handled:
    raise RuntimeError("A target-only utterance should not execute an action")

pickup_handled, pickup_reply = unpack(
    agent.try_execute_natural_language_action("can you pick it")
)
if not pickup_handled:
    raise RuntimeError("The follow-up 'pick it' command was not handled")
held = agent.get_held_actor()
if held is None:
    raise RuntimeError("Human2 did not hold a pistol after animated pickup")
if held != expected.actor:
    raise RuntimeError(
        f"Expected left pistol {expected.object_id}, held {held.get_name()}"
    )
if not agent.is_world_action_in_progress():
    raise RuntimeError("Pickup did not use the Human2 animation path")

action_animation = unreal.load_asset(
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_MM_Pistol_Equip"
)
action_skeleton = action_animation.get_editor_property("skeleton")
animated_meshes = []
for mesh in npc.get_components_by_class(unreal.SkeletalMeshComponent):
    skeletal_mesh = mesh.get_editor_property("skeletal_mesh")
    skeleton = (
        skeletal_mesh.get_editor_property("skeleton")
        if skeletal_mesh
        else None
    )
    if skeleton != action_skeleton:
        continue
    animation_mode = mesh.get_editor_property("animation_mode")
    animated_meshes.append(f"{mesh.get_name()}={animation_mode}")
    if animation_mode != unreal.AnimationMode.ANIMATION_SINGLE_NODE:
        raise RuntimeError(
            f"Compatible mesh {mesh.get_name()} did not join the action animation"
        )
if len(animated_meshes) < 2:
    raise RuntimeError(
        f"Expected body and clothing meshes, found {animated_meshes}"
    )

drop_success, _ = unpack(agent.drop_held_actor(False))
if not drop_success:
    raise RuntimeError("Could not drop the pistol after verification")

log(
    f"SUCCESS npc={npc.get_name()} object={expected.object_id} "
    f"distance={expected.distance:.1f} meshes={animated_meshes} "
    f"reply={pickup_reply}"
)
log("END")
