// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ConversationTurnManager.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeConversationTurnManager() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationTurnManager();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationTurnManager_NoRegister();
NATURALNPCCONVERSATIONGOVERNANCE_API UEnum* Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState();
NATURALNPCCONVERSATIONGOVERNANCE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_NaturalNPCConversationGovernance();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UConversationTurnManager Function BeginEmergencyInterrupt ****************
struct Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics
{
	struct ConversationTurnManager_eventBeginEmergencyInterrupt_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventBeginEmergencyInterrupt_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventBeginEmergencyInterrupt_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventBeginEmergencyInterrupt_Parms), &Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::PropPointers) < 2048);
// ********** End Function BeginEmergencyInterrupt Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "BeginEmergencyInterrupt", 	Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::ConversationTurnManager_eventBeginEmergencyInterrupt_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::ConversationTurnManager_eventBeginEmergencyInterrupt_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execBeginEmergencyInterrupt)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->BeginEmergencyInterrupt(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function BeginEmergencyInterrupt ******************

// ********** Begin Class UConversationTurnManager Function BeginSideComment ***********************
struct Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics
{
	struct ConversationTurnManager_eventBeginSideComment_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventBeginSideComment_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventBeginSideComment_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventBeginSideComment_Parms), &Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::PropPointers) < 2048);
// ********** End Function BeginSideComment Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "BeginSideComment", 	Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::ConversationTurnManager_eventBeginSideComment_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::ConversationTurnManager_eventBeginSideComment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_BeginSideComment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_BeginSideComment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execBeginSideComment)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->BeginSideComment(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function BeginSideComment *************************

// ********** Begin Class UConversationTurnManager Function EndSideComment *************************
struct Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics
{
	struct ConversationTurnManager_eventEndSideComment_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventEndSideComment_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventEndSideComment_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventEndSideComment_Parms), &Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::PropPointers) < 2048);
// ********** End Function EndSideComment Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "EndSideComment", 	Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::ConversationTurnManager_eventEndSideComment_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::ConversationTurnManager_eventEndSideComment_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_EndSideComment()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_EndSideComment_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execEndSideComment)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->EndSideComment(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function EndSideComment ***************************

// ********** Begin Class UConversationTurnManager Function EnterPermissionPending *****************
struct Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics
{
	struct ConversationTurnManager_eventEnterPermissionPending_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function EnterPermissionPending constinit property declarations ****************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function EnterPermissionPending constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function EnterPermissionPending Property Definitions ***************************
void Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventEnterPermissionPending_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventEnterPermissionPending_Parms), &Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::PropPointers) < 2048);
// ********** End Function EnterPermissionPending Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "EnterPermissionPending", 	Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::ConversationTurnManager_eventEnterPermissionPending_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::ConversationTurnManager_eventEnterPermissionPending_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execEnterPermissionPending)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->EnterPermissionPending();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function EnterPermissionPending *******************

// ********** Begin Class UConversationTurnManager Function GetCurrentSpeaker **********************
struct Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics
{
	struct ConversationTurnManager_eventGetCurrentSpeaker_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetCurrentSpeaker constinit property declarations *********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetCurrentSpeaker constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetCurrentSpeaker Property Definitions ********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventGetCurrentSpeaker_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::PropPointers) < 2048);
// ********** End Function GetCurrentSpeaker Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "GetCurrentSpeaker", 	Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::ConversationTurnManager_eventGetCurrentSpeaker_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::ConversationTurnManager_eventGetCurrentSpeaker_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execGetCurrentSpeaker)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->GetCurrentSpeaker();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function GetCurrentSpeaker ************************

// ********** Begin Class UConversationTurnManager Function GetCurrentTurnOwner ********************
struct Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics
{
	struct ConversationTurnManager_eventGetCurrentTurnOwner_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetCurrentTurnOwner constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetCurrentTurnOwner constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetCurrentTurnOwner Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventGetCurrentTurnOwner_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::PropPointers) < 2048);
// ********** End Function GetCurrentTurnOwner Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "GetCurrentTurnOwner", 	Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::ConversationTurnManager_eventGetCurrentTurnOwner_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::ConversationTurnManager_eventGetCurrentTurnOwner_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execGetCurrentTurnOwner)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->GetCurrentTurnOwner();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function GetCurrentTurnOwner **********************

// ********** Begin Class UConversationTurnManager Function GetSpeechEndTime ***********************
struct Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics
{
	struct ConversationTurnManager_eventGetSpeechEndTime_Parms
	{
		double ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetSpeechEndTime constinit property declarations **********************
	static const UECodeGen_Private::FDoublePropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSpeechEndTime constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSpeechEndTime Property Definitions *********************************
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventGetSpeechEndTime_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::PropPointers) < 2048);
// ********** End Function GetSpeechEndTime Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "GetSpeechEndTime", 	Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::ConversationTurnManager_eventGetSpeechEndTime_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::ConversationTurnManager_eventGetSpeechEndTime_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execGetSpeechEndTime)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(double*)Z_Param__Result=P_THIS->GetSpeechEndTime();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function GetSpeechEndTime *************************

// ********** Begin Class UConversationTurnManager Function GetSpeechStartTime *********************
struct Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics
{
	struct ConversationTurnManager_eventGetSpeechStartTime_Parms
	{
		double ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetSpeechStartTime constinit property declarations ********************
	static const UECodeGen_Private::FDoublePropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSpeechStartTime constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSpeechStartTime Property Definitions *******************************
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventGetSpeechStartTime_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::PropPointers) < 2048);
// ********** End Function GetSpeechStartTime Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "GetSpeechStartTime", 	Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::ConversationTurnManager_eventGetSpeechStartTime_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::ConversationTurnManager_eventGetSpeechStartTime_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execGetSpeechStartTime)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(double*)Z_Param__Result=P_THIS->GetSpeechStartTime();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function GetSpeechStartTime ***********************

// ********** Begin Class UConversationTurnManager Function GetTurnState ***************************
struct Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics
{
	struct ConversationTurnManager_eventGetTurnState_Parms
	{
		EConversationTurnState ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventGetTurnState_Parms, ReturnValue), Z_Construct_UEnum_NaturalNPCConversationGovernance_EConversationTurnState, METADATA_PARAMS(0, nullptr) }; // 1236676757
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::PropPointers) < 2048);
// ********** End Function GetTurnState Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "GetTurnState", 	Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::ConversationTurnManager_eventGetTurnState_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::ConversationTurnManager_eventGetTurnState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_GetTurnState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_GetTurnState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execGetTurnState)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EConversationTurnState*)Z_Param__Result=P_THIS->GetTurnState();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function GetTurnState *****************************

// ********** Begin Class UConversationTurnManager Function IsMainTurnAvailable ********************
struct Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics
{
	struct ConversationTurnManager_eventIsMainTurnAvailable_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsMainTurnAvailable constinit property declarations *******************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsMainTurnAvailable constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsMainTurnAvailable Property Definitions ******************************
void Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventIsMainTurnAvailable_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventIsMainTurnAvailable_Parms), &Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::PropPointers) < 2048);
// ********** End Function IsMainTurnAvailable Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "IsMainTurnAvailable", 	Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::ConversationTurnManager_eventIsMainTurnAvailable_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::ConversationTurnManager_eventIsMainTurnAvailable_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execIsMainTurnAvailable)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsMainTurnAvailable();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function IsMainTurnAvailable **********************

// ********** Begin Class UConversationTurnManager Function NotifyNPCSpeechEnded *******************
struct Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics
{
	struct ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms), &Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::PropPointers) < 2048);
// ********** End Function NotifyNPCSpeechEnded Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "NotifyNPCSpeechEnded", 	Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::ConversationTurnManager_eventNotifyNPCSpeechEnded_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execNotifyNPCSpeechEnded)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyNPCSpeechEnded(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function NotifyNPCSpeechEnded *********************

// ********** Begin Class UConversationTurnManager Function NotifyNPCSpeechStarted *****************
struct Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics
{
	struct ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms
	{
		AActor* NPCActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_NPCActor = { "NPCActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms, NPCActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms), &Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_NPCActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::PropPointers) < 2048);
// ********** End Function NotifyNPCSpeechStarted Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "NotifyNPCSpeechStarted", 	Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::ConversationTurnManager_eventNotifyNPCSpeechStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execNotifyNPCSpeechStarted)
{
	P_GET_OBJECT(AActor,Z_Param_NPCActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyNPCSpeechStarted(Z_Param_NPCActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function NotifyNPCSpeechStarted *******************

// ********** Begin Class UConversationTurnManager Function NotifyPlayerSpeechEnded ****************
struct Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics
{
	struct ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms
	{
		AActor* PlayerActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_PlayerActor = { "PlayerActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms, PlayerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms), &Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_PlayerActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::PropPointers) < 2048);
// ********** End Function NotifyPlayerSpeechEnded Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "NotifyPlayerSpeechEnded", 	Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::ConversationTurnManager_eventNotifyPlayerSpeechEnded_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execNotifyPlayerSpeechEnded)
{
	P_GET_OBJECT(AActor,Z_Param_PlayerActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyPlayerSpeechEnded(Z_Param_PlayerActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function NotifyPlayerSpeechEnded ******************

// ********** Begin Class UConversationTurnManager Function NotifyPlayerSpeechStarted **************
struct Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics
{
	struct ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms
	{
		AActor* PlayerActor;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_PlayerActor = { "PlayerActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms, PlayerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms), &Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_PlayerActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::PropPointers) < 2048);
// ********** End Function NotifyPlayerSpeechStarted Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "NotifyPlayerSpeechStarted", 	Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::ConversationTurnManager_eventNotifyPlayerSpeechStarted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execNotifyPlayerSpeechStarted)
{
	P_GET_OBJECT(AActor,Z_Param_PlayerActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->NotifyPlayerSpeechStarted(Z_Param_PlayerActor);
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function NotifyPlayerSpeechStarted ****************

// ********** Begin Class UConversationTurnManager Function ResolvePermissionPending ***************
struct Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics
{
	struct ConversationTurnManager_eventResolvePermissionPending_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ResolvePermissionPending constinit property declarations **************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ResolvePermissionPending constinit property declarations ****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ResolvePermissionPending Property Definitions *************************
void Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationTurnManager_eventResolvePermissionPending_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationTurnManager_eventResolvePermissionPending_Parms), &Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::PropPointers) < 2048);
// ********** End Function ResolvePermissionPending Property Definitions ***************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationTurnManager, nullptr, "ResolvePermissionPending", 	Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::ConversationTurnManager_eventResolvePermissionPending_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::ConversationTurnManager_eventResolvePermissionPending_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationTurnManager::execResolvePermissionPending)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ResolvePermissionPending();
	P_NATIVE_END;
}
// ********** End Class UConversationTurnManager Function ResolvePermissionPending *****************

// ********** Begin Class UConversationTurnManager *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UConversationTurnManager;
UClass* UConversationTurnManager::GetPrivateStaticClass()
{
	using TClass = UConversationTurnManager;
	if (!Z_Registration_Info_UClass_UConversationTurnManager.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("ConversationTurnManager"),
			Z_Registration_Info_UClass_UConversationTurnManager.InnerSingleton,
			StaticRegisterNativesUConversationTurnManager,
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
	return Z_Registration_Info_UClass_UConversationTurnManager.InnerSingleton;
}
UClass* Z_Construct_UClass_UConversationTurnManager_NoRegister()
{
	return UConversationTurnManager::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UConversationTurnManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "ConversationTurnManager.h" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnTurnOwnerChanged_MetaData[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MinimumTransitionPauseSeconds_MetaData[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
		{ "Units", "s" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bInterruptionAllowed_MetaData[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSideCommentPlaying_MetaData[] = {
		{ "Category", "Conversation Governance|Turn" },
		{ "ModuleRelativePath", "Public/ConversationTurnManager.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UConversationTurnManager constinit property declarations *****************
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnTurnOwnerChanged;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MinimumTransitionPauseSeconds;
	static void NewProp_bInterruptionAllowed_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bInterruptionAllowed;
	static void NewProp_bSideCommentPlaying_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSideCommentPlaying;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UConversationTurnManager constinit property declarations *******************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("BeginEmergencyInterrupt"), .Pointer = &UConversationTurnManager::execBeginEmergencyInterrupt },
		{ .NameUTF8 = UTF8TEXT("BeginSideComment"), .Pointer = &UConversationTurnManager::execBeginSideComment },
		{ .NameUTF8 = UTF8TEXT("EndSideComment"), .Pointer = &UConversationTurnManager::execEndSideComment },
		{ .NameUTF8 = UTF8TEXT("EnterPermissionPending"), .Pointer = &UConversationTurnManager::execEnterPermissionPending },
		{ .NameUTF8 = UTF8TEXT("GetCurrentSpeaker"), .Pointer = &UConversationTurnManager::execGetCurrentSpeaker },
		{ .NameUTF8 = UTF8TEXT("GetCurrentTurnOwner"), .Pointer = &UConversationTurnManager::execGetCurrentTurnOwner },
		{ .NameUTF8 = UTF8TEXT("GetSpeechEndTime"), .Pointer = &UConversationTurnManager::execGetSpeechEndTime },
		{ .NameUTF8 = UTF8TEXT("GetSpeechStartTime"), .Pointer = &UConversationTurnManager::execGetSpeechStartTime },
		{ .NameUTF8 = UTF8TEXT("GetTurnState"), .Pointer = &UConversationTurnManager::execGetTurnState },
		{ .NameUTF8 = UTF8TEXT("IsMainTurnAvailable"), .Pointer = &UConversationTurnManager::execIsMainTurnAvailable },
		{ .NameUTF8 = UTF8TEXT("NotifyNPCSpeechEnded"), .Pointer = &UConversationTurnManager::execNotifyNPCSpeechEnded },
		{ .NameUTF8 = UTF8TEXT("NotifyNPCSpeechStarted"), .Pointer = &UConversationTurnManager::execNotifyNPCSpeechStarted },
		{ .NameUTF8 = UTF8TEXT("NotifyPlayerSpeechEnded"), .Pointer = &UConversationTurnManager::execNotifyPlayerSpeechEnded },
		{ .NameUTF8 = UTF8TEXT("NotifyPlayerSpeechStarted"), .Pointer = &UConversationTurnManager::execNotifyPlayerSpeechStarted },
		{ .NameUTF8 = UTF8TEXT("ResolvePermissionPending"), .Pointer = &UConversationTurnManager::execResolvePermissionPending },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UConversationTurnManager_BeginEmergencyInterrupt, "BeginEmergencyInterrupt" }, // 2399614004
		{ &Z_Construct_UFunction_UConversationTurnManager_BeginSideComment, "BeginSideComment" }, // 2290411241
		{ &Z_Construct_UFunction_UConversationTurnManager_EndSideComment, "EndSideComment" }, // 1489299245
		{ &Z_Construct_UFunction_UConversationTurnManager_EnterPermissionPending, "EnterPermissionPending" }, // 3641039274
		{ &Z_Construct_UFunction_UConversationTurnManager_GetCurrentSpeaker, "GetCurrentSpeaker" }, // 3389949496
		{ &Z_Construct_UFunction_UConversationTurnManager_GetCurrentTurnOwner, "GetCurrentTurnOwner" }, // 50949994
		{ &Z_Construct_UFunction_UConversationTurnManager_GetSpeechEndTime, "GetSpeechEndTime" }, // 4230341881
		{ &Z_Construct_UFunction_UConversationTurnManager_GetSpeechStartTime, "GetSpeechStartTime" }, // 858059713
		{ &Z_Construct_UFunction_UConversationTurnManager_GetTurnState, "GetTurnState" }, // 441799520
		{ &Z_Construct_UFunction_UConversationTurnManager_IsMainTurnAvailable, "IsMainTurnAvailable" }, // 14088976
		{ &Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechEnded, "NotifyNPCSpeechEnded" }, // 4204703684
		{ &Z_Construct_UFunction_UConversationTurnManager_NotifyNPCSpeechStarted, "NotifyNPCSpeechStarted" }, // 436874764
		{ &Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechEnded, "NotifyPlayerSpeechEnded" }, // 2949570862
		{ &Z_Construct_UFunction_UConversationTurnManager_NotifyPlayerSpeechStarted, "NotifyPlayerSpeechStarted" }, // 2749463604
		{ &Z_Construct_UFunction_UConversationTurnManager_ResolvePermissionPending, "ResolvePermissionPending" }, // 2227800778
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UConversationTurnManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UConversationTurnManager_Statics

// ********** Begin Class UConversationTurnManager Property Definitions ****************************
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_OnTurnOwnerChanged = { "OnTurnOwnerChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationTurnManager, OnTurnOwnerChanged), Z_Construct_UDelegateFunction_NaturalNPCConversationGovernance_ConversationTurnOwnerChangedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnTurnOwnerChanged_MetaData), NewProp_OnTurnOwnerChanged_MetaData) }; // 1655774746
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_MinimumTransitionPauseSeconds = { "MinimumTransitionPauseSeconds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationTurnManager, MinimumTransitionPauseSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MinimumTransitionPauseSeconds_MetaData), NewProp_MinimumTransitionPauseSeconds_MetaData) };
void Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bInterruptionAllowed_SetBit(void* Obj)
{
	((UConversationTurnManager*)Obj)->bInterruptionAllowed = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bInterruptionAllowed = { "bInterruptionAllowed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationTurnManager), &Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bInterruptionAllowed_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bInterruptionAllowed_MetaData), NewProp_bInterruptionAllowed_MetaData) };
void Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bSideCommentPlaying_SetBit(void* Obj)
{
	((UConversationTurnManager*)Obj)->bSideCommentPlaying = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bSideCommentPlaying = { "bSideCommentPlaying", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationTurnManager), &Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bSideCommentPlaying_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSideCommentPlaying_MetaData), NewProp_bSideCommentPlaying_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UConversationTurnManager_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_OnTurnOwnerChanged,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_MinimumTransitionPauseSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bInterruptionAllowed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationTurnManager_Statics::NewProp_bSideCommentPlaying,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationTurnManager_Statics::PropPointers) < 2048);
// ********** End Class UConversationTurnManager Property Definitions ******************************
UObject* (*const Z_Construct_UClass_UConversationTurnManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationTurnManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UConversationTurnManager_Statics::ClassParams = {
	&UConversationTurnManager::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UConversationTurnManager_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UConversationTurnManager_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationTurnManager_Statics::Class_MetaDataParams), Z_Construct_UClass_UConversationTurnManager_Statics::Class_MetaDataParams)
};
void UConversationTurnManager::StaticRegisterNativesUConversationTurnManager()
{
	UClass* Class = UConversationTurnManager::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UConversationTurnManager_Statics::Funcs));
}
UClass* Z_Construct_UClass_UConversationTurnManager()
{
	if (!Z_Registration_Info_UClass_UConversationTurnManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UConversationTurnManager.OuterSingleton, Z_Construct_UClass_UConversationTurnManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UConversationTurnManager.OuterSingleton;
}
UConversationTurnManager::UConversationTurnManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UConversationTurnManager);
UConversationTurnManager::~UConversationTurnManager() {}
// ********** End Class UConversationTurnManager ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h__Script_NaturalNPCConversationGovernance_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UConversationTurnManager, UConversationTurnManager::StaticClass, TEXT("UConversationTurnManager"), &Z_Registration_Info_UClass_UConversationTurnManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UConversationTurnManager), 4091822351U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h__Script_NaturalNPCConversationGovernance_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h__Script_NaturalNPCConversationGovernance_2983613619{
	TEXT("/Script/NaturalNPCConversationGovernance"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationTurnManager_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
