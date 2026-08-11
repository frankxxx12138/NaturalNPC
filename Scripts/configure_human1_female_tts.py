import unreal


HUMAN1_PATH = "/Game/MetaHumans/Human1/BP_Human1"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"
VOICE_PROMPT = "Saved/VoicePrompts/Oliver_Female_Reference.wav"


blueprint = unreal.load_asset(HUMAN1_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {HUMAN1_PATH}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
openai_component = None
seen = set()

for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
        openai_component = obj
        break

if openai_component is None:
    raise RuntimeError("Human1 is missing OpenAIJackComponent")

openai_component.set_editor_property("enable_http_tts", True)
openai_component.set_editor_property("enable_windows_tts", False)
openai_component.set_editor_property("enable_open_aitts", False)
openai_component.set_editor_property(
    "http_tts_url",
    "http://127.0.0.1:8020/tts",
)
openai_component.set_editor_property("http_tts_voice", VOICE_PROMPT)
openai_component.set_editor_property("http_tts_language_code", "en-US")
openai_component.set_editor_property("http_tts_sample_rate_hz", 44100)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(
    blueprint,
    only_if_is_dirty=False,
):
    raise RuntimeError("Could not save Human1 Blueprint")

unreal.log(
    "HUMAN1_FEMALE_TTS_CONFIG "
    f"success http_tts={int(openai_component.get_editor_property('enable_http_tts'))} "
    f"windows_tts={int(openai_component.get_editor_property('enable_windows_tts'))} "
    f"openai_tts={int(openai_component.get_editor_property('enable_open_aitts'))} "
    f"voice={openai_component.get_editor_property('http_tts_voice')} "
    f"url={openai_component.get_editor_property('http_tts_url')} "
    f"language={openai_component.get_editor_property('http_tts_language_code')} "
    f"sample_rate={openai_component.get_editor_property('http_tts_sample_rate_hz')}"
)
