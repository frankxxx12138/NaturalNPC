import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_BAR_NAV_FINALIZE"
NAV_FLOOR_LABEL = "NavFloor_Bar"
NAV_FLOOR_LOCATION = unreal.Vector(-1050.0, -150.0, 145.0)
NAV_FLOOR_SCALE = unreal.Vector(21.0, 15.0, 0.1)


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

nav_volume = next(
    (
        actor
        for actor in actors
        if actor.get_class().get_name() == "NavMeshBoundsVolume"
    ),
    None,
)
if nav_volume is None:
    raise RuntimeError("VRTest has no NavMeshBoundsVolume")

for actor in actors:
    if actor.get_actor_label() == NAV_FLOOR_LABEL:
        if not actor_subsystem.destroy_actor(actor):
            raise RuntimeError("Could not replace the existing Bar navigation floor")

cube_mesh = unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
if cube_mesh is None:
    raise RuntimeError("Could not load /Engine/BasicShapes/Cube")

nav_floor = actor_subsystem.spawn_actor_from_class(
    unreal.StaticMeshActor, NAV_FLOOR_LOCATION
)
if nav_floor is None:
    raise RuntimeError("Could not create NavFloor_Bar")

nav_floor.set_actor_label(NAV_FLOOR_LABEL)
nav_floor.set_folder_path("Bar/Navigation")
nav_floor.set_actor_scale3d(NAV_FLOOR_SCALE)
nav_floor.set_actor_hidden_in_game(True)
nav_floor.set_actor_enable_collision(True)

nav_floor_component = nav_floor.get_component_by_class(unreal.StaticMeshComponent)
nav_floor_component.set_static_mesh(cube_mesh)
nav_floor_component.set_collision_profile_name("BlockAll")
nav_floor_component.set_editor_property("can_ever_affect_navigation", True)
nav_floor_component.set_visibility(False, True)

navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if navigation_system is None:
    raise RuntimeError("NavigationSystemV1 is unavailable")
navigation_system.on_navigation_bounds_updated(nav_volume)
unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")

npcs = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_component_by_class(unreal.OpenAIJackComponent) is not None
]
ball = next(
    (
        actor
        for actor in actor_subsystem.get_all_level_actors()
        if actor.get_actor_label() == "SM_Ball_01"
    ),
    None,
)
if len(npcs) != 2 or ball is None:
    raise RuntimeError(f"Expected two NPCs and one ball; npcs={len(npcs)} ball={ball}")

for npc in npcs:
    path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
        world, npc.get_actor_location(), ball.get_actor_location()
    )
    if path is None:
        raise RuntimeError(f"No navigation path object for {npc.get_actor_label()}")
    points = path.get_editor_property("path_points")
    if not path.is_valid() or len(points) < 2:
        raise RuntimeError(
            f"Invalid navigation path for {npc.get_actor_label()}: points={points}"
        )
    log(
        f"PATH npc={npc.get_actor_label()} valid=1 points={len(points)} "
        f"start={points[0]} end={points[-1]}"
    )

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest with the Bar navigation floor")

floor_origin, floor_extent = nav_floor.get_actor_bounds(False)
log(
    f"SUCCESS floor={NAV_FLOOR_LABEL} origin={floor_origin} "
    f"extent={floor_extent} visible={nav_floor_component.is_visible()}"
)
