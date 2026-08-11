// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ConversationResponseQueue.h"

#ifdef NATURALNPCCONVERSATIONGOVERNANCE_ConversationResponseQueue_generated_h
#error "ConversationResponseQueue.generated.h already included, missing '#pragma once' in ConversationResponseQueue.h"
#endif
#define NATURALNPCCONVERSATIONGOVERNANCE_ConversationResponseQueue_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FCandidateNPCResponse;
struct FGuid;

// ********** Begin Class UConversationResponseQueue ***********************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execNum); \
	DECLARE_FUNCTION(execCountResponsesFromNPC); \
	DECLARE_FUNCTION(execHasResponseFromNPC); \
	DECLARE_FUNCTION(execGetQueuedResponses); \
	DECLARE_FUNCTION(execGetResponseByID); \
	DECLARE_FUNCTION(execPeekNextResponse); \
	DECLARE_FUNCTION(execPopNextResponse); \
	DECLARE_FUNCTION(execReprioritizeResponse); \
	DECLARE_FUNCTION(execRemoveAllFromConversation); \
	DECLARE_FUNCTION(execRemoveAllFromNPC); \
	DECLARE_FUNCTION(execRemoveByResponseID);


struct Z_Construct_UClass_UConversationResponseQueue_Statics;
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationResponseQueue_NoRegister();

#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUConversationResponseQueue(); \
	friend struct ::Z_Construct_UClass_UConversationResponseQueue_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend NATURALNPCCONVERSATIONGOVERNANCE_API UClass* ::Z_Construct_UClass_UConversationResponseQueue_NoRegister(); \
public: \
	DECLARE_CLASS2(UConversationResponseQueue, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/NaturalNPCConversationGovernance"), Z_Construct_UClass_UConversationResponseQueue_NoRegister) \
	DECLARE_SERIALIZER(UConversationResponseQueue)


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UConversationResponseQueue(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UConversationResponseQueue(UConversationResponseQueue&&) = delete; \
	UConversationResponseQueue(const UConversationResponseQueue&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UConversationResponseQueue); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UConversationResponseQueue); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UConversationResponseQueue) \
	NO_API virtual ~UConversationResponseQueue();


#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_7_PROLOG
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_INCLASS_NO_PURE_DECLS \
	FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h_11_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UConversationResponseQueue;

// ********** End Class UConversationResponseQueue *************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
