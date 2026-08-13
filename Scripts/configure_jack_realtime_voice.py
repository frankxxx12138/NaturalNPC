import unreal


JACK_BLUEPRINT_PATH = "/Game/MetaHumans/Human2/BP_Human2"
OPENAI_COMPONENT_CLASS = "/Script/OpenAIJackNPC.OpenAIJackComponent"
JACK_VOICE = "cedar"


blueprint = unreal.load_asset(JACK_BLUEPRINT_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {JACK_BLUEPRINT_PATH}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
openai_component = None
seen = set()
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() == OPENAI_COMPONENT_CLASS:
        openai_component = obj
        break

if openai_component is None:
    raise RuntimeError("BP_Human2 is missing OpenAIJackComponent")

openai_component.set_editor_property("open_ai_realtime_voice", JACK_VOICE)
unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(
    blueprint, only_if_is_dirty=False
):
    raise RuntimeError("Could not save BP_Human2")

configured_voice = openai_component.get_editor_property(
    "open_ai_realtime_voice"
)
if configured_voice != JACK_VOICE:
    raise RuntimeError(f"Unexpected Jack voice after save: {configured_voice}")

unreal.log(
    "JACK_REALTIME_VOICE_CONFIGURED "
    f"success voice={configured_voice} blueprint={JACK_BLUEPRINT_PATH}"
)
