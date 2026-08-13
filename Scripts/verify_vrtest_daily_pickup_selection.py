import unreal


PREFIX = "NATURALNPC_DAILY_PICKUP_VRTEST"
MAP_PATH = "/Game/VRTest"


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actors = unreal.get_editor_subsystem(
    unreal.EditorActorSubsystem
).get_all_level_actors()

npcs = [
    actor
    for actor in actors
    if actor.get_actor_label() in {"BP_Human1", "BP_Human2"}
]
if len(npcs) != 2:
    raise RuntimeError(f"Expected Human1 and Human2, found {npcs}")

world_state = unreal.NPCWorldStateBlueprintLibrary.get_npc_world_state_subsystem(
    world
)
world_state.refresh_auto_discovery()

for npc in npcs:
    agent = unreal.NPCWorldStateBlueprintLibrary.ensure_npc_world_state_agent(
        npc
    )
    if agent is None:
        raise RuntimeError(f"{npc.get_actor_label()} has no world-state agent")
    states = world_state.get_world_state(npc, 0.0)
    relevant = [
        state for state in states if str(state.category) in {"pistol", "ball"}
    ]
    if not relevant:
        raise RuntimeError(f"No pistol or ball is visible to {npc.get_actor_label()}")
    for state in relevant:
        animation = agent.get_pickup_animation_for_target(state.actor)
        if animation is None:
            raise RuntimeError(
                f"No pickup animation resolved for {state.object_id}"
            )
        variant = "table" if "Pickup_Table" in animation.get_name() else "ground"
        unreal.log(
            f"{PREFIX} npc={npc.get_actor_label()} "
            f"object={state.object_id} category={state.category} "
            f"location={state.location} variant={variant} "
            f"animation={animation.get_path_name()}"
        )

unreal.log(f"{PREFIX} SUCCESS npcs={len(npcs)}")
