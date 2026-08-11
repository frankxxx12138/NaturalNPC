import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_NAV_OBSTACLES"
FOLDER = "Bar/Navigation/SeatObstacles"
LABEL_PREFIX = "NavObstacle_"
AGENT_MARGIN = 5.0
VERTICAL_EXTENT = 150.0


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

for actor in actors:
    try:
        folder = str(actor.get_folder_path())
    except Exception:
        folder = ""
    if not (folder == "Bar" or folder.startswith("Bar/")):
        continue
    if folder.startswith("Bar/Interactables") or folder.startswith("Bar/Navigation"):
        continue
    for component in actor.get_components_by_class(unreal.StaticMeshComponent):
        component.set_editor_property("fill_collision_underneath_for_navmesh", False)

for actor in list(actor_subsystem.get_all_level_actors()):
    if actor.get_actor_label().startswith(LABEL_PREFIX):
        if not actor_subsystem.destroy_actor(actor):
            raise RuntimeError(f"Could not replace {actor.get_actor_label()}")

seats = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if unreal.Name("NPCSeat") in list(actor.get_editor_property("tags"))
]
if len(seats) != 10:
    raise RuntimeError(f"Expected 10 tagged seats, found {len(seats)}")

created = []
for seat in seats:
    origin, extent = seat.get_actor_bounds(False)
    desired_extent = unreal.Vector(
        extent.x + AGENT_MARGIN,
        extent.y + AGENT_MARGIN,
        VERTICAL_EXTENT,
    )
    desired_center = unreal.Vector(origin.x, origin.y, 250.0)
    volume = actor_subsystem.spawn_actor_from_class(
        unreal.NavModifierVolume,
        desired_center,
    )
    if volume is None:
        raise RuntimeError(f"Could not create obstacle for {seat.get_actor_label()}")

    volume.set_actor_label(f"{LABEL_PREFIX}{seat.get_actor_label()}")
    volume.set_folder_path(FOLDER)
    volume.set_editor_property("area_class", unreal.NavArea_Null)
    _, initial_extent = volume.get_actor_bounds(False)
    if initial_extent.x <= 1.0 or initial_extent.y <= 1.0 or initial_extent.z <= 1.0:
        raise RuntimeError(
            f"Invalid default NavModifierVolume extent for {seat.get_actor_label()}: "
            f"{initial_extent}"
        )
    current_scale = volume.get_actor_scale3d()
    volume.set_actor_scale3d(
        unreal.Vector(
            current_scale.x * desired_extent.x / initial_extent.x,
            current_scale.y * desired_extent.y / initial_extent.y,
            current_scale.z * desired_extent.z / initial_extent.z,
        )
    )
    created.append(volume)
    log(
        f"CREATED label={volume.get_actor_label()} center={desired_center} "
        f"extent={desired_extent}"
    )

nav_volume = next(
    (
        actor
        for actor in actor_subsystem.get_all_level_actors()
        if actor.get_class().get_name() == "NavMeshBoundsVolume"
    ),
    None,
)
navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if nav_volume is None or navigation_system is None:
    raise RuntimeError("Navigation system or NavMeshBoundsVolume is unavailable")

navigation_system.on_navigation_bounds_updated(nav_volume)
unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest navigation obstacles")

log(f"SUCCESS created={len(created)}")
