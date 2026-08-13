import unreal


PREFIX = "NATURALNPC_DAILY_PICKUP_CONTACT"
TARGET_MESH_PATH = "/Game/MetaHumans/Human2/Body/SKM_Human2_BodyMesh"
ANIMATIONS = {
    "ground": (
        "/Game/MetaHumans/Human2/Animations/Actions/"
        "Human2_Pickup_Floor"
    ),
    "table": (
        "/Game/MetaHumans/Human2/Animations/Actions/"
        "Human2_Pickup_Table"
    ),
}


def require_asset(path):
    asset = unreal.load_asset(path)
    if asset is None:
        raise RuntimeError(f"Required asset is missing: {path}")
    return asset


def bone_position(pose, bone_name):
    return unreal.AnimPoseExtensions.get_bone_pose(
        pose,
        bone_name,
        unreal.AnimPoseSpaces.WORLD,
    ).translation


mesh = require_asset(TARGET_MESH_PATH)
options = unreal.AnimPoseEvaluationOptions()
options.set_editor_property("optional_skeletal_mesh", mesh)

for variant, path in ANIMATIONS.items():
    animation = require_asset(path)
    frame_count = animation.data_model_interface.get_number_of_frames()
    samples = []
    for frame in range(frame_count):
        pose = unreal.AnimPoseExtensions.get_anim_pose_at_frame(
            animation,
            frame,
            options,
        )
        right = bone_position(pose, "hand_r")
        left = bone_position(pose, "hand_l")
        pelvis = bone_position(pose, "pelvis")
        foot_r = bone_position(pose, "foot_r")
        foot_l = bone_position(pose, "foot_l")
        samples.append((frame, right, left, pelvis, foot_r, foot_l))

    initial_right = samples[0][1]
    initial_left = samples[0][2]
    if variant == "ground":
        contact = max(
            samples,
            key=lambda sample: (
                sample[1].y
                + sample[2].y
                - initial_right.y
                - initial_left.y
            ) * 0.5,
        )
    else:
        contact = max(
            samples,
            key=lambda sample: sample[1].y - initial_right.y,
        )

    frame, right, left, pelvis, foot_r, foot_l = contact
    normalized = frame / max(1, frame_count - 1)
    unreal.log(
        f"{PREFIX} RESULT variant={variant} frame={frame}/{frame_count - 1} "
        f"normalized={normalized:.4f} right={right} left={left} "
        f"pelvis_z={pelvis.z:.2f} feet_z=({foot_r.z:.2f},{foot_l.z:.2f})"
    )

    for sample_index in range(0, 11):
        sample_frame = round((frame_count - 1) * sample_index / 10)
        _, sample_right, sample_left, sample_pelvis, _, _ = samples[
            sample_frame
        ]
        unreal.log(
            f"{PREFIX} SAMPLE variant={variant} frame={sample_frame} "
            f"normalized={sample_frame / max(1, frame_count - 1):.2f} "
            f"right={sample_right} left={sample_left} "
            f"pelvis_z={sample_pelvis.z:.2f}"
        )
