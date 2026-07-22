// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NPCWorldStateSubsystem.h"

#ifdef NATURALNPCWORLDSTATE_NPCWorldStateSubsystem_generated_h
#error "NPCWorldStateSubsystem.generated.h already included, missing '#pragma once' in NPCWorldStateSubsystem.h"
#endif
#define NATURALNPCWORLDSTATE_NPCWorldStateSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UNPCWorldStateObjectComponent;
struct FNPCWorldObjectState;

// ********** Begin Delegate FNPCWorldObjectEvent **************************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_15_DELEGATE \
NATURALNPCWORLDSTATE_API void FNPCWorldObjectEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldObjectEvent, FName ObjectId, AActor* Actor);


// ********** End Delegate FNPCWorldObjectEvent ****************************************************

// ********** Begin Class UNPCWorldStateSubsystem **************************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execBuildWorldStateText); \
	DECLARE_FUNCTION(execBuildWorldStateJson); \
	DECLARE_FUNCTION(execGetWorldState); \
	DECLARE_FUNCTION(execFindWorldObject); \
	DECLARE_FUNCTION(execRefreshAutoDiscovery);


struct Z_Construct_UClass_UNPCWorldStateSubsystem_Statics;
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUNPCWorldStateSubsystem(); \
	friend struct ::Z_Construct_UClass_UNPCWorldStateSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCWORLDSTATE_API UClass* ::Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(UNPCWorldStateSubsystem, UWorldSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NaturalNPCWorldState"), Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister) \
	DECLARE_SERIALIZER(UNPCWorldStateSubsystem)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UNPCWorldStateSubsystem(); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UNPCWorldStateSubsystem(UNPCWorldStateSubsystem&&) = delete; \
	UNPCWorldStateSubsystem(const UNPCWorldStateSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNPCWorldStateSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNPCWorldStateSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UNPCWorldStateSubsystem) \
	NO_API virtual ~UNPCWorldStateSubsystem();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_17_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h_20_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UNPCWorldStateSubsystem;

// ********** End Class UNPCWorldStateSubsystem ****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
