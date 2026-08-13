import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
SMOOTH_MOVE_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SmoothMove"
SMOOTH_TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SmoothTurn"
JUMP_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_VRJump"
ACTION_TEMPLATE_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSmoothLocomotionComponent"
)
PREFIX = "NATURALNPC_VR_LOCOMOTION_CONFIGURE"

MOVE_KEYS = (
    "OculusTouch_Left_Thumbstick_2D",
    "ValveIndex_Left_Thumbstick_2D",
    "ValveIndex_Left_Trackpad_2D",
    "Vive_Left_Trackpad_2D",
    "MixedReality_Left_Thumbstick_2D",
    "Gamepad_Left2D",
)
TURN_KEYS = (
    "OculusTouch_Right_Thumbstick_X",
    "ValveIndex_Right_Thumbstick_X",
    "ValveIndex_Right_Trackpad_X",
    "Vive_Right_Trackpad_X",
    "MixedReality_Right_Thumbstick_X",
    "Gamepad_RightX",
)
JUMP_KEYS = (
    "OculusTouch_Right_B_Click",
    "Gamepad_FaceButton_Right",
)


def load_or_duplicate_action(path, value_type, description):
    action = unreal.load_asset(path)
    created = False
    if action is None:
        action = unreal.EditorAssetLibrary.duplicate_asset(
            ACTION_TEMPLATE_PATH,
            path,
        )
        created = True
    if action is None:
        raise RuntimeError(f"Could not create input action: {path}")
    action.set_editor_property("value_type", value_type)
    action.set_editor_property(
        "action_description",
        unreal.Text(description),
    )
    return action, created


def replace_action_keys(mapping_context, action, key_names):
    mapping_context.unmap_all_keys_from_action(action)
    for key_name in key_names:
        key = unreal.Key()
        key.set_editor_property("key_name", unreal.Name(key_name))
        mapping_context.map_key(action, key)


def remove_key_from_other_actions(mapping_context, key_name, kept_action):
    key = unreal.Key()
    key.set_editor_property("key_name", unreal.Name(key_name))
    mapping_data = mapping_context.get_editor_property("default_key_mappings")
    conflicting_actions = []
    for mapping in mapping_data.get_editor_property("mappings"):
        action = mapping.get_editor_property("action")
        mapped_key = mapping.get_editor_property("key")
        mapped_name = str(mapped_key.get_editor_property("key_name"))
        if (
            action is not None
            and action != kept_action
            and mapped_name == key_name
        ):
            if action not in conflicting_actions:
                conflicting_actions.append(action)
    for action in conflicting_actions:
        mapping_context.unmap_key(action, key)


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
component_class = unreal.load_class(None, COMPONENT_CLASS_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
turn_action = unreal.load_asset(TURN_ACTION_PATH)
smooth_move_action, created_move = load_or_duplicate_action(
    SMOOTH_MOVE_ACTION_PATH,
    unreal.InputActionValueType.AXIS2D,
    "Smooth Move",
)
smooth_turn_action, created_turn = load_or_duplicate_action(
    SMOOTH_TURN_ACTION_PATH,
    unreal.InputActionValueType.AXIS1D,
    "Smooth Turn",
)
jump_action, created_jump = load_or_duplicate_action(
    JUMP_ACTION_PATH,
    unreal.InputActionValueType.BOOLEAN,
    "VR Jump",
)
if blueprint is None:
    raise RuntimeError(f"Could not load XR pawn: {PAWN_PATH}")
if component_class is None:
    raise RuntimeError(f"Could not load component class: {COMPONENT_CLASS_PATH}")
if mapping_context is None or turn_action is None:
    raise RuntimeError("Could not load XR input assets")

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
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(f"Could not add smooth locomotion: {fail_reason}")
    subsystem.rename_subobject(new_handle, unreal.Text("VRSmoothLocomotion"))
    new_data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(new_handle)
    component = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(new_data)
    added_component = True

if component is None:
    raise RuntimeError("Could not access smooth locomotion component")

component.set_editor_property("smooth_locomotion_enabled", True)
component.set_editor_property("maximum_speed", 250.0)
component.set_editor_property("acceleration_interp_speed", 8.0)
component.set_editor_property("deceleration_interp_speed", 12.0)
component.set_editor_property("thumbstick_dead_zone", 0.2)
component.set_editor_property("smooth_turning_enabled", True)
component.set_editor_property("maximum_turn_speed", 180.0)
component.set_editor_property("turn_dead_zone", 0.25)
component.set_editor_property("collision_radius", 24.0)
component.set_editor_property("collision_half_height", 88.0)
component.set_editor_property("maximum_step_up", 35.0)
component.set_editor_property("maximum_step_down", 70.0)
component.set_editor_property("gravity_enabled", True)
component.set_editor_property("gravity_acceleration", 980.0)
component.set_editor_property("maximum_fall_speed", 1200.0)
component.set_editor_property("ground_snap_distance", 4.0)
component.set_editor_property("ground_trace_distance", 1200.0)
component.set_editor_property("jump_enabled", True)
component.set_editor_property("jump_velocity", 420.0)
component.set_editor_property("smooth_move_action", smooth_move_action)
component.set_editor_property("smooth_turn_action", smooth_turn_action)
component.set_editor_property("jump_action", jump_action)

# OpenXR only publishes controls declared in its Enhanced Input contexts.
replace_action_keys(mapping_context, smooth_move_action, MOVE_KEYS)
replace_action_keys(mapping_context, smooth_turn_action, TURN_KEYS)
for jump_key in JUMP_KEYS:
    remove_key_from_other_actions(mapping_context, jump_key, jump_action)
replace_action_keys(mapping_context, jump_action, JUMP_KEYS)
mapping_context.unmap_all_keys_from_action(turn_action)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(
    blueprint, only_if_is_dirty=False
):
    raise RuntimeError("Could not save XR pawn Blueprint")
for asset in (
    smooth_move_action,
    smooth_turn_action,
    jump_action,
    mapping_context,
):
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        asset, only_if_is_dirty=False
    ):
        raise RuntimeError(f"Could not save {asset.get_path_name()}")

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"added={int(added_component)} speed=250.0 turn_speed=180.0 "
    f"created_move={int(created_move)} created_turn={int(created_turn)} "
    f"created_jump={int(created_jump)} "
    "left_2d_move=1 right_x_turn=1 right_y_teleport_preserved=1 "
    "gravity=1 gravity_accel=980.0 max_fall=1200.0 "
    "right_b_jump=1 jump_velocity=420.0"
)
