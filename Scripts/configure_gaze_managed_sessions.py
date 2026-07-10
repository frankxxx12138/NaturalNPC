import unreal


NPC_PATHS = [
    "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter",
    "/Game/MetaHumans/Human2/BP_Human2",
]


def unique_components(blueprint):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    result = []
    seen = set()
    for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None or obj.get_path_name() in seen:
            continue
        seen.add(obj.get_path_name())
        result.append(obj)
    return result


for asset_path in NPC_PATHS:
    blueprint = unreal.load_asset(asset_path)
    if blueprint is None:
        raise RuntimeError(f"Could not load {asset_path}")

    chatbots = [
        obj
        for obj in unique_components(blueprint)
        if "BP_ConvaiChatbotComponent" in obj.get_class().get_path_name()
    ]
    if len(chatbots) != 1:
        raise RuntimeError(f"{asset_path} has {len(chatbots)} chatbot components")

    chatbot = chatbots[0]
    chatbot.set_editor_property("auto_initialize_session", False)
    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        blueprint, only_if_is_dirty=False
    ):
        raise RuntimeError(f"Could not save {asset_path}")

    unreal.log(
        f"NATURALNPC_GAZE_SESSION_CONFIG asset={asset_path} "
        f"auto_initialize={chatbot.get_editor_property('auto_initialize_session')}"
    )
