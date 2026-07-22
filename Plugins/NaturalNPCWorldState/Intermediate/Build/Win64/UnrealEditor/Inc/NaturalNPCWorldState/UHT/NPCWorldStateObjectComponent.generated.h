// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NPCWorldStateObjectComponent.h"

#ifdef NATURALNPCWORLDSTATE_NPCWorldStateObjectComponent_generated_h
#error "NPCWorldStateObjectComponent.generated.h already included, missing '#pragma once' in NPCWorldStateObjectComponent.h"
#endif
#define NATURALNPCWORLDSTATE_NPCWorldStateObjectComponent_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
struct FNPCWorldActionDefinition;

// ********** Begin Class UNPCWorldStateObjectComponent ********************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execRemoveWorldStateValue); \
	DECLARE_FUNCTION(execSetWorldStateValue); \
	DECLARE_FUNCTION(execGetAvailableActions); \
	DECLARE_FUNCTION(execGetResolvedDisplayName); \
	DECLARE_FUNCTION(execGetResolvedObjectId);


struct Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics;
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUNPCWorldStateObjectComponent(); \
	friend struct ::Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCWORLDSTATE_API UClass* ::Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister(); \
public: \
	DECLARE_CLASS2(UNPCWorldStateObjectComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NaturalNPCWorldState"), Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister) \
	DECLARE_SERIALIZER(UNPCWorldStateObjectComponent)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UNPCWorldStateObjectComponent(UNPCWorldStateObjectComponent&&) = delete; \
	UNPCWorldStateObjectComponent(const UNPCWorldStateObjectComponent&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNPCWorldStateObjectComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNPCWorldStateObjectComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UNPCWorldStateObjectComponent) \
	NO_API virtual ~UNPCWorldStateObjectComponent();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_7_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h_11_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UNPCWorldStateObjectComponent;

// ********** End Class UNPCWorldStateObjectComponent **********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
