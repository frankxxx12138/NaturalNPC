import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_BAR_COLLISION_VERIFY"
DIRECT_GOAL = unreal.Vector(-1260.61, -42.92, 150.0)
CAPSULE_RADIUS = 34.0
CAPSULE_HALF_HEIGHT = 90.0


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()
npc = next(
    actor
    for actor in actors
    if actor.get_class().get_name() == "BP_Human1_C"
)

start = npc.get_actor_location()
goal = unreal.Vector(DIRECT_GOAL.x, DIRECT_GOAL.y, start.z)
sweep_z = max(start.z, goal.z) + CAPSULE_HALF_HEIGHT + 2.0
sweep_start = unreal.Vector(start.x, start.y, sweep_z)
sweep_end = unreal.Vector(goal.x, goal.y, sweep_z)
def capsule_hit(object_types, trace_complex):
    outcome = unreal.SystemLibrary.capsule_trace_single_for_objects(
        world,
        sweep_start,
        sweep_end,
        CAPSULE_RADIUS,
        CAPSULE_HALF_HEIGHT,
        object_types,
        trace_complex,
        [npc],
        unreal.DrawDebugTrace.NONE,
        True,
    )
    return None if outcome is None else outcome.to_tuple()


static_simple = capsule_hit(
    [unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY1],
    False,
)
static_complex = capsule_hit(
    [unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY1],
    True,
)
runtime_hit = capsule_hit(
    [
        unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY1,
        unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY2,
    ],
    False,
)


def hit_label(fields):
    if fields is None or not fields[0] or fields[9] is None:
        return "none"
    return fields[9].get_actor_label()


if runtime_hit is None or not runtime_hit[0] or runtime_hit[9] is None:
    raise RuntimeError(
        "A direct NPC capsule sweep crossed the bar without any runtime hit"
    )

blocker = runtime_hit[9]
blocker_label = blocker.get_actor_label()
if blocker_label in (
    "Floor1",
    "Floor2",
    "Floor3",
    "Stairs1",
    "Stairs2",
    "Stairs3",
    "Stairs4",
):
    raise RuntimeError(f"Direct sweep only hit walkable surface: {blocker_label}")

path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
    world,
    start,
    goal,
)
points = [] if path is None else list(path.get_editor_property("path_points"))
if path is None or not path.is_valid() or len(points) < 2:
    raise RuntimeError("No Recast route was found around the bar")

log(
    f"SUCCESS npc={npc.get_actor_label()} blocker={blocker_label} "
    f"static_simple={hit_label(static_simple)} "
    f"static_complex={hit_label(static_complex)} "
    f"start_penetrating={int(runtime_hit[1])} "
    f"hit_time={runtime_hit[2]:.3f} path_points={len(points)} "
    f"points={points} start={start} goal={goal}"
)
