// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "NPCWorldStateAgentComponent.h"

#ifdef NATURALNPCWORLDSTATE_NPCWorldStateAgentComponent_generated_h
#error "NPCWorldStateAgentComponent.generated.h already included, missing '#pragma once' in NPCWorldStateAgentComponent.h"
#endif
#define NATURALNPCWORLDSTATE_NPCWorldStateAgentComponent_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UAnimSequence;
struct FNPCWorldActionResult;
struct FNPCWorldObjectState;

// ********** Begin Delegate FNPCWorldStateUpdatedEvent ********************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_19_DELEGATE \
NATURALNPCWORLDSTATE_API void FNPCWorldStateUpdatedEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldStateUpdatedEvent, const FString& WorldStateJson);


// ********** End Delegate FNPCWorldStateUpdatedEvent **********************************************

// ********** Begin Delegate FNPCWorldActionCompletedEvent *****************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_25_DELEGATE \
NATURALNPCWORLDSTATE_API void FNPCWorldActionCompletedEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldActionCompletedEvent, FNPCWorldActionResult const& Result);


// ********** End Delegate FNPCWorldActionCompletedEvent *******************************************

// ********** Begin Class UNPCWorldStateAgentComponent *********************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execResumeHeldIdleAnimation); \
	DECLARE_FUNCTION(execSuspendHeldIdleAnimation); \
	DECLARE_FUNCTION(execGetCurrentHeldIdleAnimation); \
	DECLARE_FUNCTION(execGetHeldWalkAnimation); \
	DECLARE_FUNCTION(execDropHeldActor); \
	DECLARE_FUNCTION(execIsWorldActionInProgress); \
	DECLARE_FUNCTION(execGetHeldActor); \
	DECLARE_FUNCTION(execTryExecuteNaturalLanguageAction); \
	DECLARE_FUNCTION(execExecuteWorldAction); \
	DECLARE_FUNCTION(execGetVisibleWorldObjects); \
	DECLARE_FUNCTION(execGetWorldStateText); \
	DECLARE_FUNCTION(execGetPickupAnimationForTarget); \
	DECLARE_FUNCTION(execGetWorldStateJson); \
	DECLARE_FUNCTION(execRefreshWorldState);


struct Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics;
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUNPCWorldStateAgentComponent(); \
	friend struct ::Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCWORLDSTATE_API UClass* ::Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister(); \
public: \
	DECLARE_CLASS2(UNPCWorldStateAgentComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NaturalNPCWorldState"), Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister) \
	DECLARE_SERIALIZER(UNPCWorldStateAgentComponent)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UNPCWorldStateAgentComponent(UNPCWorldStateAgentComponent&&) = delete; \
	UNPCWorldStateAgentComponent(const UNPCWorldStateAgentComponent&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UNPCWorldStateAgentComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UNPCWorldStateAgentComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UNPCWorldStateAgentComponent) \
	NO_API virtual ~UNPCWorldStateAgentComponent();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_27_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h_31_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UNPCWorldStateAgentComponent;

// ********** End Class UNPCWorldStateAgentComponent ***********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
