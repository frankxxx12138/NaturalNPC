// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NPCWorldActionTarget.h"

#ifdef NATURALNPCWORLDSTATE_NPCWorldActionTarget_generated_h
#error "NPCWorldActionTarget.generated.h already included, missing '#pragma once' in NPCWorldActionTarget.h"
#endif
#define NATURALNPCWORLDSTATE_NPCWorldActionTarget_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;

// ********** Begin Interface UNPCWorldActionTarget ************************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual bool ExecuteNPCWorldAction_Implementation(AActor* NPC, FName ActionId, const FString& Parameters, FString& ResultMessage) { return false; }; \
	DECLARE_FUNCTION(execExecuteNPCWorldAction);


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UNPCWorldActionTarget_Statics;
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldActionTarget_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UNPCWorldActionTarget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UNPCWorldActionTarget(UNPCWorldActionTarget&&) = delete; \
	UNPCWorldActionTarget(const UNPCWorldActionTarget&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNPCWorldActionTarget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNPCWorldActionTarget); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UNPCWorldActionTarget) \
	virtual ~UNPCWorldActionTarget() = default;


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUNPCWorldActionTarget(); \
	friend struct ::Z_Construct_UClass_UNPCWorldActionTarget_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCWORLDSTATE_API UClass* ::Z_Construct_UClass_UNPCWorldActionTarget_NoRegister(); \
public: \
	DECLARE_CLASS2(UNPCWorldActionTarget, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/NaturalNPCWorldState"), Z_Construct_UClass_UNPCWorldActionTarget_NoRegister) \
	DECLARE_SERIALIZER(UNPCWorldActionTarget)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_GENERATED_UINTERFACE_BODY() \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~INPCWorldActionTarget() {} \
public: \
	typedef UNPCWorldActionTarget UClassType; \
	typedef INPCWorldActionTarget ThisClass; \
	static bool Execute_ExecuteNPCWorldAction(UObject* O, AActor* NPC, FName ActionId, const FString& Parameters, FString& ResultMessage); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_9_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_CALLBACK_WRAPPERS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h_12_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UNPCWorldActionTarget;

// ********** End Interface UNPCWorldActionTarget **************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
