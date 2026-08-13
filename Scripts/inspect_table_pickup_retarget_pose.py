import unreal


PREFIX = "NATURALNPC_TABLE_RETARGET_POSE"
SOURCE_MESH_PATH = (
    "/Game/DailyAnimation/motifect_daily_life_complete_v1_0_fbx/Import/"
    "Pickup_object_table/pick_up_object_table"
)
SOURCE_ANIMATION_PATH = (
    "/Game/DailyAnimation/motifect_daily_life_complete_v1_0_fbx/Import/"
    "Pickup_object_table/pick_up_object_table_Anim"
)
SOURCE_RIG_PATH = "/Game/ImportedAnimations/DailyPickup/IK_DailyPickup_Table"
RETARGETER_PATH = (
    "/Game/ImportedAnimations/DailyPickup/RTG_DailyPickupTable_Human2"
)
TARGET_MESH_PATH = "/Game/MetaHumans/Human2/Body/SKM_Human2_BodyMesh"
TARGET_ANIMATION_PATH = (
    "/Game/MetaHumans/Human2/Animations/Actions/Human2_Pickup_Table"
)


def require_asset(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset is missing: {path}")
    return asset


def pose_at(animation, mesh, frame):
    options = unreal.AnimPoseEvaluationOptions()
    options.set_editor_property("optional_skeletal_mesh", mesh)
    return unreal.AnimPoseExtensions.get_anim_pose_at_frame(
        animation,
        frame,
        options,
    )


def transform(pose, bone, space):
    return unreal.AnimPoseExtensions.get_bone_pose(pose, bone, space)


source_mesh = require_asset(SOURCE_MESH_PATH)
source_animation = require_asset(SOURCE_ANIMATION_PATH)
source_rig = require_asset(SOURCE_RIG_PATH)
retargeter = require_asset(RETARGETER_PATH)
target_mesh = require_asset(TARGET_MESH_PATH)
target_animation = require_asset(TARGET_ANIMATION_PATH)

source_rig_controller = unreal.IKRigController.get_controller(source_rig)
retarget_controller = unreal.IKRetargeterController.get_controller(retargeter)
target_rig = retarget_controller.get_ik_rig(
    unreal.RetargetSourceOrTarget.TARGET
)
target_rig_controller = unreal.IKRigController.get_controller(target_rig)


def describe_chains(controller):
    descriptions = []
    for chain in controller.get_retarget_chains():
        name = chain.chain_name
        start = controller.get_retarget_chain_start_bone(name)
        end = controller.get_retarget_chain_end_bone(name)
        descriptions.append(f"{name}:{start}-{end}")
    return descriptions


unreal.log(
    f"{PREFIX} SOURCE_CHAINS "
    f"chains={describe_chains(source_rig_controller)}"
)
unreal.log(
    f"{PREFIX} TARGET_CHAINS "
    f"chains={describe_chains(target_rig_controller)}"
)
for op_index in range(retarget_controller.get_num_retarget_ops()):
    op_name = retarget_controller.get_op_name(op_index)
    mappings = []
    for chain in target_rig_controller.get_retarget_chains():
        target_chain = chain.chain_name
        source_chain = retarget_controller.get_source_chain(
            target_chain,
            op_name,
        )
        if str(source_chain) not in {"None", ""}:
            mappings.append(f"{target_chain}<-{source_chain}")
    unreal.log(
        f"{PREFIX} MAPPING op={op_name} mappings={mappings}"
    )

source_bones = (
    "Hips",
    "Spine1",
    "Spine2",
    "Chest",
    "Neck1",
    "Neck2",
    "Head",
    "RightShoulder",
    "RightArm",
    "RightForeArm",
    "RightHand",
)
target_bones = (
    "pelvis",
    "spine_01",
    "spine_02",
    "spine_03",
    "spine_04",
    "spine_05",
    "neck_01",
    "neck_02",
    "head",
    "clavicle_r",
    "upperarm_r",
    "lowerarm_r",
    "hand_r",
)

for normalized in (0.0, 0.2, 0.3, 0.41, 0.5, 0.7, 1.0):
    source_frame = round(
        (source_animation.data_model_interface.get_number_of_frames() - 1)
        * normalized
    )
    target_frame = round(
        (target_animation.data_model_interface.get_number_of_frames() - 1)
        * normalized
    )
    source_pose = pose_at(source_animation, source_mesh, source_frame)
    target_pose = pose_at(target_animation, target_mesh, target_frame)
    source_values = []
    for bone in source_bones:
        world = transform(source_pose, bone, unreal.AnimPoseSpaces.WORLD)
        source_values.append(f"{bone}=P{world.translation} R{world.rotation}")
    target_values = []
    for bone in target_bones:
        world = transform(target_pose, bone, unreal.AnimPoseSpaces.WORLD)
        local = transform(target_pose, bone, unreal.AnimPoseSpaces.LOCAL)
        target_values.append(
            f"{bone}=P{world.translation} LR{local.rotation} "
            f"LT{local.translation}"
        )
    unreal.log(
        f"{PREFIX} SOURCE_POSE normalized={normalized:.2f} "
        + " | ".join(source_values)
    )
    unreal.log(
        f"{PREFIX} TARGET_POSE normalized={normalized:.2f} "
        + " | ".join(target_values)
    )

unreal.log(f"{PREFIX} SUCCESS")
