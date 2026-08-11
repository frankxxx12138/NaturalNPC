import unreal


BLUEPRINT_PATHS = {
    "Human1": "/Game/MetaHumans/Human1/BP_Human1",
    "Human2": "/Game/MetaHumans/Human2/BP_Human2",
    "Player": "/Game/XRFramework/Blueprints/BP_XRPawn",
}
RELEVANT_CLASS_MARKERS = (
    "OpenAIJack",
    "ConversationGovernance",
    "ConversationListener",
    "Speech",
    "AudioCapture",
)
DIALOGUE_PROPERTY_NAMES = (
    "npcid",
    "model",
    "memory_file_name",
    "subtitle_speaker_name",
    "http_tts_voice",
    "enable_keyboard_push_to_talk",
    "enable_http_stt",
    "enable_http_tts",
    "enable_windows_tts",
    "enable_autonomous_listening",
    "autonomous_listening_radius",
    "maximum_autonomous_response_characters",
    "autonomous_knowledge_summary",
    "autonomous_interest_summary",
)


def gather_component_templates(blueprint):
    subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    result = []
    seen = set()
    for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is None:
            continue
        object_path = obj.get_path_name()
        if object_path in seen:
            continue
        seen.add(object_path)
        if isinstance(obj, unreal.ActorComponent):
            result.append(obj)
    return result


def is_relevant(class_path):
    return any(marker in class_path for marker in RELEVANT_CLASS_MARKERS)


def resolve_component_npc_id(component):
    explicit_id = str(component.get_editor_property("npcid")).strip()
    if explicit_id and explicit_id.lower() != "none":
        return explicit_id

    speaker_name = str(
        component.get_editor_property("subtitle_speaker_name")
    ).strip()
    return speaker_name if speaker_name else "<owner-fallback>"


def inspect_blueprint(label, asset_path):
    blueprint = unreal.load_asset(asset_path)
    if blueprint is None:
        unreal.log_error(
            f"THREE_PERSON_INSPECT missing_blueprint label={label} "
            f"path={asset_path}"
        )
        return

    components = gather_component_templates(blueprint)
    relevant = [
        component
        for component in components
        if is_relevant(component.get_class().get_path_name())
    ]
    unreal.log(
        f"THREE_PERSON_INSPECT blueprint={label} "
        f"component_count={len(components)} relevant_count={len(relevant)}"
    )
    for component in relevant:
        class_path = component.get_class().get_path_name()
        unreal.log(
            f"THREE_PERSON_INSPECT blueprint_component={label} "
            f"name={component.get_name()} class={class_path}"
        )
        if "OpenAIJackComponent" not in class_path:
            continue
        for property_name in DIALOGUE_PROPERTY_NAMES:
            try:
                value = component.get_editor_property(property_name)
            except Exception as error:
                unreal.log_warning(
                    f"THREE_PERSON_INSPECT unavailable_property={label} "
                    f"name={property_name} error={error}"
                )
                continue
            unreal.log(
                f"THREE_PERSON_INSPECT dialogue_property={label} "
                f"name={property_name} value={value}"
            )
        instructions = component.get_editor_property("character_instructions")
        compact_instructions = " ".join(instructions.split())
        unreal.log(
            f"THREE_PERSON_INSPECT dialogue_property={label} "
            f"name=character_instructions value={compact_instructions}"
        )
        unreal.log(
            f"THREE_PERSON_INSPECT dialogue_property={label} "
            f"name=resolved_npc_id value={resolve_component_npc_id(component)}"
        )


def inspect_level():
    world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
    if world is None:
        unreal.log_error("THREE_PERSON_INSPECT missing_map=/Game/VRTest")
        return

    for actor in unreal.EditorLevelLibrary.get_all_level_actors():
        class_path = actor.get_class().get_path_name()
        if "BP_Human1" not in class_path and "BP_Human2" not in class_path:
            continue

        components = actor.get_components_by_class(unreal.ActorComponent)
        relevant = [
            component
            for component in components
            if is_relevant(component.get_class().get_path_name())
        ]
        unreal.log(
            f"THREE_PERSON_INSPECT level_actor={actor.get_actor_label()} "
            f"class={class_path} location={actor.get_actor_location()} "
            f"relevant_count={len(relevant)}"
        )
        for component in relevant:
            unreal.log(
                f"THREE_PERSON_INSPECT actor_component="
                f"{actor.get_actor_label()} name={component.get_name()} "
                f"class={component.get_class().get_path_name()}"
            )


unreal.log("THREE_PERSON_INSPECT begin")
for blueprint_label, blueprint_path in BLUEPRINT_PATHS.items():
    inspect_blueprint(blueprint_label, blueprint_path)
inspect_level()
unreal.log("THREE_PERSON_INSPECT end")
