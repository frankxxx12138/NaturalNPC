import unreal


def public_names(value):
    return sorted(name for name in dir(value) if not name.startswith("_"))


unreal.log("NATURALNPC_INSPECT_BEGIN")
for type_name in [
    "BlueprintEditorLibrary",
    "SubobjectDataSubsystem",
    "SubobjectDataBlueprintFunctionLibrary",
    "KismetEditorUtilities",
]:
    value = getattr(unreal, type_name, None)
    unreal.log(f"NATURALNPC_TYPE {type_name}: {value}")
    if value is not None:
        unreal.log(f"NATURALNPC_MEMBERS {type_name}: {public_names(value)}")

for owner_name, method_name in [
    ("BlueprintEditorLibrary", "reparent_blueprint"),
    ("BlueprintEditorLibrary", "compile_blueprint"),
    ("SubobjectDataSubsystem", "k2_gather_subobject_data_for_blueprint"),
    ("SubobjectDataSubsystem", "create_new_bp_component"),
    ("SubobjectDataSubsystem", "add_new_subobject"),
]:
    owner = getattr(unreal, owner_name)
    method = getattr(owner, method_name)
    unreal.log(f"NATURALNPC_DOC {owner_name}.{method_name}: {method.__doc__}")

params_type = getattr(unreal, "AddNewSubobjectParams", None)
unreal.log(f"NATURALNPC_TYPE AddNewSubobjectParams: {params_type}")
if params_type is not None:
    unreal.log(f"NATURALNPC_DOC AddNewSubobjectParams: {params_type.__doc__}")
    params = params_type()
    unreal.log(f"NATURALNPC_MEMBERS AddNewSubobjectParams: {public_names(params)}")

for asset_path in [
    "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter",
    "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter",
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiPlayerComponent",
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiChatbotComponent",
    "/Convai/Widgets/Chat_WB",
]:
    asset = unreal.load_asset(asset_path)
    unreal.log(f"NATURALNPC_ASSET {asset_path}: {asset}")
    if asset is not None:
        unreal.log(f"NATURALNPC_ASSET_CLASS {asset_path}: {asset.get_class()}")
        if hasattr(asset, "get_editor_property"):
            try:
                unreal.log(
                    f"NATURALNPC_PARENT {asset_path}: "
                    f"{asset.get_editor_property('parent_class')}"
                )
            except Exception as error:
                unreal.log(f"NATURALNPC_PARENT_ERROR {asset_path}: {error}")
        if asset_path.startswith("/Game/"):
            subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
            handles = subsystem.k2_gather_subobject_data_for_blueprint(asset)
            for handle in handles:
                data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
                unreal.log(
                    f"NATURALNPC_SUBOBJECT {asset_path}: "
                    f"name={unreal.SubobjectDataBlueprintFunctionLibrary.get_display_name(data)} "
                    f"class={unreal.SubobjectDataBlueprintFunctionLibrary.get_class(data)} "
                    f"inherited={unreal.SubobjectDataBlueprintFunctionLibrary.is_inherited_component(data)}"
                )

unreal.log("NATURALNPC_INSPECT_END")
