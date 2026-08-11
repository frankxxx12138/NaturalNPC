@echo off
setlocal

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1

if not exist "Intermediate\Mixamo" mkdir "Intermediate\Mixamo"

cl /nologo /std:c++17 /EHsc /MD ^
  /I"D:\UnrealEngine\UE_5.7\Engine\Source\ThirdParty\FBX\2020.2\include" ^
  "Scripts\Tools\FbxAttachDummyMesh.cpp" ^
  /link ^
  /LIBPATH:"D:\UnrealEngine\UE_5.7\Engine\Source\ThirdParty\FBX\2020.2\lib\vs2017\x64\release" ^
  libfbxsdk.lib ^
  /OUT:"Intermediate\Mixamo\FbxAttachDummyMesh.exe"
if errorlevel 1 exit /b 1

copy /Y ^
  "D:\UnrealEngine\UE_5.7\Engine\Binaries\ThirdParty\FBX\2020.2\Win64\libfbxsdk.dll" ^
  "Intermediate\Mixamo\libfbxsdk.dll" >nul
if errorlevel 1 exit /b 1

exit /b 0
