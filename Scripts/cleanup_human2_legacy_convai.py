import unreal


NPC_BP_PATH = "/Game/MetaHumans/Human2/BP_Human2"
LEGACY_CLASS_PATH = "/Script/Convai.ConvaiChatbotComponent"

blueprint = unreal.load_asset(NPC_BP_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {NPC_BP_PATH}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
if not handles:
    raise RuntimeError("Human2 Blueprint has no root subobject")

removed = 0
seen = set()
for handle in handles:
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    if obj.get_class().get_path_name() != LEGACY_CLASS_PATH:
        continue
    deleted = subsystem.delete_subobject(handles[0], handle, blueprint)
    if deleted < 1:
        raise RuntimeError(f"Could not delete legacy component {obj.get_path_name()}")
    removed += 1

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(f"NATURALNPC_HUMAN2_CLEANUP removed={removed}")
