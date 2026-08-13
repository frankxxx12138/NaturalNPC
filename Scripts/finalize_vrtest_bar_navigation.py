import os
import runpy

import unreal


# Kept as a compatibility entry point for older project notes. The Bar now
# bakes navigation from Floor1-3 and Stairs1-4 instead of a hidden flat mesh.
script_path = os.path.join(
    unreal.Paths.project_dir(),
    "Scripts",
    "configure_vrtest_surface_navigation.py",
)
runpy.run_path(script_path, run_name="__main__")
