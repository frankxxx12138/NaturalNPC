import unreal


PREFIX = "NATURALNPC_PICKUP_NPC_COMPARE"
MAP_PATH = "/Game/VRTest"
NPC_LABELS = {"BP_Human1", "BP_Human2"}


def log(message):
    unreal.log(f"{PREFIX} {message}")


def path_name(value):
    if value is None:
        return "None"
    if hasattr(value, "get_path_name"):
        return value.get_path_name()
    return str(value)


def safe_property(obj, name):
    try:
        return obj.get_editor_property(name)
    except Exception as exc:
        return f"<{type(exc).__name__}>"


def inspect_mesh(actor, mesh):
    skeletal_mesh = safe_property(mesh, "skeletal_mesh")
    skeleton = (
        safe_property(skeletal_mesh, "skeleton")
        if hasattr(skeletal_mesh, "get_editor_property")
        else None
    )
    parent = mesh.get_attach_parent()
    leader = safe_property(mesh, "leader_pose_component")
    log(
        f"MESH npc={actor.get_actor_label()} component={mesh.get_name()} "
        f"parent={parent.get_name() if parent else 'None'} "
        f"relative_location={safe_property(mesh, 'relative_location')} "
        f"relative_rotation={safe_property(mesh, 'relative_rotation')} "
        f"relative_scale={safe_property(mesh, 'relative_scale3d')} "
        f"world_location={mesh.get_world_location()} "
        f"mesh={path_name(skeletal_mesh)} skeleton={path_name(skeleton)} "
        f"animation_mode={safe_property(mesh, 'animation_mode')} "
        f"anim_class={path_name(safe_property(mesh, 'anim_class'))} "
        f"leader={path_name(leader)}"
    )


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_actor_label() in NPC_LABELS
]

if len(actors) != 2:
    raise RuntimeError(f"Expected BP_Human1 and BP_Human2, found {actors}")

world_state = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
world_state.refresh_auto_discovery()

for actor in sorted(actors, key=lambda item: item.get_actor_label()):
    log(
        f"NPC label={actor.get_actor_label()} actor={actor.get_name()} "
        f"class={actor.get_class().get_path_name()} "
        f"location={actor.get_actor_location()} rotation={actor.get_actor_rotation()}"
    )

    agent = unreal.NPCWorldStateBlueprintLibrary.ensure_npc_world_state_agent(
        actor
    )
    if agent is None:
        raise RuntimeError(f"{actor.get_actor_label()} has no world-state agent")

    for property_name in (
        "default_pickup_animation",
        "ground_pickup_animation",
        "table_pickup_animation",
        "table_pickup_minimum_height",
        "ground_pickup_effect_trigger_normalized_time",
        "table_pickup_effect_trigger_normalized_time",
        "b_enable_adaptive_pickup_ik",
        "adaptive_pickup_ik_blend_window",
        "adaptive_pickup_pelvis_influence",
        "adaptive_pickup_max_pelvis_offset",
        "adaptive_pickup_max_arm_stretch",
    ):
        log(
            f"AGENT npc={actor.get_actor_label()} property={property_name} "
            f"value={path_name(safe_property(agent, property_name))}"
        )

    for mesh in actor.get_components_by_class(unreal.SkeletalMeshComponent):
        inspect_mesh(actor, mesh)

    for state in world_state.get_world_state(actor, 0.0):
        if str(state.category) not in {"pistol", "ball"}:
            continue
        animation = agent.get_pickup_animation_for_target(state.actor)
        log(
            f"RESOLVE npc={actor.get_actor_label()} object={state.object_id} "
            f"category={state.category} object_location={state.location} "
            f"relative_height={state.location.z - actor.get_actor_location().z:.1f} "
            f"animation={path_name(animation)}"
        )

log("SUCCESS")
