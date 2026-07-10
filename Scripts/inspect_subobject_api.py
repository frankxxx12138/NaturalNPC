import unreal


def public_names(value):
    return sorted(name for name in dir(value) if not name.startswith("_"))


for type_name in (
    "SubobjectDataSubsystem",
    "SubobjectDataBlueprintFunctionLibrary",
):
    value = getattr(unreal, type_name)
    unreal.log(f"NATURALNPC_SUBOBJECT_API {type_name}={public_names(value)}")

subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
for method_name in ("delete_subobject", "delete_subobjects"):
    method = getattr(subsystem, method_name, None)
    unreal.log(f"NATURALNPC_SUBOBJECT_API {method_name}={getattr(method, '__doc__', None)}")
