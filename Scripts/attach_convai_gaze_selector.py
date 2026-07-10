import unreal


PLAYER_BP_PATH = "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"
COMPONENT_CLASS_PATH = "/Script/NaturalNPCGazeSelector.ConvaiGazeSelectorComponent"

blueprint = unreal.load_asset(PLAYER_BP_PATH)
component_class = unreal.load_class(None, COMPONENT_CLASS_PATH)
if blueprint is None or component_class is None:
    raise RuntimeError("Player Blueprint or gaze selector class could not be loaded")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
if not handles:
    raise RuntimeError("Player Blueprint has no root subobject")

found = False
seen = set()
for handle in handles:
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() == COMPONENT_CLASS_PATH:
        found = True
        break

if not found:
    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=component_class,
        parent_handle=handles[0],
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(f"Could not add gaze selector: {fail_reason}")
    subsystem.rename_subobject(new_handle, unreal.Text("ConvaiGazeSelector"))

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save player Blueprint")

unreal.log(f"NATURALNPC_GAZE_ATTACH success already_present={found}")
