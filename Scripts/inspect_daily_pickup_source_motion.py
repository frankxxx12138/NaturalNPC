import unreal


PREFIX = "NATURALNPC_DAILY_PICKUP_SOURCE"
SOURCE_MESH_PATH = (
    "/Game/ImportedAnimations/DailyPickup/Daily_Pickup_Floor_Source"
)
ANIMATION_PATH = (
    "/Game/ImportedAnimations/DailyPickup/Daily_Pickup_Floor_Source_Anim"
)


mesh = unreal.load_asset(SOURCE_MESH_PATH)
animation = unreal.load_asset(ANIMATION_PATH)
if mesh is None or animation is None:
    raise RuntimeError("Daily pickup source mesh or animation is missing")

options = unreal.AnimPoseEvaluationOptions()
options.set_editor_property("optional_skeletal_mesh", mesh)
first_pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
    animation,
    0,
    options,
)
bone_names = [str(name) for name in unreal.AnimPoseExtensions.get_bone_names(
    first_pose
)]
unreal.log(f"{PREFIX} BONES names={bone_names}")


def find_bone(*tokens):
    for name in bone_names:
        normalized = name.lower().replace("mixamorig:", "")
        if normalized in tokens:
            return name
    raise RuntimeError(f"Could not find source bone matching {tokens}")


hips_name = find_bone("hips", "pelvis")
right_hand_name = find_bone("righthand", "hand_r")
left_hand_name = find_bone("lefthand", "hand_l")
right_foot_name = find_bone("rightfoot", "foot_r")
left_foot_name = find_bone("leftfoot", "foot_l")


def position(pose, name):
    return unreal.AnimPoseExtensions.get_bone_pose(
        pose,
        name,
        unreal.AnimPoseSpaces.WORLD,
    ).translation


frame_count = animation.data_model_interface.get_number_of_frames()
for frame in (0, 18, 36, 54, 71, 89, 125, 178):
    pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
        animation,
        min(frame, frame_count - 1),
        options,
    )
    unreal.log(
        f"{PREFIX} FRAME frame={frame} "
        f"hips={position(pose, hips_name)} "
        f"right={position(pose, right_hand_name)} "
        f"left={position(pose, left_hand_name)} "
        f"feet=({position(pose, right_foot_name)},"
        f"{position(pose, left_foot_name)})"
    )
