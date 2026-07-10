import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
FACE_ANIM_CLASS_PATH = "/Game/MetaHumans/Common/Face/Face_AnimBP.Face_AnimBP_C"
FACE_POST_CLASS_PATH = "/Game/MetaHumans/Common/Face/Face_PostProcess_AnimBP.Face_PostProcess_AnimBP_C"
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


def is_human2_face_component(obj):
    if obj.get_class().get_name() != "SkeletalMeshComponent":
        return False
    if obj.get_name().lower().startswith("face"):
        return True
    try:
        mesh = obj.get_editor_property("skeletal_mesh")
    except Exception:
        mesh = None
    return mesh is not None and "/MetaHumans/Human2/Face/" in mesh.get_path_name()


blueprint = unreal.load_asset(HUMAN2_PATH)
face_anim_class = unreal.load_class(None, FACE_ANIM_CLASS_PATH)
face_post_class = unreal.load_class(None, FACE_POST_CLASS_PATH)

if blueprint is None:
    raise RuntimeError(f"Could not load {HUMAN2_PATH}")
if face_anim_class is None:
    raise RuntimeError(f"Could not load {FACE_ANIM_CLASS_PATH}")
if face_post_class is None:
    raise RuntimeError(f"Could not load {FACE_POST_CLASS_PATH}")

subobjects = unique_subobjects(blueprint)
face_component = None
openai_component = None

for obj in subobjects:
    if is_human2_face_component(obj):
        face_component = obj
    if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
        openai_component = obj

if face_component is None:
    raise RuntimeError("Could not find Human2 Face SkeletalMeshComponent")

try:
    face_component.set_editor_property("animation_mode", unreal.AnimationMode.ANIMATION_BLUEPRINT)
except Exception as exc:
    unreal.log_warning(f"HUMAN2_KAIROS_PIPELINE animation_mode skipped: {exc}")

face_component.set_editor_property("anim_class", face_anim_class)

face_mesh = None
try:
    face_mesh = face_component.get_editor_property("skeletal_mesh")
except Exception:
    face_mesh = None

post_process_set_on = "none"
if face_mesh is not None:
    try:
        face_mesh.set_editor_property("post_process_anim_blueprint", face_post_class)
        unreal.EditorAssetLibrary.save_loaded_asset(face_mesh, only_if_is_dirty=False)
        post_process_set_on = face_mesh.get_path_name()
    except Exception as exc:
        unreal.log_warning(f"HUMAN2_KAIROS_PIPELINE mesh post_process skipped: {exc}")

try:
    face_component.set_editor_property("post_process_anim_blueprint", face_post_class)
    post_process_set_on = face_component.get_path_name()
except Exception as exc:
    unreal.log_warning(f"HUMAN2_KAIROS_PIPELINE component post_process skipped: {exc}")

if openai_component is not None:
    openai_component.set_editor_property("enable_ace_audio2_face", True)
    openai_component.set_editor_property("ace_audio2_face_provider_name", "LocalA2F-Mark")
    openai_component.set_editor_property("enable_ace_direct_morph_bridge", False)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(
    "HUMAN2_KAIROS_PIPELINE_APPLY "
    f"success face_component={face_component.get_name()} "
    f"anim_class={face_anim_class.get_path_name()} "
    f"post_process_set_on={post_process_set_on} "
    f"direct_morph_bridge={int(False)}"
)
