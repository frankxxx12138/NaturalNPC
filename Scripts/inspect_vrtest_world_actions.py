import unreal


PREFIX = "NATURALNPC_VRTEST_INSPECT"


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
world = unreal.EditorLevelLibrary.get_editor_world()
subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()

actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()
agents = []
objects = []

for actor in actors:
    components = actor.get_components_by_class(unreal.ActorComponent)
    component_classes = [component.get_class().get_name() for component in components]
    has_agent = "NPCWorldStateAgentComponent" in component_classes
    has_object = "NPCWorldStateObjectComponent" in component_classes
    is_npc = has_agent or any(
        name in ("OpenAIJackComponent", "ConvaiChatbotComponent")
        for name in component_classes
    )
    is_grabbable = any("GrabComponent" in name for name in component_classes)

    if is_npc:
        agents.append(actor)
        log(
            f"NPC label={actor.get_actor_label()} actor={actor.get_name()} "
            f"class={actor.get_class().get_name()} "
            f"location={actor.get_actor_location()} "
            f"components={component_classes}"
        )
        for mesh in actor.get_components_by_class(unreal.SkeletalMeshComponent):
            skeletal_mesh = mesh.get_editor_property("skeletal_mesh")
            anim_class = mesh.get_editor_property("anim_class")
            mesh_path = skeletal_mesh.get_path_name() if skeletal_mesh else "None"
            skeleton = (
                skeletal_mesh.get_editor_property("skeleton")
                if skeletal_mesh
                else None
            )
            skeleton_path = skeleton.get_path_name() if skeleton else "None"
            anim_path = anim_class.get_path_name() if anim_class else "None"
            log(
                f"MESH npc={actor.get_name()} component={mesh.get_name()} "
                f"mesh={mesh_path} skeleton={skeleton_path} "
                f"anim_class={anim_path} hand_r={mesh.does_socket_exist('hand_r')}"
            )

    if has_object or is_grabbable:
        objects.append(actor)

states = subsystem.get_world_state(None, 0.0)
log(
    f"SUMMARY actors={len(actors)} npcs={len(agents)} "
    f"grabbables={len(objects)} registered_objects={len(states)}"
)

for agent_actor in agents:
    agent = agent_actor.get_component_by_class(
        unreal.NPCWorldStateAgentComponent
    )
    if agent is None:
        log(f"AGENT_MISSING npc={agent_actor.get_name()}")
        continue
    pickup_animation = agent.get_editor_property("default_pickup_animation")
    pickup_path = (
        pickup_animation.get_path_name()
        if hasattr(pickup_animation, "get_path_name")
        else str(pickup_animation)
    )
    nearby = subsystem.get_world_state(
        agent_actor,
        agent.get_editor_property("perception_radius"),
    )
    log(
        f"AGENT npc={agent_actor.get_name()} hand_socket="
        f"{agent.get_editor_property('hand_socket')} "
        f"mesh_hint={agent.get_editor_property('hand_mesh_name_hint')} "
        f"pickup_distance={agent.get_editor_property('default_pickup_distance')} "
        f"pickup_animation={pickup_path} visible_objects={len(nearby)}"
    )
    for state in nearby:
        action_ids = [str(action.action_id) for action in state.available_actions]
        log(
            f"VISIBLE npc={agent_actor.get_name()} id={state.object_id} "
            f"category={state.category} distance={state.distance:.1f} "
            f"actions={action_ids}"
        )

log("END")
