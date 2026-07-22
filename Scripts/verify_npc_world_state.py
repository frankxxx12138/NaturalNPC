import unreal


PREFIX = "NATURALNPC_WORLDSTATE_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


log("BEGIN")
if not hasattr(unreal.OpenAIJackComponent, "execute_world_action"):
    raise RuntimeError("OpenAIJackComponent world-action API was not loaded")
if not hasattr(unreal.OpenAIJackComponent, "get_world_state_json"):
    raise RuntimeError("OpenAIJackComponent world-state API was not loaded")

agent_defaults = unreal.get_default_object(unreal.NPCWorldStateAgentComponent)
pickup_animation = agent_defaults.get_editor_property(
    "default_pickup_animation"
)
pickup_animation_path = (
    pickup_animation.get_path_name()
    if hasattr(pickup_animation, "get_path_name")
    else str(pickup_animation)
)
if "Human2_Pickup_TwoHand" not in pickup_animation_path:
    raise RuntimeError(
        f"Unexpected default pickup animation: {pickup_animation_path}"
    )

held_idle_animation = agent_defaults.get_editor_property(
    "default_held_idle_animation"
)
held_idle_animation_path = (
    held_idle_animation.get_path_name()
    if hasattr(held_idle_animation, "get_path_name")
    else str(held_idle_animation)
)
if "Human2_Held_Idle_TwoHand" not in held_idle_animation_path:
    raise RuntimeError(
        f"Unexpected default held idle animation: {held_idle_animation_path}"
    )

profiles = agent_defaults.get_editor_property("item_animation_profiles")
profile_categories = {str(profile.category) for profile in profiles}
required_profiles = {"pistol", "ball", "cube", "log", "object"}
if not required_profiles.issubset(profile_categories):
    raise RuntimeError(
        "Missing item animation profiles: "
        f"{sorted(required_profiles - profile_categories)}"
    )

action_definition = unreal.NPCWorldActionDefinition()
for property_name in (
    "action_animation",
    "held_idle_animation",
    "animation_play_rate",
    "effect_trigger_normalized_time",
    "play_animation_in_reverse",
):
    try:
        action_definition.get_editor_property(property_name)
    except Exception as exc:
        raise RuntimeError(
            f"Missing action animation property {property_name}: {exc}"
        ) from exc
log(
    f"ANIMATION_CONFIG pickup={pickup_animation_path} "
    f"held_idle={held_idle_animation_path} "
    f"profiles={sorted(profile_categories)}"
)

unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTemplate/VRTemplateMap")
world = unreal.EditorLevelLibrary.get_editor_world()
subsystem = (
    unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(world)
)
if subsystem is None:
    raise RuntimeError("NPCWorldStateSubsystem was not created")

subsystem.refresh_auto_discovery()
states = subsystem.get_world_state(None, 0.0)
if not states:
    raise RuntimeError("World-state registry did not discover any objects")

categories = {}
action_ids = set()
for state in states:
    category = str(state.category)
    categories[category] = categories.get(category, 0) + 1
    for action in state.available_actions:
        action_ids.add(str(action.action_id))

required_actions = {"inspect", "pickup"}
if not required_actions.issubset(action_ids):
    raise RuntimeError(
        f"Missing inferred actions: {sorted(required_actions - action_ids)}"
    )
if "pistol" not in categories or "ball" not in categories:
    raise RuntimeError(
        f"Expected VR pistol and ball categories, found {categories}"
    )

json_text = subsystem.build_world_state_json(None, 0.0)
if '"objects"' not in json_text:
    raise RuntimeError("World-state JSON did not contain an objects array")

log(
    f"SUCCESS objects={len(states)} categories={categories} "
    f"actions={sorted(action_ids)} json_chars={len(json_text)}"
)

log("END")
