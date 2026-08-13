import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
MOVE_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Move"
TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
SMOOTH_MOVE_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SmoothMove"
SMOOTH_TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_SmoothTurn"
JUMP_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_VRJump"
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSmoothLocomotionComponent"
)
EXPECTED_RIGHT_TELEPORT_KEYS = {
    "ValveIndex_Right_Thumbstick_Y",
    "OculusTouch_Right_Thumbstick_Y",
    "Vive_Right_Trackpad_Y",
    "MixedReality_Right_Thumbstick_Y",
}
EXPECTED_SMOOTH_MOVE_KEYS = {
    "OculusTouch_Left_Thumbstick_2D",
    "ValveIndex_Left_Thumbstick_2D",
    "Vive_Left_Trackpad_2D",
    "MixedReality_Left_Thumbstick_2D",
}
EXPECTED_SMOOTH_TURN_KEYS = {
    "OculusTouch_Right_Thumbstick_X",
    "ValveIndex_Right_Thumbstick_X",
    "Vive_Right_Trackpad_X",
    "MixedReality_Right_Thumbstick_X",
}
EXPECTED_JUMP_KEYS = {
    "OculusTouch_Right_B_Click",
    "Gamepad_FaceButton_Right",
}
PREFIX = "NATURALNPC_VR_LOCOMOTION_VERIFY"


def key_name(key):
    try:
        return str(key.get_editor_property("key_name"))
    except Exception:
        return str(key)


blueprint = unreal.load_asset(PAWN_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
smooth_move_action = unreal.load_asset(SMOOTH_MOVE_ACTION_PATH)
smooth_turn_action = unreal.load_asset(SMOOTH_TURN_ACTION_PATH)
jump_action = unreal.load_asset(JUMP_ACTION_PATH)
if (blueprint is None or mapping_context is None or
        smooth_move_action is None or smooth_turn_action is None or
        jump_action is None):
    raise RuntimeError("Could not load XR pawn or input mapping context")
if smooth_move_action.get_editor_property(
    "value_type"
) != unreal.InputActionValueType.AXIS2D:
    raise RuntimeError("IA_SmoothMove is not Axis2D")
if smooth_turn_action.get_editor_property(
    "value_type"
) != unreal.InputActionValueType.AXIS1D:
    raise RuntimeError("IA_SmoothTurn is not Axis1D")
if jump_action.get_editor_property(
    "value_type"
) != unreal.InputActionValueType.BOOLEAN:
    raise RuntimeError("IA_VRJump is not Boolean")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
seen = set()
components = []
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() == COMPONENT_CLASS_PATH:
        components.append(obj)

if len(components) != 1:
    raise RuntimeError(
        f"Expected one smooth locomotion component, found {len(components)}"
    )
component = components[0]
if not component.get_editor_property("smooth_locomotion_enabled"):
    raise RuntimeError("Smooth locomotion component is disabled")
if not component.get_editor_property("smooth_turning_enabled"):
    raise RuntimeError("Right-stick smooth turning is disabled")
speed = component.get_editor_property("maximum_speed")
if abs(speed - 250.0) > 0.01:
    raise RuntimeError(f"Wrong smooth movement speed: {speed}")
turn_speed = component.get_editor_property("maximum_turn_speed")
if abs(turn_speed - 180.0) > 0.1:
    raise RuntimeError(f"Expected smooth turn speed 180.0, got {turn_speed}")
collision_radius = component.get_editor_property("collision_radius")
maximum_step_up = component.get_editor_property("maximum_step_up")
if abs(collision_radius - 24.0) > 0.1:
    raise RuntimeError(f"Expected collision radius 24.0, got {collision_radius}")
if abs(maximum_step_up - 35.0) > 0.1:
    raise RuntimeError(f"Expected maximum step-up 35.0, got {maximum_step_up}")
if not component.get_editor_property("gravity_enabled"):
    raise RuntimeError("VR gravity is disabled")
gravity = component.get_editor_property("gravity_acceleration")
if abs(gravity - 980.0) > 0.1:
    raise RuntimeError(f"Expected gravity acceleration 980.0, got {gravity}")
if component.get_editor_property("ground_trace_distance") < 1200.0:
    raise RuntimeError("VR ground trace distance is too short")
if not component.get_editor_property("jump_enabled"):
    raise RuntimeError("VR jump is disabled")
jump_velocity = component.get_editor_property("jump_velocity")
if abs(jump_velocity - 420.0) > 0.1:
    raise RuntimeError(f"Expected jump velocity 420.0, got {jump_velocity}")

move_keys = set()
turn_keys = set()
smooth_move_keys = set()
smooth_turn_keys = set()
jump_keys = set()
b_key_actions = set()
mapping_data = mapping_context.get_editor_property("default_key_mappings")
for mapping in mapping_data.get_editor_property("mappings"):
    action = mapping.get_editor_property("action")
    if action is None:
        continue
    action_name = action.get_name()
    mapped_key_name = key_name(mapping.get_editor_property("key"))
    if mapped_key_name == "OculusTouch_Right_B_Click":
        b_key_actions.add(action_name)
    if action_name == "IA_Move":
        move_keys.add(key_name(mapping.get_editor_property("key")))
    elif action_name == "IA_Turn":
        turn_keys.add(key_name(mapping.get_editor_property("key")))
    elif action_name == "IA_SmoothMove":
        smooth_move_keys.add(key_name(mapping.get_editor_property("key")))
    elif action_name == "IA_SmoothTurn":
        smooth_turn_keys.add(key_name(mapping.get_editor_property("key")))
    elif action_name == "IA_VRJump":
        jump_keys.add(mapped_key_name)

missing_keys = EXPECTED_RIGHT_TELEPORT_KEYS - move_keys
if missing_keys:
    raise RuntimeError(
        f"Right-stick teleport mappings are missing: {sorted(missing_keys)}"
    )
if turn_keys:
    raise RuntimeError(f"Left-stick turn mappings remain: {sorted(turn_keys)}")
if any("Left" in value for value in move_keys):
    raise RuntimeError(f"Teleport unexpectedly uses a left-hand key: {move_keys}")
missing_move_keys = EXPECTED_SMOOTH_MOVE_KEYS - smooth_move_keys
if missing_move_keys:
    raise RuntimeError(
        f"Smooth movement mappings are missing: {sorted(missing_move_keys)}"
    )
missing_turn_keys = EXPECTED_SMOOTH_TURN_KEYS - smooth_turn_keys
if missing_turn_keys:
    raise RuntimeError(
        f"Smooth turn mappings are missing: {sorted(missing_turn_keys)}"
    )
missing_jump_keys = EXPECTED_JUMP_KEYS - jump_keys
if missing_jump_keys:
    raise RuntimeError(
        f"VR jump mappings are missing: {sorted(missing_jump_keys)}"
    )
if b_key_actions != {"IA_VRJump"}:
    raise RuntimeError(
        f"Right B must map only to IA_VRJump, got {sorted(b_key_actions)}"
    )
configured_move = component.get_editor_property("smooth_move_action")
configured_turn = component.get_editor_property("smooth_turn_action")
configured_jump = component.get_editor_property("jump_action")
if configured_move is None or configured_move.get_name() != "IA_SmoothMove":
    raise RuntimeError("Component is not bound to IA_SmoothMove")
if configured_turn is None or configured_turn.get_name() != "IA_SmoothTurn":
    raise RuntimeError("Component is not bound to IA_SmoothTurn")
if configured_jump is None or configured_jump.get_name() != "IA_VRJump":
    raise RuntimeError("Component is not bound to IA_VRJump")

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"speed={speed:.1f} turn_speed={turn_speed:.1f} "
    f"collision_radius={collision_radius:.1f} step_up={maximum_step_up:.1f} "
    f"gravity=1 gravity_accel={gravity:.1f} "
    f"right_b_jump=1 jump_velocity={jump_velocity:.1f} "
    f"right_x_turn=1 right_teleport_keys={sorted(move_keys)} "
    f"smooth_move_keys={sorted(smooth_move_keys)} "
    f"smooth_turn_keys={sorted(smooth_turn_keys)} enhanced_turn_keys=0"
)
