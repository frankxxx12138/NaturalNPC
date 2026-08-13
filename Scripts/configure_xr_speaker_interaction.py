import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SpeakerInteract"
ACTION_TEMPLATE_PATH = "/Game/XRFramework/Input/Actions/IA_Grab_Right_Pressed"
LEFT_FALLBACK_ACTION_PATH = (
    "/Game/XRFramework/Input/Actions/IA_Grab_Left_Pressed"
)
RIGHT_FALLBACK_ACTION_PATH = (
    "/Game/XRFramework/Input/Actions/IA_Grab_Right_Pressed"
)
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSpeakerInteractorComponent"
)
PREFIX = "NATURALNPC_SPEAKER_CONFIGURE"
TRIGGER_KEYS = (
    "OculusTouch_Right_Trigger_Axis",
    "OculusTouch_Right_Trigger_Click",
    "ValveIndex_Right_Trigger_Axis",
    "ValveIndex_Right_Trigger_Click",
    "MixedReality_Right_Trigger_Axis",
    "MixedReality_Right_Trigger_Click",
    "Vive_Right_Trigger_Axis",
    "Vive_Right_Trigger_Click",
    "Gamepad_RightTriggerAxis",
)


def unique_subobjects(blueprint):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    seen = set()
    result = []
    for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None:
            continue
        path = obj.get_path_name()
        if path in seen:
            continue
        seen.add(path)
        result.append((handle, obj))
    return subsystem, result


blueprint = unreal.load_asset(PAWN_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
component_class = unreal.load_class(None, COMPONENT_CLASS_PATH)
action = unreal.load_asset(ACTION_PATH)
left_fallback_action = unreal.load_asset(LEFT_FALLBACK_ACTION_PATH)
right_fallback_action = unreal.load_asset(RIGHT_FALLBACK_ACTION_PATH)
created_action = False
if action is None:
    action = unreal.EditorAssetLibrary.duplicate_asset(
        ACTION_TEMPLATE_PATH,
        ACTION_PATH,
    )
    created_action = True
if blueprint is None or mapping_context is None:
    raise RuntimeError("Could not load XR pawn or default input context")
if component_class is None:
    raise RuntimeError(f"Could not load component class: {COMPONENT_CLASS_PATH}")
if action is None:
    raise RuntimeError(f"Could not create speaker input action: {ACTION_PATH}")
if left_fallback_action is None or right_fallback_action is None:
    raise RuntimeError("Could not load the existing XR grab trigger actions")

action.set_editor_property("value_type", unreal.InputActionValueType.BOOLEAN)
action.set_editor_property(
    "action_description",
    unreal.Text("Interact With Speaker"),
)
mapping_context.unmap_all_keys_from_action(action)
for key_name in TRIGGER_KEYS:
    key = unreal.Key()
    key.set_editor_property("key_name", unreal.Name(key_name))
    mapping_context.map_key(action, key)

subsystem, subobjects = unique_subobjects(blueprint)
if not subobjects:
    raise RuntimeError("XR pawn Blueprint has no root subobject")
component = None
for _, obj in subobjects:
    if obj.get_class().get_path_name() == COMPONENT_CLASS_PATH:
        component = obj
        break

added_component = False
if component is None:
    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=component_class,
        parent_handle=subobjects[0][0],
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(handle):
        raise RuntimeError(f"Could not add speaker interactor: {fail_reason}")
    subsystem.rename_subobject(handle, unreal.Text("VRSpeakerInteractor"))
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    component = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    added_component = True

component.set_editor_property("speaker_interaction_enabled", True)
component.set_editor_property("trigger_threshold", 0.55)
component.set_editor_property("touch_radius", 9.0)
# Touch only selects a button; activation always requires a trigger press.
component.set_editor_property("activate_speaker_buttons_on_touch", True)
component.set_editor_property("touch_forward_offset", 10.0)
component.set_editor_property("pointer_distance", 300.0)
component.set_editor_property("pointer_radius", 9.0)
component.set_editor_property("speaker_interact_action", action)
component.set_editor_property(
    "left_trigger_fallback_action",
    left_fallback_action,
)
component.set_editor_property(
    "right_trigger_fallback_action",
    right_fallback_action,
)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
for asset in (blueprint, action, mapping_context):
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        asset,
        only_if_is_dirty=False,
    ):
        raise RuntimeError(f"Could not save {asset.get_path_name()}")

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"added={int(added_component)} action_created={int(created_action)} "
    f"keys={list(TRIGGER_KEYS)} touch=9.0 trigger_required=1 "
    "fallback_actions=grab_pressed pointer=300.0"
)
