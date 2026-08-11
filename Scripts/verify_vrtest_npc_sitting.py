import unreal


PREFIX = "NATURALNPC_SIT_VERIFY"
MAP_PATH = "/Game/VRTest"
ANIMATION_PATHS = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_SitDown",
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_SitIdle",
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_StandUp",
)


def log(message):
    unreal.log(f"{PREFIX} {message}")


def unpack(outcome):
    if isinstance(outcome, tuple):
        return bool(outcome[0]), outcome[1] if len(outcome) > 1 else ""
    return bool(outcome), ""


animations = []
for path in ANIMATION_PATHS:
    animation = unreal.load_asset(path)
    if animation is None:
        raise RuntimeError(f"Missing sitting animation: {path}")
    if animation.get_editor_property("enable_root_motion"):
        raise RuntimeError(f"Root motion must be disabled: {path}")
    if animation.get_editor_property("force_root_lock"):
        raise RuntimeError(f"Root lock must be disabled: {path}")
    animations.append(animation)

target_skeleton = animations[0].get_editor_property("skeleton")
unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
world = unreal.EditorLevelLibrary.get_editor_world()
seats = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if unreal.Name("NPCSeat") in list(actor.get_editor_property("tags"))
]
if len(seats) != 10:
    raise RuntimeError(f"Expected 10 tagged NPC seats, found {len(seats)}")

npcs = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_class().get_name() in ("BP_Human1_C", "BP_Human2_C")
]
if len(npcs) < 2:
    raise RuntimeError(f"Expected Human1 and Human2 in VRTest, found {len(npcs)}")

for npc in npcs:
    jack = npc.get_component_by_class(unreal.OpenAIJackComponent)
    if jack is None:
        raise RuntimeError(f"NPC is missing OpenAIJackComponent: {npc.get_name()}")

    configured_paths = (
        jack.get_editor_property("sit_down_animation"),
        jack.get_editor_property("sit_idle_animation"),
        jack.get_editor_property("stand_up_animation"),
    )
    if any(value is None for value in configured_paths):
        raise RuntimeError(f"NPC has an empty sitting animation: {npc.get_name()}")

    handled, reply = unpack(jack.try_execute_movement_command("sit down"))
    if not handled or not jack.is_executing_move_command():
        raise RuntimeError(
            f"Sit command failed for {npc.get_name()}: reply={reply}"
        )
    if jack.is_sitting():
        raise RuntimeError(
            f"NPC sat before reaching a real seat: {npc.get_name()}"
        )

    destination = jack.get_move_command_destination()
    path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
        world,
        npc.get_actor_location(),
        destination,
    )
    if path is None or not path.is_valid():
        raise RuntimeError(
            f"Seat approach is not navigable for {npc.get_name()}: {destination}"
        )

    mesh_skeletons = []
    for mesh in npc.get_components_by_class(unreal.SkeletalMeshComponent):
        if "face" in mesh.get_name().lower():
            continue
        skeletal_mesh = mesh.get_editor_property("skeletal_mesh")
        skeleton = (
            skeletal_mesh.get_editor_property("skeleton")
            if skeletal_mesh is not None
            else None
        )
        if skeleton is not None:
            mesh_skeletons.append(skeleton.get_path_name())
    jack.stop_following_player()
    if jack.is_executing_move_command():
        raise RuntimeError(f"Seat approach did not cancel for {npc.get_name()}")

    log(
        f"npc={npc.get_name()} destination={destination} "
        f"mesh_skeletons={sorted(set(mesh_skeletons))} "
        f"target_skeleton={target_skeleton.get_path_name()}"
    )

log(
    f"SUCCESS npcs={len(npcs)} seats={len(seats)} "
    f"animations={len(animations)}"
)
