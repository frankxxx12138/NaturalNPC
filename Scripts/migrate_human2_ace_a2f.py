import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
ACE_CURVE_CLASS_PATH = "/Script/ACERuntime.ACEAudioCurveSourceComponent"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"


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


blueprint = unreal.load_asset(HUMAN2_PATH)
ace_class = unreal.load_class(None, ACE_CURVE_CLASS_PATH)
if blueprint is None or ace_class is None:
    raise RuntimeError("Human2 Blueprint or ACE Audio Curve Source class could not be loaded")

subsystem, subobjects = unique_subobjects(blueprint)
if not subobjects:
    raise RuntimeError("Human2 Blueprint has no root subobject")

root_handle = subobjects[0][0]
ace_component = None
openai_component = None
for handle, obj in subobjects:
    class_path = obj.get_class().get_path_name()
    if class_path == ACE_CURVE_CLASS_PATH:
        ace_component = obj
    if class_path == OPENAI_CLASS_PATH:
        openai_component = obj

added_ace = False
if ace_component is None:
    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=ace_class,
        parent_handle=root_handle,
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(f"Could not add ACE Audio Curve Source: {fail_reason}")
    subsystem.rename_subobject(new_handle, unreal.Text("ACEAudioCurveSource"))
    new_data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(new_handle)
    ace_component = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(new_data)
    added_ace = True

if ace_component is None:
    raise RuntimeError("Could not access ACE Audio Curve Source component")

ace_component.set_editor_property("buffer_length_in_seconds", 0.1)

if openai_component is not None:
    openai_component.set_editor_property("enable_ace_audio2_face", True)
    openai_component.set_editor_property("ace_audio2_face_provider_name", "LocalA2F-Mark")

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(
    "HUMAN2_ACE_A2F_MIGRATE "
    f"success added_ace={int(added_ace)} "
    f"has_openai={int(openai_component is not None)} "
    f"component={ace_component.get_name()}"
)
