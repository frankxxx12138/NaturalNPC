import unreal


IDLE_SOURCE = (
    "/Game/MetaHumans/Human2/Animations/Locomotion/Human2_MM_Idle"
)
POSE_SOURCE = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_MM_ChargedAttack"
)
PICKUP_OUTPUT = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Pickup_TwoHand"
)
HELD_OUTPUT = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Held_Idle_TwoHand"
)
TARGET_MESH = "/Game/MetaHumans/Human2/Body/SKM_Human2_BodyMesh"
HOLD_POSE_FRAME = 45


def require_asset(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset could not be loaded: {path}")
    return asset


def duplicate_or_load(source_path, output_path):
    asset = unreal.load_asset(output_path)
    if asset is not None:
        return asset
    asset = unreal.EditorAssetLibrary.duplicate_asset(source_path, output_path)
    if asset is None:
        raise RuntimeError(f"Could not duplicate {source_path} to {output_path}")
    return asset


def vector_copy(value):
    return unreal.Vector(value.x, value.y, value.z)


def quat_copy(value):
    return unreal.Quat(value.x, value.y, value.z, value.w)


idle_source = require_asset(IDLE_SOURCE)
pose_source = require_asset(POSE_SOURCE)
target_mesh = require_asset(TARGET_MESH)
pickup = duplicate_or_load(POSE_SOURCE, PICKUP_OUTPUT)
held = duplicate_or_load(IDLE_SOURCE, HELD_OUTPUT)

options = unreal.AnimPoseEvaluationOptions()
options.set_editor_property("optional_skeletal_mesh", target_mesh)
source_pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
    pose_source,
    HOLD_POSE_FRAME,
    options,
)
bone_names = unreal.AnimPoseExtensions.get_bone_names(source_pose)
arm_bones = [
    name
    for name in bone_names
    if str(name) in {
        "clavicle_l",
        "clavicle_r",
        "upperarm_l",
        "upperarm_r",
        "lowerarm_l",
        "lowerarm_r",
        "hand_l",
        "hand_r",
    }
    or any(
        token in str(name)
        for token in (
            "upperarm_corrective",
            "upperarm_bck",
            "upperarm_fwd",
            "upperarm_in",
            "upperarm_out",
            "lowerarm_corrective",
            "lowerarm_twist",
        )
    )
]

key_count = held.data_model_interface.get_number_of_keys()
controller = held.controller
controller.open_bracket("Create Human2 two-hand carry pose", False)
try:
    for bone_name in arm_bones:
        transform = unreal.AnimPoseExtensions.get_bone_pose(
            source_pose,
            bone_name,
            unreal.AnimPoseSpaces.LOCAL,
        )
        positions = [
            vector_copy(transform.translation) for _ in range(key_count)
        ]
        rotations = [quat_copy(transform.rotation) for _ in range(key_count)]
        scales = [vector_copy(transform.scale3d) for _ in range(key_count)]
        if not controller.set_bone_track_keys(
            bone_name,
            positions,
            rotations,
            scales,
            False,
        ):
            raise RuntimeError(f"Could not set carry pose track: {bone_name}")
finally:
    controller.close_bracket(False)

unreal.EditorAssetLibrary.save_loaded_asset(pickup, False)
unreal.EditorAssetLibrary.save_loaded_asset(held, False)

held_pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
    held,
    key_count // 2,
    options,
)
left = unreal.AnimPoseExtensions.get_bone_pose(
    held_pose,
    "hand_l",
    unreal.AnimPoseSpaces.WORLD,
).translation
right = unreal.AnimPoseExtensions.get_bone_pose(
    held_pose,
    "hand_r",
    unreal.AnimPoseSpaces.WORLD,
).translation
distance = (left - right).length()
if not 20.0 <= distance <= 45.0:
    raise RuntimeError(f"Unexpected carry hand spacing: {distance:.2f} cm")

target_skeleton = target_mesh.get_editor_property("skeleton")
for asset in (pickup, held):
    if asset.get_editor_property("skeleton") != target_skeleton:
        raise RuntimeError(f"Wrong skeleton on {asset.get_path_name()}")
    unreal.log(
        "NATURALNPC_CARRY_ANIM_VERIFY "
        f"asset={asset.get_path_name()} length={asset.get_play_length():.3f}"
    )

unreal.log(
    "NATURALNPC_CARRY_ANIM COMPLETE "
    f"pose_frame={HOLD_POSE_FRAME} tracks={len(arm_bones)} "
    f"hand_spacing={distance:.2f}"
)
