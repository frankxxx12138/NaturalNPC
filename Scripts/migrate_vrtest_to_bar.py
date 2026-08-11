import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_BAR_MIGRATE"
BAR_MARGIN_XY = 100.0
BAR_MIN_NAV_Z = -100.0
BAR_MAX_NAV_Z = 600.0


def log(message):
    unreal.log(f"{PREFIX} {message}")


def actor_folder(actor):
    try:
        return str(actor.get_folder_path())
    except Exception:
        return ""


def actor_bounds(actor):
    try:
        return actor.get_actor_bounds(False)
    except Exception:
        return actor.get_actor_location(), unreal.Vector()


def has_grab_component(actor):
    return any(
        "GrabComponent" in component.get_class().get_name()
        for component in actor.get_components_by_class(unreal.ActorComponent)
    )


def is_retained_world_object(actor):
    class_name = actor.get_class().get_name().lower()
    label = actor.get_actor_label().lower()
    name = actor.get_name().lower()
    return (
        class_name == "bp_pistol_c"
        or "pistol" in label
        or "pistol" in name
        or "ball" in label
        or "ball" in name
    )


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

bar_structure = []
original_actors = []
extra_grabbables = []
retained_grabbables = []
nav_volume = None

for actor in actors:
    folder = actor_folder(actor)
    class_name = actor.get_class().get_name()
    is_bar_helper = (
        folder == "Bar/Navigation"
        or folder.startswith("Bar/Navigation/")
        or folder == "Bar/Interactables"
        or folder.startswith("Bar/Interactables/")
    )
    if (folder == "Bar" or folder.startswith("Bar/")) and not is_bar_helper:
        bar_structure.append(actor)
    if folder == "Original" or folder.startswith("Original/"):
        original_actors.append(actor)
    if class_name == "NavMeshBoundsVolume":
        nav_volume = actor
    if has_grab_component(actor):
        if is_retained_world_object(actor):
            retained_grabbables.append(actor)
        else:
            extra_grabbables.append(actor)

if not bar_structure:
    raise RuntimeError("No actors were found in the Bar folder")
if nav_volume is None:
    raise RuntimeError("VRTest has no NavMeshBoundsVolume")
if len(retained_grabbables) != 3:
    labels = [actor.get_actor_label() for actor in retained_grabbables]
    raise RuntimeError(
        f"Expected two pistols and one ball, found {len(labels)}: {labels}"
    )

minimum = unreal.Vector(float("inf"), float("inf"), float("inf"))
maximum = unreal.Vector(float("-inf"), float("-inf"), float("-inf"))
for actor in bar_structure:
    origin, extent = actor_bounds(actor)
    minimum.x = min(minimum.x, origin.x - extent.x)
    minimum.y = min(minimum.y, origin.y - extent.y)
    minimum.z = min(minimum.z, origin.z - extent.z)
    maximum.x = max(maximum.x, origin.x + extent.x)
    maximum.y = max(maximum.y, origin.y + extent.y)
    maximum.z = max(maximum.z, origin.z + extent.z)

target_minimum = unreal.Vector(
    minimum.x - BAR_MARGIN_XY,
    minimum.y - BAR_MARGIN_XY,
    min(minimum.z, BAR_MIN_NAV_Z),
)
target_maximum = unreal.Vector(
    maximum.x + BAR_MARGIN_XY,
    maximum.y + BAR_MARGIN_XY,
    max(maximum.z, BAR_MAX_NAV_Z),
)
target_center = (target_minimum + target_maximum) * 0.5
target_extent = (target_maximum - target_minimum) * 0.5

current_origin, current_extent = actor_bounds(nav_volume)
current_scale = nav_volume.get_actor_scale3d()
if current_extent.x <= 1.0 or current_extent.y <= 1.0 or current_extent.z <= 1.0:
    raise RuntimeError(f"Invalid existing NavMesh bounds extent: {current_extent}")

nav_scale = unreal.Vector(
    current_scale.x * target_extent.x / current_extent.x,
    current_scale.y * target_extent.y / current_extent.y,
    current_scale.z * target_extent.z / current_extent.z,
)
nav_volume.set_actor_location(target_center, False, False)
nav_volume.set_actor_scale3d(nav_scale)
nav_volume.set_actor_label("NavMeshBounds_Bar")
nav_volume.set_folder_path("Bar/Navigation")
navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if navigation_system is None:
    raise RuntimeError("NavigationSystemV1 is unavailable")
navigation_system.on_navigation_bounds_updated(nav_volume)

for actor in retained_grabbables:
    actor.set_folder_path("Bar/Interactables")
    log(
        f"KEEP_GRABBABLE label={actor.get_actor_label()} "
        f"class={actor.get_class().get_name()} location={actor.get_actor_location()}"
    )

for actor in original_actors:
    log(f"DELETE_ORIGINAL label={actor.get_actor_label()} name={actor.get_name()}")
    if not actor_subsystem.destroy_actor(actor):
        raise RuntimeError(f"Could not delete Original actor {actor.get_name()}")

for actor in extra_grabbables:
    log(
        f"DELETE_GRABBABLE label={actor.get_actor_label()} "
        f"name={actor.get_name()} class={actor.get_class().get_name()}"
    )
    if not actor_subsystem.destroy_actor(actor):
        raise RuntimeError(f"Could not delete grabbable actor {actor.get_name()}")

unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest after Bar migration")

updated_origin, updated_extent = actor_bounds(nav_volume)
log(
    f"NAVMESH label={nav_volume.get_actor_label()} center={updated_origin} "
    f"extent={updated_extent} scale={nav_volume.get_actor_scale3d()}"
)
log(
    f"SUCCESS deleted_original={len(original_actors)} "
    f"deleted_grabbables={len(extra_grabbables)} "
    f"retained_grabbables={len(retained_grabbables)}"
)
