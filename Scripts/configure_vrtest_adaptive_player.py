import unreal


MAP_PATH = "/Game/VRTest"
GAME_MODE_CLASS_PATH = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCAdaptiveGameMode"
)
PREFIX = "NATURALNPC_ADAPTIVE_PLAYER_CONFIGURE"


game_mode_class = unreal.load_class(None, GAME_MODE_CLASS_PATH)
if game_mode_class is None:
    raise RuntimeError(f"Could not load {GAME_MODE_CLASS_PATH}")

world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
if world is None:
    raise RuntimeError(f"Could not load {MAP_PATH}")

world.get_world_settings().set_editor_property(
    "default_game_mode", game_mode_class
)

actors = unreal.get_editor_subsystem(
    unreal.EditorActorSubsystem
).get_all_level_actors()
player_starts = [
    actor for actor in actors if isinstance(actor, unreal.PlayerStart)
]
if not player_starts:
    raise RuntimeError("VRTest has no PlayerStart")

if not unreal.EditorLevelLibrary.save_current_level():
    raise RuntimeError("Could not save VRTest")

unreal.log(
    f"{PREFIX} success map={MAP_PATH} "
    f"game_mode={game_mode_class.get_path_name()} "
    f"player_starts={len(player_starts)}"
)
