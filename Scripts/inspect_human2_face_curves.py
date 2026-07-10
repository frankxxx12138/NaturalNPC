import unreal


FACE_MESH_PATH = "/Game/MetaHumans/Human2/Face/SKM_Human2_FaceMesh"
ACE_CURVE_NAMES = [
    "JawOpen",
    "MouthClose",
    "MouthFunnel",
    "MouthPucker",
    "MouthSmileLeft",
    "MouthSmileRight",
    "EyeBlinkLeft",
    "EyeBlinkRight",
]


mesh = unreal.load_asset(FACE_MESH_PATH)
if mesh is None:
    raise RuntimeError("Human2 face mesh could not be loaded")

morph_names = []
try:
    morph_targets = mesh.get_editor_property("morph_targets")
    morph_names = [target.get_name() for target in morph_targets if target is not None]
except Exception as exc:
    unreal.log(f"HUMAN2_FACE_CURVES morph_targets_error={exc}")

lower_morphs = {name.lower(): name for name in morph_names}
matches = {
    ace_name: lower_morphs.get(ace_name.lower(), None)
    for ace_name in ACE_CURVE_NAMES
}

unreal.log(
    "HUMAN2_FACE_CURVES "
    f"morph_count={len(morph_names)} "
    f"matches={matches} "
    f"sample={morph_names[:40]}"
)
