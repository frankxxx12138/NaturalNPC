import unreal


mesh = unreal.load_asset("/Game/MetaHumans/Human2/Face/SKM_Human2_FaceMesh")
if mesh is None:
    raise RuntimeError("Human2 face mesh could not be loaded")

morph_targets = mesh.get_editor_property("morph_targets")
names = [target.get_name() for target in morph_targets if target is not None]

for keyword in (
    "jaw",
    "mouth",
    "lip",
    "eye_blink",
    "eye_widen",
    "squint",
    "brow",
    "cheek",
):
    filtered = [name for name in names if keyword.lower() in name.lower()]
    unreal.log(
        f"HUMAN2_RELEVANT_MORPHS keyword={keyword} "
        f"count={len(filtered)} sample={filtered[:80]}"
    )
