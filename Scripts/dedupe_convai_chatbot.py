import unreal


NPC_BP_PATH = "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter"
TARGET_CLASS_FRAGMENT = "BP_ConvaiChatbotComponent"

blueprint = unreal.load_asset(NPC_BP_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {NPC_BP_PATH}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
matches = []

for handle in handles:
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or TARGET_CLASS_FRAGMENT not in obj.get_class().get_path_name():
        continue
    name = str(unreal.SubobjectDataBlueprintFunctionLibrary.get_display_name(data))
    character_id = None
    for property_name in ("character_id", "characterID", "CharacterID"):
        try:
            character_id = obj.get_editor_property(property_name)
            break
        except Exception:
            continue
    matches.append((handle, name, character_id))
    unreal.log(
        f"NATURALNPC_DEDUPE found name={name} character_id={character_id}"
    )

if len(matches) > 1:
    # Prefer the original component name and any component carrying a Character ID.
    matches.sort(
        key=lambda item: (
            bool(item[2]),
            item[1].lower() == "convaichatbot",
            not item[1].lower().endswith("1"),
        ),
        reverse=True,
    )
    keep = matches[0]
    for duplicate in matches[1:]:
        if not subsystem.delete_subobject(handles[0], duplicate[0]):
            raise RuntimeError(f"Could not delete duplicate {duplicate[1]}")
        unreal.log(f"NATURALNPC_DEDUPE removed {duplicate[1]}")
    unreal.log(f"NATURALNPC_DEDUPE kept {keep[1]}")

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError(f"Failed to save {NPC_BP_PATH}")

unreal.log("NATURALNPC_DEDUPE_END")
