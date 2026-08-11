import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_NAV_ROUTE_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

npcs = [
    actor
    for actor in actors
    if actor.get_class().get_name() in ("BP_Human1_C", "BP_Human2_C")
]
targets = [
    actor
    for actor in actors
    if actor.get_actor_label() in ("Chair1", "Chair4")
]
if len(npcs) != 2 or len(targets) != 2:
    raise RuntimeError(f"Unexpected route actors: npcs={len(npcs)} targets={len(targets)}")

routes_with_corners = 0
for npc in npcs:
    for target in targets:
        origin, _ = target.get_actor_bounds(False)
        destination = origin - target.get_actor_forward_vector() * 70.0
        destination.z = npc.get_actor_location().z
        path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
            world,
            npc.get_actor_location(),
            destination,
        )
        points = [] if path is None else list(path.get_editor_property("path_points"))
        if path is None or not path.is_valid():
            raise RuntimeError(
                f"No route from {npc.get_actor_label()} to {target.get_actor_label()}"
            )
        if len(points) > 2:
            routes_with_corners += 1
        log(
            f"ROUTE npc={npc.get_actor_label()} target={target.get_actor_label()} "
            f"points={len(points)} values={points}"
        )

if routes_with_corners == 0:
    raise RuntimeError("NavMesh still returns only obstacle-crossing straight routes")

log(f"SUCCESS routes_with_corners={routes_with_corners} total=4")
