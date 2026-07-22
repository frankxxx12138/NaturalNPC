# Natural NPC World State

This runtime plugin keeps a live registry of meaningful map objects and exposes
per-NPC snapshots as Blueprint structs, compact JSON, and prompt-friendly text.

## Runtime model

- `NPCWorldStateObjectComponent` describes an object's identity, state, aliases,
  and available actions.
- `NPCWorldStateSubsystem` is the world-level registry. It automatically adds
  VR Template actors that contain `BP_GrabComponent` and actors tagged
  `NPCWorldObject`.
- `NPCWorldStateAgentComponent` gives an NPC a perception radius, cached world
  state, and `ExecuteWorldAction(ObjectId, ActionId, Parameters)`.
- `NPCWorldActionTarget` is the Blueprint interface for custom actions such as
  `open`, `drink`, `fire`, or `sit`.

Built-in actions are `inspect`, `pickup`, `drop`, and `throw`. Pickup attaches a
physics object to the NPC's `hand_r` socket and restores physics when dropped.
Non-humanoid NPCs can set `HandSocket` to `None` and use a named scene component
as their hand anchor.

Pickup uses a staged approach. An object outside the action's reach distance is
not attached remotely: the agent resolves an approach point, follows a
navigation path when available (with swept direct movement as fallback), faces
the target, and only then starts the pickup animation. The default reach is 150
cm, while `MaxPickupApproachDistance` controls how far the NPC is willing to
travel. Human2 uses synchronized walk/run sequences on both body and clothing.

Each action can provide an `ActionAnimation`, playback rate, reverse-play flag,
and normalized effect trigger. At runtime the agent temporarily plays the
sequence, applies the object action at the trigger point, then restores the
skeletal mesh's previous animation mode and AnimBP. The Human2 defaults use
`Human2_MM_Pistol_Equip` for pickup and the same sequence in reverse for drop;
`DefaultThrowAnimation` is intentionally exposed separately for a dedicated
throw sequence.

For non-VR objects, add `NPCWorldStateObjectComponent` in the actor Blueprint,
set a stable `ObjectId`, and add action definitions. Implement
`NPCWorldActionTarget` when an action needs custom game logic.
