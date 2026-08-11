// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ConversationGovernanceTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeConversationGovernanceTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType();
NATURALNPCCONVERSATIONGOVERNANCE_API UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState();
NATURALNPCCONVERSATIONGOVERNANCE_API UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature();
NATURALNPCCONVERSATIONGOVERNANCE_API UScriptStruct* Z_Construct_UScriptStruct_FCandidateNPCResponse();
NATURALNPCCONVERSATIONGOVERNANCE_API UScriptStruct* Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot();
NATURALNPCCONVERSATIONGOVERNANCE_API UScriptStruct* Z_Construct_UScriptStruct_FConversationRuntimeState();
UPackage* Z_Construct_UPackage__Script_NaturalNPCConversationGovernance();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum EConversationResponseType *************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EConversationResponseType;
static UEnum* EConversationResponseType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EConversationResponseType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EConversationResponseType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("EConversationResponseType"));
	}
	return Z_Registration_Info_UEnum_EConversationResponseType.OuterSingleton;
}
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<EConversationResponseType>()
{
	return EConversationResponseType_StaticEnum();
}
struct Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "AssociativeTopic.Name", "EConversationResponseType::AssociativeTopic" },
		{ "BlueprintType", "true" },
		{ "Emergency.Name", "EConversationResponseType::Emergency" },
		{ "Invalid.Name", "EConversationResponseType::Invalid" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
		{ "RelevantContribution.Name", "EConversationResponseType::RelevantContribution" },
		{ "SideComment.Name", "EConversationResponseType::SideComment" },
		{ "Silence.Name", "EConversationResponseType::Silence" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EConversationResponseType::Emergency", (int64)EConversationResponseType::Emergency },
		{ "EConversationResponseType::RelevantContribution", (int64)EConversationResponseType::RelevantContribution },
		{ "EConversationResponseType::SideComment", (int64)EConversationResponseType::SideComment },
		{ "EConversationResponseType::AssociativeTopic", (int64)EConversationResponseType::AssociativeTopic },
		{ "EConversationResponseType::Silence", (int64)EConversationResponseType::Silence },
		{ "EConversationResponseType::Invalid", (int64)EConversationResponseType::Invalid },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	"EConversationResponseType",
	"EConversationResponseType",
	Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType()
{
	if (!Z_Registration_Info_UEnum_EConversationResponseType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EConversationResponseType.InnerSingleton, Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EConversationResponseType.InnerSingleton;
}
// ********** End Enum EConversationResponseType ***************************************************

// ********** Begin Enum EConversationTurnState ****************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EConversationTurnState;
static UEnum* EConversationTurnState_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EConversationTurnState.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EConversationTurnState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("EConversationTurnState"));
	}
	return Z_Registration_Info_UEnum_EConversationTurnState.OuterSingleton;
}
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<EConversationTurnState>()
{
	return EConversationTurnState_StaticEnum();
}
struct Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "EmergencyInterrupt.Name", "EConversationTurnState::EmergencyInterrupt" },
		{ "Idle.Name", "EConversationTurnState::Idle" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
		{ "PermissionPending.Name", "EConversationTurnState::PermissionPending" },
		{ "PlayerSpeaking.Name", "EConversationTurnState::PlayerSpeaking" },
		{ "PrimaryNPCSpeaking.Name", "EConversationTurnState::PrimaryNPCSpeaking" },
		{ "SecondaryNPCSpeaking.Name", "EConversationTurnState::SecondaryNPCSpeaking" },
		{ "TransitionPause.Name", "EConversationTurnState::TransitionPause" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EConversationTurnState::Idle", (int64)EConversationTurnState::Idle },
		{ "EConversationTurnState::PlayerSpeaking", (int64)EConversationTurnState::PlayerSpeaking },
		{ "EConversationTurnState::PrimaryNPCSpeaking", (int64)EConversationTurnState::PrimaryNPCSpeaking },
		{ "EConversationTurnState::SecondaryNPCSpeaking", (int64)EConversationTurnState::SecondaryNPCSpeaking },
		{ "EConversationTurnState::PermissionPending", (int64)EConversationTurnState::PermissionPending },
		{ "EConversationTurnState::EmergencyInterrupt", (int64)EConversationTurnState::EmergencyInterrupt },
		{ "EConversationTurnState::TransitionPause", (int64)EConversationTurnState::TransitionPause },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	"EConversationTurnState",
	"EConversationTurnState",
	Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState()
{
	if (!Z_Registration_Info_UEnum_EConversationTurnState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EConversationTurnState.InnerSingleton, Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EConversationTurnState.InnerSingleton;
}
// ********** End Enum EConversationTurnState ******************************************************

// ********** Begin Enum ENonVerbalReactionType ****************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ENonVerbalReactionType;
static UEnum* ENonVerbalReactionType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ENonVerbalReactionType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ENonVerbalReactionType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("ENonVerbalReactionType"));
	}
	return Z_Registration_Info_UEnum_ENonVerbalReactionType.OuterSingleton;
}
template<> NATURALNPCCONVERSATIONGOVERNANCE_NON_ATTRIBUTED_API UEnum* StaticEnum<ENonVerbalReactionType>()
{
	return ENonVerbalReactionType_StaticEnum();
}
struct Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Concern.Name", "ENonVerbalReactionType::Concern" },
		{ "Fear.Name", "ENonVerbalReactionType::Fear" },
		{ "Laugh.Name", "ENonVerbalReactionType::Laugh" },
		{ "LeanForward.Name", "ENonVerbalReactionType::LeanForward" },
		{ "LookAtPlayer.Name", "ENonVerbalReactionType::LookAtPlayer" },
		{ "LookAtSpeaker.Name", "ENonVerbalReactionType::LookAtSpeaker" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
		{ "Nod.Name", "ENonVerbalReactionType::Nod" },
		{ "None.Name", "ENonVerbalReactionType::None" },
		{ "RaiseHand.Name", "ENonVerbalReactionType::RaiseHand" },
		{ "ShakeHead.Name", "ENonVerbalReactionType::ShakeHead" },
		{ "Smile.Name", "ENonVerbalReactionType::Smile" },
		{ "Surprise.Name", "ENonVerbalReactionType::Surprise" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ENonVerbalReactionType::None", (int64)ENonVerbalReactionType::None },
		{ "ENonVerbalReactionType::LookAtSpeaker", (int64)ENonVerbalReactionType::LookAtSpeaker },
		{ "ENonVerbalReactionType::LookAtPlayer", (int64)ENonVerbalReactionType::LookAtPlayer },
		{ "ENonVerbalReactionType::Nod", (int64)ENonVerbalReactionType::Nod },
		{ "ENonVerbalReactionType::ShakeHead", (int64)ENonVerbalReactionType::ShakeHead },
		{ "ENonVerbalReactionType::Smile", (int64)ENonVerbalReactionType::Smile },
		{ "ENonVerbalReactionType::Laugh", (int64)ENonVerbalReactionType::Laugh },
		{ "ENonVerbalReactionType::Surprise", (int64)ENonVerbalReactionType::Surprise },
		{ "ENonVerbalReactionType::Concern", (int64)ENonVerbalReactionType::Concern },
		{ "ENonVerbalReactionType::Fear", (int64)ENonVerbalReactionType::Fear },
		{ "ENonVerbalReactionType::RaiseHand", (int64)ENonVerbalReactionType::RaiseHand },
		{ "ENonVerbalReactionType::LeanForward", (int64)ENonVerbalReactionType::LeanForward },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	"ENonVerbalReactionType",
	"ENonVerbalReactionType",
	Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType()
{
	if (!Z_Registration_Info_UEnum_ENonVerbalReactionType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ENonVerbalReactionType.InnerSingleton, Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ENonVerbalReactionType.InnerSingleton;
}
// ********** End Enum ENonVerbalReactionType ******************************************************

// ********** Begin ScriptStruct FCandidateNPCResponse *********************************************
struct Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FCandidateNPCResponse); }
	static inline consteval int16 GetStructAlignment() { return alignof(FCandidateNPCResponse); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResponseID_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConversationID_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NPCID_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NPCActor_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResponseText_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
		{ "MultiLine", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResponseType_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RelevanceScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UrgencyScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TopicShiftScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TopicAffinityScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConversationImpactScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConfidenceScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PriorityScore_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CreatedTime_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExpiryTime_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRequiresPermission_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanOverlapMainTurn_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bOwnsTurnWhenSpoken_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bHasUniqueInformation_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bTopicTransitionReady_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VerifiedEmergencyEventID_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PermissionRequestText_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
		{ "MultiLine", "TRUE" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FCandidateNPCResponse constinit property declarations *************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ConversationID;
	static const UECodeGen_Private::FNamePropertyParams NewProp_NPCID;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_NPCActor;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ResponseText;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ResponseType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ResponseType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RelevanceScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_UrgencyScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TopicShiftScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TopicAffinityScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ConversationImpactScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ConfidenceScore;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PriorityScore;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_CreatedTime;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_ExpiryTime;
	static void NewProp_bRequiresPermission_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRequiresPermission;
	static void NewProp_bCanOverlapMainTurn_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanOverlapMainTurn;
	static void NewProp_bOwnsTurnWhenSpoken_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOwnsTurnWhenSpoken;
	static void NewProp_bHasUniqueInformation_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bHasUniqueInformation;
	static void NewProp_bTopicTransitionReady_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bTopicTransitionReady;
	static const UECodeGen_Private::FNamePropertyParams NewProp_VerifiedEmergencyEventID;
	static const UECodeGen_Private::FStrPropertyParams NewProp_PermissionRequestText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FCandidateNPCResponse constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FCandidateNPCResponse>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FCandidateNPCResponse;
class UScriptStruct* FCandidateNPCResponse::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FCandidateNPCResponse, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("CandidateNPCResponse"));
	}
	return Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.OuterSingleton;
	}

// ********** Begin ScriptStruct FCandidateNPCResponse Property Definitions ************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResponseID_MetaData), NewProp_ResponseID_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConversationID = { "ConversationID", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ConversationID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConversationID_MetaData), NewProp_ConversationID_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_NPCID = { "NPCID", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, NPCID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NPCID_MetaData), NewProp_NPCID_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0014000000000004, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NPCActor_MetaData), NewProp_NPCActor_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseText = { "ResponseText", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ResponseText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResponseText_MetaData), NewProp_ResponseText_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseType = { "ResponseType", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ResponseType), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResponseType_MetaData), NewProp_ResponseType_MetaData) }; // 3150995788
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_RelevanceScore = { "RelevanceScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, RelevanceScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RelevanceScore_MetaData), NewProp_RelevanceScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_UrgencyScore = { "UrgencyScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, UrgencyScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UrgencyScore_MetaData), NewProp_UrgencyScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_TopicShiftScore = { "TopicShiftScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, TopicShiftScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TopicShiftScore_MetaData), NewProp_TopicShiftScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_TopicAffinityScore = { "TopicAffinityScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, TopicAffinityScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TopicAffinityScore_MetaData), NewProp_TopicAffinityScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConversationImpactScore = { "ConversationImpactScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ConversationImpactScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConversationImpactScore_MetaData), NewProp_ConversationImpactScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConfidenceScore = { "ConfidenceScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ConfidenceScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConfidenceScore_MetaData), NewProp_ConfidenceScore_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_PriorityScore = { "PriorityScore", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, PriorityScore), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PriorityScore_MetaData), NewProp_PriorityScore_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_CreatedTime = { "CreatedTime", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, CreatedTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CreatedTime_MetaData), NewProp_CreatedTime_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ExpiryTime = { "ExpiryTime", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, ExpiryTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExpiryTime_MetaData), NewProp_ExpiryTime_MetaData) };
void Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bRequiresPermission_SetBit(void* Obj)
{
	((FCandidateNPCResponse*)Obj)->bRequiresPermission = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bRequiresPermission = { "bRequiresPermission", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FCandidateNPCResponse), &Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bRequiresPermission_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRequiresPermission_MetaData), NewProp_bRequiresPermission_MetaData) };
void Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bCanOverlapMainTurn_SetBit(void* Obj)
{
	((FCandidateNPCResponse*)Obj)->bCanOverlapMainTurn = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bCanOverlapMainTurn = { "bCanOverlapMainTurn", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FCandidateNPCResponse), &Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bCanOverlapMainTurn_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanOverlapMainTurn_MetaData), NewProp_bCanOverlapMainTurn_MetaData) };
void Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bOwnsTurnWhenSpoken_SetBit(void* Obj)
{
	((FCandidateNPCResponse*)Obj)->bOwnsTurnWhenSpoken = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bOwnsTurnWhenSpoken = { "bOwnsTurnWhenSpoken", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FCandidateNPCResponse), &Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bOwnsTurnWhenSpoken_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bOwnsTurnWhenSpoken_MetaData), NewProp_bOwnsTurnWhenSpoken_MetaData) };
void Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bHasUniqueInformation_SetBit(void* Obj)
{
	((FCandidateNPCResponse*)Obj)->bHasUniqueInformation = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bHasUniqueInformation = { "bHasUniqueInformation", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FCandidateNPCResponse), &Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bHasUniqueInformation_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bHasUniqueInformation_MetaData), NewProp_bHasUniqueInformation_MetaData) };
void Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bTopicTransitionReady_SetBit(void* Obj)
{
	((FCandidateNPCResponse*)Obj)->bTopicTransitionReady = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bTopicTransitionReady = { "bTopicTransitionReady", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FCandidateNPCResponse), &Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bTopicTransitionReady_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bTopicTransitionReady_MetaData), NewProp_bTopicTransitionReady_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_VerifiedEmergencyEventID = { "VerifiedEmergencyEventID", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, VerifiedEmergencyEventID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VerifiedEmergencyEventID_MetaData), NewProp_VerifiedEmergencyEventID_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_PermissionRequestText = { "PermissionRequestText", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FCandidateNPCResponse, PermissionRequestText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PermissionRequestText_MetaData), NewProp_PermissionRequestText_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConversationID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_NPCID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ResponseType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_RelevanceScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_UrgencyScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_TopicShiftScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_TopicAffinityScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConversationImpactScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ConfidenceScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_PriorityScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_CreatedTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_ExpiryTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bRequiresPermission,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bCanOverlapMainTurn,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bOwnsTurnWhenSpoken,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bHasUniqueInformation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_bTopicTransitionReady,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_VerifiedEmergencyEventID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewProp_PermissionRequestText,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FCandidateNPCResponse Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	&NewStructOps,
	"CandidateNPCResponse",
	Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::PropPointers),
	sizeof(FCandidateNPCResponse),
	alignof(FCandidateNPCResponse),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FCandidateNPCResponse()
{
	if (!Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.InnerSingleton, Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FCandidateNPCResponse.InnerSingleton);
}
// ********** End ScriptStruct FCandidateNPCResponse ***********************************************

// ********** Begin ScriptStruct FConversationRuntimeState *****************************************
struct Z_Construct_UScriptStruct_FConversationRuntimeState_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FConversationRuntimeState); }
	static inline consteval int16 GetStructAlignment() { return alignof(FConversationRuntimeState); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConversationID_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsActive_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PrimaryNPC_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TurnState_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentTurnOwner_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentSpeaker_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConversationStartTime_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConversationEndTime_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FConversationRuntimeState constinit property declarations *********
	static const UECodeGen_Private::FNamePropertyParams NewProp_ConversationID;
	static void NewProp_bIsActive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsActive;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_PrimaryNPC;
	static const UECodeGen_Private::FBytePropertyParams NewProp_TurnState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_TurnState;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_CurrentTurnOwner;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_CurrentSpeaker;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_ConversationStartTime;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_ConversationEndTime;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FConversationRuntimeState constinit property declarations ***********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FConversationRuntimeState>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FConversationRuntimeState_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FConversationRuntimeState;
class UScriptStruct* FConversationRuntimeState::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FConversationRuntimeState.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FConversationRuntimeState.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FConversationRuntimeState, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("ConversationRuntimeState"));
	}
	return Z_Registration_Info_UScriptStruct_FConversationRuntimeState.OuterSingleton;
	}

// ********** Begin ScriptStruct FConversationRuntimeState Property Definitions ********************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationID = { "ConversationID", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, ConversationID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConversationID_MetaData), NewProp_ConversationID_MetaData) };
void Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_bIsActive_SetBit(void* Obj)
{
	((FConversationRuntimeState*)Obj)->bIsActive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_bIsActive = { "bIsActive", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FConversationRuntimeState), &Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_bIsActive_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsActive_MetaData), NewProp_bIsActive_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_PrimaryNPC = { "PrimaryNPC", nullptr, (EPropertyFlags)0x0014000000000014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, PrimaryNPC), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PrimaryNPC_MetaData), NewProp_PrimaryNPC_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_TurnState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_TurnState = { "TurnState", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, TurnState), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TurnState_MetaData), NewProp_TurnState_MetaData) }; // 1236676757
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_CurrentTurnOwner = { "CurrentTurnOwner", nullptr, (EPropertyFlags)0x0014000000000014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, CurrentTurnOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentTurnOwner_MetaData), NewProp_CurrentTurnOwner_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_CurrentSpeaker = { "CurrentSpeaker", nullptr, (EPropertyFlags)0x0014000000000014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, CurrentSpeaker), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentSpeaker_MetaData), NewProp_CurrentSpeaker_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationStartTime = { "ConversationStartTime", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, ConversationStartTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConversationStartTime_MetaData), NewProp_ConversationStartTime_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationEndTime = { "ConversationEndTime", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationRuntimeState, ConversationEndTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConversationEndTime_MetaData), NewProp_ConversationEndTime_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_bIsActive,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_PrimaryNPC,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_TurnState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_TurnState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_CurrentTurnOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_CurrentSpeaker,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationStartTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewProp_ConversationEndTime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FConversationRuntimeState Property Definitions **********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	&NewStructOps,
	"ConversationRuntimeState",
	Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::PropPointers),
	sizeof(FConversationRuntimeState),
	alignof(FConversationRuntimeState),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FConversationRuntimeState()
{
	if (!Z_Registration_Info_UScriptStruct_FConversationRuntimeState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FConversationRuntimeState.InnerSingleton, Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FConversationRuntimeState.InnerSingleton);
}
// ********** End ScriptStruct FConversationRuntimeState *******************************************

// ********** Begin ScriptStruct FConversationGovernanceDebugSnapshot ******************************
struct Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FConversationGovernanceDebugSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FConversationGovernanceDebugSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuntimeState_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EligibleListeners_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_QueuedResponses_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LastDecision_MetaData[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FConversationGovernanceDebugSnapshot constinit property declarations 
	static const UECodeGen_Private::FStructPropertyParams NewProp_RuntimeState;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_EligibleListeners_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_EligibleListeners;
	static const UECodeGen_Private::FStructPropertyParams NewProp_QueuedResponses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_QueuedResponses;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LastDecision;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FConversationGovernanceDebugSnapshot constinit property declarations 
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FConversationGovernanceDebugSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot;
class UScriptStruct* FConversationGovernanceDebugSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot, (UObject*)Z_Construct_UPackage__Script_NaturalNPCConversationGovernance(), TEXT("ConversationGovernanceDebugSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FConversationGovernanceDebugSnapshot Property Definitions *********
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_RuntimeState = { "RuntimeState", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationGovernanceDebugSnapshot, RuntimeState), Z_Construct_UScriptStruct_FConversationRuntimeState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuntimeState_MetaData), NewProp_RuntimeState_MetaData) }; // 2795981430
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_EligibleListeners_Inner = { "EligibleListeners", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_EligibleListeners = { "EligibleListeners", nullptr, (EPropertyFlags)0x0114000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationGovernanceDebugSnapshot, EligibleListeners), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EligibleListeners_MetaData), NewProp_EligibleListeners_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_QueuedResponses_Inner = { "QueuedResponses", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_QueuedResponses = { "QueuedResponses", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationGovernanceDebugSnapshot, QueuedResponses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_QueuedResponses_MetaData), NewProp_QueuedResponses_MetaData) }; // 3013045577
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_LastDecision = { "LastDecision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FConversationGovernanceDebugSnapshot, LastDecision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LastDecision_MetaData), NewProp_LastDecision_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_RuntimeState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_EligibleListeners_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_EligibleListeners,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_QueuedResponses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_QueuedResponses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewProp_LastDecision,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FConversationGovernanceDebugSnapshot Property Definitions ***********
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
	nullptr,
	&NewStructOps,
	"ConversationGovernanceDebugSnapshot",
	Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::PropPointers),
	sizeof(FConversationGovernanceDebugSnapshot),
	alignof(FConversationGovernanceDebugSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FConversationGovernanceDebugSnapshot ********************************

// ********** Begin Delegate FConversationListenerChangedEvent *************************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms
	{
		AActor* ListenerActor;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationListenerChangedEvent constinit property declarations *****
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ListenerActor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationListenerChangedEvent constinit property declarations *******
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationListenerChangedEvent Property Definitions ****************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::NewProp_ListenerActor = { "ListenerActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms, ListenerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::NewProp_ListenerActor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationListenerChangedEvent Property Definitions ******************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationListenerChangedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationListenerChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationListenerChangedEvent, AActor* ListenerActor)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms
	{
		AActor* ListenerActor;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationListenerChangedEvent_Parms Parms;
	Parms.ListenerActor=ListenerActor;
	ConversationListenerChangedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationListenerChangedEvent ***************************************

// ********** Begin Delegate FConversationCandidateEvent *******************************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms
	{
		FCandidateNPCResponse Response;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Response_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationCandidateEvent constinit property declarations ***********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Response;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationCandidateEvent constinit property declarations *************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationCandidateEvent Property Definitions **********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::NewProp_Response = { "Response", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms, Response), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Response_MetaData), NewProp_Response_MetaData) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::NewProp_Response,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationCandidateEvent Property Definitions ************************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationCandidateEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00530000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationCandidateEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationCandidateEvent, FCandidateNPCResponse const& Response)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms
	{
		FCandidateNPCResponse Response;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationCandidateEvent_Parms Parms;
	Parms.Response=Response;
	ConversationCandidateEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationCandidateEvent *********************************************

// ********** Begin Delegate FConversationPermissionResolvedEvent **********************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms
	{
		FCandidateNPCResponse Response;
		bool bGranted;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Response_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationPermissionResolvedEvent constinit property declarations **
	static const UECodeGen_Private::FStructPropertyParams NewProp_Response;
	static void NewProp_bGranted_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bGranted;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationPermissionResolvedEvent constinit property declarations ****
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationPermissionResolvedEvent Property Definitions *************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_Response = { "Response", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms, Response), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Response_MetaData), NewProp_Response_MetaData) }; // 3013045577
void Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_bGranted_SetBit(void* Obj)
{
	((_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms*)Obj)->bGranted = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_bGranted = { "bGranted", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms), &Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_bGranted_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_Response,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::NewProp_bGranted,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationPermissionResolvedEvent Property Definitions ***************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationPermissionResolvedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00530000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationPermissionResolvedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationPermissionResolvedEvent, FCandidateNPCResponse const& Response, bool bGranted)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms
	{
		FCandidateNPCResponse Response;
		bool bGranted;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationPermissionResolvedEvent_Parms Parms;
	Parms.Response=Response;
	Parms.bGranted=bGranted ? true : false;
	ConversationPermissionResolvedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationPermissionResolvedEvent ************************************

// ********** Begin Delegate FConversationNonVerbalReactionEvent ***********************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms
	{
		AActor* NPCActor;
		ENonVerbalReactionType ReactionType;
		float Intensity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationNonVerbalReactionEvent constinit property declarations ***
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReactionType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReactionType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Intensity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationNonVerbalReactionEvent constinit property declarations *****
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationNonVerbalReactionEvent Property Definitions **************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_ReactionType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_ReactionType = { "ReactionType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms, ReactionType), Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType, METADATA_PARAMS(0, nullptr) }; // 3545513746
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_Intensity = { "Intensity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms, Intensity), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_ReactionType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_ReactionType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::NewProp_Intensity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationNonVerbalReactionEvent Property Definitions ****************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationNonVerbalReactionEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationNonVerbalReactionEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationNonVerbalReactionEvent, AActor* NPCActor, ENonVerbalReactionType ReactionType, float Intensity)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms
	{
		AActor* NPCActor;
		ENonVerbalReactionType ReactionType;
		float Intensity;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationNonVerbalReactionEvent_Parms Parms;
	Parms.NPCActor=NPCActor;
	Parms.ReactionType=ReactionType;
	Parms.Intensity=Intensity;
	ConversationNonVerbalReactionEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationNonVerbalReactionEvent *************************************

// ********** Begin Delegate FConversationResponseDiscardedEvent ***********************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms
	{
		FCandidateNPCResponse Response;
		FString Reason;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Response_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Reason_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationResponseDiscardedEvent constinit property declarations ***
	static const UECodeGen_Private::FStructPropertyParams NewProp_Response;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Reason;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationResponseDiscardedEvent constinit property declarations *****
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationResponseDiscardedEvent Property Definitions **************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::NewProp_Response = { "Response", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms, Response), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Response_MetaData), NewProp_Response_MetaData) }; // 3013045577
const UECodeGen_Private::FStrPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::NewProp_Reason = { "Reason", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms, Reason), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Reason_MetaData), NewProp_Reason_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::NewProp_Response,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::NewProp_Reason,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationResponseDiscardedEvent Property Definitions ****************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationResponseDiscardedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00530000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationResponseDiscardedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationResponseDiscardedEvent, FCandidateNPCResponse const& Response, const FString& Reason)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms
	{
		FCandidateNPCResponse Response;
		FString Reason;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationResponseDiscardedEvent_Parms Parms;
	Parms.Response=Response;
	Parms.Reason=Reason;
	ConversationResponseDiscardedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationResponseDiscardedEvent *************************************

// ********** Begin Delegate FConversationTurnOwnerChangedEvent ************************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms
	{
		AActor* PreviousOwner;
		AActor* NewOwner;
		EConversationTurnState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationTurnOwnerChangedEvent constinit property declarations ****
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PreviousOwner;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewOwner;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationTurnOwnerChangedEvent constinit property declarations ******
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationTurnOwnerChangedEvent Property Definitions ***************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_PreviousOwner = { "PreviousOwner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms, PreviousOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewOwner = { "NewOwner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms, NewOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms, NewState), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, METADATA_PARAMS(0, nullptr) }; // 1236676757
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_PreviousOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationTurnOwnerChangedEvent Property Definitions *****************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationTurnOwnerChangedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationTurnOwnerChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationTurnOwnerChangedEvent, AActor* PreviousOwner, AActor* NewOwner, EConversationTurnState NewState)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms
	{
		AActor* PreviousOwner;
		AActor* NewOwner;
		EConversationTurnState NewState;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationTurnOwnerChangedEvent_Parms Parms;
	Parms.PreviousOwner=PreviousOwner;
	Parms.NewOwner=NewOwner;
	Parms.NewState=NewState;
	ConversationTurnOwnerChangedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationTurnOwnerChangedEvent **************************************

// ********** Begin Delegate FConversationGovernanceDebugEvent *************************************
struct Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms
	{
		FString Message;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Message_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FConversationGovernanceDebugEvent constinit property declarations *****
	static const UECodeGen_Private::FStrPropertyParams NewProp_Message;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FConversationGovernanceDebugEvent constinit property declarations *******
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FConversationGovernanceDebugEvent Property Definitions ****************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms, Message), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Message_MetaData), NewProp_Message_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::NewProp_Message,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FConversationGovernanceDebugEvent Property Definitions ******************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance, nullptr, "ConversationGovernanceDebugEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::_Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FConversationGovernanceDebugEvent_DelegateWrapper(const FMulticastScriptDelegate& ConversationGovernanceDebugEvent, const FString& Message)
{
	struct _Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms
	{
		FString Message;
	};
	_Script_NaturalNPCConversationGovernance_eventConversationGovernanceDebugEvent_Parms Parms;
	Parms.Message=Message;
	ConversationGovernanceDebugEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FConversationGovernanceDebugEvent ***************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EConversationResponseType_StaticEnum, TEXT("EConversationResponseType"), &Z_Registration_Info_UEnum_EConversationResponseType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3150995788U) },
		{ EConversationTurnState_StaticEnum, TEXT("EConversationTurnState"), &Z_Registration_Info_UEnum_EConversationTurnState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1236676757U) },
		{ ENonVerbalReactionType_StaticEnum, TEXT("ENonVerbalReactionType"), &Z_Registration_Info_UEnum_ENonVerbalReactionType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3545513746U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FCandidateNPCResponse::StaticStruct, Z_Construct_UScriptStruct_FCandidateNPCResponse_Statics::NewStructOps, TEXT("CandidateNPCResponse"),&Z_Registration_Info_UScriptStruct_FCandidateNPCResponse, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FCandidateNPCResponse), 3013045577U) },
		{ FConversationRuntimeState::StaticStruct, Z_Construct_UScriptStruct_FConversationRuntimeState_Statics::NewStructOps, TEXT("ConversationRuntimeState"),&Z_Registration_Info_UScriptStruct_FConversationRuntimeState, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FConversationRuntimeState), 2795981430U) },
		{ FConversationGovernanceDebugSnapshot::StaticStruct, Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot_Statics::NewStructOps, TEXT("ConversationGovernanceDebugSnapshot"),&Z_Registration_Info_UScriptStruct_FConversationGovernanceDebugSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FConversationGovernanceDebugSnapshot), 2100528209U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_462857104{
	TEXT("/Script/NaturalNPCConversationGovernance"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceTypes_h__Script_NaturalNPCConversationGovernance_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
