import unreal


PREFIX = "NATURALNPC_DAILY_PICKUP_VERIFY"
TARGET_MESH_PATH = "/Game/MetaHumans/Human2/Body/SKM_Human2_BodyMesh"
FLOOR_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Pickup_Floor"
)
TABLE_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Pickup_Table"
)
SOURCE_MESHES = {
    "ground": (
        "/Game/DailyAnimation/"
        "motifect_daily_life_complete_v1_0_fbx/Import/"
        "Pickup_object_floor/pick_up_object_floor"
    ),
    "table": (
        "/Game/DailyAnimation/"
        "motifect_daily_life_complete_v1_0_fbx/Import/"
        "Pickup_object_table/pick_up_object_table"
    ),
}
RETARGETERS = {
    "ground": (
        "/Game/ImportedAnimations/DailyPickup/"
        "RTG_DailyPickupFloor_Human2"
    ),
    "table": (
        "/Game/ImportedAnimations/DailyPickup/"
        "RTG_DailyPickupTable_Human2"
    ),
}
SOURCE_RIGS = {
    "ground": "/Game/ImportedAnimations/DailyPickup/IK_DailyPickup_Floor",
    "table": "/Game/ImportedAnimations/DailyPickup/IK_DailyPickup_Table",
}


def require_asset(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset is missing: {path}")
    return asset


target_mesh = require_asset(TARGET_MESH_PATH)
target_skeleton = target_mesh.get_editor_property("skeleton")
animations = {
    "ground": require_asset(FLOOR_PATH),
    "table": require_asset(TABLE_PATH),
}

for variant, animation in animations.items():
    if animation.get_editor_property("skeleton") != target_skeleton:
        raise RuntimeError(f"{variant} animation uses the wrong skeleton")
    if animation.get_play_length() <= 0.0:
        raise RuntimeError(f"{variant} animation has no playable frames")
    unreal.log(
        f"{PREFIX} ASSET variant={variant} "
        f"path={animation.get_path_name()} "
        f"length={animation.get_play_length():.3f} "
        f"frames={animation.data_model_interface.get_number_of_frames()}"
    )

    source_mesh = require_asset(SOURCE_MESHES[variant])
    source_rig = require_asset(SOURCE_RIGS[variant])
    rig_controller = unreal.IKRigController.get_controller(source_rig)
    if str(rig_controller.get_retarget_chain_start_bone("Spine")) != "Spine1":
        raise RuntimeError(f"{variant} Spine chain has the wrong start bone")
    if str(rig_controller.get_retarget_chain_end_bone("Spine")) != "Chest":
        raise RuntimeError(f"{variant} Spine chain does not include Chest")
    if str(rig_controller.get_retarget_chain_start_bone("Neck")) != "Neck1":
        raise RuntimeError(f"{variant} Neck chain has the wrong start bone")
    if str(rig_controller.get_retarget_chain_end_bone("Neck")) != "Neck2":
        raise RuntimeError(f"{variant} Neck chain has the wrong end bone")
    retargeter = require_asset(RETARGETERS[variant])
    controller = unreal.IKRetargeterController.get_controller(retargeter)
    preview_mesh = controller.get_preview_mesh(
        unreal.RetargetSourceOrTarget.SOURCE
    )
    if preview_mesh != source_mesh:
        raise RuntimeError(
            f"{variant} retargeter does not use its reimported source mesh"
        )
    unreal.log(
        f"{PREFIX} SOURCE variant={variant} "
        f"mesh={source_mesh.get_path_name()} "
        f"spine=Spine1-Chest neck=Neck1-Neck2 "
        f"retargeter={retargeter.get_path_name()}"
    )

defaults = unreal.get_default_object(unreal.NPCWorldStateAgentComponent)
configured_floor = defaults.get_editor_property("ground_pickup_animation")
configured_table = defaults.get_editor_property("table_pickup_animation")
if configured_floor.get_path_name().split(".")[0] != FLOOR_PATH:
    raise RuntimeError(f"Ground default is wrong: {configured_floor}")
if configured_table.get_path_name().split(".")[0] != TABLE_PATH:
    raise RuntimeError(f"Table default is wrong: {configured_table}")
if abs(
    defaults.get_editor_property(
        "ground_pickup_effect_trigger_normalized_time"
    )
    - 0.29
) > 0.001:
    raise RuntimeError("Ground pickup contact time is not calibrated")
if abs(
    defaults.get_editor_property(
        "table_pickup_effect_trigger_normalized_time"
    )
    - 0.41
) > 0.001:
    raise RuntimeError("Table pickup contact time is not calibrated")

unreal.log(
    f"{PREFIX} SUCCESS threshold="
    f"{defaults.get_editor_property('table_pickup_minimum_height'):.1f} "
    f"ground={configured_floor.get_path_name()} "
    f"table={configured_table.get_path_name()}"
)
