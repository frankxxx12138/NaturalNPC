import unreal


PLAYER_PATH = "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"
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


def character_id(component):
    for property_name in ("character_id", "characterID", "CharacterID"):
        try:
            return component.get_editor_property(property_name)
        except Exception:
            pass
    return None


player = unreal.load_asset(PLAYER_PATH)
if player is None:
    raise RuntimeError("Player Blueprint could not be loaded")
unreal.BlueprintEditorLibrary.compile_blueprint(player)

player_classes = [obj.get_class().get_path_name() for obj in unique_components(player)]
gaze_count = sum("ConvaiGazeSelectorComponent" in path for path in player_classes)
convai_player_count = sum("BP_ConvaiPlayerComponent" in path for path in player_classes)
if gaze_count != 1 or convai_player_count != 1:
    raise RuntimeError(
        f"Unexpected player components: gaze={gaze_count}, convai={convai_player_count}"
    )

ids = []
for npc_path in NPC_PATHS:
    blueprint = unreal.load_asset(npc_path)
    if blueprint is None:
        raise RuntimeError(f"NPC Blueprint could not be loaded: {npc_path}")
    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
    chatbots = [
        obj
        for obj in unique_components(blueprint)
        if "BP_ConvaiChatbotComponent" in obj.get_class().get_path_name()
    ]
    if len(chatbots) != 1:
        raise RuntimeError(f"{npc_path} has {len(chatbots)} new chatbot components")
    if chatbots[0].get_editor_property("auto_initialize_session"):
        raise RuntimeError(f"{npc_path} still auto-initializes its session")
    value = character_id(chatbots[0])
    if not value:
        raise RuntimeError(f"{npc_path} has an empty Character ID")
    ids.append(value)

if len(set(ids)) != len(ids):
    raise RuntimeError(f"NPC Character IDs are not unique: {ids}")

unreal.log(
    f"NATURALNPC_GAZE_VERIFY success gaze={gaze_count} "
    f"convai_player={convai_player_count} npc_ids={ids}"
)
