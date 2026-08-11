import unreal


MAP_PATH = "/Game/VRTest"
PREFIX = "NATURALNPC_SEAT_CONFIG"
EXPECTED_SEAT_COUNT = 10


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = actor_subsystem.get_all_level_actors()

seats = []
for actor in actors:
    label = actor.get_actor_label()
    lower_label = label.lower()
    if not (lower_label.startswith("chair") or lower_label.startswith("barstool")):
        continue

    seat_type = "NPCSeat.BarStool" if lower_label.startswith("barstool") else "NPCSeat.Chair"
    tags = list(actor.get_editor_property("tags"))
    for required_tag in ("NPCSeat", seat_type):
        tag = unreal.Name(required_tag)
        if tag not in tags:
            tags.append(tag)
    actor.set_editor_property("tags", tags)
    seats.append(actor)
    log(
        f"TAGGED label={label} type={seat_type} "
        f"location={actor.get_actor_location()}"
    )

if len(seats) != EXPECTED_SEAT_COUNT:
    raise RuntimeError(
        f"Expected {EXPECTED_SEAT_COUNT} Chair/BarStool actors, found {len(seats)}"
    )

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest after tagging NPC seats")

navigation_system = unreal.NavigationSystemV1.get_navigation_system(world)
if navigation_system is None:
    raise RuntimeError("NavigationSystemV1 is unavailable")

npcs = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_component_by_class(unreal.OpenAIJackComponent) is not None
]
if len(npcs) != 2:
    raise RuntimeError(f"Expected two OpenAI NPCs, found {len(npcs)}")

for npc in npcs:
    reachable = 0
    for seat in seats:
        origin, _ = seat.get_actor_bounds(False)
        forward = seat.get_actor_forward_vector()
        if "barstool" in seat.get_actor_label().lower():
            forward = npc.get_actor_location() - origin
            forward.z = 0.0
            forward = forward.normal()
        approach = origin - forward * 70.0
        approach.z = npc.get_actor_location().z
        path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
            world,
            npc.get_actor_location(),
            approach,
        )
        if path is not None and path.is_valid():
            reachable += 1
    if reachable == 0:
        raise RuntimeError(
            f"No tagged seat is reachable from {npc.get_actor_label()}"
        )
    log(f"REACHABLE npc={npc.get_actor_label()} seats={reachable}")

log(f"SUCCESS seats={len(seats)} npcs={len(npcs)}")
