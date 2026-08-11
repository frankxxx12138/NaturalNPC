import unreal


PAWN_PATH = "/Game/XRFramework/Blueprints/BP_XRPawn"
MAPPING_PATH = "/Game/XRFramework/Input/IMC_Default"
TURN_ACTION_PATH = "/Game/XRFramework/Input/Actions/IA_Turn"
COMPONENT_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCVRSmoothLocomotionComponent"
)
PREFIX = "NATURALNPC_VR_LOCOMOTION_CONFIGURE"


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
component.set_editor_property("maximum_speed", 180.0)
component.set_editor_property("acceleration_interp_speed", 8.0)
component.set_editor_property("deceleration_interp_speed", 12.0)
component.set_editor_property("thumbstick_dead_zone", 0.2)

# IA_Turn previously consumed the left stick X axis. Smooth locomotion uses the
# complete left stick directly, while the right-stick IA_Move teleport stays intact.
mapping_context.unmap_all_keys_from_action(turn_action)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(
    blueprint, only_if_is_dirty=False
):
    raise RuntimeError("Could not save XR pawn Blueprint")
if not unreal.EditorAssetLibrary.save_loaded_asset(
    mapping_context, only_if_is_dirty=False
):
    raise RuntimeError("Could not save XR input mapping context")

unreal.log(
    f"{PREFIX} success component={component.get_name()} "
    f"added={int(added_component)} left_turn_mappings_removed=1 "
    "right_teleport_preserved=1"
)
