import unreal


BLUEPRINTS = [
    "/Game/MetaHumans/Human2/ACE/ABP_Human2_Face_ACE",
    "/Game/MetaHumans/Common/Animation/ABP_MH_LiveLink",
    "/ConvAI/MetaHumans/Animations/Convai_MetaHuman_FaceAnim",
]


def pin_label(pin):
    try:
        return f"{pin.get_name()}:{pin.direction}"
    except Exception:
        return str(pin)


for path in BLUEPRINTS:
    bp = unreal.load_asset(path)
    if bp is None:
        unreal.log(f"FACE_ABP_GRAPH path={path} loaded=0")
        continue

    unreal.log(f"FACE_ABP_GRAPH path={path} loaded=1 class={bp.get_class().get_path_name()}")
    graphs = []
    for prop in ("ubergraph_pages", "function_graphs"):
        try:
            values = bp.get_editor_property(prop)
            graphs.extend(values)
        except Exception as exc:
            unreal.log(f"FACE_ABP_GRAPH path={path} prop={prop} error={exc}")

    try:
        anim_graph = unreal.BlueprintEditorLibrary.find_graph(bp, "AnimGraph")
        if anim_graph is not None and anim_graph not in graphs:
            graphs.append(anim_graph)
    except Exception as exc:
        unreal.log(f"FACE_ABP_GRAPH path={path} find_anim_graph_error={exc}")

    for graph in graphs:
        if graph is None:
            continue
        try:
            nodes = graph.get_editor_property("nodes")
        except Exception as exc:
            unreal.log(f"FACE_ABP_GRAPH graph={graph.get_name()} nodes_error={exc}")
            continue
        unreal.log(f"FACE_ABP_GRAPH graph={graph.get_name()} node_count={len(nodes)}")
        for node in nodes:
            cls = node.get_class().get_path_name()
            title = ""
            try:
                title = str(node.get_node_title(unreal.NodeTitleType.FULL_TITLE))
            except Exception:
                pass
            interesting = any(
                token.lower() in (cls + " " + title).lower()
                for token in (
                    "ACE",
                    "Pose",
                    "LiveLink",
                    "Curve",
                    "Output",
                    "Sequence",
                    "Ref",
                    "Modify",
                    "Layered",
                )
            )
            if interesting:
                unreal.log(
                    "FACE_ABP_NODE "
                    f"bp={path} graph={graph.get_name()} "
                    f"class={cls} title={title}"
                )
