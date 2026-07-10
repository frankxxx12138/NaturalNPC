import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
EXPECTED_FACE_ANIM = "/Game/MetaHumans/Common/Face/Face_AnimBP.Face_AnimBP_C"
EXPECTED_FACE_POST = "/Game/MetaHumans/Common/Face/Face_PostProcess_AnimBP.Face_PostProcess_AnimBP_C"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"


blueprint = unreal.load_asset(HUMAN2_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {HUMAN2_PATH}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
seen = set()
face_component = None
openai_component = None

for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())

    if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
        openai_component = obj

    if obj.get_class().get_name() != "SkeletalMeshComponent":
        continue
    if obj.get_name().lower().startswith("face"):
        face_component = obj

if face_component is None:
    raise RuntimeError("Human2 Face component not found")
if openai_component is None:
    raise RuntimeError("OpenAIJackComponent not found")

anim_class = face_component.get_editor_property("anim_class")
anim_path = "None" if anim_class is None else anim_class.get_path_name()
if anim_path != EXPECTED_FACE_ANIM:
    raise RuntimeError(f"Unexpected Human2 face anim class: {anim_path}")

face_mesh = face_component.get_editor_property("skeletal_mesh")
if face_mesh is None:
    raise RuntimeError("Human2 Face component has no skeletal mesh")

post_class = face_mesh.get_editor_property("post_process_anim_blueprint")
post_path = "None" if post_class is None else post_class.get_path_name()
if post_path != EXPECTED_FACE_POST:
    raise RuntimeError(f"Unexpected Human2 face post process: {post_path}")

direct_bridge = openai_component.get_editor_property("enable_ace_direct_morph_bridge")
if direct_bridge:
    raise RuntimeError("ACE direct morph bridge is still enabled")

unreal.log(
    "HUMAN2_KAIROS_PIPELINE_VERIFY "
    f"success anim_class={anim_path} "
    f"post_process={post_path} "
    f"direct_morph_bridge={int(direct_bridge)}"
)
