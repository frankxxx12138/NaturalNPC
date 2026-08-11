// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ConversationGovernanceTypes.h"

#ifdef NATURALNPCCONVERSATIONGOVERNANCE_ConversationGovernanceTypes_generated_h
#error "ConversationGovernanceTypes.generated.h already included, missing '#pragma once' in ConversationGovernanceTypes.h"
#endif
#define NATURALNPCCONVERSATIONGOVERNANCE_ConversationGovernanceTypes_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
enum class EConversationTurnState : uint8;
enum class ENonVerbalReactionType : uint8;
struct FCandidateNPCResponse;

// ********** Begin ScriptStruct FCandidateNPCResponse *********************************************
struct Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics;
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_51_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics; \
	static class UScriptStruct* StaticStruct();


struct FCandidateNPCResponse;
// ********** End ScriptStruct FCandidateNPCResponse ***********************************************

// ********** Begin ScriptStruct FConversationRuntimeState *****************************************
struct Z_Construct_UScriptStruct_FConversationRuntimeState_Statics;
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_126_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FConversationRuntimeState_Statics; \
	static class UScriptStruct* StaticStruct();


struct FConversationRuntimeState;
// ********** End ScriptStruct FConversationRuntimeState *******************************************

// ********** Begin ScriptStruct FConversationGovernanceDebugSnapshot ******************************
struct Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics;
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_156_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics; \
	static class UScriptStruct* StaticStruct();


struct FConversationGovernanceDebugSnapshot;
// ********** End ScriptStruct FConversationGovernanceDebugSnapshot ********************************

// ********** Begin Delegate FConversationListenerChangedEvent *************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_175_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationListenerChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationListenerChangedEvent, AActor* ListenerActor);


// ********** End Delegate FConversationListenerChangedEvent ***************************************

// ********** Begin Delegate FConversationCandidateEvent *******************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_181_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationCandidateEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationCandidateEvent, FCandidateNPCResponse const& Response);


// ********** End Delegate FConversationCandidateEvent *********************************************

// ********** Begin Delegate FConversationPermissionResolvedEvent **********************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_189_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationPermissionResolvedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationPermissionResolvedEvent, FCandidateNPCResponse const& Response, bool bGranted);


// ********** End Delegate FConversationPermissionResolvedEvent ************************************

// ********** Begin Delegate FConversationNonVerbalReactionEvent ***********************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_199_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationNonVerbalReactionEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationNonVerbalReactionEvent, AActor* NPCActor, ENonVerbalReactionType ReactionType, float Intensity);


// ********** End Delegate FConversationNonVerbalReactionEvent *************************************

// ********** Begin Delegate FConversationResponseDiscardedEvent ***********************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_207_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationResponseDiscardedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationResponseDiscardedEvent, FCandidateNPCResponse const& Response, const FString& Reason);


// ********** End Delegate FConversationResponseDiscardedEvent *************************************

// ********** Begin Delegate FConversationTurnOwnerChangedEvent ************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_217_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationTurnOwnerChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationTurnOwnerChangedEvent, AActor* PreviousOwner, AActor* NewOwner, EConversationTurnState NewState);


// ********** End Delegate FConversationTurnOwnerChangedEvent **************************************

// ********** Begin Delegate FConversationGovernanceDebugEvent *************************************
#define FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h_223_DELEGATE \
NATURALNPCCONVERSATIONGOVERNANCE_API void FConversationGovernanceDebugEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationGovernanceDebugEvent, const FString& Message);


// ********** End Delegate FConversationGovernanceDebugEvent ***************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h

// ********** Begin Enum EConversationResponseType *************************************************
#define FOREACH_ENUM_ECONVERSATIONRESPONSETYPE(op) \
	op(EConversationResponseType::Emergency) \
	op(EConversationResponseType::RelevantContribution) \
	op(EConversationResponseType::SideComment) \
	op(EConversationResponseType::AssociativeTopic) \
	op(EConversationResponseType::Silence) \
	op(EConversationResponseType::Invalid) 

enum class EConversationResponseType : uint8;
template<> struct TIsUEnumClass<EConversationResponseType> { enum { Value = true }; };
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<EConversationResponseType>();
// ********** End Enum EConversationResponseType ***************************************************

// ********** Begin Enum EConversationTurnState ****************************************************
#define FOREACH_ENUM_ECONVERSATIONTURNSTATE(op) \
	op(EConversationTurnState::Idle) \
	op(EConversationTurnState::PlayerSpeaking) \
	op(EConversationTurnState::PrimaryNPCSpeaking) \
	op(EConversationTurnState::SecondaryNPCSpeaking) \
	op(EConversationTurnState::PermissionPending) \
	op(EConversationTurnState::EmergencyInterrupt) \
	op(EConversationTurnState::TransitionPause) 

enum class EConversationTurnState : uint8;
template<> struct TIsUEnumClass<EConversationTurnState> { enum { Value = true }; };
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<EConversationTurnState>();
// ********** End Enum EConversationTurnState ******************************************************

// ********** Begin Enum ENonVerbalReactionType ****************************************************
#define FOREACH_ENUM_ENONVERBALREACTIONTYPE(op) \
	op(ENonVerbalReactionType::None) \
	op(ENonVerbalReactionType::LookAtSpeaker) \
	op(ENonVerbalReactionType::LookAtPlayer) \
	op(ENonVerbalReactionType::Nod) \
	op(ENonVerbalReactionType::ShakeHead) \
	op(ENonVerbalReactionType::Smile) \
	op(ENonVerbalReactionType::Laugh) \
	op(ENonVerbalReactionType::Surprise) \
	op(ENonVerbalReactionType::Concern) \
	op(ENonVerbalReactionType::Fear) \
	op(ENonVerbalReactionType::RaiseHand) \
	op(ENonVerbalReactionType::LeanForward) 

enum class ENonVerbalReactionType : uint8;
template<> struct TIsUEnumClass<ENonVerbalReactionType> { enum { Value = true }; };
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<ENonVerbalReactionType>();
// ********** End Enum ENonVerbalReactionType ******************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
