import unreal


MAP_PATH = "/Game/VRTest"
SURFACE_LABELS = (
    "Floor1",
    "Floor2",
    "Floor3",
    "Stairs1",
    "Stairs2",
    "Stairs3",
    "Stairs4",
    "NavFloor_Bar",
)
PREFIX = "NATURALNPC_WALKABLE_INSPECT"


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
mesh_subsystem = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
actors = actor_subsystem.get_all_level_actors()
actors_by_label = {actor.get_actor_label(): actor for actor in actors}

for label in SURFACE_LABELS:
    actor = actors_by_label.get(label)
    if actor is None:
        log(f"MISSING label={label}")
        continue
    origin, extent = actor.get_actor_bounds(False)
    component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if component is None:
        log(
            f"SURFACE label={label} class={actor.get_class().get_name()} "
            f"origin={origin} extent={extent} static_mesh=none"
        )
        continue
    mesh = component.get_editor_property("static_mesh")
    simple_count = 0 if mesh is None else mesh_subsystem.get_simple_collision_count(mesh)
    try:
        complex_count = mesh_subsystem.get_convex_collision_count(mesh)
    except Exception:
        complex_count = "unavailable"
    try:
        collision_complexity = mesh.get_editor_property(
            "body_setup"
        ).get_editor_property("collision_trace_flag")
    except Exception:
        collision_complexity = "unavailable"
    try:
        section_zero_collision = mesh_subsystem.is_section_collision_enabled(
            mesh, 0, 0
        )
    except Exception:
        section_zero_collision = "unavailable"
    log(
        f"SURFACE label={label} folder={actor.get_folder_path()} "
        f"location={actor.get_actor_location()} origin={origin} extent={extent} "
        f"rotation={actor.get_actor_rotation()} scale={actor.get_actor_scale3d()} "
        f"actor_collision={int(actor.get_actor_enable_collision())} "
        f"component_collision={component.get_collision_enabled()} "
        f"profile={component.get_collision_profile_name()} "
        f"affects_nav={int(component.get_editor_property('can_ever_affect_navigation'))} "
        f"fill_under={int(component.get_editor_property('fill_collision_underneath_for_navmesh'))} "
        f"mesh={mesh.get_path_name() if mesh else 'none'} "
        f"simple_collision={simple_count} convex_collision={complex_count} "
        f"complexity={collision_complexity} "
        f"section0_collision={section_zero_collision}"
    )

for actor in actors:
    if actor.get_class().get_name() == "NavMeshBoundsVolume":
        origin, extent = actor.get_actor_bounds(False)
        log(
            f"NAV_BOUNDS label={actor.get_actor_label()} "
            f"origin={origin} extent={extent}"
        )

try:
    recast_actors = unreal.GameplayStatics.get_all_actors_of_class(
        unreal.EditorLevelLibrary.get_editor_world(),
        unreal.RecastNavMesh,
    )
except Exception as error:
    recast_actors = []
    log(f"RECAST_LOOKUP_FAILED error={error}")

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
    log(f"RECAST name={recast.get_name()} properties={values}")

try:
    nav_cdo = unreal.get_default_object(unreal.NavigationSystemV1)
    cdo_agents = nav_cdo.get_editor_property("supported_agents")
    log(f"NAV_CDO_SUPPORTED_AGENTS count={len(cdo_agents)} values={cdo_agents}")
    for index, agent in enumerate(cdo_agents):
        log(
            f"NAV_CDO_AGENT index={index} tuple={agent.to_tuple()} "
            f"text={agent.export_text()}"
        )
except Exception as error:
    log(f"NAV_CDO_INSPECT_FAILED error={error}")

try:
    nav_system = unreal.NavigationSystemV1.get_navigation_system(
        unreal.EditorLevelLibrary.get_editor_world()
    )
    world_agents = nav_system.get_editor_property("supported_agents")
    log(f"NAV_WORLD_SUPPORTED_AGENTS count={len(world_agents)} values={world_agents}")
    for index, agent in enumerate(world_agents):
        log(
            f"NAV_WORLD_AGENT index={index} tuple={agent.to_tuple()} "
            f"text={agent.export_text()}"
        )
except Exception as error:
    log(f"NAV_WORLD_INSPECT_FAILED error={error}")

log("COMPLETE")
