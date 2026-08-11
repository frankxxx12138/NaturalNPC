import unreal


PREFIX = "NATURALNPC_MIXAMO_VERIFY"
MAP_PATH = "/Game/VRTest"
PICKUP_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_PickingUp_Small"
)
BOX_WALK_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_BoxWalk_Carry"
)
OLD_LARGE_PICKUP_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Pickup_TwoHand"
)


def log(message):
    unreal.log(f"{PREFIX} {message}")


def asset_path(asset):
    return asset.get_path_name().split(".")[0] if asset else ""


def unpack_action_outcome(outcome):
    if isinstance(outcome, tuple):
        return bool(outcome[0]), outcome[1] if len(outcome) > 1 else None
    return bool(outcome), None


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()
human2 = next(
    actor for actor in actors if actor.get_actor_label() == "BP_Human2"
)

agent = unreal.NPCWorldStateBlueprintLibrary.ensure_npc_world_state_agent(human2)
if agent is None:
    raise RuntimeError("Human2 has no world-state agent")

profiles = list(agent.get_editor_property("item_animation_profiles"))
pistol_profile = next(
    profile for profile in profiles if str(profile.category) == "pistol"
)
ball_profile = next(
    profile for profile in profiles if str(profile.category) == "ball"
)
if asset_path(pistol_profile.pickup_animation) != PICKUP_PATH:
    raise RuntimeError(
        f"Pistol pickup is wrong: {pistol_profile.pickup_animation}"
    )
if abs(pistol_profile.pickup_animation_play_rate - 2.5) > 0.001:
    raise RuntimeError("Pistol pickup play rate is not 2.5")
if asset_path(ball_profile.pickup_animation) != OLD_LARGE_PICKUP_PATH:
    raise RuntimeError(
        f"Large-object pickup changed unexpectedly: {ball_profile.pickup_animation}"
    )

default_held_walk = agent.get_editor_property("default_held_walk_animation")
if asset_path(default_held_walk) != BOX_WALK_PATH:
    raise RuntimeError(f"Default held walk is wrong: {default_held_walk}")

subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()
states = subsystem.get_world_state(human2, 0.0)
pistol_state = next(state for state in states if str(state.category) == "pistol")
pistol_component = pistol_state.actor.get_component_by_class(
    unreal.NPCWorldStateObjectComponent
)
pickup_action = unreal.NPCWorldActionDefinition()
pickup_action.set_editor_property("action_id", unreal.Name("pickup"))
pickup_action.set_editor_property("effect_trigger_normalized_time", 0.0)
pistol_component.set_editor_property("actions", [pickup_action])

human2.set_actor_location(
    pistol_state.location + unreal.Vector(100.0, 0.0, 0.0),
    False,
    False,
)
success, result = unpack_action_outcome(
    agent.execute_world_action(pistol_state.object_id, "pickup", "")
)
if not success:
    message = result.message if result else "unknown error"
    raise RuntimeError(f"Pistol pickup failed: {message}")
if not agent.is_world_action_in_progress():
    raise RuntimeError("Pistol pickup did not start an animation")
if agent.get_held_actor() != pistol_state.actor:
    raise RuntimeError("Pistol was not attached at the pickup trigger")

held_walk = agent.get_held_walk_animation()
if asset_path(held_walk) != BOX_WALK_PATH:
    raise RuntimeError(f"Held walk did not resolve to Box Walk: {held_walk}")

drop_success, drop_result = unpack_action_outcome(agent.drop_held_actor(False))
if not drop_success:
    message = drop_result.message if drop_result else "unknown error"
    raise RuntimeError(f"Verification cleanup drop failed: {message}")

pickup_animation = unreal.load_asset(PICKUP_PATH)
effective_duration = (
    pickup_animation.get_play_length()
    / pistol_profile.pickup_animation_play_rate
)
log(
    f"SUCCESS pickup={PICKUP_PATH} play_rate=2.5 "
    f"effective_duration={effective_duration:.3f} "
    f"held_walk={BOX_WALK_PATH} large_pickup={OLD_LARGE_PICKUP_PATH}"
)
