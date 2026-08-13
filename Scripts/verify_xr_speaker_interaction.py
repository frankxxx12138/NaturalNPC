import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SpeakerInteract"
LEFT_FALLBACK_ACTION = "IA_Grab_Left_Pressed"
RIGHT_FALLBACK_ACTION = "IA_Grab_Right_Pressed"
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSpeakerInteractorComponent"
)
EXPECTED_KEYS = {
    "OculusTouch_Right_Trigger_Axis",
    "OculusTouch_Right_Trigger_Click",
    "ValveIndex_Right_Trigger_Axis",
    "ValveIndex_Right_Trigger_Click",
}
PREFIX = "NATURALNPC_SPEAKER_VERIFY"


def key_name(key):
    return str(key.get_editor_property("key_name"))


blueprint = unreal.load_asset(PAWN_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
action = unreal.load_asset(ACTION_PATH)
if blueprint is None or mapping_context is None or action is None:
    raise RuntimeError("Speaker interaction assets are missing")
if action.get_editor_property("value_type") != unreal.InputActionValueType.BOOLEAN:
    raise RuntimeError("IA_SpeakerInteract must be Boolean")

mapped_keys = set()
mapping_data = mapping_context.get_editor_property("default_key_mappings")
for mapping in mapping_data.get_editor_property("mappings"):
    mapped_action = mapping.get_editor_property("action")
    if mapped_action is not None and mapped_action.get_name() == action.get_name():
        mapped_keys.add(key_name(mapping.get_editor_property("key")))
missing_keys = EXPECTED_KEYS - mapped_keys
if missing_keys:
    raise RuntimeError(f"Missing speaker trigger mappings: {sorted(missing_keys)}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
component = None
seen = set()
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() == COMPONENT_CLASS_PATH:
        component = obj
        break
if component is None:
    raise RuntimeError("BP_XRPawn has no VR speaker interactor")
configured_action = component.get_editor_property("speaker_interact_action")
if configured_action is None or configured_action.get_name() != action.get_name():
    raise RuntimeError("Speaker interactor has the wrong input action")
left_fallback = component.get_editor_property("left_trigger_fallback_action")
right_fallback = component.get_editor_property("right_trigger_fallback_action")
if left_fallback is None or left_fallback.get_name() != LEFT_FALLBACK_ACTION:
    raise RuntimeError("Speaker interactor has the wrong left trigger fallback")
if right_fallback is None or right_fallback.get_name() != RIGHT_FALLBACK_ACTION:
    raise RuntimeError("Speaker interactor has the wrong right trigger fallback")
if not component.get_editor_property("activate_speaker_buttons_on_touch"):
    raise RuntimeError("Speaker touch targeting is disabled")
touch_radius = component.get_editor_property("touch_radius")
if abs(touch_radius - 9.0) > 0.01:
    raise RuntimeError(f"Expected speaker touch radius 9.0, got {touch_radius}")

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
speakers = [
    actor for actor in actors
    if "BP_InteractiveSpeaker" in actor.get_class().get_name()
]
if len(speakers) != 1:
    raise RuntimeError(f"Expected one speaker instance, got {len(speakers)}")
audio = speakers[0].get_component_by_class(unreal.AudioComponent)
if audio is None:
    raise RuntimeError("Speaker has no AudioComponent")
button_names = {
    component.get_name()
    for component in speakers[0].get_components_by_class(unreal.PrimitiveComponent)
    if component.get_name().startswith("Button_")
}
expected_buttons = {
    "Button_Jazz",
    "Button_Blues",
    "Button_Dance",
    "Button_Pause",
}
if not expected_buttons.issubset(button_names):
    raise RuntimeError(
        f"Speaker buttons are missing: {sorted(expected_buttons - button_names)}"
    )

selection_tests = (
    ("PressJazzButton", "SW_RelaxedJazz"),
    ("PressBluesButton", "SW_QuietBlues"),
    ("PressDanceButton", "SW_EnergeticDance"),
)
selected_sounds = []
for function_name, expected_sound in selection_tests:
    speakers[0].call_method(function_name)
    selected = audio.get_editor_property("sound")
    selected_name = selected.get_name() if selected is not None else "None"
    if selected_name != expected_sound:
        raise RuntimeError(
            f"{function_name} selected {selected_name}, expected {expected_sound}"
        )
    selected_sounds.append(selected_name)

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"action={action.get_name()} keys={sorted(mapped_keys)} "
    f"touch_targeting=1 trigger_required=1 "
    f"fallbacks={[left_fallback.get_name(), right_fallback.get_name()]} "
    f"speaker={speakers[0].get_name()} buttons={sorted(button_names)} "
    f"selection_tests={selected_sounds}"
)
