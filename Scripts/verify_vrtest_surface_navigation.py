import unreal


MAP_PATH = "/Game/VRTest"
SURFACE_LABELS = (
    "Floor1",
    "Floor2",
    "Floor3",
    "Stairs1",
    "Stairs2",
    "Stairs3",
    "Stairs4",
)
AGENT_RADIUS = 24.0
AGENT_HEIGHT = 180.0
PREFIX = "NATURALNPC_SURFACE_NAV_VERIFY"


def log(message):
    unreal.log(f"{PREFIX} {message}")


unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
world = unreal.EditorLevelLibrary.get_editor_world()
actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
mesh_subsystem = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
actors = actor_subsystem.get_all_level_actors()
actors_by_label = {actor.get_actor_label(): actor for actor in actors}

if "NavFloor_Bar" in actors_by_label:
    raise RuntimeError("Legacy flat NavFloor_Bar still exists")

for label in SURFACE_LABELS:
    actor = actors_by_label.get(label)
    if actor is None or not actor.get_actor_enable_collision():
        raise RuntimeError(f"Missing or non-colliding surface: {label}")
    component = actor.get_component_by_class(unreal.StaticMeshComponent)
    if component is None:
        raise RuntimeError(f"Surface has no StaticMeshComponent: {label}")
    if component.get_collision_enabled() == unreal.CollisionEnabled.NO_COLLISION:
        raise RuntimeError(f"Surface collision is disabled: {label}")
    if str(component.get_collision_profile_name()) != "BlockAll":
        raise RuntimeError(f"Surface is not BlockAll: {label}")
    if not component.get_editor_property("can_ever_affect_navigation"):
        raise RuntimeError(f"Surface does not affect navigation: {label}")
    mesh = component.get_editor_property("static_mesh")
    body_setup = mesh.get_editor_property("body_setup")
    complexity = body_setup.get_editor_property("collision_trace_flag")
    if label.startswith("Stairs"):
        if complexity != unreal.CollisionTraceFlag.CTF_USE_SIMPLE_AND_COMPLEX:
            raise RuntimeError(f"Stair is not Simple-and-Complex: {label}")
        if mesh_subsystem.get_convex_collision_count(mesh) < 1:
            raise RuntimeError(f"Stair has no convex collision: {label}")
    elif complexity != unreal.CollisionTraceFlag.CTF_USE_COMPLEX_AS_SIMPLE:
        raise RuntimeError(f"Floor is not Complex-as-Simple: {label}")
    log(f"SURFACE label={label} mesh={mesh.get_path_name()} valid=1")

recast_actors = unreal.GameplayStatics.get_all_actors_of_class(
    world,
    unreal.RecastNavMesh,
)
if not recast_actors:
    raise RuntimeError("No RecastNavMesh was generated")
for recast in recast_actors:
    radius = recast.get_editor_property("agent_radius")
    height = recast.get_editor_property("agent_height")
    if abs(radius - AGENT_RADIUS) > 0.1:
        raise RuntimeError(f"Wrong Recast agent radius: {radius}")
    if abs(height - AGENT_HEIGHT) > 0.1:
        raise RuntimeError(f"Wrong Recast agent height: {height}")
    log(f"RECAST name={recast.get_name()} radius={radius} height={height}")

# Existing NPC-to-seat routes cross the central Bar topology and must remain
# valid after the flat helper floor is removed.
npcs = [
    actor
    for actor in actors
    if actor.get_class().get_name() in ("BP_Human1_C", "BP_Human2_C")
]
targets = [
    actor
    for actor in actors
    if actor.get_actor_label() in ("Chair1", "Chair4")
]
if len(npcs) != 2 or len(targets) != 2:
    raise RuntimeError(f"Unexpected route actors: npcs={len(npcs)} targets={len(targets)}")

routes = 0
vertical_routes = 0
for npc in npcs:
    for target in targets:
        origin, _ = target.get_actor_bounds(False)
        destination = origin - target.get_actor_forward_vector() * 70.0
        path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
            world,
            npc.get_actor_location(),
            destination,
        )
        points = [] if path is None else list(path.get_editor_property("path_points"))
        if path is None or not path.is_valid() or len(points) < 2:
            raise RuntimeError(
                f"No surface route from {npc.get_actor_label()} "
                f"to {target.get_actor_label()}"
            )
        z_values = [point.z for point in points]
        z_span = max(z_values) - min(z_values)
        routes += 1
        if z_span > 5.0:
            vertical_routes += 1
        log(
            f"ROUTE npc={npc.get_actor_label()} target={target.get_actor_label()} "
            f"points={len(points)} z_min={min(z_values):.1f} "
            f"z_max={max(z_values):.1f} z_span={z_span:.1f}"
        )

if routes != 4:
    raise RuntimeError(f"Expected four navigation routes, got {routes}")

# Floor1 also contains a four-step staircase that is part of the floor mesh
# rather than one of the separately named Stairs actors.
floor1 = actors_by_label["Floor1"]
floor1_origin, floor1_extent = floor1.get_actor_bounds(False)
embedded_stair_start = unreal.Vector(
    floor1_origin.x - floor1_extent.x + 20.0,
    floor1_origin.y,
    floor1_origin.z,
)
embedded_stair_end = unreal.Vector(
    embedded_stair_start.x + 100.0,
    embedded_stair_start.y,
    embedded_stair_start.z + 50.0,
)
embedded_path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
    world,
    embedded_stair_start,
    embedded_stair_end,
)
embedded_points = (
    []
    if embedded_path is None
    else list(embedded_path.get_editor_property("path_points"))
)
if (
    embedded_path is None
    or not embedded_path.is_valid()
    or len(embedded_points) < 2
):
    raise RuntimeError(
        "No navigable route across Floor1's embedded staircase"
    )
log(
    f"EMBEDDED_STAIR_ROUTE label=Floor1 points={len(embedded_points)} "
    f"start={embedded_stair_start} end={embedded_stair_end}"
)

stair_routes = 0
for label in ("Stairs1", "Stairs2", "Stairs3", "Stairs4"):
    stair = actors_by_label[label]
    origin, extent = stair.get_actor_bounds(False)
    if extent.x >= extent.y:
        axis = unreal.Vector(1.0, 0.0, 0.0)
        cross_axis = unreal.Vector(0.0, 1.0, 0.0)
        long_extent = extent.x
        cross_extent = extent.y
    else:
        axis = unreal.Vector(0.0, 1.0, 0.0)
        cross_axis = unreal.Vector(1.0, 0.0, 0.0)
        long_extent = extent.y
        cross_extent = extent.x

    ignored_surfaces = [
        actors_by_label[surface_label]
        for surface_label in SURFACE_LABELS
        if surface_label != label
    ]
    stair_hits = []
    for long_fraction in (-0.8, -0.6, -0.4, 0.0, 0.4, 0.6, 0.8):
        for cross_fraction in (-0.6, -0.3, 0.0, 0.3, 0.6):
            probe = (
                origin
                + axis * long_extent * long_fraction
                + cross_axis * cross_extent * cross_fraction
            )
            for trace_complex in (True, False):
                result = unreal.SystemLibrary.line_trace_single_for_objects(
                    world,
                    unreal.Vector(
                        probe.x,
                        probe.y,
                        origin.z + extent.z + 100.0,
                    ),
                    unreal.Vector(
                        probe.x,
                        probe.y,
                        origin.z - extent.z - 100.0,
                    ),
                    [
                        unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY1,
                        unreal.ObjectTypeQuery.OBJECT_TYPE_QUERY2,
                    ],
                    trace_complex,
                    ignored_surfaces,
                    unreal.DrawDebugTrace.NONE,
                    True,
                )
                if result is None:
                    continue
                fields = result.to_tuple()
                candidate_actor = fields[9]
                if (
                    fields[0]
                    and candidate_actor is not None
                    and candidate_actor.get_actor_label() == label
                ):
                    stair_hits.append(fields[5])
                    break

    if len(stair_hits) < 2:
        raise RuntimeError(
            f"Could not sample the collision surface for {label}; "
            f"hits={len(stair_hits)}"
        )
    stair_hits.sort(key=lambda point: point.x if axis.x else point.y)
    low_hit = stair_hits[0]
    high_hit = stair_hits[-1]
    endpoints = [
        low_hit + unreal.Vector(0.0, 0.0, 20.0),
        high_hit + unreal.Vector(0.0, 0.0, 20.0),
    ]
    endpoint_hits = [f"{low_hit.z:.1f}", f"{high_hit.z:.1f}"]

    path = unreal.NavigationSystemV1.find_path_to_location_synchronously(
        world,
        endpoints[0],
        endpoints[1],
    )
    points = [] if path is None else list(path.get_editor_property("path_points"))
    if path is None or not path.is_valid() or len(points) < 2:
        raise RuntimeError(
            f"No navigable route across {label}; hits={endpoint_hits}"
        )
    z_values = [point.z for point in points]
    stair_routes += 1
    log(
        f"STAIR_ROUTE label={label} hits={endpoint_hits} "
        f"points={len(points)} z_min={min(z_values):.1f} "
        f"z_max={max(z_values):.1f} "
        f"z_span={max(z_values) - min(z_values):.1f}"
    )

log(
    f"SUCCESS surfaces={len(SURFACE_LABELS)} routes={routes} "
    f"vertical_routes={vertical_routes} stair_routes={stair_routes} "
    f"embedded_floor1_stair=1 legacy_floor=0"
)
