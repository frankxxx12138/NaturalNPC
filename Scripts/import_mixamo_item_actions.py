import unreal


PREFIX = "NATURALNPC_MIXAMO_IMPORT"
DESTINATION = "/Game/ImportedAnimations/MixamoItemActions"
PICKUP_FBX = (
    r"D:\UE_Projects\NaturalNPC\Intermediate\Mixamo"
    r"\PickingUp_WithDummyMesh.fbx"
)
BOX_WALK_FBX = r"C:\Users\12600\Downloads\Box Walk Arc.fbx"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def import_task(filename, destination_name, import_mesh, skeleton=None):
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", filename)
    task.set_editor_property("destination_path", DESTINATION)
    task.set_editor_property("destination_name", destination_name)
    task.set_editor_property("automated", True)
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("replace_existing_settings", True)
    task.set_editor_property("save", True)

    options = unreal.FbxImportUI()
    options.set_editor_property("automated_import_should_detect_type", False)
    options.set_editor_property("import_as_skeletal", True)
    options.set_editor_property("import_mesh", import_mesh)
    options.set_editor_property("import_animations", True)
    options.set_editor_property("import_materials", False)
    options.set_editor_property("import_textures", False)
    options.set_editor_property(
        "mesh_type_to_import",
        unreal.FBXImportType.FBXIT_SKELETAL_MESH
        if import_mesh
        else unreal.FBXImportType.FBXIT_ANIMATION,
    )
    if skeleton is not None:
        options.set_editor_property("skeleton", skeleton)
    if import_mesh:
        options.set_editor_property("create_physics_asset", False)

    task.set_editor_property("options", options)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    paths = list(task.get_editor_property("imported_object_paths"))
    log(
        f"TASK file={filename} import_mesh={int(import_mesh)} "
        f"paths={paths}"
    )
    return [unreal.load_asset(path) for path in paths]


unreal.EditorAssetLibrary.make_directory(DESTINATION)
pickup_assets = import_task(
    PICKUP_FBX,
    "Mixamo_PickingUp_Source",
    True,
)
source_meshes = [
    asset for asset in pickup_assets if isinstance(asset, unreal.SkeletalMesh)
]
source_animations = [
    asset for asset in pickup_assets if isinstance(asset, unreal.AnimSequence)
]
if len(source_meshes) != 1:
    raise RuntimeError(f"Expected one Mixamo source mesh, found {source_meshes}")
if len(source_animations) != 1:
    raise RuntimeError(
        f"Expected one Picking Up animation, found {source_animations}"
    )

source_mesh = source_meshes[0]
source_skeleton = source_mesh.get_editor_property("skeleton")
box_walk_assets = import_task(
    BOX_WALK_FBX,
    "A_Mixamo_BoxWalkArc",
    False,
    source_skeleton,
)
box_walk_animations = [
    asset for asset in box_walk_assets if isinstance(asset, unreal.AnimSequence)
]
if len(box_walk_animations) != 1:
    raise RuntimeError(
        f"Expected one Box Walk animation, found {box_walk_animations}"
    )

pickup_animation = source_animations[0]
box_walk_animation = box_walk_animations[0]
for animation in (pickup_animation, box_walk_animation):
    if animation.get_editor_property("skeleton") != source_skeleton:
        raise RuntimeError(f"Animation uses the wrong skeleton: {animation}")
    unreal.EditorAssetLibrary.save_loaded_asset(animation, False)
    log(
        f"ANIMATION path={animation.get_path_name()} "
        f"length={animation.get_play_length():.3f} "
        f"frames={animation.data_model_interface.get_number_of_frames()}"
    )

unreal.EditorAssetLibrary.save_loaded_asset(source_mesh, False)
unreal.EditorAssetLibrary.save_loaded_asset(source_skeleton, False)
log(
    f"SUCCESS mesh={source_mesh.get_path_name()} "
    f"skeleton={source_skeleton.get_path_name()} "
    f"pickup={pickup_animation.get_path_name()} "
    f"box_walk={box_walk_animation.get_path_name()}"
)
