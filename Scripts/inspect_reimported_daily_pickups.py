import unreal


PREFIX = "NATURALNPC_REIMPORTED_PICKUP"
ASSETS = {
    "ground": {
        "mesh": (
            "/Game/DailyAnimation/"
            "motifect_daily_life_complete_v1_0_fbx/Import/"
            "Pickup_object_floor/pick_up_object_floor"
        ),
        "animation": (
            "/Game/DailyAnimation/"
            "motifect_daily_life_complete_v1_0_fbx/Import/"
            "Pickup_object_floor/pick_up_object_floor_Anim"
        ),
    },
    "table": {
        "mesh": (
            "/Game/DailyAnimation/"
            "motifect_daily_life_complete_v1_0_fbx/Import/"
            "Pickup_object_table/pick_up_object_table"
        ),
        "animation": (
            "/Game/DailyAnimation/"
            "motifect_daily_life_complete_v1_0_fbx/Import/"
            "Pickup_object_table/pick_up_object_table_Anim"
        ),
    },
}


def require_asset(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset is missing: {path}")
    return asset


def find_bone(bone_names, *candidates):
    for bone_name in bone_names:
        normalized = bone_name.lower().replace("mixamorig:", "")
        if normalized in candidates:
            return bone_name
    raise RuntimeError(f"Could not find any of {candidates}")


def bone_position(pose, bone_name):
    return unreal.AnimPoseExtensions.get_bone_pose(
        pose,
        bone_name,
        unreal.AnimPoseSpaces.WORLD,
    ).translation


for variant, paths in ASSETS.items():
    mesh = require_asset(paths["mesh"])
    animation = require_asset(paths["animation"])
    mesh_skeleton = mesh.get_editor_property("skeleton")
    animation_skeleton = animation.get_editor_property("skeleton")
    if mesh_skeleton != animation_skeleton:
        raise RuntimeError(f"{variant} mesh and animation skeletons differ")

    options = unreal.AnimPoseEvaluationOptions()
    options.set_editor_property("optional_skeletal_mesh", mesh)
    frame_count = animation.data_model_interface.get_number_of_frames()
    first_pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
        animation,
        0,
        options,
    )
    bone_names = [
        str(name)
        for name in unreal.AnimPoseExtensions.get_bone_names(first_pose)
    ]
    hips = find_bone(bone_names, "hips", "pelvis")
    right_hand = find_bone(bone_names, "righthand", "hand_r")
    left_hand = find_bone(bone_names, "lefthand", "hand_l")
    right_foot = find_bone(bone_names, "rightfoot", "foot_r")
    left_foot = find_bone(bone_names, "leftfoot", "foot_l")

    samples = []
    for frame in range(frame_count):
        pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
            animation,
            frame,
            options,
        )
        samples.append(
            (
                frame,
                bone_position(pose, hips),
                bone_position(pose, right_hand),
                bone_position(pose, left_hand),
                bone_position(pose, right_foot),
                bone_position(pose, left_foot),
            )
        )

    first = samples[0]
    if variant == "ground":
        contact = max(
            samples,
            key=lambda sample: (
                sample[2].y
                + sample[3].y
                - first[2].y
                - first[3].y
            )
            * 0.5,
        )
    else:
        contact = max(
            samples,
            key=lambda sample: sample[2].y - first[2].y,
        )

    frame, pelvis, right, left, foot_r, foot_l = contact
    normalized = frame / max(1, frame_count - 1)
    unreal.log(
        f"{PREFIX} RESULT variant={variant} "
        f"mesh={mesh.get_path_name()} "
        f"skeleton={mesh_skeleton.get_path_name()} "
        f"animation={animation.get_path_name()} "
        f"length={animation.get_play_length():.3f} "
        f"frames={frame_count} contact={frame}/{frame_count - 1} "
        f"normalized={normalized:.4f} pelvis={pelvis} "
        f"right={right} left={left} feet=({foot_r},{foot_l})"
    )

unreal.log(f"{PREFIX} SUCCESS variants={len(ASSETS)}")
