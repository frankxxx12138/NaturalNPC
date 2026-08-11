// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ConversationResponseQueue.h"
#include "ConversationGovernanceTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeConversationResponseQueue() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationResponseQueue();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationResponseQueue_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UScriptStruct* Z_Construct_UScriptStruct_FCandidateNPCResponse();
UPackage* Z_Construct_UPackage__Script_NaturalNPCConversationGovernance();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UConversationResponseQueue Function CountResponsesFromNPC ****************
struct Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics
{
	struct ConversationResponseQueue_eventCountResponsesFromNPC_Parms
	{
		FName NPCID;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CountResponsesFromNPC constinit property declarations *****************
	static const UECodeGen_Private::FNamePropertyParams NewProp_NPCID;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CountResponsesFromNPC constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CountResponsesFromNPC Property Definitions ****************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::NewProp_NPCID = { "NPCID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventCountResponsesFromNPC_Parms, NPCID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventCountResponsesFromNPC_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::NewProp_NPCID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::PropPointers) < 2048);
// ********** End Function CountResponsesFromNPC Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "CountResponsesFromNPC", 	Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::ConversationResponseQueue_eventCountResponsesFromNPC_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::ConversationResponseQueue_eventCountResponsesFromNPC_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execCountResponsesFromNPC)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_NPCID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->CountResponsesFromNPC(Z_Param_NPCID);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function CountResponsesFromNPC ******************

// ********** Begin Class UConversationResponseQueue Function GetQueuedResponses *******************
struct Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics
{
	struct ConversationResponseQueue_eventGetQueuedResponses_Parms
	{
		TArray<FCandidateNPCResponse> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
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
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventGetQueuedResponses_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::PropPointers) < 2048);
// ********** End Function GetQueuedResponses Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "GetQueuedResponses", 	Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::ConversationResponseQueue_eventGetQueuedResponses_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::ConversationResponseQueue_eventGetQueuedResponses_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execGetQueuedResponses)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FCandidateNPCResponse>*)Z_Param__Result=P_THIS->GetQueuedResponses();
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function GetQueuedResponses *********************

// ********** Begin Class UConversationResponseQueue Function GetResponseByID **********************
struct Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics
{
	struct ConversationResponseQueue_eventGetResponseByID_Parms
	{
		FGuid ResponseID;
		FCandidateNPCResponse OutResponse;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResponseByID constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResponse;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResponseByID constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResponseByID Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventGetResponseByID_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_OutResponse = { "OutResponse", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventGetResponseByID_Parms, OutResponse), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
void Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventGetResponseByID_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventGetResponseByID_Parms), &Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_OutResponse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::PropPointers) < 2048);
// ********** End Function GetResponseByID Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "GetResponseByID", 	Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::ConversationResponseQueue_eventGetResponseByID_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::ConversationResponseQueue_eventGetResponseByID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execGetResponseByID)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_OutResponse);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->GetResponseByID(Z_Param_ResponseID,Z_Param_Out_OutResponse);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function GetResponseByID ************************

// ********** Begin Class UConversationResponseQueue Function HasResponseFromNPC *******************
struct Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics
{
	struct ConversationResponseQueue_eventHasResponseFromNPC_Parms
	{
		FName NPCID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function HasResponseFromNPC constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_NPCID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function HasResponseFromNPC constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function HasResponseFromNPC Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_NPCID = { "NPCID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventHasResponseFromNPC_Parms, NPCID), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventHasResponseFromNPC_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventHasResponseFromNPC_Parms), &Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_NPCID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::PropPointers) < 2048);
// ********** End Function HasResponseFromNPC Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "HasResponseFromNPC", 	Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::ConversationResponseQueue_eventHasResponseFromNPC_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::ConversationResponseQueue_eventHasResponseFromNPC_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execHasResponseFromNPC)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_NPCID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->HasResponseFromNPC(Z_Param_NPCID);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function HasResponseFromNPC *********************

// ********** Begin Class UConversationResponseQueue Function Num **********************************
struct Z_Construct_UFunction_UConversationResponseQueue_Num_Statics
{
	struct ConversationResponseQueue_eventNum_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Num constinit property declarations ***********************************
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Num constinit property declarations *************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Num Property Definitions **********************************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventNum_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::PropPointers) < 2048);
// ********** End Function Num Property Definitions ************************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "Num", 	Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::ConversationResponseQueue_eventNum_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::ConversationResponseQueue_eventNum_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_Num()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_Num_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execNum)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->Num();
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function Num ************************************

// ********** Begin Class UConversationResponseQueue Function PeekNextResponse *********************
struct Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics
{
	struct ConversationResponseQueue_eventPeekNextResponse_Parms
	{
		FCandidateNPCResponse OutResponse;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function PeekNextResponse constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResponse;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function PeekNextResponse constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function PeekNextResponse Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_OutResponse = { "OutResponse", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventPeekNextResponse_Parms, OutResponse), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
void Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventPeekNextResponse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventPeekNextResponse_Parms), &Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_OutResponse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::PropPointers) < 2048);
// ********** End Function PeekNextResponse Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "PeekNextResponse", 	Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::ConversationResponseQueue_eventPeekNextResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::ConversationResponseQueue_eventPeekNextResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execPeekNextResponse)
{
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_OutResponse);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->PeekNextResponse(Z_Param_Out_OutResponse);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function PeekNextResponse ***********************

// ********** Begin Class UConversationResponseQueue Function PopNextResponse **********************
struct Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics
{
	struct ConversationResponseQueue_eventPopNextResponse_Parms
	{
		FCandidateNPCResponse OutResponse;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function PopNextResponse constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResponse;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function PopNextResponse constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function PopNextResponse Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_OutResponse = { "OutResponse", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventPopNextResponse_Parms, OutResponse), Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
void Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventPopNextResponse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventPopNextResponse_Parms), &Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_OutResponse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::PropPointers) < 2048);
// ********** End Function PopNextResponse Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "PopNextResponse", 	Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::ConversationResponseQueue_eventPopNextResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::ConversationResponseQueue_eventPopNextResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execPopNextResponse)
{
	P_GET_STRUCT_REF(FCandidateNPCResponse,Z_Param_Out_OutResponse);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->PopNextResponse(Z_Param_Out_OutResponse);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function PopNextResponse ************************

// ********** Begin Class UConversationResponseQueue Function RemoveAllFromConversation ************
struct Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics
{
	struct ConversationResponseQueue_eventRemoveAllFromConversation_Parms
	{
		FName ConversationID;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveAllFromConversation constinit property declarations *************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ConversationID;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveAllFromConversation constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveAllFromConversation Property Definitions ************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::NewProp_ConversationID = { "ConversationID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventRemoveAllFromConversation_Parms, ConversationID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventRemoveAllFromConversation_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::NewProp_ConversationID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::PropPointers) < 2048);
// ********** End Function RemoveAllFromConversation Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "RemoveAllFromConversation", 	Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::ConversationResponseQueue_eventRemoveAllFromConversation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::ConversationResponseQueue_eventRemoveAllFromConversation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execRemoveAllFromConversation)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_ConversationID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->RemoveAllFromConversation(Z_Param_ConversationID);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function RemoveAllFromConversation **************

// ********** Begin Class UConversationResponseQueue Function RemoveAllFromNPC *********************
struct Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics
{
	struct ConversationResponseQueue_eventRemoveAllFromNPC_Parms
	{
		FName NPCID;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveAllFromNPC constinit property declarations **********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_NPCID;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveAllFromNPC constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveAllFromNPC Property Definitions *********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::NewProp_NPCID = { "NPCID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventRemoveAllFromNPC_Parms, NPCID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventRemoveAllFromNPC_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::NewProp_NPCID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::PropPointers) < 2048);
// ********** End Function RemoveAllFromNPC Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "RemoveAllFromNPC", 	Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::ConversationResponseQueue_eventRemoveAllFromNPC_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::ConversationResponseQueue_eventRemoveAllFromNPC_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execRemoveAllFromNPC)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_NPCID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->RemoveAllFromNPC(Z_Param_NPCID);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function RemoveAllFromNPC ***********************

// ********** Begin Class UConversationResponseQueue Function RemoveByResponseID *******************
struct Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics
{
	struct ConversationResponseQueue_eventRemoveByResponseID_Parms
	{
		FGuid ResponseID;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveByResponseID constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveByResponseID constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveByResponseID Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventRemoveByResponseID_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventRemoveByResponseID_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventRemoveByResponseID_Parms), &Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::PropPointers) < 2048);
// ********** End Function RemoveByResponseID Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "RemoveByResponseID", 	Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::ConversationResponseQueue_eventRemoveByResponseID_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::ConversationResponseQueue_eventRemoveByResponseID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execRemoveByResponseID)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RemoveByResponseID(Z_Param_ResponseID);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function RemoveByResponseID *********************

// ********** Begin Class UConversationResponseQueue Function ReprioritizeResponse *****************
struct Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics
{
	struct ConversationResponseQueue_eventReprioritizeResponse_Parms
	{
		FGuid ResponseID;
		float NewPriorityScore;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Queue" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ReprioritizeResponse constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ResponseID;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewPriorityScore;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ReprioritizeResponse constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ReprioritizeResponse Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ResponseID = { "ResponseID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventReprioritizeResponse_Parms, ResponseID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_NewPriorityScore = { "NewPriorityScore", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationResponseQueue_eventReprioritizeResponse_Parms, NewPriorityScore), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationResponseQueue_eventReprioritizeResponse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationResponseQueue_eventReprioritizeResponse_Parms), &Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ResponseID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_NewPriorityScore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::PropPointers) < 2048);
// ********** End Function ReprioritizeResponse Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationResponseQueue, nullptr, "ReprioritizeResponse", 	Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::ConversationResponseQueue_eventReprioritizeResponse_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::ConversationResponseQueue_eventReprioritizeResponse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationResponseQueue::execReprioritizeResponse)
{
	P_GET_STRUCT(FGuid,Z_Param_ResponseID);
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewPriorityScore);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ReprioritizeResponse(Z_Param_ResponseID,Z_Param_NewPriorityScore);
	P_NATIVE_END;
}
// ********** End Class UConversationResponseQueue Function ReprioritizeResponse *******************

// ********** Begin Class UConversationResponseQueue ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UConversationResponseQueue;
UClass* UConversationResponseQueue::GetPrivateStaticClass()
{
	using TClass = UConversationResponseQueue;
	if (!Z_Registration_Info_UClass_UConversationResponseQueue.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("ConversationResponseQueue"),
			Z_Registration_Info_UClass_UConversationResponseQueue.InnerSingleton,
			StaticRegisterNativesUConversationResponseQueue,
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
	return Z_Registration_Info_UClass_UConversationResponseQueue.InnerSingleton;
}
UClass* Z_Construct_UClass_UConversationResponseQueue_NoRegister()
{
	return UConversationResponseQueue::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UConversationResponseQueue_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "ConversationResponseQueue.h" },
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Responses_MetaData[] = {
		{ "ModuleRelativePath", "Public/ConversationResponseQueue.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UConversationResponseQueue constinit property declarations ***************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Responses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Responses;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UConversationResponseQueue constinit property declarations *****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("CountResponsesFromNPC"), .Pointer = &UConversationResponseQueue::execCountResponsesFromNPC },
		{ .NameUTF8 = UTF8TEXT("GetQueuedResponses"), .Pointer = &UConversationResponseQueue::execGetQueuedResponses },
		{ .NameUTF8 = UTF8TEXT("GetResponseByID"), .Pointer = &UConversationResponseQueue::execGetResponseByID },
		{ .NameUTF8 = UTF8TEXT("HasResponseFromNPC"), .Pointer = &UConversationResponseQueue::execHasResponseFromNPC },
		{ .NameUTF8 = UTF8TEXT("Num"), .Pointer = &UConversationResponseQueue::execNum },
		{ .NameUTF8 = UTF8TEXT("PeekNextResponse"), .Pointer = &UConversationResponseQueue::execPeekNextResponse },
		{ .NameUTF8 = UTF8TEXT("PopNextResponse"), .Pointer = &UConversationResponseQueue::execPopNextResponse },
		{ .NameUTF8 = UTF8TEXT("RemoveAllFromConversation"), .Pointer = &UConversationResponseQueue::execRemoveAllFromConversation },
		{ .NameUTF8 = UTF8TEXT("RemoveAllFromNPC"), .Pointer = &UConversationResponseQueue::execRemoveAllFromNPC },
		{ .NameUTF8 = UTF8TEXT("RemoveByResponseID"), .Pointer = &UConversationResponseQueue::execRemoveByResponseID },
		{ .NameUTF8 = UTF8TEXT("ReprioritizeResponse"), .Pointer = &UConversationResponseQueue::execReprioritizeResponse },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UConversationResponseQueue_CountResponsesFromNPC, "CountResponsesFromNPC" }, // 689662718
		{ &Z_Construct_UFunction_UConversationResponseQueue_GetQueuedResponses, "GetQueuedResponses" }, // 2625854031
		{ &Z_Construct_UFunction_UConversationResponseQueue_GetResponseByID, "GetResponseByID" }, // 3939479819
		{ &Z_Construct_UFunction_UConversationResponseQueue_HasResponseFromNPC, "HasResponseFromNPC" }, // 3546438826
		{ &Z_Construct_UFunction_UConversationResponseQueue_Num, "Num" }, // 811852794
		{ &Z_Construct_UFunction_UConversationResponseQueue_PeekNextResponse, "PeekNextResponse" }, // 2359696384
		{ &Z_Construct_UFunction_UConversationResponseQueue_PopNextResponse, "PopNextResponse" }, // 1435378348
		{ &Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromConversation, "RemoveAllFromConversation" }, // 1281461917
		{ &Z_Construct_UFunction_UConversationResponseQueue_RemoveAllFromNPC, "RemoveAllFromNPC" }, // 3455400348
		{ &Z_Construct_UFunction_UConversationResponseQueue_RemoveByResponseID, "RemoveByResponseID" }, // 2820016308
		{ &Z_Construct_UFunction_UConversationResponseQueue_ReprioritizeResponse, "ReprioritizeResponse" }, // 419130710
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UConversationResponseQueue>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UConversationResponseQueue_Statics

// ********** Begin Class UConversationResponseQueue Property Definitions **************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UConversationResponseQueue_Statics::NewProp_Responses_Inner = { "Responses", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FCandidateNPCResponse, METADATA_PARAMS(0, nullptr) }; // 3013045577
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UConversationResponseQueue_Statics::NewProp_Responses = { "Responses", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationResponseQueue, Responses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Responses_MetaData), NewProp_Responses_MetaData) }; // 3013045577
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UConversationResponseQueue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationResponseQueue_Statics::NewProp_Responses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationResponseQueue_Statics::NewProp_Responses,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationResponseQueue_Statics::PropPointers) < 2048);
// ********** End Class UConversationResponseQueue Property Definitions ****************************
UObject* (*const Z_Construct_UClass_UConversationResponseQueue_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationResponseQueue_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UConversationResponseQueue_Statics::ClassParams = {
	&UConversationResponseQueue::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UConversationResponseQueue_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UConversationResponseQueue_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationResponseQueue_Statics::Class_MetaDataParams), Z_Construct_UClass_UConversationResponseQueue_Statics::Class_MetaDataParams)
};
void UConversationResponseQueue::StaticRegisterNativesUConversationResponseQueue()
{
	UClass* Class = UConversationResponseQueue::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UConversationResponseQueue_Statics::Funcs));
}
UClass* Z_Construct_UClass_UConversationResponseQueue()
{
	if (!Z_Registration_Info_UClass_UConversationResponseQueue.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UConversationResponseQueue.OuterSingleton, Z_Construct_UClass_UConversationResponseQueue_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UConversationResponseQueue.OuterSingleton;
}
UConversationResponseQueue::UConversationResponseQueue(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UConversationResponseQueue);
UConversationResponseQueue::~UConversationResponseQueue() {}
// ********** End Class UConversationResponseQueue *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h__Script_NaturalNPCConversationGovernance_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UConversationResponseQueue, UConversationResponseQueue::StaticClass, TEXT("UConversationResponseQueue"), &Z_Registration_Info_UClass_UConversationResponseQueue, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UConversationResponseQueue), 1456639127U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h__Script_NaturalNPCConversationGovernance_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h__Script_NaturalNPCConversationGovernance_2827241568{
	TEXT("/Script/NaturalNPCConversationGovernance"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationResponseQueue_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
