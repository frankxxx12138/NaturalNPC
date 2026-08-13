import unreal


ACTION_DIR = "/Game/XRFramework/Input/Actions"
ACTION_NAME = "IA_PushToTalk"
ACTION_PATH = f"{ACTION_DIR}/{ACTION_NAME}"
ACTION_TEMPLATE_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
NPC_PATHS = (
    "/Game/MetaHumans/Human1/BP_Human1",
    "/Game/MetaHumans/Human2/BP_Human2",
)
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"
KEY_NAMES = (
    "OculusTouch_Right_A_Click",
    "Gamepad_FaceButton_Bottom",
)
PREFIX = "NATURALNPC_XR_PTT_CONFIGURE"


action = unreal.load_asset(ACTION_PATH)
created = False
if action is None:
    action = unreal.EditorAssetLibrary.duplicate_asset(
        ACTION_TEMPLATE_PATH,
        ACTION_PATH,
    )
    created = True
if action is None:
    raise RuntimeError(f"Could not create {ACTION_PATH}")

action.set_editor_property("value_type", unreal.InputActionValueType.BOOLEAN)
action.set_editor_property(
    "action_description",
    unreal.Text("Push To Talk"),
)
mapping_context = unreal.load_asset(MAPPING_PATH)
if mapping_context is None:
    raise RuntimeError(f"Could not load {MAPPING_PATH}")

mapping_context.unmap_all_keys_from_action(action)
for key_name in KEY_NAMES:
    key = unreal.Key()
    key.set_editor_property("key_name", unreal.Name(key_name))
    mapping_context.map_key(action, key)

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
configured_npcs = []
for npc_path in NPC_PATHS:
    blueprint = unreal.load_asset(npc_path)
    if blueprint is None:
        raise RuntimeError(f"Could not load {npc_path}")

    seen = set()
    component = None
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

    component.set_editor_property("enable_keyboard_push_to_talk", True)
    component.set_editor_property("enable_vr_controller_push_to_talk", True)
    component.set_editor_property("vr_push_to_talk_action", action)
    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        blueprint, only_if_is_dirty=False
    ):
        raise RuntimeError(f"Could not save {npc_path}")
    configured_npcs.append(npc_path)

for asset in (action, mapping_context):
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        asset, only_if_is_dirty=False
    ):
        raise RuntimeError(f"Could not save {asset.get_path_name()}")

unreal.log(
    f"{PREFIX} success created={int(created)} action={ACTION_PATH} "
    f"keys={list(KEY_NAMES)} npcs={list(configured_npcs)}"
)
