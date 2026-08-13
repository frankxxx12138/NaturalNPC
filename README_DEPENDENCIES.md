# NaturalNPC External Dependencies

This repository keeps source code, project configuration, scripts, and lightweight Unreal assets in Git.
Large runtime files are stored separately in OneDrive so the Git repository can be pushed and cloned reliably.

## OneDrive Package

Download the external dependency package from OneDrive:

```text
TODO: https://uob-my.sharepoint.com/:u:/r/personal/lh25052_bristol_ac_uk/Documents/Natural_NPC/NaturalNPC_ExternalDependencies_2026-07-12.zip?csf=1&web=1&e=3iKVPd
```

Recommended archive name:

```text
NaturalNPC_ExternalDependencies_YYYYMMDD.7z
```

The archive must preserve paths relative to the project root. After extraction, the files should merge directly into:

```text
D:\UE_Projects\NaturalNPC
```

## What Goes In The OneDrive Package

Put files here if they are too large for normal Git hosting, especially files over 100 MB.

Current known large dependencies:

```text
Plugins/NV_ACE_Reference/ThirdParty/Nvigi/Models/
Plugins/NvAudio2FaceMark/ThirdParty/Nvigi/Models/
Plugins/NV_ACE_Reference/ThirdParty/Nvigi/Binaries/Win64/cublasLt64_12.dll
Plugins/NV_ACE_Reference/ThirdParty/Nvigi/Binaries/Win64/cublas64_12.dll
Plugins/NV_ACE_Reference/ThirdParty/Nvigi/Binaries/Win64/nvinfer_10.dll
Plugins/NV_ACE_Reference/ThirdParty/Nvigi/Binaries/Win64/nvinfer_plugin_10.dll
```

If Git LFS is not being used, also put very large Unreal assets in the OneDrive package:

```text
Content/Human2.uasset
Content/NewMetaHumanCharacter.uasset
Content/MetaHumans/Common/Face/Face_Archetype.uasset
Content/MetaHumans/Human2/Face/SKM_Human2_FaceMesh.uasset
Content/MetaHumans/NewMetaHumanCharacter/Face/SKM_NewMetaHumanCharacter_FaceMesh.uasset
```

Important: if `Content/Human2.uasset` is stored in OneDrive instead of Git, then any later changes to BP_Human2 must be exported into a new OneDrive dependency package.

## What Should Not Go In The OneDrive Package

These are local generated files and should be recreated locally:

```text
Binaries/
DerivedDataCache/
Intermediate/
Saved/
.venv-tts/
.venv-stt/
Saved/TTSCache/
Saved/NPCMemory/
Saved/ServicePids/
```

Python environments should be rebuilt with the project scripts instead of stored in Git or OneDrive.

## Restore Steps After Cloning

1. Clone this repository.

2. Download the OneDrive dependency archive.

3. Extract the archive into the project root so paths merge with the repository:

```text
D:\UE_Projects\NaturalNPC
```

For example, after extraction these files should exist:

```text
D:\UE_Projects\NaturalNPC\Plugins\NV_ACE_Reference\ThirdParty\Nvigi\Binaries\Win64\cublasLt64_12.dll
D:\UE_Projects\NaturalNPC\Plugins\NV_ACE_Reference\ThirdParty\Nvigi\Models
D:\UE_Projects\NaturalNPC\Plugins\NvAudio2FaceMark\ThirdParty\Nvigi\Models
D:\UE_Projects\NaturalNPC\Content\Human2.uasset
```

4. Recreate local Python service environments if needed:

```powershell
.\Scripts\Install-WhisperSTT.ps1
```

The Chatterbox TTS environment is local machine setup and should remain outside Git.

5. Start the local services:

```text
Start-NaturalNPCServices.bat
```

The default launcher uses a memory-optimized idle mode:

- Ollama models load on first use and requests keep them resident for 60
  seconds instead of 10 minutes.
- The dedicated Ollama service starts a lightweight runner watchdog. It checks
  that every `llama-server.exe` still has a live parent from the expected
  Ollama installation, removes verified orphan runners during startup and
  service shutdown, and also cleans them after an unexpected daemon exit.
- Chatterbox and faster-whisper run heavy inference in disposable workers.
  Consecutive requests reuse the worker, then the worker exits after 60 idle
  seconds while the lightweight HTTP health service remains available.
- Chatterbox inference is serialized across NPCs because one PyTorch model is
  shared by Jack, Oliver, and other listeners.
- Chatterbox analyzes each distinct reference WAV once. Up to four recent
  voice conditions remain in the worker, and serialized condition tensors are
  stored under `Saved/TTSCache/Chatterbox/VoiceConditionals` so a restarted
  worker can restore a voice without analyzing the WAV again. The cache key
  includes the model type plus the WAV path, size, and modification time, so
  replacing a reference recording invalidates it automatically.
- ACE remains prewarmed for dialogue latency, but the OpenAI NPC components
  coordinate one shared provider allocation instead of warming once per NPC.

Use explicit warmup only when lower first-request latency is more important
than idle memory:

```powershell
.\Scripts\Start-NaturalNPCServices.ps1 -Warmup -TtsPrewarmText "Sure."
```

Always stop the project services with the provided script. It checks both PID
files and the real listening ports so virtual-environment child processes are
not left behind:

```powershell
.\Scripts\Stop-NaturalNPCServices.ps1
```

## Switching Between OpenAI And The Local LLM

`OpenAIJackComponent` now exposes `LLM Provider` under
`Local AI > LLM Backend`:

- `Ollama Local` keeps the existing Ollama + Whisper + Chatterbox path.
- `OpenAI API` sends text chat to OpenAI and, when `Enable OpenAI Realtime
  Voice` is enabled, uses the local Realtime bridge for speech-to-speech input
  and output. The bridge keeps the API key outside Unreal assets. By default,
  changing to this provider also asks Ollama to unload the chat and embedding
  models, skips local memory embeddings, and therefore releases their VRAM.

The property can be changed in each NPC Blueprint or at runtime with the
Blueprint-callable `Set LLM Provider` function. A launch-wide override is also
available:

```text
-NaturalNPCLLMProvider=OpenAI
-NaturalNPCLLMProvider=Ollama
```

The `NATURALNPC_LLM_PROVIDER` environment variable accepts the same values.
The command-line override takes priority, followed by the environment variable,
then the component property.

For OpenAI mode, configure the key once as a user environment variable and
open a new terminal:

```powershell
setx OPENAI_API_KEY "your-api-key"
```

Do not put the key in an `.ini`, Blueprint, source file, or batch file. Install
the one extra Python dependency, then start only the cloud bridge:

```powershell
.\Scripts\Install-OpenAIRealtime.ps1
.\Start-NaturalNPCOpenAI.bat
```

The default voice model is `gpt-realtime-2.1`; it can be changed with the
launcher's `-Model` parameter. The Realtime path is multimodal audio
(speech-to-speech). Although the model also accepts image input, this project
does not currently send viewport frames.

When using OpenAI mode to reduce VRAM pressure, stop the local stack instead of
running both backends. The component unloads Ollama models, while this command
also stops local Whisper/TTS services:

```powershell
.\Stop-NaturalNPCServices.bat
```

To return to offline/local inference, stop the bridge, select `Ollama Local`,
and start the original services:

```powershell
.\Stop-NaturalNPCOpenAI.bat
.\Start-NaturalNPCServices.bat
```

6. Open the Unreal project:

```text
NaturalNPC.uproject
```

## Updating The OneDrive Package

Update the OneDrive archive when any external large dependency changes, for example:

```text
ACE model files changed
NVIDIA runtime DLLs changed
BP_Human2 / Human2.uasset changed and is not tracked in Git
MetaHuman face mesh assets changed and are not tracked in Git
```

Use a dated package name so old working versions can be recovered:

```text
NaturalNPC_ExternalDependencies_2026-07-12.7z
```

Then update the OneDrive link in this file if the share link changes.

## Git Notes

Do not commit the OneDrive dependency package itself.

The Git repository should stay focused on:

```text
Config/
Content/ lightweight assets
Plugins/ project source code
Scripts/
NaturalNPC.uproject
.gitignore
README_DEPENDENCIES.md
```

Large binary dependencies should either be handled by Git LFS or kept in the OneDrive package described above.
