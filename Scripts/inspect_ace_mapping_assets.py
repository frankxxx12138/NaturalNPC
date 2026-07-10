import unreal


ASSETS = [
    "/NV_ACE_Reference/mh_arkit_mapping_anim_A2F",
    "/NV_ACE_Reference/mh_arkit_mapping_pose_A2F",
    "/Game/MetaHumans/Human2/ACE/ABP_Human2_Face_ACE",
    "/Game/MetaHumans/Human2/Face/SKM_Human2_FaceMesh",
    "/Game/MetaHumans/Common/Face/Face_Archetype_Skeleton",
]


for path in ASSETS:
    asset = unreal.load_asset(path)
    if asset is None:
        unreal.log(f"ACE_MAPPING_ASSET path={path} loaded=0")
        continue

    cls = asset.get_class().get_path_name()
    extra = []
    for prop in ("skeleton", "preview_skeletal_mesh", "target_skeleton"):
        try:
            value = asset.get_editor_property(prop)
            if value is not None:
                extra.append(f"{prop}={value.get_path_name()}")
        except Exception:
            pass
    unreal.log(
        f"ACE_MAPPING_ASSET path={path} loaded=1 class={cls} "
        + " ".join(extra)
    )
