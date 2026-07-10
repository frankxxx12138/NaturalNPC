import unreal


NPC_PATHS = [
    "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter",
    "/Game/MetaHumans/Human2/BP_Human2",
]


def get_character_id(obj):
    for property_name in ("character_id", "characterID", "CharacterID"):
        try:
            return obj.get_editor_property(property_name)
        except Exception:
            pass
    return None


subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
unreal.log("NATURALNPC_HUMAN2_INSPECT_BEGIN")

for asset_path in NPC_PATHS:
    blueprint = unreal.load_asset(asset_path)
    if blueprint is None:
        unreal.log_error(f"NATURALNPC_HUMAN2 missing asset={asset_path}")
        continue

    unreal.log(f"NATURALNPC_HUMAN2 asset={asset_path}")
    handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
    for handle in handles:
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None:
            continue
        class_path = obj.get_class().get_path_name()
        if "Convai" not in class_path:
            continue
        name = unreal.SubobjectDataBlueprintFunctionLibrary.get_display_name(data)
        unreal.log(
            f"NATURALNPC_HUMAN2 component asset={asset_path} name={name} "
            f"class={class_path} character_id={get_character_id(obj)}"
        )

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Test")
if world is not None:
    for actor in unreal.EditorLevelLibrary.get_all_level_actors():
        class_path = actor.get_class().get_path_name()
        if "BP_Human2" not in class_path and "BP_NewMetaHumanCharacter" not in class_path:
            continue
        unreal.log(
            f"NATURALNPC_HUMAN2 actor={actor.get_actor_label()} class={class_path}"
        )
        for component in actor.get_components_by_class(unreal.ActorComponent):
            component_class = component.get_class().get_path_name()
            if "Convai" not in component_class:
                continue
            unreal.log(
                f"NATURALNPC_HUMAN2 actor_component actor={actor.get_actor_label()} "
                f"name={component.get_name()} class={component_class} "
                f"character_id={get_character_id(component)}"
            )

unreal.log("NATURALNPC_HUMAN2_INSPECT_END")
