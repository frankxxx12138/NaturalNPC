# NaturalNPC External Dependencies

This repository keeps source code, project configuration, scripts, and lightweight Unreal assets in Git.
Large runtime files are stored separately in OneDrive so the Git repository can be pushed and cloned reliably.

## OneDrive Package

Download the external dependency package from OneDrive:

```text
TODO: paste OneDrive share link here
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
