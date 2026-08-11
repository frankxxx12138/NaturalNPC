# Natural NPC Conversation Governance

This runtime plugin governs when a nearby non-primary NPC may participate in a
conversation. It is dialogue-provider-independent: it owns eligibility,
candidate validation, queueing, turn state, permission state, execution events,
and structured logs, but does not generate speech or depend on Convai, OpenAI,
Ollama, TTS, ACE, animation, or input modules.

`OpenAIJackNPC` contains the project adapter,
`UOpenAINPCConversationSubsystem`. The adapter discovers project-owned
`UOpenAIJackComponent` instances, requests structured decisions from eligible
listeners, validates their evidence, and sends accepted text through the
existing project-owned speech path.

## Participation categories

Every listener decision is exactly one of these categories:

- `Emergency`: a warning about an active, externally verified urgent event,
  such as a rapidly deteriorating patient or a fire. It may interrupt current
  output and forcibly take the turn. Model text alone cannot create an
  emergency; its `emergency_event_id` must match the active event reported by
  gameplay.
- `RelevantContribution`: the listener has useful information that the current
  speakers do not have. The listener first speaks a short permission request,
  the system enters `PermissionPending`, and the actual contribution is spoken
  only after permission is granted.
- `SideComment`: a short, low-impact evaluation or emotional reaction. It waits
  for a natural pause, does not replace the main turn owner, and must not steer
  the conversation.
- `AssociativeTopic`: a related question, recommendation, or topic extension
  supported by the NPC's interests or strong relevance. It waits for the
  primary reply and natural pause, then becomes a normal secondary turn.
- `Silence`: the default when none of the above has enough evidence. The NPC may
  still request a non-verbal reaction such as looking, nodding, smiling,
  surprise, or concern.

There is no time-based participation cooldown. Frequency is managed from
category evidence, current turn state, recent participation context, queue
limits, one autonomous response per exchange, and stale-candidate expiry.
Recent participation is context for the decision, not a hard prohibition.

## Runtime behavior

For a normal player-to-NPC exchange, the adapter:

1. Resolves the explicitly addressed NPC as the primary speaker.
2. Registers only nearby, enabled NPCs as listeners.
3. Lets the primary NPC generate and finish its answer.
4. Asks each eligible listener for structured JSON containing category,
   response text, confidence, urgency, relevance, topic affinity, conversation
   impact, unique-information evidence, topic-transition readiness, emergency
   event ID, optional permission request, and optional non-verbal reaction.
5. Rejects malformed or unsupported decisions as silence.
6. Executes the accepted category through its dedicated turn path.

Only one autonomous third-party response is executed per exchange. Emergency
responses are the exception to normal ordering and may preempt active primary
or secondary output.

## Designer configuration

Each `UOpenAIJackComponent` exposes:

- `NPCID`: stable routing and speaker identity.
- `bEnableAutonomousListening`: whether the NPC may participate as a listener.
- `AutonomousListeningRadius`: hearing/eligibility radius.
- `MaximumAutonomousResponseCharacters`: maximum generated response length.
- `AutonomousKnowledgeSummary`: facts this NPC can uniquely contribute.
- `AutonomousInterestSummary`: topics strong enough to justify an associative
  contribution.

The transient `UConversationListenerComponent` also supplies queue limits and
per-NPC switches for side comments, permission requests, and emergency
interruptions.

## Blueprint integration

The project adapter creates its coordinator automatically. For custom
integration, use `UConversationGovernanceComponent` and:

1. Register `UConversationListenerComponent` instances.
2. Start the conversation with the already-selected primary NPC.
3. Forward real player/NPC speech start and finish events.
4. Submit fully populated `FCandidateNPCResponse` values.
5. Handle `OnSpeakingPermissionRequested`,
   `OnSpeakingPermissionResolved`, `OnResponseExecutionStarted`,
   `OnResponseExecutionFinished`, and `OnNonVerbalReactionRequested`.

Gameplay must explicitly report real emergencies through
`UOpenAINPCConversationSubsystem::ReportEmergencyEvent`. Call
`ClearEmergencyEvent` when the situation ends. The current state is available
through `GetEmergencyEventState`.

Permission can be resolved by calling `GrantPendingSpeakingPermission` or
`DenyPendingSpeakingPermission`. In the built-in text route, clear player
answers such as `yes`, `go ahead`, `sure`, `no`, `not now`, `可以`, `说吧`,
`不用`, and `不要` are also consumed as permission decisions.

## Testing and diagnostics

In PIE, a routed English test can be sent with:

```text
NPC.Say Jack This place looks amazing tonight and I really love the atmosphere here.
```

The exact category remains autonomous. For example, a short reaction can become
`SideComment`, while a reaction followed by a drink recommendation should
become `AssociativeTopic`.

Filter logs for:

- `OPENAI_NPC_ROUTE`: addressed-NPC resolution.
- `NATURALNPC_AUTONOMY`: exchange, classification, validation, and execution.
- `NATURALNPC_GOV`: listeners, queue, permission, turn ownership, and category
  execution.

Autonomous candidate generation can be toggled with
`npc.Autonomy.Enabled 0|1`. Headless tests may pass
`-NaturalNPCDisableACE` to avoid starting ACE audio/render workers; this flag
does not change normal gameplay defaults.

Automation suites:

```text
NaturalNPC.OpenAINPC
NaturalNPC.ConversationGovernance
```
