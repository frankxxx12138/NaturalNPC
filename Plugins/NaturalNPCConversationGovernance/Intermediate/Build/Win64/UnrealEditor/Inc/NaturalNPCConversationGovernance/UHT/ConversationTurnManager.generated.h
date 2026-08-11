// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ConversationTurnManager.h"

#ifdef NATURALNPCCONVERSATIONGOVERNANCE_ConversationTurnManager_generated_h
#error "ConversationTurnManager.generated.h already included, missing '#pragma once' in ConversationTurnManager.h"
#endif
#define NATURALNPCCONVERSATIONGOVERNANCE_ConversationTurnManager_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
enum class EConversationTurnState : uint8;

// ********** Begin Class UConversationTurnManager *************************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetSpeechEndTime); \
	DECLARE_FUNCTION(execGetSpeechStartTime); \
	DECLARE_FUNCTION(execIsMainTurnAvailable); \
	DECLARE_FUNCTION(execGetTurnState); \
	DECLARE_FUNCTION(execGetCurrentSpeaker); \
	DECLARE_FUNCTION(execGetCurrentTurnOwner); \
	DECLARE_FUNCTION(execResolvePermissionPending); \
	DECLARE_FUNCTION(execEnterPermissionPending); \
	DECLARE_FUNCTION(execBeginEmergencyInterrupt); \
	DECLARE_FUNCTION(execEndSideComment); \
	DECLARE_FUNCTION(execBeginSideComment); \
	DECLARE_FUNCTION(execNotifyNPCSpeechEnded); \
	DECLARE_FUNCTION(execNotifyNPCSpeechStarted); \
	DECLARE_FUNCTION(execNotifyPlayerSpeechEnded); \
	DECLARE_FUNCTION(execNotifyPlayerSpeechStarted);


struct Z_Construct_UClass_UConversationTurnManager_Statics;
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationTurnManager_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUConversationTurnManager(); \
	friend struct ::Z_Construct_UClass_UConversationTurnManager_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCCONVERSATIONGOVERNANCE_API UClass* ::Z_Construct_UClass_UConversationTurnManager_NoRegister(); \
public: \
	DECLARE_CLASS2(UConversationTurnManager, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NaturalNPCConversationGovernance"), Z_Construct_UClass_UConversationTurnManager_NoRegister) \
	DECLARE_SERIALIZER(UConversationTurnManager)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UConversationTurnManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UConversationTurnManager(UConversationTurnManager&&) = delete; \
	UConversationTurnManager(const UConversationTurnManager&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UConversationTurnManager); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UConversationTurnManager); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UConversationTurnManager) \
	NO_API virtual ~UConversationTurnManager();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_7_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h_11_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UConversationTurnManager;

// ********** End Class UConversationTurnManager ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
