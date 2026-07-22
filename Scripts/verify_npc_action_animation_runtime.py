import unreal


PREFIX = "NATURALNPC_ACTION_RUNTIME_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def unpack_action_outcome(outcome):
    if isinstance(outcome, tuple):
        success = bool(outcome[0])
        result = outcome[1] if len(outcome) > 1 else None
        return success, result
    return bool(outcome), None


log("BEGIN")
unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTemplate/VRTemplateMap")
world = unreal.EditorLevelLibrary.get_editor_world()
subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()
states = subsystem.get_world_state(None, 0.0)
ball_state = next(state for state in states if str(state.category) == "ball")
ball_component = ball_state.actor.get_component_by_class(
    unreal.NPCWorldStateObjectComponent
)
if ball_component is None:
    raise RuntimeError("Ball does not have an NPCWorldStateObjectComponent")

pickup = unreal.NPCWorldActionDefinition()
pickup.set_editor_property("action_id", unreal.Name("pickup"))
pickup.set_editor_property("effect_trigger_normalized_time", 0.0)
ball_component.set_editor_property("actions", [pickup])

human2_class = unreal.EditorAssetLibrary.load_blueprint_class(
    "/Game/MetaHumans/Human2/BP_Human2"
)
if human2_class is None:
    raise RuntimeError("BP_Human2 class could not be loaded")

actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
npc = actor_subsystem.spawn_actor_from_class(
    human2_class,
    ball_state.location + unreal.Vector(100.0, 0.0, 0.0),
    unreal.Rotator(),
)
if npc is None:
    raise RuntimeError("BP_Human2 could not be spawned")

try:
    agent = unreal.NPCWorldStateBlueprintLibrary.ensure_npc_world_state_agent(
        npc
    )
    if agent is None:
        raise RuntimeError("Human2 world-state agent could not be created")

    success, result = unpack_action_outcome(
        agent.execute_world_action(ball_state.object_id, "pickup", "")
    )
    message = result.message if result else ""
    if not success:
        raise RuntimeError(f"Animated pickup was rejected: {message}")
    if not agent.is_world_action_in_progress():
        raise RuntimeError("Human2 pickup used the non-animated fallback path")
    if ball_state.actor.get_attach_parent_actor() != npc:
        raise RuntimeError("Ball was not attached at the animation trigger")

    drop_success, drop_result = unpack_action_outcome(
        agent.drop_held_actor(False)
    )
    if not drop_success:
        message = drop_result.message if drop_result else "unknown error"
        raise RuntimeError(f"Cleanup drop failed: {message}")
    log(
        f"SUCCESS npc={npc.get_name()} target={ball_state.object_id} "
        f"message={message}"
    )
finally:
    actor_subsystem.destroy_actor(npc)

log("END")
