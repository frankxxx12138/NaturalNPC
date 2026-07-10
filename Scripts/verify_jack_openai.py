import unreal


blueprint = unreal.load_asset("/Game/MetaHumans/Human2/BP_Human2")
if blueprint is None:
    raise RuntimeError("Jack Blueprint could not be loaded")

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
classes = set()
seen = set()
openai_component = None
for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
    data = unreal.SubobjectDataBlueprintFunctionLibrary.get_data(handle)
    obj = unreal.SubobjectDataBlueprintFunctionLibrary.get_object(data)
    if obj is None or obj.get_path_name() in seen:
        continue
    seen.add(obj.get_path_name())
    classes.add(obj.get_class().get_path_name())
    if "OpenAIJackComponent" in obj.get_class().get_path_name():
        openai_component = obj

openai_count = sum("OpenAIJackComponent" in path for path in classes)
convai_count = sum("BP_ConvaiChatbotComponent" in path for path in classes)
if openai_count != 1 or convai_count != 0:
    raise RuntimeError(
        f"Unexpected Jack components: openai={openai_count}, convai={convai_count}"
    )

model = openai_component.get_editor_property("model")
ollama_url = openai_component.get_editor_property("ollama_chat_url")
context_length = openai_component.get_editor_property("context_length")
embedding_model = openai_component.get_editor_property("embedding_model")
embedding_url = openai_component.get_editor_property("ollama_embed_url")
recent_turns = openai_component.get_editor_property("max_conversation_turns")
if model != "gemma4:e4b":
    raise RuntimeError(f"Unexpected Jack model: {model}")
if ollama_url != "http://127.0.0.1:11435/api/chat":
    raise RuntimeError(f"Unexpected Ollama URL: {ollama_url}")
if context_length != 4096:
    raise RuntimeError(f"Unexpected context length: {context_length}")
if embedding_model != "embeddinggemma":
    raise RuntimeError(f"Unexpected embedding model: {embedding_model}")
if embedding_url != "http://127.0.0.1:11435/api/embed":
    raise RuntimeError(f"Unexpected embedding URL: {embedding_url}")
if recent_turns != 8:
    raise RuntimeError(f"Unexpected recent turn count: {recent_turns}")

unreal.log(
    f"OPENAI_JACK_VERIFY success openai={openai_count} convai={convai_count} "
    f"model={model} context={context_length} embedding={embedding_model} "
    f"recent={recent_turns} "
    f"url={ollama_url}"
)
