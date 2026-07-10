import unreal


PLAYER_BP_PATH = "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"
NPC_BP_PATH = "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter"
PLAYER_COMPONENT_PATH = (
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiPlayerComponent"
)
NPC_COMPONENT_PATH = (
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiChatbotComponent"
)


def load_required(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset could not be loaded: {path}")
    return asset


def component_objects(blueprint):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
    result = []
    for handle in handles:
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        result.append((handle, data, obj))
    return result


def find_character_id(blueprint):
    for _, _, obj in component_objects(blueprint):
        if obj is None:
            continue
        for property_name in ("character_id", "characterID", "CharacterID"):
            try:
                value = obj.get_editor_property(property_name)
            except Exception:
                continue
            if value:
                unreal.log(
                    f"NATURALNPC_MIGRATE captured Character ID from {obj}: {value}"
                )
                return value
    return None


def has_component(blueprint, component_class):
    target_path = component_class.get_path_name()
    for _, _, obj in component_objects(blueprint):
        if obj is not None and obj.get_class().get_path_name() == target_path:
            return True
    return False


def add_blueprint_component(blueprint, component_class, variable_name):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    existing = component_objects(blueprint)
    if not existing:
        raise RuntimeError(f"No root subobject was found in {blueprint.get_path_name()}")

    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=component_class,
        parent_handle=existing[0][0],
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(
            f"Could not add {component_class.get_path_name()}: {fail_reason}"
        )
    subsystem.rename_subobject(new_handle, unreal.Text(variable_name))
    return new_handle


def set_character_id(blueprint, character_id):
    if not character_id:
        return False
    for _, _, obj in component_objects(blueprint):
        if obj is None or "BP_ConvaiChatbotComponent" not in obj.get_class().get_path_name():
            continue
        for property_name in ("character_id", "characterID", "CharacterID"):
            try:
                obj.set_editor_property(property_name, character_id)
                unreal.log(
                    f"NATURALNPC_MIGRATE restored Character ID on {obj}: {character_id}"
                )
                return True
            except Exception:
                continue
    unreal.log_warning(
        "NATURALNPC_MIGRATE could not restore Character ID automatically; "
        "the placed NPC instance may still retain its override."
    )
    return False


def migrate(blueprint_path, parent_class, component_path, variable_name, character_id=None):
    blueprint = load_required(blueprint_path)
    component_blueprint = load_required(component_path)
    component_class = component_blueprint.generated_class()

    unreal.BlueprintEditorLibrary.reparent_blueprint(blueprint, parent_class)
    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

    if not has_component(blueprint, component_class):
        add_blueprint_component(blueprint, component_class, variable_name)

    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
    if character_id:
        set_character_id(blueprint, character_id)
        unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

    if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
        raise RuntimeError(f"Failed to save {blueprint_path}")

    unreal.log(
        f"NATURALNPC_MIGRATE migrated {blueprint_path} with {component_class.get_path_name()}"
    )


unreal.log("NATURALNPC_MIGRATE_BEGIN")
npc_blueprint = load_required(NPC_BP_PATH)
saved_character_id = find_character_id(npc_blueprint)

migrate(
    PLAYER_BP_PATH,
    unreal.Character.static_class(),
    PLAYER_COMPONENT_PATH,
    "ConvaiPlayer",
)
migrate(
    NPC_BP_PATH,
    unreal.Pawn.static_class(),
    NPC_COMPONENT_PATH,
    "ConvaiChatbot",
    saved_character_id,
)
unreal.log("NATURALNPC_MIGRATE_END")
