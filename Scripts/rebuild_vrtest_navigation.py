import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_NAV_REBUILD"


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

updated_components = 0
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
        if not component.get_editor_property("can_ever_affect_navigation"):
            component.set_editor_property("can_ever_affect_navigation", True)
            updated_components += 1

navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if navigation_system is None:
    raise RuntimeError("NavigationSystemV1 is unavailable")

navigation_system.on_navigation_bounds_updated(nav_volume)
unreal.SystemLibrary.execute_console_command(world, "RebuildNavigation")

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest after rebuilding navigation")

log(
    f"SUCCESS updated_components={updated_components} "
    f"nav_volume={nav_volume.get_actor_label()}"
)
