// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ConversationGovernanceComponent.h"
#include "ConversationGovernanceTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeConversationGovernanceComponent() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationGovernanceComponent();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationGovernanceComponent_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationListenerComponent_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationResponseQueue_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationTurnManager_NoRegister();
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

// ********** Begin Class UConversationGovernanceComponent Function BeginEmergencyInterrupt ********
struct Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics
{
	struct ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function BeginEmergencyInterrupt constinit property declarations ***************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function BeginEmergencyInterrupt constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function BeginEmergencyInterrupt Property Definitions **************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::PropPointers) < 2048);
// ********** End Function BeginEmergencyInterrupt Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "BeginEmergencyInterrupt", 	Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::ConversationGovernanceComponent_eventBeginEmergencyInterrupt_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execBeginEmergencyInterrupt)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->BeginEmergencyInterrupt(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function BeginEmergencyInterrupt **********

// ********** Begin Class UConversationGovernanceComponent Function BeginSideComment ***************
struct Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics
{
	struct ConversationGovernanceComponent_eventBeginSideComment_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function BeginSideComment constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function BeginSideComment constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function BeginSideComment Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventBeginSideComment_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventBeginSideComment_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventBeginSideComment_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::PropPointers) < 2048);
// ********** End Function BeginSideComment Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "BeginSideComment", 	Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::ConversationGovernanceComponent_eventBeginSideComment_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::ConversationGovernanceComponent_eventBeginSideComment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execBeginSideComment)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->BeginSideComment(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function BeginSideComment *****************

// ********** Begin Class UConversationGovernanceComponent Function CancelResponse *****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics
{
	struct ConversationGovernanceComponent_eventCancelResponse_Parms
	{
		FGuid ResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CancelResponse constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CancelResponse constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CancelResponse Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventCancelResponse_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventCancelResponse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventCancelResponse_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::PropPointers) < 2048);
// ********** End Function CancelResponse Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "CancelResponse", 	Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::ConversationGovernanceComponent_eventCancelResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::ConversationGovernanceComponent_eventCancelResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execCancelResponse)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CancelResponse(Z_Param_ResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function CancelResponse *******************

// ********** Begin Class UConversationGovernanceComponent Function DenyPermission *****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics
{
	struct ConversationGovernanceComponent_eventDenyPermission_Parms
	{
		FGuid ResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Permission" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function DenyPermission constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function DenyPermission constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function DenyPermission Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventDenyPermission_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventDenyPermission_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventDenyPermission_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::PropPointers) < 2048);
// ********** End Function DenyPermission Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "DenyPermission", 	Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::ConversationGovernanceComponent_eventDenyPermission_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::ConversationGovernanceComponent_eventDenyPermission_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execDenyPermission)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->DenyPermission(Z_Param_ResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function DenyPermission *******************

// ********** Begin Class UConversationGovernanceComponent Function EndConversation ****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function EndConversation constinit property declarations ***********************
// ********** End Function EndConversation constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "EndConversation", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execEndConversation)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->EndConversation();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function EndConversation ******************

// ********** Begin Class UConversationGovernanceComponent Function EndSideComment *****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics
{
	struct ConversationGovernanceComponent_eventEndSideComment_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function EndSideComment constinit property declarations ************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function EndSideComment constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function EndSideComment Property Definitions ***********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventEndSideComment_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventEndSideComment_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventEndSideComment_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::PropPointers) < 2048);
// ********** End Function EndSideComment Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "EndSideComment", 	Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::ConversationGovernanceComponent_eventEndSideComment_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::ConversationGovernanceComponent_eventEndSideComment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execEndSideComment)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->EndSideComment(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function EndSideComment *******************

// ********** Begin Class UConversationGovernanceComponent Function GetCurrentTurnOwner ************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics
{
	struct ConversationGovernanceComponent_eventGetCurrentTurnOwner_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetCurrentTurnOwner constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetCurrentTurnOwner constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetCurrentTurnOwner Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetCurrentTurnOwner_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::PropPointers) < 2048);
// ********** End Function GetCurrentTurnOwner Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetCurrentTurnOwner", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::ConversationGovernanceComponent_eventGetCurrentTurnOwner_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::ConversationGovernanceComponent_eventGetCurrentTurnOwner_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetCurrentTurnOwner)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->GetCurrentTurnOwner();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetCurrentTurnOwner **************

// ********** Begin Class UConversationGovernanceComponent Function GetDebugSnapshot ***************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics
{
	struct ConversationGovernanceComponent_eventGetDebugSnapshot_Parms
	{
		FConversationGovernanceDebugSnapshot ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Debug" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetDebugSnapshot constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetDebugSnapshot constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetDebugSnapshot Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetDebugSnapshot_Parms, ReturnValue), Z_Construct_UScriptStruct_FConversationGovernanceDebugSnapshot, METADATA_PARAMS(0, nullptr) }; // 2100528209
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::PropPointers) < 2048);
// ********** End Function GetDebugSnapshot Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetDebugSnapshot", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::ConversationGovernanceComponent_eventGetDebugSnapshot_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::ConversationGovernanceComponent_eventGetDebugSnapshot_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetDebugSnapshot)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FConversationGovernanceDebugSnapshot*)Z_Param__Result=P_THIS->GetDebugSnapshot();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetDebugSnapshot *****************

// ********** Begin Class UConversationGovernanceComponent Function GetPendingPermissionResponse ***
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics
{
	struct ConversationGovernanceComponent_eventGetPendingPermissionResponse_Parms
	{
		FCandidateNPCResponse ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Permission" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetPendingPermissionResponse constinit property declarations **********
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetPendingPermissionResponse constinit property declarations ************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetPendingPermissionResponse Property Definitions *********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetPendingPermissionResponse_Parms, ReturnValue), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::PropPointers) < 2048);
// ********** End Function GetPendingPermissionResponse Property Definitions ***********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetPendingPermissionResponse", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::ConversationGovernanceComponent_eventGetPendingPermissionResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::ConversationGovernanceComponent_eventGetPendingPermissionResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetPendingPermissionResponse)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FCandidateNPCResponse*)Z_Param__Result=P_THIS->GetPendingPermissionResponse();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetPendingPermissionResponse *****

// ********** Begin Class UConversationGovernanceComponent Function GetPrimaryNPC ******************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics
{
	struct ConversationGovernanceComponent_eventGetPrimaryNPC_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetPrimaryNPC constinit property declarations *************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetPrimaryNPC constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetPrimaryNPC Property Definitions ************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetPrimaryNPC_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::PropPointers) < 2048);
// ********** End Function GetPrimaryNPC Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetPrimaryNPC", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::ConversationGovernanceComponent_eventGetPrimaryNPC_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::ConversationGovernanceComponent_eventGetPrimaryNPC_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetPrimaryNPC)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->GetPrimaryNPC();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetPrimaryNPC ********************

// ********** Begin Class UConversationGovernanceComponent Function GetQueuedResponses *************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics
{
	struct ConversationGovernanceComponent_eventGetQueuedResponses_Parms
	{
		TArray<FCandidateNPCResponse> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetQueuedResponses constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetQueuedResponses constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetQueuedResponses Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetQueuedResponses_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::PropPointers) < 2048);
// ********** End Function GetQueuedResponses Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetQueuedResponses", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::ConversationGovernanceComponent_eventGetQueuedResponses_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::ConversationGovernanceComponent_eventGetQueuedResponses_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetQueuedResponses)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FCandidateNPCResponse>*)Z_Param__Result=P_THIS->GetQueuedResponses();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetQueuedResponses ***************

// ********** Begin Class UConversationGovernanceComponent Function GetResponseQueue ***************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics
{
	struct ConversationGovernanceComponent_eventGetResponseQueue_Parms
	{
		UConversationResponseQueue* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResponseQueue constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResponseQueue constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResponseQueue Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetResponseQueue_Parms, ReturnValue), Z_Construct_UClass_UConversationResponseQueue_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::PropPointers) < 2048);
// ********** End Function GetResponseQueue Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetResponseQueue", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::ConversationGovernanceComponent_eventGetResponseQueue_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::ConversationGovernanceComponent_eventGetResponseQueue_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetResponseQueue)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UConversationResponseQueue**)Z_Param__Result=P_THIS->GetResponseQueue();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetResponseQueue *****************

// ********** Begin Class UConversationGovernanceComponent Function GetRuntimeState ****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics
{
	struct ConversationGovernanceComponent_eventGetRuntimeState_Parms
	{
		FConversationRuntimeState ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetRuntimeState constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetRuntimeState constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetRuntimeState Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetRuntimeState_Parms, ReturnValue), Z_Construct_UScriptStruct_FConversationRuntimeState, METADATA_PARAMS(0, nullptr) }; // 2795981430
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::PropPointers) < 2048);
// ********** End Function GetRuntimeState Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetRuntimeState", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::ConversationGovernanceComponent_eventGetRuntimeState_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::ConversationGovernanceComponent_eventGetRuntimeState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetRuntimeState)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FConversationRuntimeState*)Z_Param__Result=P_THIS->GetRuntimeState();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetRuntimeState ******************

// ********** Begin Class UConversationGovernanceComponent Function GetTurnManager *****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics
{
	struct ConversationGovernanceComponent_eventGetTurnManager_Parms
	{
		UConversationTurnManager* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetTurnManager constinit property declarations ************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetTurnManager constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetTurnManager Property Definitions ***********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetTurnManager_Parms, ReturnValue), Z_Construct_UClass_UConversationTurnManager_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::PropPointers) < 2048);
// ********** End Function GetTurnManager Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetTurnManager", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::ConversationGovernanceComponent_eventGetTurnManager_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::ConversationGovernanceComponent_eventGetTurnManager_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetTurnManager)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UConversationTurnManager**)Z_Param__Result=P_THIS->GetTurnManager();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetTurnManager *******************

// ********** Begin Class UConversationGovernanceComponent Function GetTurnState *******************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics
{
	struct ConversationGovernanceComponent_eventGetTurnState_Parms
	{
		EConversationTurnState ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetTurnState constinit property declarations **************************
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetTurnState constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetTurnState Property Definitions *************************************
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGetTurnState_Parms, ReturnValue), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, METADATA_PARAMS(0, nullptr) }; // 1236676757
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::PropPointers) < 2048);
// ********** End Function GetTurnState Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GetTurnState", 	Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::ConversationGovernanceComponent_eventGetTurnState_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::ConversationGovernanceComponent_eventGetTurnState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGetTurnState)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EConversationTurnState*)Z_Param__Result=P_THIS->GetTurnState();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GetTurnState *********************

// ********** Begin Class UConversationGovernanceComponent Function GrantPermission ****************
struct Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics
{
	struct ConversationGovernanceComponent_eventGrantPermission_Parms
	{
		FGuid ResponseID;
		FCandidateNPCResponse OutApprovedResponse;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Permission" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GrantPermission constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutApprovedResponse;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GrantPermission constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GrantPermission Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGrantPermission_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_OutApprovedResponse = { "OutApprovedResponse", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventGrantPermission_Parms, OutApprovedResponse), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
void Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventGrantPermission_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventGrantPermission_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_OutApprovedResponse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::PropPointers) < 2048);
// ********** End Function GrantPermission Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "GrantPermission", 	Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::ConversationGovernanceComponent_eventGrantPermission_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::ConversationGovernanceComponent_eventGrantPermission_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execGrantPermission)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_OutApprovedResponse);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GrantPermission(Z_Param_ResponseID,Z_Param_Out_OutApprovedResponse);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function GrantPermission ******************

// ********** Begin Class UConversationGovernanceComponent Function HandleTurnOwnerChanged *********
struct Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics
{
	struct ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms
	{
		AActor* PreviousOwner;
		AActor* NewOwner;
		EConversationTurnState NewState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function HandleTurnOwnerChanged constinit property declarations ****************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PreviousOwner;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewOwner;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewState_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function HandleTurnOwnerChanged constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function HandleTurnOwnerChanged Property Definitions ***************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_PreviousOwner = { "PreviousOwner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms, PreviousOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewOwner = { "NewOwner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms, NewOwner), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewState = { "NewState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms, NewState), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, METADATA_PARAMS(0, nullptr) }; // 1236676757
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_PreviousOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewOwner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewState_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::NewProp_NewState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::PropPointers) < 2048);
// ********** End Function HandleTurnOwnerChanged Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "HandleTurnOwnerChanged", 	Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::ConversationGovernanceComponent_eventHandleTurnOwnerChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execHandleTurnOwnerChanged)
{
	P_GET_OBJECT(AActor,Z_Param_PreviousOwner);
	P_GET_OBJECT(AActor,Z_Param_NewOwner);
	P_GET_ENUM(EConversationTurnState,Z_Param_NewState);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleTurnOwnerChanged(Z_Param_PreviousOwner,Z_Param_NewOwner,EConversationTurnState(Z_Param_NewState));
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function HandleTurnOwnerChanged ***********

// ********** Begin Class UConversationGovernanceComponent Function HasPendingPermissionRequest ****
struct Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics
{
	struct ConversationGovernanceComponent_eventHasPendingPermissionRequest_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Permission" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function HasPendingPermissionRequest constinit property declarations ***********
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function HasPendingPermissionRequest constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function HasPendingPermissionRequest Property Definitions **********************
void Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventHasPendingPermissionRequest_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventHasPendingPermissionRequest_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::PropPointers) < 2048);
// ********** End Function HasPendingPermissionRequest Property Definitions ************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "HasPendingPermissionRequest", 	Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::ConversationGovernanceComponent_eventHasPendingPermissionRequest_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::ConversationGovernanceComponent_eventHasPendingPermissionRequest_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execHasPendingPermissionRequest)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->HasPendingPermissionRequest();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function HasPendingPermissionRequest ******

// ********** Begin Class UConversationGovernanceComponent Function IsConversationActive ***********
struct Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics
{
	struct ConversationGovernanceComponent_eventIsConversationActive_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsConversationActive constinit property declarations ******************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsConversationActive constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsConversationActive Property Definitions *****************************
void Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventIsConversationActive_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventIsConversationActive_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::PropPointers) < 2048);
// ********** End Function IsConversationActive Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "IsConversationActive", 	Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::ConversationGovernanceComponent_eventIsConversationActive_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::ConversationGovernanceComponent_eventIsConversationActive_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execIsConversationActive)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsConversationActive();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function IsConversationActive *************

// ********** Begin Class UConversationGovernanceComponent Function IsListenerEligible *************
struct Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics
{
	struct ConversationGovernanceComponent_eventIsListenerEligible_Parms
	{
		const UConversationListenerComponent* Listener;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Listener_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsListenerEligible constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Listener;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsListenerEligible constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsListenerEligible Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_Listener = { "Listener", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventIsListenerEligible_Parms, Listener), Z_Construct_UClass_UConversationListenerComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Listener_MetaData), NewProp_Listener_MetaData) };
void Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventIsListenerEligible_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventIsListenerEligible_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_Listener,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::PropPointers) < 2048);
// ********** End Function IsListenerEligible Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "IsListenerEligible", 	Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::ConversationGovernanceComponent_eventIsListenerEligible_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::ConversationGovernanceComponent_eventIsListenerEligible_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execIsListenerEligible)
{
	P_GET_OBJECT(UConversationListenerComponent,Z_Param_Listener);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsListenerEligible(Z_Param_Listener);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function IsListenerEligible ***************

// ********** Begin Class UConversationGovernanceComponent Function NotifyNPCSpeechEnded ***********
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics
{
	struct ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyNPCSpeechEnded constinit property declarations ******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyNPCSpeechEnded constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyNPCSpeechEnded Property Definitions *****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::PropPointers) < 2048);
// ********** End Function NotifyNPCSpeechEnded Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyNPCSpeechEnded", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::ConversationGovernanceComponent_eventNotifyNPCSpeechEnded_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyNPCSpeechEnded)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyNPCSpeechEnded(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyNPCSpeechEnded *************

// ********** Begin Class UConversationGovernanceComponent Function NotifyNPCSpeechStarted *********
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics
{
	struct ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyNPCSpeechStarted constinit property declarations ****************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyNPCSpeechStarted constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyNPCSpeechStarted Property Definitions ***************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::PropPointers) < 2048);
// ********** End Function NotifyNPCSpeechStarted Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyNPCSpeechStarted", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::ConversationGovernanceComponent_eventNotifyNPCSpeechStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyNPCSpeechStarted)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyNPCSpeechStarted(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyNPCSpeechStarted ***********

// ********** Begin Class UConversationGovernanceComponent Function NotifyPlayerSpeechEnded ********
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics
{
	struct ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms
	{
		AActor* PlayerActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyPlayerSpeechEnded constinit property declarations ***************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyPlayerSpeechEnded constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyPlayerSpeechEnded Property Definitions **************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_PlayerActor = { "PlayerActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms, PlayerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_PlayerActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::PropPointers) < 2048);
// ********** End Function NotifyPlayerSpeechEnded Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyPlayerSpeechEnded", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::ConversationGovernanceComponent_eventNotifyPlayerSpeechEnded_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyPlayerSpeechEnded)
{
	P_GET_OBJECT(AActor,Z_Param_PlayerActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyPlayerSpeechEnded(Z_Param_PlayerActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyPlayerSpeechEnded **********

// ********** Begin Class UConversationGovernanceComponent Function NotifyPlayerSpeechStarted ******
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics
{
	struct ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms
	{
		AActor* PlayerActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyPlayerSpeechStarted constinit property declarations *************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerActor;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyPlayerSpeechStarted constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyPlayerSpeechStarted Property Definitions ************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_PlayerActor = { "PlayerActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms, PlayerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_PlayerActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::PropPointers) < 2048);
// ********** End Function NotifyPlayerSpeechStarted Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyPlayerSpeechStarted", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::ConversationGovernanceComponent_eventNotifyPlayerSpeechStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyPlayerSpeechStarted)
{
	P_GET_OBJECT(AActor,Z_Param_PlayerActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyPlayerSpeechStarted(Z_Param_PlayerActor);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyPlayerSpeechStarted ********

// ********** Begin Class UConversationGovernanceComponent Function NotifyResponseExecutionFinished 
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics
{
	struct ConversationGovernanceComponent_eventNotifyResponseExecutionFinished_Parms
	{
		FCandidateNPCResponse Response;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Execution" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Response_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyResponseExecutionFinished constinit property declarations *******
	static const UECodeGen_Private::FStructPropertyParams NewProp_Response;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyResponseExecutionFinished constinit property declarations *********
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyResponseExecutionFinished Property Definitions ******************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::NewProp_Response = { "Response", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyResponseExecutionFinished_Parms, Response), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Response_MetaData), NewProp_Response_MetaData) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::NewProp_Response,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::PropPointers) < 2048);
// ********** End Function NotifyResponseExecutionFinished Property Definitions ********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyResponseExecutionFinished", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::ConversationGovernanceComponent_eventNotifyResponseExecutionFinished_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::ConversationGovernanceComponent_eventNotifyResponseExecutionFinished_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyResponseExecutionFinished)
{
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_Response);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NotifyResponseExecutionFinished(Z_Param_Out_Response);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyResponseExecutionFinished **

// ********** Begin Class UConversationGovernanceComponent Function NotifyResponseExecutionStarted *
struct Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics
{
	struct ConversationGovernanceComponent_eventNotifyResponseExecutionStarted_Parms
	{
		FCandidateNPCResponse Response;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Execution" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Response_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function NotifyResponseExecutionStarted constinit property declarations ********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Response;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function NotifyResponseExecutionStarted constinit property declarations **********
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function NotifyResponseExecutionStarted Property Definitions *******************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::NewProp_Response = { "Response", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventNotifyResponseExecutionStarted_Parms, Response), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Response_MetaData), NewProp_Response_MetaData) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::NewProp_Response,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::PropPointers) < 2048);
// ********** End Function NotifyResponseExecutionStarted Property Definitions *********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "NotifyResponseExecutionStarted", 	Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::ConversationGovernanceComponent_eventNotifyResponseExecutionStarted_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::ConversationGovernanceComponent_eventNotifyResponseExecutionStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execNotifyResponseExecutionStarted)
{
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_Response);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NotifyResponseExecutionStarted(Z_Param_Out_Response);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function NotifyResponseExecutionStarted ***

// ********** Begin Class UConversationGovernanceComponent Function RefreshListenerEligibility *****
struct Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RefreshListenerEligibility constinit property declarations ************
// ********** End Function RefreshListenerEligibility constinit property declarations **************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "RefreshListenerEligibility", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execRefreshListenerEligibility)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RefreshListenerEligibility();
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function RefreshListenerEligibility *******

// ********** Begin Class UConversationGovernanceComponent Function RegisterListener ***************
struct Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics
{
	struct ConversationGovernanceComponent_eventRegisterListener_Parms
	{
		UConversationListenerComponent* Listener;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Listener_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function RegisterListener constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Listener;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RegisterListener constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RegisterListener Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_Listener = { "Listener", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventRegisterListener_Parms, Listener), Z_Construct_UClass_UConversationListenerComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Listener_MetaData), NewProp_Listener_MetaData) };
void Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventRegisterListener_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventRegisterListener_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_Listener,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::PropPointers) < 2048);
// ********** End Function RegisterListener Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "RegisterListener", 	Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::ConversationGovernanceComponent_eventRegisterListener_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::ConversationGovernanceComponent_eventRegisterListener_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execRegisterListener)
{
	P_GET_OBJECT(UConversationListenerComponent,Z_Param_Listener);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RegisterListener(Z_Param_Listener);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function RegisterListener *****************

// ********** Begin Class UConversationGovernanceComponent Function RequestNonVerbalReaction *******
struct Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics
{
	struct ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms
	{
		AActor* NPCActor;
		ENonVerbalReactionType ReactionType;
		float Intensity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Reaction" },
		{ "CPP_Default_Intensity", "1.000000" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RequestNonVerbalReaction constinit property declarations **************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReactionType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReactionType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Intensity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RequestNonVerbalReaction constinit property declarations ****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RequestNonVerbalReaction Property Definitions *************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_ReactionType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_ReactionType = { "ReactionType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms, ReactionType), Z_Construct_UEnum_NaturalNPCConversationGovernance_ENonVerbalReactionType, METADATA_PARAMS(0, nullptr) }; // 3545513746
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_Intensity = { "Intensity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms, Intensity), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_ReactionType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_ReactionType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::NewProp_Intensity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::PropPointers) < 2048);
// ********** End Function RequestNonVerbalReaction Property Definitions ***************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "RequestNonVerbalReaction", 	Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::ConversationGovernanceComponent_eventRequestNonVerbalReaction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execRequestNonVerbalReaction)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_GET_ENUM(ENonVerbalReactionType,Z_Param_ReactionType);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Intensity);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RequestNonVerbalReaction(Z_Param_NPCActor,ENonVerbalReactionType(Z_Param_ReactionType),Z_Param_Intensity);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function RequestNonVerbalReaction *********

// ********** Begin Class UConversationGovernanceComponent Function RequestSpeakingPermission ******
struct Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics
{
	struct ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms
	{
		FGuid ResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Permission" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RequestSpeakingPermission constinit property declarations *************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RequestSpeakingPermission constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RequestSpeakingPermission Property Definitions ************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::PropPointers) < 2048);
// ********** End Function RequestSpeakingPermission Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "RequestSpeakingPermission", 	Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::ConversationGovernanceComponent_eventRequestSpeakingPermission_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execRequestSpeakingPermission)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RequestSpeakingPermission(Z_Param_ResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function RequestSpeakingPermission ********

// ********** Begin Class UConversationGovernanceComponent Function StartConversation **************
struct Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics
{
	struct ConversationGovernanceComponent_eventStartConversation_Parms
	{
		FName ConversationID;
		AActor* PrimaryNPC;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function StartConversation constinit property declarations *********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ConversationID;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PrimaryNPC;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function StartConversation constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function StartConversation Property Definitions ********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ConversationID = { "ConversationID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventStartConversation_Parms, ConversationID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_PrimaryNPC = { "PrimaryNPC", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventStartConversation_Parms, PrimaryNPC), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventStartConversation_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventStartConversation_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ConversationID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_PrimaryNPC,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::PropPointers) < 2048);
// ********** End Function StartConversation Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "StartConversation", 	Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::ConversationGovernanceComponent_eventStartConversation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::ConversationGovernanceComponent_eventStartConversation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execStartConversation)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_ConversationID);
	P_GET_OBJECT(AActor,Z_Param_PrimaryNPC);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->StartConversation(Z_Param_ConversationID,Z_Param_PrimaryNPC);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function StartConversation ****************

// ********** Begin Class UConversationGovernanceComponent Function SubmitCandidateResponse ********
struct Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics
{
	struct ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms
	{
		AActor* NPCActor;
		FString ResponseText;
		EConversationResponseType ResponseType;
		float PriorityScore;
		FGuid OutResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Candidate" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResponseText_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SubmitCandidateResponse constinit property declarations ***************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPCActor;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ResponseText;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ResponseType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ResponseType;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PriorityScore;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SubmitCandidateResponse constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SubmitCandidateResponse Property Definitions **************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseText = { "ResponseText", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms, ResponseText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResponseText_MetaData), NewProp_ResponseText_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseType = { "ResponseType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms, ResponseType), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationResponseType, METADATA_PARAMS(0, nullptr) }; // 3150995788
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_PriorityScore = { "PriorityScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms, PriorityScore), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_OutResponseID = { "OutResponseID", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms, OutResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ResponseType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_PriorityScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_OutResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::PropPointers) < 2048);
// ********** End Function SubmitCandidateResponse Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "SubmitCandidateResponse", 	Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::ConversationGovernanceComponent_eventSubmitCandidateResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execSubmitCandidateResponse)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_GET_PROPERTY(FStrProperty,Z_Param_ResponseText);
	P_GET_ENUM(EConversationResponseType,Z_Param_ResponseType);
	P_GET_PROPERTY(FFloatProperty,Z_Param_PriorityScore);
	P_GET_STRUCT_REF(FGuid,Z_Param_Out_OutResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SubmitCandidateResponse(Z_Param_NPCActor,Z_Param_ResponseText,EConversationResponseType(Z_Param_ResponseType),Z_Param_PriorityScore,Z_Param_Out_OutResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function SubmitCandidateResponse **********

// ********** Begin Class UConversationGovernanceComponent Function SubmitCandidateResponseData ****
struct Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics
{
	struct ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms
	{
		FCandidateNPCResponse Candidate;
		FGuid OutResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Candidate" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Candidate_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SubmitCandidateResponseData constinit property declarations ***********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Candidate;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SubmitCandidateResponseData constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SubmitCandidateResponseData Property Definitions **********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_Candidate = { "Candidate", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms, Candidate), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Candidate_MetaData), NewProp_Candidate_MetaData) }; // 3013045577
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_OutResponseID = { "OutResponseID", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms, OutResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_Candidate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_OutResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::PropPointers) < 2048);
// ********** End Function SubmitCandidateResponseData Property Definitions ************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "SubmitCandidateResponseData", 	Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::ConversationGovernanceComponent_eventSubmitCandidateResponseData_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execSubmitCandidateResponseData)
{
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_Candidate);
	P_GET_STRUCT_REF(FGuid,Z_Param_Out_OutResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SubmitCandidateResponseData(Z_Param_Out_Candidate,Z_Param_Out_OutResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function SubmitCandidateResponseData ******

// ********** Begin Class UConversationGovernanceComponent Function UnregisterListener *************
struct Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics
{
	struct ConversationGovernanceComponent_eventUnregisterListener_Parms
	{
		UConversationListenerComponent* Listener;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Listener_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function UnregisterListener constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Listener;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function UnregisterListener constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function UnregisterListener Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_Listener = { "Listener", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationGovernanceComponent_eventUnregisterListener_Parms, Listener), Z_Construct_UClass_UConversationListenerComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Listener_MetaData), NewProp_Listener_MetaData) };
void Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationGovernanceComponent_eventUnregisterListener_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationGovernanceComponent_eventUnregisterListener_Parms), &Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_Listener,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::PropPointers) < 2048);
// ********** End Function UnregisterListener Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationGovernanceComponent, nullptr, "UnregisterListener", 	Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::ConversationGovernanceComponent_eventUnregisterListener_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::ConversationGovernanceComponent_eventUnregisterListener_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationGovernanceComponent::execUnregisterListener)
{
	P_GET_OBJECT(UConversationListenerComponent,Z_Param_Listener);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->UnregisterListener(Z_Param_Listener);
	P_NATIVE_END;
}
// ********** End Class UConversationGovernanceComponent Function UnregisterListener ***************

// ********** Begin Class UConversationGovernanceComponent *****************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UConversationGovernanceComponent;
UClass* UConversationGovernanceComponent::GetPrivateStaticClass()
{
	using TClass = UConversationGovernanceComponent;
	if (!Z_Registration_Info_UClass_UConversationGovernanceComponent.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("ConversationGovernanceComponent"),
			Z_Registration_Info_UClass_UConversationGovernanceComponent.InnerSingleton,
			StaticRegisterNativesUConversationGovernanceComponent,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UConversationGovernanceComponent.InnerSingleton;
}
UClass* Z_Construct_UClass_UConversationGovernanceComponent_NoRegister()
{
	return UConversationGovernanceComponent::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UConversationGovernanceComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "ConversationGovernance" },
		{ "IncludePath", "ConversationGovernanceComponent.h" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaximumTotalQueueLength_MetaData[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ClampMax", "256" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDebugLogging_MetaData[] = {
		{ "Category", "Conversation Governance|Debug" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnListenerEnteredConversation_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnListenerLeftConversation_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnCandidateResponseGenerated_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnResponseQueued_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnSpeakingPermissionRequested_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnSpeakingPermissionResolved_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnResponseExecutionStarted_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnResponseExecutionFinished_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnNonVerbalReactionRequested_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnResponseDiscarded_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnTurnOwnerChanged_MetaData[] = {
		{ "Category", "Conversation Governance|Events" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnGovernanceDebugMessage_MetaData[] = {
		{ "Category", "Conversation Governance|Debug" },
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TurnManager_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ResponseQueue_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuntimeState_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredListeners_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EligibleListeners_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PendingPermissionResponse_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationGovernanceComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UConversationGovernanceComponent constinit property declarations *********
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaximumTotalQueueLength;
	static void NewProp_bEnableDebugLogging_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDebugLogging;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnListenerEnteredConversation;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnListenerLeftConversation;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnCandidateResponseGenerated;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnResponseQueued;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSpeakingPermissionRequested;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSpeakingPermissionResolved;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnResponseExecutionStarted;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnResponseExecutionFinished;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnNonVerbalReactionRequested;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnResponseDiscarded;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnTurnOwnerChanged;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnGovernanceDebugMessage;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TurnManager;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ResponseQueue;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RuntimeState;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_RegisteredListeners_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredListeners;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_EligibleListeners_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_EligibleListeners;
	static const UECodeGen_Private::FStructPropertyParams NewProp_PendingPermissionResponse;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UConversationGovernanceComponent constinit property declarations ***********
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("BeginEmergencyInterrupt"), .Pointer = &UConversationGovernanceComponent::execBeginEmergencyInterrupt },
		{ .NameUTF8 = UTF8TEXT("BeginSideComment"), .Pointer = &UConversationGovernanceComponent::execBeginSideComment },
		{ .NameUTF8 = UTF8TEXT("CancelResponse"), .Pointer = &UConversationGovernanceComponent::execCancelResponse },
		{ .NameUTF8 = UTF8TEXT("DenyPermission"), .Pointer = &UConversationGovernanceComponent::execDenyPermission },
		{ .NameUTF8 = UTF8TEXT("EndConversation"), .Pointer = &UConversationGovernanceComponent::execEndConversation },
		{ .NameUTF8 = UTF8TEXT("EndSideComment"), .Pointer = &UConversationGovernanceComponent::execEndSideComment },
		{ .NameUTF8 = UTF8TEXT("GetCurrentTurnOwner"), .Pointer = &UConversationGovernanceComponent::execGetCurrentTurnOwner },
		{ .NameUTF8 = UTF8TEXT("GetDebugSnapshot"), .Pointer = &UConversationGovernanceComponent::execGetDebugSnapshot },
		{ .NameUTF8 = UTF8TEXT("GetPendingPermissionResponse"), .Pointer = &UConversationGovernanceComponent::execGetPendingPermissionResponse },
		{ .NameUTF8 = UTF8TEXT("GetPrimaryNPC"), .Pointer = &UConversationGovernanceComponent::execGetPrimaryNPC },
		{ .NameUTF8 = UTF8TEXT("GetQueuedResponses"), .Pointer = &UConversationGovernanceComponent::execGetQueuedResponses },
		{ .NameUTF8 = UTF8TEXT("GetResponseQueue"), .Pointer = &UConversationGovernanceComponent::execGetResponseQueue },
		{ .NameUTF8 = UTF8TEXT("GetRuntimeState"), .Pointer = &UConversationGovernanceComponent::execGetRuntimeState },
		{ .NameUTF8 = UTF8TEXT("GetTurnManager"), .Pointer = &UConversationGovernanceComponent::execGetTurnManager },
		{ .NameUTF8 = UTF8TEXT("GetTurnState"), .Pointer = &UConversationGovernanceComponent::execGetTurnState },
		{ .NameUTF8 = UTF8TEXT("GrantPermission"), .Pointer = &UConversationGovernanceComponent::execGrantPermission },
		{ .NameUTF8 = UTF8TEXT("HandleTurnOwnerChanged"), .Pointer = &UConversationGovernanceComponent::execHandleTurnOwnerChanged },
		{ .NameUTF8 = UTF8TEXT("HasPendingPermissionRequest"), .Pointer = &UConversationGovernanceComponent::execHasPendingPermissionRequest },
		{ .NameUTF8 = UTF8TEXT("IsConversationActive"), .Pointer = &UConversationGovernanceComponent::execIsConversationActive },
		{ .NameUTF8 = UTF8TEXT("IsListenerEligible"), .Pointer = &UConversationGovernanceComponent::execIsListenerEligible },
		{ .NameUTF8 = UTF8TEXT("NotifyNPCSpeechEnded"), .Pointer = &UConversationGovernanceComponent::execNotifyNPCSpeechEnded },
		{ .NameUTF8 = UTF8TEXT("NotifyNPCSpeechStarted"), .Pointer = &UConversationGovernanceComponent::execNotifyNPCSpeechStarted },
		{ .NameUTF8 = UTF8TEXT("NotifyPlayerSpeechEnded"), .Pointer = &UConversationGovernanceComponent::execNotifyPlayerSpeechEnded },
		{ .NameUTF8 = UTF8TEXT("NotifyPlayerSpeechStarted"), .Pointer = &UConversationGovernanceComponent::execNotifyPlayerSpeechStarted },
		{ .NameUTF8 = UTF8TEXT("NotifyResponseExecutionFinished"), .Pointer = &UConversationGovernanceComponent::execNotifyResponseExecutionFinished },
		{ .NameUTF8 = UTF8TEXT("NotifyResponseExecutionStarted"), .Pointer = &UConversationGovernanceComponent::execNotifyResponseExecutionStarted },
		{ .NameUTF8 = UTF8TEXT("RefreshListenerEligibility"), .Pointer = &UConversationGovernanceComponent::execRefreshListenerEligibility },
		{ .NameUTF8 = UTF8TEXT("RegisterListener"), .Pointer = &UConversationGovernanceComponent::execRegisterListener },
		{ .NameUTF8 = UTF8TEXT("RequestNonVerbalReaction"), .Pointer = &UConversationGovernanceComponent::execRequestNonVerbalReaction },
		{ .NameUTF8 = UTF8TEXT("RequestSpeakingPermission"), .Pointer = &UConversationGovernanceComponent::execRequestSpeakingPermission },
		{ .NameUTF8 = UTF8TEXT("StartConversation"), .Pointer = &UConversationGovernanceComponent::execStartConversation },
		{ .NameUTF8 = UTF8TEXT("SubmitCandidateResponse"), .Pointer = &UConversationGovernanceComponent::execSubmitCandidateResponse },
		{ .NameUTF8 = UTF8TEXT("SubmitCandidateResponseData"), .Pointer = &UConversationGovernanceComponent::execSubmitCandidateResponseData },
		{ .NameUTF8 = UTF8TEXT("UnregisterListener"), .Pointer = &UConversationGovernanceComponent::execUnregisterListener },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_BeginEmergencyInterrupt, "BeginEmergencyInterrupt" }, // 2383665678
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_BeginSideComment, "BeginSideComment" }, // 2978014126
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_CancelResponse, "CancelResponse" }, // 1905749395
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_DenyPermission, "DenyPermission" }, // 2383344727
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_EndConversation, "EndConversation" }, // 3658278135
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_EndSideComment, "EndSideComment" }, // 874733464
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetCurrentTurnOwner, "GetCurrentTurnOwner" }, // 1772360709
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetDebugSnapshot, "GetDebugSnapshot" }, // 2255790951
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetPendingPermissionResponse, "GetPendingPermissionResponse" }, // 3268863174
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetPrimaryNPC, "GetPrimaryNPC" }, // 3264128963
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetQueuedResponses, "GetQueuedResponses" }, // 1773362389
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetResponseQueue, "GetResponseQueue" }, // 1360542839
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetRuntimeState, "GetRuntimeState" }, // 2950534503
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnManager, "GetTurnManager" }, // 3264487218
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GetTurnState, "GetTurnState" }, // 3320250745
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_GrantPermission, "GrantPermission" }, // 133221448
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_HandleTurnOwnerChanged, "HandleTurnOwnerChanged" }, // 3151270491
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_HasPendingPermissionRequest, "HasPendingPermissionRequest" }, // 1717381202
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_IsConversationActive, "IsConversationActive" }, // 3765886783
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_IsListenerEligible, "IsListenerEligible" }, // 647725555
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechEnded, "NotifyNPCSpeechEnded" }, // 2783175037
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyNPCSpeechStarted, "NotifyNPCSpeechStarted" }, // 2970322702
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechEnded, "NotifyPlayerSpeechEnded" }, // 3720998846
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyPlayerSpeechStarted, "NotifyPlayerSpeechStarted" }, // 3991643790
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionFinished, "NotifyResponseExecutionFinished" }, // 2886156988
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_NotifyResponseExecutionStarted, "NotifyResponseExecutionStarted" }, // 458834591
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_RefreshListenerEligibility, "RefreshListenerEligibility" }, // 1705297940
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_RegisterListener, "RegisterListener" }, // 1878485931
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_RequestNonVerbalReaction, "RequestNonVerbalReaction" }, // 1772857951
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_RequestSpeakingPermission, "RequestSpeakingPermission" }, // 938330994
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_StartConversation, "StartConversation" }, // 1892220958
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponse, "SubmitCandidateResponse" }, // 2802967691
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_SubmitCandidateResponseData, "SubmitCandidateResponseData" }, // 3455390002
		{ &Z_Construct_UFunction_UConversationGovernanceComponent_UnregisterListener, "UnregisterListener" }, // 2647447508
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UConversationGovernanceComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UConversationGovernanceComponent_Statics

// ********** Begin Class UConversationGovernanceComponent Property Definitions ********************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_MaximumTotalQueueLength = { "MaximumTotalQueueLength", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, MaximumTotalQueueLength), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaximumTotalQueueLength_MetaData), NewProp_MaximumTotalQueueLength_MetaData) };
void Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_bEnableDebugLogging_SetBit(void* Obj)
{
	((UConversationGovernanceComponent*)Obj)->bEnableDebugLogging = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_bEnableDebugLogging = { "bEnableDebugLogging", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationGovernanceComponent), &Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_bEnableDebugLogging_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDebugLogging_MetaData), NewProp_bEnableDebugLogging_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnListenerEnteredConversation = { "OnListenerEnteredConversation", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnListenerEnteredConversation), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnListenerEnteredConversation_MetaData), NewProp_OnListenerEnteredConversation_MetaData) }; // 2230465113
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnListenerLeftConversation = { "OnListenerLeftConversation", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnListenerLeftConversation), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationListenerChangedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnListenerLeftConversation_MetaData), NewProp_OnListenerLeftConversation_MetaData) }; // 2230465113
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnCandidateResponseGenerated = { "OnCandidateResponseGenerated", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnCandidateResponseGenerated), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnCandidateResponseGenerated_MetaData), NewProp_OnCandidateResponseGenerated_MetaData) }; // 1945607715
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseQueued = { "OnResponseQueued", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnResponseQueued), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnResponseQueued_MetaData), NewProp_OnResponseQueued_MetaData) }; // 1945607715
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnSpeakingPermissionRequested = { "OnSpeakingPermissionRequested", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnSpeakingPermissionRequested), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnSpeakingPermissionRequested_MetaData), NewProp_OnSpeakingPermissionRequested_MetaData) }; // 1945607715
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnSpeakingPermissionResolved = { "OnSpeakingPermissionResolved", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnSpeakingPermissionResolved), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationPermissionResolvedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnSpeakingPermissionResolved_MetaData), NewProp_OnSpeakingPermissionResolved_MetaData) }; // 264237262
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseExecutionStarted = { "OnResponseExecutionStarted", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnResponseExecutionStarted), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnResponseExecutionStarted_MetaData), NewProp_OnResponseExecutionStarted_MetaData) }; // 1945607715
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseExecutionFinished = { "OnResponseExecutionFinished", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnResponseExecutionFinished), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationCandidateEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnResponseExecutionFinished_MetaData), NewProp_OnResponseExecutionFinished_MetaData) }; // 1945607715
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnNonVerbalReactionRequested = { "OnNonVerbalReactionRequested", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnNonVerbalReactionRequested), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationNonVerbalReactionEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnNonVerbalReactionRequested_MetaData), NewProp_OnNonVerbalReactionRequested_MetaData) }; // 4074306298
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseDiscarded = { "OnResponseDiscarded", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnResponseDiscarded), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationResponseDiscardedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnResponseDiscarded_MetaData), NewProp_OnResponseDiscarded_MetaData) }; // 197808150
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnTurnOwnerChanged = { "OnTurnOwnerChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnTurnOwnerChanged), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnTurnOwnerChanged_MetaData), NewProp_OnTurnOwnerChanged_MetaData) }; // 1655774746
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnGovernanceDebugMessage = { "OnGovernanceDebugMessage", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, OnGovernanceDebugMessage), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationGovernanceDebugEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnGovernanceDebugMessage_MetaData), NewProp_OnGovernanceDebugMessage_MetaData) }; // 2243786460
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_TurnManager = { "TurnManager", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, TurnManager), Z_Construct_UClass_UConversationTurnManager_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TurnManager_MetaData), NewProp_TurnManager_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_ResponseQueue = { "ResponseQueue", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, ResponseQueue), Z_Construct_UClass_UConversationResponseQueue_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ResponseQueue_MetaData), NewProp_ResponseQueue_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RuntimeState = { "RuntimeState", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, RuntimeState), Z_Construct_UScriptStruct_FConversationRuntimeState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuntimeState_MetaData), NewProp_RuntimeState_MetaData) }; // 2795981430
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RegisteredListeners_Inner = { "RegisteredListeners", nullptr, (EPropertyFlags)0x0004000000080008, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UConversationListenerComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RegisteredListeners = { "RegisteredListeners", nullptr, (EPropertyFlags)0x0044008000002008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, RegisteredListeners), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RegisteredListeners_MetaData), NewProp_RegisteredListeners_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_EligibleListeners_Inner = { "EligibleListeners", nullptr, (EPropertyFlags)0x0004000000080008, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UConversationListenerComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_EligibleListeners = { "EligibleListeners", nullptr, (EPropertyFlags)0x0044008000002008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, EligibleListeners), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EligibleListeners_MetaData), NewProp_EligibleListeners_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_PendingPermissionResponse = { "PendingPermissionResponse", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationGovernanceComponent, PendingPermissionResponse), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PendingPermissionResponse_MetaData), NewProp_PendingPermissionResponse_MetaData) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UConversationGovernanceComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_MaximumTotalQueueLength,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_bEnableDebugLogging,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnListenerEnteredConversation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnListenerLeftConversation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnCandidateResponseGenerated,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseQueued,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnSpeakingPermissionRequested,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnSpeakingPermissionResolved,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseExecutionStarted,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseExecutionFinished,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnNonVerbalReactionRequested,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnResponseDiscarded,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnTurnOwnerChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_OnGovernanceDebugMessage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_TurnManager,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_ResponseQueue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RuntimeState,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RegisteredListeners_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_RegisteredListeners,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_EligibleListeners_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_EligibleListeners,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationGovernanceComponent_Statics::NewProp_PendingPermissionResponse,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationGovernanceComponent_Statics::PropPointers) < 2048);
// ********** End Class UConversationGovernanceComponent Property Definitions **********************
UObject* (*const Z_Construct_UClass_UConversationGovernanceComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationGovernanceComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UConversationGovernanceComponent_Statics::ClassParams = {
	&UConversationGovernanceComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UConversationGovernanceComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UConversationGovernanceComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationGovernanceComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UConversationGovernanceComponent_Statics::Class_MetaDataParams)
};
void UConversationGovernanceComponent::StaticRegisterNativesUConversationGovernanceComponent()
{
	UClass* Class = UConversationGovernanceComponent::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UConversationGovernanceComponent_Statics::Funcs));
}
UClass* Z_Construct_UClass_UConversationGovernanceComponent()
{
	if (!Z_Registration_Info_UClass_UConversationGovernanceComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UConversationGovernanceComponent.OuterSingleton, Z_Construct_UClass_UConversationGovernanceComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UConversationGovernanceComponent.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UConversationGovernanceComponent);
UConversationGovernanceComponent::~UConversationGovernanceComponent() {}
// ********** End Class UConversationGovernanceComponent *******************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h__Script_NaturalNPCConversationGovernance_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UConversationGovernanceComponent, UConversationGovernanceComponent::StaticClass, TEXT("UConversationGovernanceComponent"), &Z_Registration_Info_UClass_UConversationGovernanceComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UConversationGovernanceComponent), 8270202U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h__Script_NaturalNPCConversationGovernance_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h__Script_NaturalNPCConversationGovernance_750517256{
	TEXT("/Script/NaturalNPCConversationGovernance"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationGovernanceComponent_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
