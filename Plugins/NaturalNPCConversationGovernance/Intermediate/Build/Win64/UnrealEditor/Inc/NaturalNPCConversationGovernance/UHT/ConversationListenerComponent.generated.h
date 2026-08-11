// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ConversationListenerComponent.h"

#ifdef NATURALNPCCONVERSATIONGOVERNANCE_ConversationListenerComponent_generated_h
#error "ConversationListenerComponent.generated.h already included, missing '#pragma once' in ConversationListenerComponent.h"
#endif
#define NATURALNPCCONVERSATIONGOVERNANCE_ConversationListenerComponent_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;

// ********** Begin Class UConversationListenerComponent *******************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetDistanceToActor); \
	DECLARE_FUNCTION(execCanHearActor); \
	DECLARE_FUNCTION(execGetResolvedDisplayName); \
	DECLARE_FUNCTION(execGetResolvedNPCID);


struct Z_Construct_UClass_UConversationListenerComponent_Statics;
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationListenerComponent_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUConversationListenerComponent(); \
	friend struct ::Z_Construct_UClass_UConversationListenerComponent_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCCONVERSATIONGOVERNANCE_API UClass* ::Z_Construct_UClass_UConversationListenerComponent_NoRegister(); \
public: \
	DECLARE_CLASS2(UConversationListenerComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NaturalNPCConversationGovernance"), Z_Construct_UClass_UConversationListenerComponent_NoRegister) \
	DECLARE_SERIALIZER(UConversationListenerComponent)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UConversationListenerComponent(UConversationListenerComponent&&) = delete; \
	UConversationListenerComponent(const UConversationListenerComponent&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UConversationListenerComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UConversationListenerComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UConversationListenerComponent) \
	NO_API virtual ~UConversationListenerComponent();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_9_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UConversationListenerComponent;

// ********** End Class UConversationListenerComponent *********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
