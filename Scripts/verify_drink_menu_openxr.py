from pathlib import Path

import unreal


DISPLAY_PATH = "/Game/DrinkSystem/Blueprints/BP_DrinkMenuDisplay"
CONTEXT_PATH = "/Game/DrinkSystem/Input/IMC_DrinkMenu"
MENU_PATH = "/Game/DrinkSystem/UI/WBP_DrinkMenu"
EXPECTED_ACTION = "IA_Menu_Interact_Right_Pressed"
EXPECTED_KEYS = {
    "OculusTouch_Right_Trigger_Axis",
    "ValveIndex_Right_Trigger_Axis",
}
PREFIX = "NATURALNPC_DRINK_MENU_VERIFY"


def verify_openxr_default_context():
    config_path = Path(unreal.Paths.project_config_dir()) / "DefaultInput.ini"
    section = ""
    for raw_line in config_path.read_text(encoding="utf-8-sig").splitlines():
        line = raw_line.strip()
        if line.startswith("[") and line.endswith("]"):
            section = line
            continue
        if section != "[/Script/EnhancedInput.EnhancedInputDeveloperSettings]":
            continue
        compact = line.replace(" ", "")
        if (
            compact.startswith("+DefaultMappingContexts=(")
            and 'InputMappingContext="/Game/DrinkSystem/Input/'
            'IMC_DrinkMenu.IMC_DrinkMenu"' in compact
            and "bAddImmediately=False" in compact
        ):
            return
    raise RuntimeError(
        "IMC_DrinkMenu is not registered as a non-immediate OpenXR "
        "default mapping context"
    )


def key_name(key):
    try:
        return str(key.get_editor_property("key_name"))
    except Exception:
        return str(key)


display = unreal.load_asset(DISPLAY_PATH)
context = unreal.load_asset(CONTEXT_PATH)
menu = unreal.load_asset(MENU_PATH)
if display is None or context is None or menu is None:
    raise RuntimeError("Drink menu assets are missing")
verify_openxr_default_context()

mapped_keys = set()
mapping_data = context.get_editor_property("default_key_mappings")
for mapping in mapping_data.get_editor_property("mappings"):
    action = mapping.get_editor_property("action")
    if action is not None and action.get_name() == EXPECTED_ACTION:
        mapped_keys.add(key_name(mapping.get_editor_property("key")))
missing_keys = EXPECTED_KEYS - mapped_keys
if missing_keys:
    raise RuntimeError(
        f"Drink menu right-trigger mappings are missing: {sorted(missing_keys)}"
    )

referencers = unreal.EditorAssetLibrary.find_package_referencers_for_asset(
    CONTEXT_PATH,
    True,
)
if DISPLAY_PATH not in referencers:
    raise RuntimeError("BP_DrinkMenuDisplay does not reference IMC_DrinkMenu")

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/VRTest")
if world is None:
    raise RuntimeError("Could not load VRTest")
actors = unreal.get_editor_subsystem(
    unreal.EditorActorSubsystem
).get_all_level_actors()
display_actors = [
    actor for actor in actors
    if "drinkmenudisplay" in (
        actor.get_name() + " " + actor.get_actor_label()
    ).lower()
]
if len(display_actors) != 1:
    raise RuntimeError(
        f"Expected one DrinkMenuDisplay in VRTest, found {len(display_actors)}"
    )
widget = display_actors[0].get_component_by_class(unreal.WidgetComponent)
if widget is None:
    raise RuntimeError("DrinkMenuDisplay is missing its WidgetComponent")
widget_class = widget.get_editor_property("widget_class")
if widget_class is None or widget_class.get_name() != "WBP_DrinkMenu_C":
    raise RuntimeError(f"Wrong drink menu widget class: {widget_class}")
if widget.get_collision_enabled() == unreal.CollisionEnabled.NO_COLLISION:
    raise RuntimeError("DrinkMenuDisplay WidgetComponent has no collision")

unreal.log(
    f"{PREFIX} success actor={display_actors[0].get_name()} "
    f"widget={widget_class.get_name()} collision={widget.get_collision_enabled()} "
    f"action={EXPECTED_ACTION} keys={sorted(mapped_keys)} "
    "openxr_default_context=IMC_DrinkMenu add_immediately=0"
)
