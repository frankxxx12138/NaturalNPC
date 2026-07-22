// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NPCWorldStateBlueprintLibrary.h"

#ifdef NATURALNPCWORLDSTATE_NPCWorldStateBlueprintLibrary_generated_h
#error "NPCWorldStateBlueprintLibrary.generated.h already included, missing '#pragma once' in NPCWorldStateBlueprintLibrary.h"
#endif
#define NATURALNPCWORLDSTATE_NPCWorldStateBlueprintLibrary_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UNPCWorldStateAgentComponent;
class UNPCWorldStateSubsystem;
class UObject;

// ********** Begin Class UNPCWorldStateBlueprintLibrary *******************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execEnsureNPCWorldStateAgent); \
	DECLARE_FUNCTION(execGetNPCWorldStateSubsystem);


struct Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics;
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUNPCWorldStateBlueprintLibrary(); \
	friend struct ::Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCWORLDSTATE_API UClass* ::Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_NoRegister(); \
public: \
	DECLARE_CLASS2(UNPCWorldStateBlueprintLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NaturalNPCWorldState"), Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_NoRegister) \
	DECLARE_SERIALIZER(UNPCWorldStateBlueprintLibrary)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UNPCWorldStateBlueprintLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UNPCWorldStateBlueprintLibrary(UNPCWorldStateBlueprintLibrary&&) = delete; \
	UNPCWorldStateBlueprintLibrary(const UNPCWorldStateBlueprintLibrary&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNPCWorldStateBlueprintLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNPCWorldStateBlueprintLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UNPCWorldStateBlueprintLibrary) \
	NO_API virtual ~UNPCWorldStateBlueprintLibrary();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_10_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UNPCWorldStateBlueprintLibrary;

// ********** End Class UNPCWorldStateBlueprintLibrary *********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
