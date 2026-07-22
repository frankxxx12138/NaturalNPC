import unreal


PREFIX = "NATURALNPC_VRTEST_APPROACH_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


def unpack(outcome):
    if isinstance(outcome, tuple):
        return bool(outcome[0]), outcome[1] if len(outcome) > 1 else None
    return bool(outcome), None


unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
world = unreal.EditorLevelLibrary.get_editor_world()
subsystem = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
subsystem.refresh_auto_discovery()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
npc = next(
    actor
    for actor in actor_subsystem.get_all_level_actors()
    if actor.get_class().get_name() == "BP_Human2_C"
)
agent = npc.get_component_by_class(unreal.NPCWorldStateAgentComponent)
states = subsystem.get_world_state(npc, 2500.0)
pistol = next(
    state
    for state in states
    if str(state.category) == "pistol" and state.distance > 200.0
)

start_location = npc.get_actor_location()
handled, result = unpack(
    agent.execute_world_action(pistol.object_id, "pickup", "")
)
message = result.message if result else ""
if not handled:
    raise RuntimeError(f"Approach request was rejected: {message}")
if not agent.is_world_action_in_progress():
    raise RuntimeError("Approach did not enter the world-action state")
if agent.get_held_actor() is not None:
    raise RuntimeError("Pistol was attached before the NPC approached it")
if pistol.actor.get_attach_parent_actor() is not None:
    raise RuntimeError("Pistol left the table before the NPC approached it")
if npc.get_actor_location() != start_location:
    raise RuntimeError("NPC moved synchronously instead of using runtime ticks")

action_animation = unreal.load_asset(
    "/Game/MetaHumans/Human2/Animations/Locomotion/"
    "Human2_MF_Unarmed_Walk_Fwd"
)
action_skeleton = action_animation.get_editor_property("skeleton")
animated_meshes = []
for mesh in npc.get_components_by_class(unreal.SkeletalMeshComponent):
    skeletal_mesh = mesh.get_editor_property("skeletal_mesh")
    skeleton = (
        skeletal_mesh.get_editor_property("skeleton")
        if skeletal_mesh
        else None
    )
    if skeleton != action_skeleton:
        continue
    mode = mesh.get_editor_property("animation_mode")
    animated_meshes.append(f"{mesh.get_name()}={mode}")
    if mode != unreal.AnimationMode.ANIMATION_SINGLE_NODE:
        raise RuntimeError(
            f"Approach animation did not start on {mesh.get_name()}"
        )
if len(animated_meshes) < 2:
    raise RuntimeError(f"Body/clothing sync missing: {animated_meshes}")

log(
    f"SUCCESS npc={npc.get_name()} object={pistol.object_id} "
    f"distance={pistol.distance:.1f} message={message} "
    f"meshes={animated_meshes}"
)
log("END")
