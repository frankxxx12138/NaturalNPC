import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_BAR_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def actor_folder(actor):
    try:
        return str(actor.get_folder_path())
    except Exception:
        return ""


def has_grab_component(actor):
    return any(
        "GrabComponent" in component.get_class().get_name()
        for component in actor.get_components_by_class(unreal.ActorComponent)
    )


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

original_actors = [
    actor
    for actor in actors
    if actor_folder(actor) == "Original"
    or actor_folder(actor).startswith("Original/")
]
if original_actors:
    raise RuntimeError(
        "Original folder still contains actors: "
        + ", ".join(actor.get_actor_label() for actor in original_actors)
    )

grabbables = [actor for actor in actors if has_grab_component(actor)]
grabbable_labels = sorted(actor.get_actor_label() for actor in grabbables)
expected_labels = ["BP_Pistol_00", "BP_Pistol_01", "SM_Ball_01"]
if grabbable_labels != expected_labels:
    raise RuntimeError(
        f"Unexpected grabbables: expected={expected_labels} actual={grabbable_labels}"
    )

nav_volumes = [
    actor
    for actor in actors
    if actor.get_class().get_name() == "NavMeshBoundsVolume"
]
if len(nav_volumes) != 1:
    raise RuntimeError(f"Expected one NavMeshBoundsVolume, found {len(nav_volumes)}")
nav_volume = nav_volumes[0]
nav_origin, nav_extent = nav_volume.get_actor_bounds(False)
nav_minimum = nav_origin - nav_extent
nav_maximum = nav_origin + nav_extent
expected_minimum = unreal.Vector(-2200.0, -1000.0, -100.0)
expected_maximum = unreal.Vector(100.0, 700.0, 600.0)
if (
    nav_minimum.x > expected_minimum.x + 1.0
    or nav_minimum.y > expected_minimum.y + 1.0
    or nav_minimum.z > expected_minimum.z + 1.0
    or nav_maximum.x < expected_maximum.x - 1.0
    or nav_maximum.y < expected_maximum.y - 1.0
    or nav_maximum.z < expected_maximum.z - 1.0
):
    raise RuntimeError(
        f"NavMesh bounds do not cover Bar: min={nav_minimum} max={nav_maximum}"
    )

subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
if subsystem is None:
    raise RuntimeError("NPCWorldStateSubsystem is unavailable")
subsystem.refresh_auto_discovery()
states = subsystem.get_world_state(None, 0.0)
categories = {}
for state in states:
    category = str(state.category)
    categories[category] = categories.get(category, 0) + 1
if len(states) != 3 or categories != {"pistol": 2, "ball": 1}:
    details = [
        f"{state.object_id}:{state.category}"
        for state in states
    ]
    raise RuntimeError(
        f"Unexpected runtime world state: categories={categories} objects={details}"
    )

unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")
npcs = [
    actor
    for actor in actors
    if actor.get_component_by_class(unreal.OpenAIJackComponent) is not None
]
ball = next(actor for actor in grabbables if "Ball" in actor.get_actor_label())
path_result = "not_tested"
if npcs:
    try:
        path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
            world,
            npcs[0].get_actor_location(),
            ball.get_actor_location(),
        )
        if path is None:
            path_result = "none"
        else:
            path_points = path.get_editor_property("path_points")
            is_valid = path.is_valid()
            path_result = f"valid={int(is_valid)} points={len(path_points)}"
            if not is_valid or len(path_points) < 2:
                raise RuntimeError(f"Navigation path is not valid: {path_result}")
    except Exception as exc:
        raise RuntimeError(f"Navigation path verification failed: {exc}") from exc

log(
    f"SUCCESS original=0 grabbables={grabbable_labels} "
    f"world_state={categories} nav_min={nav_minimum} nav_max={nav_maximum} "
    f"path={path_result}"
)
