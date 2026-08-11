// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ConversationGovernanceComponent.h"

#ifdef NATURALNPCCONVERSATIONGOVERNANCE_ConversationGovernanceComponent_generated_h
#error "ConversationGovernanceComponent.generated.h already included, missing '#pragma once' in ConversationGovernanceComponent.h"
#endif
#define NATURALNPCCONVERSATIONGOVERNANCE_ConversationGovernanceComponent_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UConversationListenerComponent;
class UConversationResponseQueue;
class UConversationTurnManager;
enum class EConversationResponseType : uint8;
enum class EConversationTurnState : uint8;
enum class ENonVerbalReactionType : uint8;
struct FCandidateNPCResponse;
struct FConversationGovernanceDebugSnapshot;
struct FConversationRuntimeState;
struct FGuid;

// ********** Begin Class UConversationGovernanceComponent *****************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execHandleTurnOwnerChanged); \
	DECLARE_FUNCTION(execGetResponseQueue); \
	DECLARE_FUNCTION(execGetTurnManager); \
	DECLARE_FUNCTION(execGetPrimaryNPC); \
	DECLARE_FUNCTION(execIsConversationActive); \
	DECLARE_FUNCTION(execGetDebugSnapshot); \
	DECLARE_FUNCTION(execGetRuntimeState); \
	DECLARE_FUNCTION(execGetQueuedResponses); \
	DECLARE_FUNCTION(execGetTurnState); \
	DECLARE_FUNCTION(execGetCurrentTurnOwner); \
	DECLARE_FUNCTION(execCancelResponse); \
	DECLARE_FUNCTION(execRequestNonVerbalReaction); \
	DECLARE_FUNCTION(execNotifyResponseExecutionFinished); \
	DECLARE_FUNCTION(execNotifyResponseExecutionStarted); \
	DECLARE_FUNCTION(execGetPendingPermissionResponse); \
	DECLARE_FUNCTION(execHasPendingPermissionRequest); \
	DECLARE_FUNCTION(execDenyPermission); \
	DECLARE_FUNCTION(execGrantPermission); \
	DECLARE_FUNCTION(execRequestSpeakingPermission); \
	DECLARE_FUNCTION(execBeginEmergencyInterrupt); \
	DECLARE_FUNCTION(execEndSideComment); \
	DECLARE_FUNCTION(execBeginSideComment); \
	DECLARE_FUNCTION(execNotifyNPCSpeechEnded); \
	DECLARE_FUNCTION(execNotifyNPCSpeechStarted); \
	DECLARE_FUNCTION(execNotifyPlayerSpeechEnded); \
	DECLARE_FUNCTION(execNotifyPlayerSpeechStarted); \
	DECLARE_FUNCTION(execSubmitCandidateResponseData); \
	DECLARE_FUNCTION(execSubmitCandidateResponse); \
	DECLARE_FUNCTION(execIsListenerEligible); \
	DECLARE_FUNCTION(execRefreshListenerEligibility); \
	DECLARE_FUNCTION(execUnregisterListener); \
	DECLARE_FUNCTION(execRegisterListener); \
	DECLARE_FUNCTION(execEndConversation); \
	DECLARE_FUNCTION(execStartConversation);


struct Z_Construct_UClass_UConversationGovernanceComponent_Statics;
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationGovernanceComponent_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUConversationGovernanceComponent(); \
	friend struct ::Z_Construct_UClass_UConversationGovernanceComponent_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCCONVERSATIONGOVERNANCE_API UClass* ::Z_Construct_UClass_UConversationGovernanceComponent_NoRegister(); \
public: \
	DECLARE_CLASS2(UConversationGovernanceComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/NaturalNPCConversationGovernance"), Z_Construct_UClass_UConversationGovernanceComponent_NoRegister) \
	DECLARE_SERIALIZER(UConversationGovernanceComponent)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UConversationGovernanceComponent(UConversationGovernanceComponent&&) = delete; \
	UConversationGovernanceComponent(const UConversationGovernanceComponent&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UConversationGovernanceComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UConversationGovernanceComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UConversationGovernanceComponent) \
	NO_API virtual ~UConversationGovernanceComponent();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_14_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UConversationGovernanceComponent;

// ********** End Class UConversationGovernanceComponent *******************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
