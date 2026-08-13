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
)
LEGACY_FLOOR_LABEL = "NavFloor_Bar"
AGENT_RADIUS = 24.0
AGENT_HEIGHT = 180.0
AGENT_MAX_SLOPE = 44.0
PREFIX = "NATURALNPC_SURFACE_NAV_CONFIGURE"


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
mesh_subsystem = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
actors = actor_subsystem.get_all_level_actors()
actors_by_label = {actor.get_actor_label(): actor for actor in actors}

missing = [label for label in SURFACE_LABELS if label not in actors_by_label]
if missing:
    raise RuntimeError(f"Missing walkable surfaces: {missing}")

legacy_floor = actors_by_label.get(LEGACY_FLOOR_LABEL)
if legacy_floor is not None:
    if not actor_subsystem.destroy_actor(legacy_floor):
        raise RuntimeError(f"Could not remove {LEGACY_FLOOR_LABEL}")
    log(f"REMOVED legacy_floor={LEGACY_FLOOR_LABEL}")

configured_assets = set()
stair_assets = set()
for label in SURFACE_LABELS:
    actor = actors_by_label[label]
    component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if component is None:
        raise RuntimeError(f"Surface has no StaticMeshComponent: {label}")

    actor.set_actor_enable_collision(True)
    component.set_collision_profile_name("BlockAll")
    component.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
    component.set_editor_property("generate_overlap_events", False)
    component.set_editor_property("can_ever_affect_navigation", True)
    component.set_editor_property("fill_collision_underneath_for_navmesh", False)

    mesh = component.get_editor_property("static_mesh")
    if mesh is None:
        raise RuntimeError(f"Surface has no static mesh: {label}")
    body_setup = mesh.get_editor_property("body_setup")
    if body_setup is None:
        raise RuntimeError(f"Surface mesh has no BodySetup: {label}")
    body_setup.set_editor_property(
        "collision_trace_flag",
        unreal.CollisionTraceFlag.CTF_USE_COMPLEX_AS_SIMPLE,
    )
    configured_assets.add(mesh)
    if label.startswith("Stairs"):
        stair_assets.add(mesh)

    origin, extent = actor.get_actor_bounds(False)
    log(
        f"SURFACE label={label} origin={origin} extent={extent} "
        f"mesh={mesh.get_path_name()} collision=BlockAll complex_as_simple=1"
    )

for mesh in stair_assets:
    convex_count = mesh_subsystem.get_convex_collision_count(mesh)
    generated = False
    if convex_count == 0:
        if not mesh_subsystem.set_convex_decomposition_collisions(
            mesh,
            16,
            32,
            100000,
        ):
            raise RuntimeError(
                f"Could not generate stair convex collision: {mesh.get_path_name()}"
            )
        generated = True
        convex_count = mesh_subsystem.get_convex_collision_count(mesh)
    body_setup = mesh.get_editor_property("body_setup")
    body_setup.set_editor_property(
        "collision_trace_flag",
        unreal.CollisionTraceFlag.CTF_USE_SIMPLE_AND_COMPLEX,
    )
    log(
        f"STAIR_COLLISION mesh={mesh.get_path_name()} "
        f"convex_hulls={convex_count} generated={int(generated)}"
    )

# Keep real Bar geometry as Recast obstacles, but do not let movable pickup
# objects or navigation helper volumes carve the static baked surface.
for actor in actor_subsystem.get_all_level_actors():
    try:
        folder = str(actor.get_folder_path())
    except Exception:
        folder = ""
    if not (folder == "Bar" or folder.startswith("Bar/")):
        continue
    if folder.startswith("Bar/Interactables") or folder.startswith(
        "Bar/Navigation"
    ):
        continue
    for component in actor.get_components_by_class(unreal.StaticMeshComponent):
        component.set_editor_property("can_ever_affect_navigation", True)
        component.set_editor_property("fill_collision_underneath_for_navmesh", False)

nav_volume = next(
    (
        actor
        for actor in actor_subsystem.get_all_level_actors()
        if actor.get_class().get_name() == "NavMeshBoundsVolume"
    ),
    None,
)
if nav_volume is None:
    raise RuntimeError("VRTest has no NavMeshBoundsVolume")

recast_actors = unreal.GameplayStatics.get_all_actors_of_class(
    world,
    unreal.RecastNavMesh,
)
if not recast_actors:
    raise RuntimeError("VRTest has no RecastNavMesh")
for recast in recast_actors:
    recast.set_editor_property("agent_radius", AGENT_RADIUS)
    recast.set_editor_property("agent_height", AGENT_HEIGHT)
    recast.set_editor_property("agent_max_slope", AGENT_MAX_SLOPE)
    log(
        f"RECAST name={recast.get_name()} radius={AGENT_RADIUS:.1f} "
        f"height={AGENT_HEIGHT:.1f} slope={AGENT_MAX_SLOPE:.1f}"
    )

for mesh in configured_assets:
    if not unreal.EditorAssetLibrary.save_loaded_asset(
        mesh, only_if_is_dirty=False
    ):
        raise RuntimeError(f"Could not save surface mesh: {mesh.get_path_name()}")

navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if navigation_system is None:
    raise RuntimeError("NavigationSystemV1 is unavailable")
navigation_system.on_navigation_bounds_updated(nav_volume)
unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save surface-based VRTest navigation")

log(
    f"SUCCESS surfaces={len(SURFACE_LABELS)} legacy_floor=0 "
    f"agent_radius={AGENT_RADIUS:.1f} agent_height={AGENT_HEIGHT:.1f}"
)
