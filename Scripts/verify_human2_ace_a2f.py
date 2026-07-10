import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
ACE_CURVE_CLASS_PATH = "/Script/ACERuntime.ACEAudioCurveSourceComponent"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"


blueprint = unreal.load_asset(HUMAN2_PATH)
if blueprint is None:
    raise RuntimeError("Human2 Blueprint could not be loaded")

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
seen = set()
ace_components = []
openai_component = None
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    class_path = obj.get_class().get_path_name()
    if class_path == ACE_CURVE_CLASS_PATH:
        ace_components.append(obj)
    if class_path == OPENAI_CLASS_PATH:
        openai_component = obj

if len(ace_components) != 1:
    raise RuntimeError(f"Unexpected ACE Audio Curve Source count: {len(ace_components)}")

ace_component = ace_components[0]
buffer_length = ace_component.get_editor_property("buffer_length_in_seconds")
if abs(buffer_length - 0.1) > 0.001:
    raise RuntimeError(f"Unexpected ACE buffer length: {buffer_length}")

if openai_component is None:
    raise RuntimeError("Human2 is missing OpenAIJackComponent")

ace_enabled = openai_component.get_editor_property("enable_ace_audio2_face")
provider = openai_component.get_editor_property("ace_audio2_face_provider_name")
if not ace_enabled:
    raise RuntimeError("OpenAIJackComponent ACE Audio2Face is disabled")
if str(provider) != "LocalA2F-Mark":
    raise RuntimeError(f"Unexpected ACE provider: {provider}")

unreal.log(
    "HUMAN2_ACE_A2F_VERIFY "
    f"success ace_components={len(ace_components)} "
    f"buffer={buffer_length:.3f} provider={provider}"
)
