import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
MOVE_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Move"
TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSmoothLocomotionComponent"
)
EXPECTED_RIGHT_TELEPORT_KEYS = {
    "ValveIndex_Right_Thumbstick_Y",
    "OculusTouch_Right_Thumbstick_Y",
    "Vive_Right_Trackpad_Y",
    "MixedReality_Right_Thumbstick_Y",
}
PREFIX = "NATURALNPC_VR_LOCOMOTION_VERIFY"


def key_name(key):
    try:
        return str(key.get_editor_property("key_name"))
    except Exception:
        return str(key)


blueprint = unreal.load_asset(PAWN_PATH)
mapping_context = unreal.load_asset(MAPPING_PATH)
if blueprint is None or mapping_context is None:
    raise RuntimeError("Could not load XR pawn or input mapping context")

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

move_keys = set()
turn_keys = set()
mapping_data = mapping_context.get_editor_property("default_key_mappings")
for mapping in mapping_data.get_editor_property("mappings"):
    action = mapping.get_editor_property("action")
    if action is None:
        continue
    action_name = action.get_name()
    if action_name == "IA_Move":
        move_keys.add(key_name(mapping.get_editor_property("key")))
    elif action_name == "IA_Turn":
        turn_keys.add(key_name(mapping.get_editor_property("key")))

missing_keys = EXPECTED_RIGHT_TELEPORT_KEYS - move_keys
if missing_keys:
    raise RuntimeError(
        f"Right-stick teleport mappings are missing: {sorted(missing_keys)}"
    )
if turn_keys:
    raise RuntimeError(f"Left-stick turn mappings remain: {sorted(turn_keys)}")
if any("Left" in value for value in move_keys):
    raise RuntimeError(f"Teleport unexpectedly uses a left-hand key: {move_keys}")

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"speed={component.get_editor_property('maximum_speed'):.1f} "
    f"right_teleport_keys={sorted(move_keys)} left_turn_keys=0"
)
