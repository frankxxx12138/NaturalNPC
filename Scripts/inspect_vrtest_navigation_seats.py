import unreal


PREFIX = "NATURALNPC_NAV_SEAT_INSPECT"
MAP_PATH = "/Game/VRTest"
ANIMATION_SAMPLES = {
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_SitDown": (
        3.0,
        3.5,
        4.4,
    ),
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_SitIdle": (0.0,),
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_StandUp": (
        0.0,
        1.2,
        2.4,
    ),
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_PickingUp_Small": (
        0.0,
        1.0,
        2.0,
        3.0,
    ),
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_BoxWalk_Carry": (
        0.0,
        0.5,
    ),
}
TARGET_MESH = "/Game/MetaHumans/Human2/Body/SKM_Human2_BodyMesh"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def collision_text(component):
    try:
        enabled = component.get_collision_enabled()
    except Exception:
        enabled = "unavailable"
    try:
        profile = component.get_collision_profile_name()
    except Exception:
        profile = "unavailable"
    return f"collision={enabled} profile={profile}"


def actor_folder(actor):
    try:
        return str(actor.get_folder_path())
    except Exception:
        return ""


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

for actor in actors:
    label = actor.get_actor_label()
    class_name = actor.get_class().get_name()
    folder = actor_folder(actor)
    tags = [str(tag) for tag in actor.tags]
    meshes = actor.get_components_by_class(unreal.StaticMeshComponent)
    mesh_paths = []
    for mesh_component in meshes:
        mesh = mesh_component.get_editor_property("static_mesh")
        if mesh is not None:
            mesh_paths.append(mesh.get_path_name())
    searchable = " ".join([label, class_name, folder, *tags, *mesh_paths]).lower()
    if any(term in searchable for term in ("chair", "barstool", "stool", "seat")):
        origin, extent = actor.get_actor_bounds(False)
        log(
            f"seat_candidate label={label} class={class_name} folder={folder} "
            f"location={actor.get_actor_location()} rotation={actor.get_actor_rotation()} "
            f"bounds_origin={origin} bounds_extent={extent} tags={tags} "
            f"meshes={mesh_paths}"
        )
        for component in meshes:
            log(
                f"seat_component actor={label} name={component.get_name()} "
                f"{collision_text(component)} "
                f"can_affect_navigation={component.get_editor_property('can_ever_affect_navigation')}"
            )

    if "navmesh" in class_name.lower() or "navmesh" in label.lower():
        origin, extent = actor.get_actor_bounds(False)
        log(
            f"navigation_actor label={label} class={class_name} folder={folder} "
            f"location={actor.get_actor_location()} bounds_origin={origin} "
            f"bounds_extent={extent}"
        )

    if folder == "Bar" or folder.startswith("Bar/"):
        for component in meshes:
            origin, extent = actor.get_actor_bounds(False)
            log(
                f"bar_obstacle label={label} class={class_name} folder={folder} "
                f"bounds_origin={origin} bounds_extent={extent} "
                f"{collision_text(component)} "
                f"can_affect_navigation={component.get_editor_property('can_ever_affect_navigation')}"
            )

try:
    recast_actors = unreal.GameplayStatics.get_all_actors_of_class(
        unreal.EditorLevelLibrary.get_editor_world(),
        unreal.RecastNavMesh,
    )
except Exception as error:
    recast_actors = []
    log(f"recast_lookup_failed error={error}")

for recast in recast_actors:
    values = {}
    for property_name in (
        "agent_radius",
        "agent_height",
        "agent_max_slope",
        "agent_max_step_height",
        "cell_size",
        "cell_height",
        "runtime_generation",
    ):
        try:
            values[property_name] = recast.get_editor_property(property_name)
        except Exception:
            values[property_name] = "unavailable"
    log(f"recast name={recast.get_name()} properties={values}")

for actor in actors:
    if actor.get_class().get_name() not in ("BP_Human1_C", "BP_Human2_C"):
        continue
    jack = actor.get_component_by_class(unreal.OpenAIJackComponent)
    if jack is not None:
        log(
            f"npc_navigation_settings actor={actor.get_actor_label()} "
            f"collision_radius={jack.get_editor_property('action_collision_radius')} "
            f"collision_half_height={jack.get_editor_property('action_collision_half_height')} "
            f"repath_interval={jack.get_editor_property('action_navigation_repath_interval_seconds')} "
            f"stall_seconds={jack.get_editor_property('action_navigation_stall_seconds')}"
        )
    root = actor.get_editor_property("root_component")
    log(
        f"npc actor={actor.get_actor_label()} location={actor.get_actor_location()} "
        f"rotation={actor.get_actor_rotation()} root={root.get_name()} "
        f"root_class={root.get_class().get_name()} {collision_text(root)}"
    )
    for component in actor.get_components_by_class(unreal.SceneComponent):
        if component.get_class().get_name() not in (
            "SkeletalMeshComponent",
            "CapsuleComponent",
            "BoxComponent",
            "SphereComponent",
        ):
            continue
        parent = component.get_attach_parent()
        log(
            f"npc_component actor={actor.get_actor_label()} "
            f"name={component.get_name()} class={component.get_class().get_name()} "
            f"parent={parent.get_name() if parent else 'none'} "
            f"relative={component.get_editor_property('relative_location')} "
            f"{collision_text(component)}"
        )
        if isinstance(component, unreal.SkeletalMeshComponent):
            mesh = component.get_editor_property("skeletal_mesh")
            log(
                f"npc_mesh actor={actor.get_actor_label()} name={component.get_name()} "
                f"asset={mesh.get_path_name() if mesh else 'none'} "
                f"foot_l={component.get_socket_location('foot_l')} "
                f"foot_r={component.get_socket_location('foot_r')}"
            )

target_mesh = unreal.load_asset(TARGET_MESH)
options = unreal.AnimPoseEvaluationOptions()
options.set_editor_property("optional_skeletal_mesh", target_mesh)
options.set_editor_property("should_retarget", False)
for animation_path, sample_times in ANIMATION_SAMPLES.items():
    animation = unreal.load_asset(animation_path)
    if animation is None:
        log(f"missing_animation path={animation_path}")
        continue
    for sample_time in sample_times:
        pose = unreal.AnimPoseExtensions.get_anim_pose_at_time(
            animation,
            min(sample_time, animation.get_play_length()),
            options,
        )
        values = {}
        for bone in ("root", "pelvis", "foot_l", "foot_r", "ball_l", "ball_r"):
            transform = unreal.AnimPoseExtensions.get_bone_pose(
                pose,
                unreal.Name(bone),
                unreal.AnimPoseSpaces.WORLD,
            )
            values[bone] = transform.translation
        log(
            f"pose animation={animation_path} time={sample_time:.2f} "
            f"root={values['root']} pelvis={values['pelvis']} "
            f"foot_l={values['foot_l']} foot_r={values['foot_r']} "
            f"ball_l={values['ball_l']} ball_r={values['ball_r']}"
        )

world = unreal.EditorLevelLibrary.get_editor_world()
route_targets = [
    actor
    for actor in actors
    if actor.get_actor_label() in ("Chair1", "Chair4")
]
for npc in [
    actor
    for actor in actors
    if actor.get_class().get_name() in ("BP_Human1_C", "BP_Human2_C")
]:
    for target in route_targets:
        target_origin, _ = target.get_actor_bounds(False)
        target_forward = target.get_actor_forward_vector()
        destination = target_origin - target_forward * 70.0
        destination.z = npc.get_actor_location().z
        path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
            world,
            npc.get_actor_location(),
            destination,
        )
        points = [] if path is None else list(path.get_editor_property("path_points"))
        log(
            f"route npc={npc.get_actor_label()} target={target.get_actor_label()} "
            f"valid={bool(path is not None and path.is_valid())} "
            f"destination={destination} points={points}"
        )

log("COMPLETE")
