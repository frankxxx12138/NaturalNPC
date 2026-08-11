import unreal


MAP_PATH = "/Game/VRTest"
EXPECTED_GAME_MODE = (
    "/Script/NaturalNPCPlayerMode.NaturalNPCAdaptiveGameMode"
)
EXPECTED_VR_PAWN = (
    "/Game/XRFramework/Blueprints/BP_XRPawn.BP_XRPawn_C"
)
EXPECTED_DESKTOP_PAWN = (
    "/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter."
    "BP_FirstPersonCharacter_C"
)
EXPECTED_DESKTOP_CONTROLLER = (
    "/Game/FirstPerson/Blueprints/BP_FirstPersonPlayerController."
    "BP_FirstPersonPlayerController_C"
)
PREFIX = "NATURALNPC_ADAPTIVE_PLAYER_VERIFY"


world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
if world is None:
    raise RuntimeError(f"Could not load {MAP_PATH}")

game_mode_class = world.get_world_settings().get_editor_property(
    "default_game_mode"
)
if game_mode_class is None or game_mode_class.get_path_name() != EXPECTED_GAME_MODE:
    raise RuntimeError(
        f"Wrong game mode: {game_mode_class.get_path_name() if game_mode_class else 'None'}"
    )

game_mode_cdo = unreal.get_default_object(game_mode_class)
checks = {
    "vr_pawn_class": EXPECTED_VR_PAWN,
    "desktop_pawn_class": EXPECTED_DESKTOP_PAWN,
    "desktop_player_controller_class": EXPECTED_DESKTOP_CONTROLLER,
}
for property_name, expected_path in checks.items():
    value = game_mode_cdo.get_editor_property(property_name)
    actual_path = value.get_path_name() if value else "None"
    if actual_path != expected_path:
        raise RuntimeError(
            f"Wrong {property_name}: expected={expected_path} actual={actual_path}"
        )

actors = unreal.get_editor_subsystem(
    unreal.EditorActorSubsystem
).get_all_level_actors()
player_start_count = len(
    [actor for actor in actors if isinstance(actor, unreal.PlayerStart)]
)
if player_start_count < 1:
    raise RuntimeError("VRTest has no PlayerStart")

unreal.log(
    f"{PREFIX} success map={MAP_PATH} mode=Auto "
    f"vr_pawn={EXPECTED_VR_PAWN} desktop_pawn={EXPECTED_DESKTOP_PAWN} "
    f"desktop_controller={EXPECTED_DESKTOP_CONTROLLER} "
    f"player_starts={player_start_count}"
)
