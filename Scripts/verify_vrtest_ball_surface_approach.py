import unreal


PREFIX = "NATURALNPC_BALL_SURFACE_APPROACH_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def unpack(outcome):
    if isinstance(outcome, tuple):
        return bool(outcome[0]), outcome[1] if len(outcome) > 1 else None
    return bool(outcome), None


unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()
npc = next(
    actor
    for actor in actors
    if actor.get_class().get_name() == "BP_Human2_C"
)
ball = next(actor for actor in actors if actor.get_actor_label() == "SM_Ball_01")
agent = npc.get_component_by_class(unreal.NPCWorldStateAgentComponent)
if agent is None:
    raise RuntimeError("Human2 world-state agent is missing")
subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()
ball_state = next(
    state for state in subsystem.get_world_state(None, 0.0) if state.actor == ball
)

handled, result = unpack(
    agent.execute_world_action(ball_state.object_id, "pickup", "")
)
message = "" if result is None else result.message
if not handled:
    raise RuntimeError(f"Ball approach was rejected: {message}")
if not agent.is_world_action_in_progress():
    raise RuntimeError("Ball pickup did not enter world-action state")
if agent.get_held_actor() is not None:
    raise RuntimeError("Ball was attached before the NPC approached it")

distance = (ball.get_actor_location() - npc.get_actor_location()).length()
log(
    f"SUCCESS npc={npc.get_actor_label()} ball={ball.get_actor_label()} "
    f"distance={distance:.1f} message={message} in_progress=1 held=0"
)
