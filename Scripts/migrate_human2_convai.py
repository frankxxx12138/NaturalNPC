import unreal


NPC_BP_PATH = "/Game/MetaHumans/Human2/BP_Human2"
NPC_COMPONENT_PATH = (
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiChatbotComponent"
)


def component_objects(blueprint):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
    result = []
    seen = set()
    for handle in handles:
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None or obj.get_path_name() in seen:
            continue
        seen.add(obj.get_path_name())
        result.append((handle, data, obj))
    return result


def get_character_id(obj):
    for property_name in ("character_id", "characterID", "CharacterID"):
        try:
            value = obj.get_editor_property(property_name)
            if value:
                return value
        except Exception:
            pass
    return None


def set_character_id(obj, character_id):
    for property_name in ("character_id", "characterID", "CharacterID"):
        try:
            obj.set_editor_property(property_name, character_id)
            return True
        except Exception:
            pass
    return False


blueprint = unreal.load_asset(NPC_BP_PATH)
component_blueprint = unreal.load_asset(NPC_COMPONENT_PATH)
if blueprint is None or component_blueprint is None:
    raise RuntimeError("Human2 or the Convai chatbot component could not be loaded")

saved_character_id = None
for _, _, obj in component_objects(blueprint):
    if "ConvaiChatbot" in obj.get_class().get_path_name():
        saved_character_id = get_character_id(obj) or saved_character_id

unreal.BlueprintEditorLibrary.reparent_blueprint(blueprint, unreal.Pawn.static_class())
unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

target_class = component_blueprint.generated_class()
existing = component_objects(blueprint)
target = None
for _, _, obj in existing:
    if obj.get_class().get_path_name() == target_class.get_path_name():
        target = obj
        break

if target is None:
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=target_class,
        parent_handle=existing[0][0],
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(f"Could not add Convai chatbot component: {fail_reason}")
    subsystem.rename_subobject(new_handle, unreal.Text("ConvaiChatbot"))

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

for _, _, obj in component_objects(blueprint):
    if obj.get_class().get_path_name() == target_class.get_path_name():
        target = obj
        break

if target is None:
    raise RuntimeError("New Convai chatbot component was not found after compilation")
if not saved_character_id:
    raise RuntimeError("Human2 Character ID was empty")
if not set_character_id(target, saved_character_id):
    raise RuntimeError("Could not restore Human2 Character ID")

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(
    f"NATURALNPC_HUMAN2_MIGRATE success character_id={saved_character_id} "
    f"component={target.get_class().get_path_name()}"
)
