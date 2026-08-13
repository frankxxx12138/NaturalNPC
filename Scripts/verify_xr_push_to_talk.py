import unreal


ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_PushToTalk"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
NPC_PATHS = (
    "/Game/MetaHumans/Human1/BP_Human1",
    "/Game/MetaHumans/Human2/BP_Human2",
)
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"
EXPECTED_KEYS = {
    "OculusTouch_Right_A_Click",
    "Gamepad_FaceButton_Bottom",
}
PREFIX = "NATURALNPC_XR_PTT_VERIFY"


def key_name(key):
    try:
        return str(key.get_editor_property("key_name"))
    except Exception:
        return str(key)


action = unreal.load_asset(ACTION_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
if action is None or mapping_context is None:
    raise RuntimeError("Push-to-talk input assets are missing")
if action.get_editor_property("value_type") != unreal.InputActionValueType.BOOLEAN:
    raise RuntimeError("IA_PushToTalk is not a Boolean action")

mapped_keys = set()
mapping_data = mapping_context.get_editor_property("default_key_mappings")
for mapping in mapping_data.get_editor_property("mappings"):
    mapped_action = mapping.get_editor_property("action")
    if mapped_action is not None and mapped_action.get_name() == "IA_PushToTalk":
        mapped_keys.add(key_name(mapping.get_editor_property("key")))
if not EXPECTED_KEYS.issubset(mapped_keys):
    raise RuntimeError(
        f"Missing PTT keys: {sorted(EXPECTED_KEYS - mapped_keys)}"
    )

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
for npc_path in NPC_PATHS:
    blueprint = unreal.load_asset(npc_path)
    if blueprint is None:
        raise RuntimeError(f"Could not load {npc_path}")
    component = None
    seen = set()
    for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None or obj.get_path_name() in seen:
            continue
        seen.add(obj.get_path_name())
        if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
            component = obj
            break
    if component is None:
        raise RuntimeError(f"{npc_path} is missing OpenAIJackComponent")
    if not component.get_editor_property("enable_keyboard_push_to_talk"):
        raise RuntimeError(f"Keyboard T PTT disabled on {npc_path}")
    if not component.get_editor_property("enable_vr_controller_push_to_talk"):
        raise RuntimeError(f"VR A-button PTT disabled on {npc_path}")
    configured_action = component.get_editor_property("vr_push_to_talk_action")
    if configured_action is None or configured_action.get_name() != "IA_PushToTalk":
        raise RuntimeError(f"Wrong VR PTT action on {npc_path}")

unreal.log(
    f"{PREFIX} success action={ACTION_PATH} keys={sorted(mapped_keys)} "
    f"npcs={list(NPC_PATHS)} keyboard_t_preserved=1"
)
