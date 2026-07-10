import unreal


JACK_PATH = "/Game/MetaHumans/Human2/BP_Human2"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"

blueprint = unreal.load_asset(JACK_PATH)
openai_class = unreal.load_class(None, OPENAI_CLASS_PATH)
if blueprint is None or openai_class is None:
    raise RuntimeError("Jack Blueprint or OpenAI Jack component could not be loaded")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
if not handles:
    raise RuntimeError("Jack Blueprint has no root subobject")

seen = set()
convai_handles = []
has_openai = False
openai_component = None
for handle in handles:
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    class_path = obj.get_class().get_path_name()
    if "BP_ConvaiChatbotComponent" in class_path:
        convai_handles.append(handle)
    if class_path == OPENAI_CLASS_PATH:
        has_openai = True
        openai_component = obj

for handle in convai_handles:
    deleted = subsystem.delete_subobject(handles[0], handle, blueprint)
    if deleted < 1:
        raise RuntimeError("Could not remove Jack's Convai chatbot component")

if not has_openai:
    refreshed_handles = subsystem.k2_gather_subobject_data_for_blueprint(blueprint)
    params = unreal.AddNewSubobjectParams(
        blueprint_context=blueprint,
        new_class=openai_class,
        parent_handle=refreshed_handles[0],
        conform_transform_to_parent=True,
        skip_mark_blueprint_modified=False,
    )
    new_handle, fail_reason = subsystem.add_new_subobject(params)
    if not unreal.SubobjectDataBlueprintFunctionLibrary.is_handle_valid(new_handle):
        raise RuntimeError(f"Could not add OpenAI Jack component: {fail_reason}")
    subsystem.rename_subobject(new_handle, unreal.Text("OpenAIJack"))
    new_data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(new_handle)
    openai_component = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(
        new_data
    )

if openai_component is None:
    raise RuntimeError("Could not access Jack's local AI component")

openai_component.set_editor_property("model", "gemma4:e4b")
openai_component.set_editor_property(
    "ollama_chat_url",
    "http://127.0.0.1:11435/api/chat",
)
openai_component.set_editor_property("context_length", 4096)
openai_component.set_editor_property("max_conversation_turns", 8)
openai_component.set_editor_property("embedding_model", "embeddinggemma")
openai_component.set_editor_property(
    "ollama_embed_url",
    "http://127.0.0.1:11435/api/embed",
)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Jack Blueprint")

unreal.log(
    f"OPENAI_JACK_MIGRATE success removed_convai={len(convai_handles)} "
    f"already_had_openai={has_openai}"
)
