import unreal


PREFIX = "NATURALNPC_BENCH_MIGRATE"
DESTINATION_CONTENT = "D:/UE_Projects/NaturalNPC/Content"
PACKAGES = (
    "/Game/Characters/UEFN_Mannequin/Animations/Interactions/Bench/"
    "M_interaction_bench_into_F_Lfoot",
    "/Game/Characters/UEFN_Mannequin/Animations/Interactions/Bench/"
    "M_interaction_bench_idle_loop",
    "/Game/Characters/UEFN_Mannequin/Animations/Interactions/Bench/"
    "M_interaction_bench_out_to_stand",
    "/Game/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin",
    "/Game/Characters/UEFN_Mannequin/Rigs/IK_UEFN_Mannequin",
)


def log(message):
    unreal.log(f"{PREFIX} {message}")


missing = [path for path in PACKAGES if unreal.load_asset(path) is None]
if missing:
    raise RuntimeError(f"Missing source packages: {missing}")

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
asset_tools.migrate_packages(list(PACKAGES), DESTINATION_CONTENT)
log(
    f"COMPLETE packages={len(PACKAGES)} "
    f"destination={DESTINATION_CONTENT}"
)
