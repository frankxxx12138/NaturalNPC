import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
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
        result.append(obj)
    return result


blueprint = unreal.load_asset(HUMAN2_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {HUMAN2_PATH}")

openai_component = None
for obj in unique_subobjects(blueprint):
    if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
        openai_component = obj
        break

if openai_component is None:
    raise RuntimeError("Human2 OpenAIJackComponent not found")

openai_component.set_editor_property("enable_windows_tts", True)
openai_component.set_editor_property("enable_open_aitts", False)
openai_component.set_editor_property("enable_ace_audio2_face", True)
openai_component.set_editor_property("enable_ace_direct_morph_bridge", False)
openai_component.set_editor_property("windows_voice_name", "Microsoft Huihui Desktop")
openai_component.set_editor_property("windows_voice_rate", 0)
openai_component.set_editor_property("windows_voice_volume", 100)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(
    "HUMAN2_WINDOWS_TTS_APPLY "
    f"success windows_tts={int(openai_component.get_editor_property('enable_windows_tts'))} "
    f"openai_tts={int(openai_component.get_editor_property('enable_open_aitts'))} "
    f"ace={int(openai_component.get_editor_property('enable_ace_audio2_face'))} "
    f"direct_morph_bridge={int(openai_component.get_editor_property('enable_ace_direct_morph_bridge'))} "
    f"voice={openai_component.get_editor_property('windows_voice_name')}"
)
