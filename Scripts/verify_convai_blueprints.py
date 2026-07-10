import unreal


ASSETS = [
    (
        "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter",
        "BP_ConvaiPlayerComponent",
    ),
    (
        "/Game/MetaHumans/NewMetaHumanCharacter/BP_NewMetaHumanCharacter",
        "BP_ConvaiChatbotComponent",
    ),
]

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
unreal.log("NATURALNPC_VERIFY_BEGIN")

for asset_path, expected_component in ASSETS:
    blueprint = unreal.load_asset(asset_path)
    if blueprint is None:
        raise RuntimeError(f"Could not load {asset_path}")

    unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
    handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
    component_paths = []
    component_objects = []
    for handle in handles:
        data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
        obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
        if obj is not None:
            component_paths.append(obj.get_class().get_path_name())
            component_objects.append(obj.get_path_name())

    if not any(expected_component in path for path in component_paths):
        raise RuntimeError(
            f"{asset_path} is missing expected component {expected_component}"
        )

    unreal.log(
        f"NATURALNPC_VERIFY asset={asset_path} "
        f"unique_objects={sorted(set(component_objects))}"
    )

chat_component = unreal.load_asset(
    "/Convai/ConvaiConveniencePack/ConvaiBPComponent/BP_ConvaiPlayerComponent"
)
chat_widget = unreal.load_asset("/Convai/Widgets/Chat_WB")
if chat_component is None or chat_widget is None:
    raise RuntimeError("Convai player component or Chat_WB could not be loaded")

unreal.log("NATURALNPC_VERIFY_END")
