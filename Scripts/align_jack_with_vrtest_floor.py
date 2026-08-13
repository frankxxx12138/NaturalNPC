import unreal


PREFIX = "NATURALNPC_ALIGN_JACK"
MAP_PATH = "/Game/VRTest"
JACK_LABEL = "BP_Human2"
EXPECTED_FLOOR_Z = 150.0


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
matches = [
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_actor_label() == JACK_LABEL
]

if len(matches) != 1:
    raise RuntimeError(f"Expected one {JACK_LABEL}, found {matches}")

jack = matches[0]
old_location = jack.get_actor_location()
new_location = unreal.Vector(
    old_location.x,
    old_location.y,
    EXPECTED_FLOOR_Z,
)

if not jack.set_actor_location(new_location, False, False):
    raise RuntimeError(
        f"Could not move {JACK_LABEL} from {old_location} to {new_location}"
    )

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest after aligning Jack")

log(f"SUCCESS old={old_location} new={jack.get_actor_location()}")
