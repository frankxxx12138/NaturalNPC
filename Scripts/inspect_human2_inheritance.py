import unreal


blueprint = unreal.load_asset("/Game/MetaHumans/Human2/BP_Human2")
generated_class = blueprint.generated_class()
unreal.log(f"NATURALNPC_HUMAN2_PARENT class={generated_class.get_path_name()}")
cursor = generated_class
depth = 0
while cursor is not None and depth < 12:
    unreal.log(f"NATURALNPC_HUMAN2_PARENT_CHAIN depth={depth} class={cursor.get_path_name()}")
    try:
        cursor = cursor.get_super_class()
    except Exception as error:
        unreal.log(f"NATURALNPC_HUMAN2_PARENT_CHAIN error={error}")
        break
    depth += 1
for property_name in ("super_struct", "super_class"):
    try:
        value = generated_class.get_editor_property(property_name)
        unreal.log(f"NATURALNPC_HUMAN2_PARENT {property_name}={value}")
    except Exception as error:
        unreal.log(f"NATURALNPC_HUMAN2_PARENT {property_name}_error={error}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
for index, handle in enumerate(handles):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or "Convai" not in obj.get_class().get_path_name():
        continue
    unreal.log(
        f"NATURALNPC_HUMAN2_INHERIT index={index} object={obj.get_path_name()} "
        f"class={obj.get_class().get_path_name()} "
        f"inherited={unreal.SubobjectDataBlueprintFunctionLibrary.is_inherited_component(data)}"
    )
