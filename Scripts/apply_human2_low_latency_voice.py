import unreal


HUMAN2_PATH = "/Game/MetaHumans/Human2/BP_Human2"
OPENAI_CLASS_PATH = "/Script/OpenAIJackNPC.OpenAIJackComponent"


LOW_LATENCY_INSTRUCTIONS = (
    "You are Jack Thompson, a grounded and observant bartender. "
    "Always reply in natural English unless the player explicitly asks you to "
    "speak another language. For voice replies, usually answer in one or two "
    "natural sentences. If the player asks for recommendations, comparisons, "
    "or explanations, you may answer in two or three concise sentences. Stay "
    "in character and never mention being an AI or language model."
)


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


blueprint = unreal.load_asset(HUMAN2_PATH)
if blueprint is None:
    raise RuntimeError(f"Could not load {HUMAN2_PATH}")

openai_component = None
for obj in unique_subobjects(blueprint):
    if obj.get_class().get_path_name() == OPENAI_CLASS_PATH:
        openai_component = obj
        break

if openai_component is None:
    raise RuntimeError("Human2 OpenAIJackComponent not found")

openai_component.set_editor_property("enable_http_tts", True)
openai_component.set_editor_property("enable_windows_tts", False)
openai_component.set_editor_property("enable_open_aitts", False)
openai_component.set_editor_property("enable_streaming_responses", True)
openai_component.set_editor_property("context_length", 4096)
openai_component.set_editor_property("enable_ace_audio2_face", True)
openai_component.set_editor_property("enable_ace_direct_morph_bridge", False)
openai_component.set_editor_property("pre_allocate_ace_audio2_face_resources", True)
openai_component.set_editor_property("warm_up_ace_audio2_face_on_begin_play", True)
openai_component.set_editor_property("ace_warmup_delay_seconds", 0.75)
openai_component.set_editor_property("ace_warmup_duration_seconds", 0.35)
openai_component.set_editor_property("ace_realtime_initial_chunk_seconds", 0.03)
openai_component.set_editor_property("ace_audio_buffer_seconds", 0.02)
openai_component.set_editor_property("http_tts_url", "http://127.0.0.1:8020/tts")
openai_component.set_editor_property("http_tts_language_code", "en-US")
openai_component.set_editor_property("enable_queued_speech", True)
openai_component.set_editor_property("enable_instant_acknowledgement", True)
openai_component.set_editor_property("instant_acknowledgement_text", "Sure.")
openai_component.set_editor_property("instant_acknowledgement_delay_seconds", 0.75)
openai_component.set_editor_property("max_speech_segment_characters", 80)
openai_component.set_editor_property("first_speech_segment_sentences", 1)
openai_component.set_editor_property("enable_first_speech_segment_soft_boundary", True)
openai_component.set_editor_property("first_speech_segment_soft_min_characters", 36)
openai_component.set_editor_property("max_sentences_per_speech_segment", 2)
openai_component.set_editor_property("speech_segment_padding_seconds", 0.03)
openai_component.set_editor_property("max_concurrent_speech_requests", 2)
openai_component.set_editor_property("stop_windows_stt_after_recognized_speech", True)
openai_component.set_editor_property("enable_keyboard_push_to_talk", True)
openai_component.set_editor_property("enable_http_stt", True)
openai_component.set_editor_property("http_stt_start_url", "http://127.0.0.1:8030/stt/start")
openai_component.set_editor_property("http_stt_stop_url", "http://127.0.0.1:8030/stt/stop")
openai_component.set_editor_property("http_stt_language", "auto")
openai_component.set_editor_property("keyboard_push_to_talk_release_grace_seconds", 0.15)
openai_component.set_editor_property("windows_stt_min_characters", 2)
openai_component.set_editor_property("max_output_tokens", 64)
openai_component.set_editor_property("max_conversation_turns", 6)
openai_component.set_editor_property("character_instructions", LOW_LATENCY_INSTRUCTIONS)

unreal.BlueprintEditorLibrary.compile_blueprint(blueprint)
if not unreal.EditorAssetLibrary.save_loaded_asset(blueprint, only_if_is_dirty=False):
    raise RuntimeError("Could not save Human2 Blueprint")

unreal.log(
    "HUMAN2_LOW_LATENCY_VOICE_APPLY "
    f"success http_tts={int(openai_component.get_editor_property('enable_http_tts'))} "
    f"stream={int(openai_component.get_editor_property('enable_streaming_responses'))} "
    f"context={openai_component.get_editor_property('context_length')} "
    f"queue={int(openai_component.get_editor_property('enable_queued_speech'))} "
    f"ack={openai_component.get_editor_property('instant_acknowledgement_text')} "
    f"ack_delay={openai_component.get_editor_property('instant_acknowledgement_delay_seconds')} "
    f"segment_chars={openai_component.get_editor_property('max_speech_segment_characters')} "
    f"first_segment_sentences={openai_component.get_editor_property('first_speech_segment_sentences')} "
    f"first_soft_boundary={int(openai_component.get_editor_property('enable_first_speech_segment_soft_boundary'))} "
    f"first_soft_min_chars={openai_component.get_editor_property('first_speech_segment_soft_min_characters')} "
    f"sentences_per_segment={openai_component.get_editor_property('max_sentences_per_speech_segment')} "
    f"tts_parallel={openai_component.get_editor_property('max_concurrent_speech_requests')} "
    f"stt_stop_after_result={int(openai_component.get_editor_property('stop_windows_stt_after_recognized_speech'))} "
    f"ptt={int(openai_component.get_editor_property('enable_keyboard_push_to_talk'))} "
    f"http_stt={int(openai_component.get_editor_property('enable_http_stt'))} "
    f"http_stt_language={openai_component.get_editor_property('http_stt_language')} "
    f"ptt_grace={openai_component.get_editor_property('keyboard_push_to_talk_release_grace_seconds')} "
    f"stt_min_chars={openai_component.get_editor_property('windows_stt_min_characters')} "
    f"max_output_tokens={openai_component.get_editor_property('max_output_tokens')} "
    f"max_turns={openai_component.get_editor_property('max_conversation_turns')} "
    f"ace_prewarm={int(openai_component.get_editor_property('pre_allocate_ace_audio2_face_resources'))} "
    f"ace_silent_warmup={int(openai_component.get_editor_property('warm_up_ace_audio2_face_on_begin_play'))} "
    f"ace_warmup_delay={openai_component.get_editor_property('ace_warmup_delay_seconds')} "
    f"ace_warmup_duration={openai_component.get_editor_property('ace_warmup_duration_seconds')} "
    f"ace_initial_chunk={openai_component.get_editor_property('ace_realtime_initial_chunk_seconds')} "
    f"ace_audio_buffer={openai_component.get_editor_property('ace_audio_buffer_seconds')} "
    f"url={openai_component.get_editor_property('http_tts_url')}"
)
