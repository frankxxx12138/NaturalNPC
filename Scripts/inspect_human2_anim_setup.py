import unreal


blueprint = unreal.load_asset("/Game/MetaHumans/Human2/BP_Human2")
if blueprint is None:
    raise RuntimeError("Human2 Blueprint could not be loaded")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
seen = set()
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    class_name = obj.get_class().get_name()
    if class_name != "SkeletalMeshComponent":
        continue

    def safe_property(name):
        try:
            value = obj.get_editor_property(name)
            return "None" if value is None else value.get_path_name()
        except Exception as exc:
            return f"<{exc}>"

    mesh = safe_property("skeletal_mesh")
    anim_class = safe_property("anim_class")
    post_process = safe_property("post_process_anim_blueprint")
    unreal.log(
        "HUMAN2_ANIM_SETUP "
        f"component={obj.get_name()} mesh={mesh} "
        f"anim_class={anim_class} post_process={post_process}"
    )
