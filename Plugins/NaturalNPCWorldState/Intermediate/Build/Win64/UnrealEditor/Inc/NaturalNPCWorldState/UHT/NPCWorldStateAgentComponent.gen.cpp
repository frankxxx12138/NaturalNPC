// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldStateAgentComponent.h"
#include "NPCWorldStateTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldStateAgentComponent() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
ENGINE_API UClass* Z_Construct_UClass_UAnimSequence_NoRegister();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister();
NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature();
NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionResult();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldObjectState();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FNPCWorldStateUpdatedEvent ********************************************
struct Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms
	{
		FString WorldStateJson;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldStateJson_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FNPCWorldStateUpdatedEvent constinit property declarations ************
	static const UECodeGen_Private::FStrPropertyParams NewProp_WorldStateJson;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FNPCWorldStateUpdatedEvent constinit property declarations **************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FNPCWorldStateUpdatedEvent Property Definitions ***********************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::NewProp_WorldStateJson = { "WorldStateJson", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms, WorldStateJson), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldStateJson_MetaData), NewProp_WorldStateJson_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::NewProp_WorldStateJson,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FNPCWorldStateUpdatedEvent Property Definitions *************************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCWorldState, nullptr, "NPCWorldStateUpdatedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FNPCWorldStateUpdatedEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldStateUpdatedEvent, const FString& WorldStateJson)
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms
	{
		FString WorldStateJson;
	};
	_Script_NaturalNPCWorldState_eventNPCWorldStateUpdatedEvent_Parms Parms;
	Parms.WorldStateJson=WorldStateJson;
	NPCWorldStateUpdatedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FNPCWorldStateUpdatedEvent **********************************************

// ********** Begin Delegate FNPCWorldActionCompletedEvent *****************************************
struct Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms
	{
		FNPCWorldActionResult Result;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Result_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FNPCWorldActionCompletedEvent constinit property declarations *********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Result;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FNPCWorldActionCompletedEvent constinit property declarations ***********
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FNPCWorldActionCompletedEvent Property Definitions ********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::NewProp_Result = { "Result", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms, Result), Z_Construct_UScriptStruct_FNPCWorldActionResult, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Result_MetaData), NewProp_Result_MetaData) }; // 3945505905
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::NewProp_Result,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FNPCWorldActionCompletedEvent Property Definitions **********************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCWorldState, nullptr, "NPCWorldActionCompletedEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00530000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FNPCWorldActionCompletedEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldActionCompletedEvent, FNPCWorldActionResult const& Result)
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms
	{
		FNPCWorldActionResult Result;
	};
	_Script_NaturalNPCWorldState_eventNPCWorldActionCompletedEvent_Parms Parms;
	Parms.Result=Result;
	NPCWorldActionCompletedEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FNPCWorldActionCompletedEvent *******************************************

// ********** Begin Class UNPCWorldStateAgentComponent Function DropHeldActor **********************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics
{
	struct NPCWorldStateAgentComponent_eventDropHeldActor_Parms
	{
		bool bThrow;
		FNPCWorldActionResult OutResult;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function DropHeldActor constinit property declarations *************************
	static void NewProp_bThrow_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bThrow;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResult;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function DropHeldActor constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function DropHeldActor Property Definitions ************************************
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_bThrow_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventDropHeldActor_Parms*)Obj)->bThrow = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_bThrow = { "bThrow", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventDropHeldActor_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_bThrow_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_OutResult = { "OutResult", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventDropHeldActor_Parms, OutResult), Z_Construct_UScriptStruct_FNPCWorldActionResult, METADATA_PARAMS(0, nullptr) }; // 3945505905
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventDropHeldActor_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventDropHeldActor_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_bThrow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_OutResult,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::PropPointers) < 2048);
// ********** End Function DropHeldActor Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "DropHeldActor", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NPCWorldStateAgentComponent_eventDropHeldActor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::NPCWorldStateAgentComponent_eventDropHeldActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execDropHeldActor)
{
	P_GET_UBOOL(Z_Param_bThrow);
	P_GET_STRUCT_REF(FNPCWorldActionResult,Z_Param_Out_OutResult);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->DropHeldActor(Z_Param_bThrow,Z_Param_Out_OutResult);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function DropHeldActor ************************

// ********** Begin Class UNPCWorldStateAgentComponent Function ExecuteWorldAction *****************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics
{
	struct NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms
	{
		FName ObjectId;
		FName ActionId;
		FString Parameters;
		FNPCWorldActionResult OutResult;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameters_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ExecuteWorldAction constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActionId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Parameters;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutResult;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ExecuteWorldAction constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ExecuteWorldAction Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms, ObjectId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ActionId = { "ActionId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms, ActionId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_Parameters = { "Parameters", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms, Parameters), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameters_MetaData), NewProp_Parameters_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_OutResult = { "OutResult", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms, OutResult), Z_Construct_UScriptStruct_FNPCWorldActionResult, METADATA_PARAMS(0, nullptr) }; // 3945505905
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ActionId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_Parameters,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_OutResult,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::PropPointers) < 2048);
// ********** End Function ExecuteWorldAction Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "ExecuteWorldAction", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::NPCWorldStateAgentComponent_eventExecuteWorldAction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execExecuteWorldAction)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_ObjectId);
	P_GET_PROPERTY(FNameProperty,Z_Param_ActionId);
	P_GET_PROPERTY(FStrProperty,Z_Param_Parameters);
	P_GET_STRUCT_REF(FNPCWorldActionResult,Z_Param_Out_OutResult);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ExecuteWorldAction(Z_Param_ObjectId,Z_Param_ActionId,Z_Param_Parameters,Z_Param_Out_OutResult);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function ExecuteWorldAction *******************

// ********** Begin Class UNPCWorldStateAgentComponent Function GetCurrentHeldIdleAnimation ********
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics
{
	struct NPCWorldStateAgentComponent_eventGetCurrentHeldIdleAnimation_Parms
	{
		UAnimSequence* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetCurrentHeldIdleAnimation constinit property declarations ***********
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetCurrentHeldIdleAnimation constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetCurrentHeldIdleAnimation Property Definitions **********************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetCurrentHeldIdleAnimation_Parms, ReturnValue), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::PropPointers) < 2048);
// ********** End Function GetCurrentHeldIdleAnimation Property Definitions ************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetCurrentHeldIdleAnimation", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::NPCWorldStateAgentComponent_eventGetCurrentHeldIdleAnimation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::NPCWorldStateAgentComponent_eventGetCurrentHeldIdleAnimation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetCurrentHeldIdleAnimation)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UAnimSequence**)Z_Param__Result=P_THIS->GetCurrentHeldIdleAnimation();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetCurrentHeldIdleAnimation **********

// ********** Begin Class UNPCWorldStateAgentComponent Function GetHeldActor ***********************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics
{
	struct NPCWorldStateAgentComponent_eventGetHeldActor_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetHeldActor constinit property declarations **************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetHeldActor constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetHeldActor Property Definitions *************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetHeldActor_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::PropPointers) < 2048);
// ********** End Function GetHeldActor Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetHeldActor", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::NPCWorldStateAgentComponent_eventGetHeldActor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::NPCWorldStateAgentComponent_eventGetHeldActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetHeldActor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->GetHeldActor();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetHeldActor *************************

// ********** Begin Class UNPCWorldStateAgentComponent Function GetHeldWalkAnimation ***************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics
{
	struct NPCWorldStateAgentComponent_eventGetHeldWalkAnimation_Parms
	{
		UAnimSequence* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetHeldWalkAnimation constinit property declarations ******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetHeldWalkAnimation constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetHeldWalkAnimation Property Definitions *****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetHeldWalkAnimation_Parms, ReturnValue), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::PropPointers) < 2048);
// ********** End Function GetHeldWalkAnimation Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetHeldWalkAnimation", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::NPCWorldStateAgentComponent_eventGetHeldWalkAnimation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::NPCWorldStateAgentComponent_eventGetHeldWalkAnimation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetHeldWalkAnimation)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UAnimSequence**)Z_Param__Result=P_THIS->GetHeldWalkAnimation();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetHeldWalkAnimation *****************

// ********** Begin Class UNPCWorldStateAgentComponent Function GetPickupAnimationForTarget ********
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics
{
	struct NPCWorldStateAgentComponent_eventGetPickupAnimationForTarget_Parms
	{
		AActor* Target;
		UAnimSequence* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetPickupAnimationForTarget constinit property declarations ***********
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Target;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetPickupAnimationForTarget constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetPickupAnimationForTarget Property Definitions **********************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NewProp_Target = { "Target", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetPickupAnimationForTarget_Parms, Target), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetPickupAnimationForTarget_Parms, ReturnValue), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NewProp_Target,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::PropPointers) < 2048);
// ********** End Function GetPickupAnimationForTarget Property Definitions ************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetPickupAnimationForTarget", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NPCWorldStateAgentComponent_eventGetPickupAnimationForTarget_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::NPCWorldStateAgentComponent_eventGetPickupAnimationForTarget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetPickupAnimationForTarget)
{
	P_GET_OBJECT(AActor,Z_Param_Target);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UAnimSequence**)Z_Param__Result=P_THIS->GetPickupAnimationForTarget(Z_Param_Target);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetPickupAnimationForTarget **********

// ********** Begin Class UNPCWorldStateAgentComponent Function GetVisibleWorldObjects *************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics
{
	struct NPCWorldStateAgentComponent_eventGetVisibleWorldObjects_Parms
	{
		TArray<FNPCWorldObjectState> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetVisibleWorldObjects constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetVisibleWorldObjects constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetVisibleWorldObjects Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldObjectState, METADATA_PARAMS(0, nullptr) }; // 871788507
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetVisibleWorldObjects_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 871788507
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::PropPointers) < 2048);
// ********** End Function GetVisibleWorldObjects Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetVisibleWorldObjects", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NPCWorldStateAgentComponent_eventGetVisibleWorldObjects_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::NPCWorldStateAgentComponent_eventGetVisibleWorldObjects_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetVisibleWorldObjects)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FNPCWorldObjectState>*)Z_Param__Result=P_THIS->GetVisibleWorldObjects();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetVisibleWorldObjects ***************

// ********** Begin Class UNPCWorldStateAgentComponent Function GetWorldStateJson ******************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics
{
	struct NPCWorldStateAgentComponent_eventGetWorldStateJson_Parms
	{
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetWorldStateJson constinit property declarations *********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetWorldStateJson constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetWorldStateJson Property Definitions ********************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetWorldStateJson_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::PropPointers) < 2048);
// ********** End Function GetWorldStateJson Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetWorldStateJson", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::NPCWorldStateAgentComponent_eventGetWorldStateJson_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::NPCWorldStateAgentComponent_eventGetWorldStateJson_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetWorldStateJson)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=P_THIS->GetWorldStateJson();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetWorldStateJson ********************

// ********** Begin Class UNPCWorldStateAgentComponent Function GetWorldStateText ******************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics
{
	struct NPCWorldStateAgentComponent_eventGetWorldStateText_Parms
	{
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetWorldStateText constinit property declarations *********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetWorldStateText constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetWorldStateText Property Definitions ********************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventGetWorldStateText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::PropPointers) < 2048);
// ********** End Function GetWorldStateText Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "GetWorldStateText", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::NPCWorldStateAgentComponent_eventGetWorldStateText_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::NPCWorldStateAgentComponent_eventGetWorldStateText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execGetWorldStateText)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=P_THIS->GetWorldStateText();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function GetWorldStateText ********************

// ********** Begin Class UNPCWorldStateAgentComponent Function IsWorldActionInProgress ************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics
{
	struct NPCWorldStateAgentComponent_eventIsWorldActionInProgress_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsWorldActionInProgress constinit property declarations ***************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsWorldActionInProgress constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsWorldActionInProgress Property Definitions **************************
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventIsWorldActionInProgress_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventIsWorldActionInProgress_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::PropPointers) < 2048);
// ********** End Function IsWorldActionInProgress Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "IsWorldActionInProgress", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NPCWorldStateAgentComponent_eventIsWorldActionInProgress_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::NPCWorldStateAgentComponent_eventIsWorldActionInProgress_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execIsWorldActionInProgress)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsWorldActionInProgress();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function IsWorldActionInProgress **************

// ********** Begin Class UNPCWorldStateAgentComponent Function RefreshWorldState ******************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RefreshWorldState constinit property declarations *********************
// ********** End Function RefreshWorldState constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "RefreshWorldState", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execRefreshWorldState)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RefreshWorldState();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function RefreshWorldState ********************

// ********** Begin Class UNPCWorldStateAgentComponent Function ResumeHeldIdleAnimation ************
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics
{
	struct NPCWorldStateAgentComponent_eventResumeHeldIdleAnimation_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ResumeHeldIdleAnimation constinit property declarations ***************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ResumeHeldIdleAnimation constinit property declarations *****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ResumeHeldIdleAnimation Property Definitions **************************
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventResumeHeldIdleAnimation_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventResumeHeldIdleAnimation_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::PropPointers) < 2048);
// ********** End Function ResumeHeldIdleAnimation Property Definitions ****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "ResumeHeldIdleAnimation", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NPCWorldStateAgentComponent_eventResumeHeldIdleAnimation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::NPCWorldStateAgentComponent_eventResumeHeldIdleAnimation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execResumeHeldIdleAnimation)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ResumeHeldIdleAnimation();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function ResumeHeldIdleAnimation **************

// ********** Begin Class UNPCWorldStateAgentComponent Function SuspendHeldIdleAnimation ***********
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SuspendHeldIdleAnimation constinit property declarations **************
// ********** End Function SuspendHeldIdleAnimation constinit property declarations ****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "SuspendHeldIdleAnimation", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execSuspendHeldIdleAnimation)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SuspendHeldIdleAnimation();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function SuspendHeldIdleAnimation *************

// ********** Begin Class UNPCWorldStateAgentComponent Function TryExecuteNaturalLanguageAction ****
struct Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics
{
	struct NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms
	{
		FString Command;
		FString OutReply;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Command_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function TryExecuteNaturalLanguageAction constinit property declarations *******
	static const UECodeGen_Private::FStrPropertyParams NewProp_Command;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OutReply;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function TryExecuteNaturalLanguageAction constinit property declarations *********
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function TryExecuteNaturalLanguageAction Property Definitions ******************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_Command = { "Command", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms, Command), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Command_MetaData), NewProp_Command_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_OutReply = { "OutReply", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms, OutReply), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms), &Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_Command,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_OutReply,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::PropPointers) < 2048);
// ********** End Function TryExecuteNaturalLanguageAction Property Definitions ********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateAgentComponent, nullptr, "TryExecuteNaturalLanguageAction", 	Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::NPCWorldStateAgentComponent_eventTryExecuteNaturalLanguageAction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateAgentComponent::execTryExecuteNaturalLanguageAction)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Command);
	P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_OutReply);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->TryExecuteNaturalLanguageAction(Z_Param_Command,Z_Param_Out_OutReply);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateAgentComponent Function TryExecuteNaturalLanguageAction ******

// ********** Begin Class UNPCWorldStateAgentComponent *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCWorldStateAgentComponent;
UClass* UNPCWorldStateAgentComponent::GetPrivateStaticClass()
{
	using TClass = UNPCWorldStateAgentComponent;
	if (!Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCWorldStateAgentComponent"),
			Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.InnerSingleton,
			StaticRegisterNativesUNPCWorldStateAgentComponent,
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
	return Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister()
{
	return UNPCWorldStateAgentComponent::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "NPCWorldState" },
		{ "IncludePath", "NPCWorldStateAgentComponent.h" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PerceptionRadius_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoRefresh_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RefreshIntervalSeconds_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ClampMin", "0.1" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HandSocket_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SupportHandSocket_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HandMeshNameHint_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultPickupDistance_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ClampMin", "20.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bApproachPickupTargets_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxPickupApproachDistance_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "100.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachWalkSpeed_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "20.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachRunSpeed_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "20.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachRunDistance_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "100.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachTimeoutSeconds_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachRotationSpeed_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMin", "0.1" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachRepathIntervalSeconds_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMax", "2.0" },
		{ "ClampMin", "0.05" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "s" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachStallSeconds_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMax", "2.0" },
		{ "ClampMin", "0.05" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "s" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachCandidateCount_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMax", "32" },
		{ "ClampMin", "4" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaximumPickupApproachRepathFailures_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ClampMax", "10" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachGroundFollowInterpSpeed_MetaData[] = {
		{ "Category", "NPC World State|Approach|Grounding" },
		{ "ClampMax", "40.0" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachMaximumGroundStepUp_MetaData[] = {
		{ "Category", "NPC World State|Approach|Grounding" },
		{ "ClampMax", "100.0" },
		{ "ClampMin", "10.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachMaximumGroundStepDown_MetaData[] = {
		{ "Category", "NPC World State|Approach|Grounding" },
		{ "ClampMax", "200.0" },
		{ "ClampMin", "20.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupFacingYawOffsetDegrees_MetaData[] = {
		{ "Category", "NPC World State|Approach" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThrowImpulse_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultPickupAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GroundPickupAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TablePickupAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TablePickupMinimumHeight_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ClampMax", "140.0" },
		{ "ClampMin", "20.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GroundPickupAnimationPlayRate_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ClampMin", "0.01" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TablePickupAnimationPlayRate_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ClampMin", "0.01" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GroundPickupEffectTriggerNormalizedTime_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TablePickupEffectTriggerNormalizedTime_MetaData[] = {
		{ "Category", "NPC World State|Animation|Pickup Variants" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultDropAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultHeldIdleAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultHeldWalkAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultThrowAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ItemAnimationProfiles_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachWalkAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupApproachRunAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableAdaptivePickupIK_MetaData[] = {
		{ "Category", "NPC World State|Animation|Adaptive Pickup" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AdaptivePickupIKBlendWindow_MetaData[] = {
		{ "Category", "NPC World State|Animation|Adaptive Pickup" },
		{ "ClampMax", "0.45" },
		{ "ClampMin", "0.05" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AdaptivePickupPelvisInfluence_MetaData[] = {
		{ "Category", "NPC World State|Animation|Adaptive Pickup" },
		{ "ClampMax", "0.75" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AdaptivePickupMaxPelvisOffset_MetaData[] = {
		{ "Category", "NPC World State|Animation|Adaptive Pickup" },
		{ "ClampMax", "65.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AdaptivePickupMaxArmStretch_MetaData[] = {
		{ "Category", "NPC World State|Animation|Adaptive Pickup" },
		{ "ClampMax", "1.2" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnWorldStateUpdated_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnWorldActionCompleted_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateAgentComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UNPCWorldStateAgentComponent constinit property declarations *************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PerceptionRadius;
	static void NewProp_bAutoRefresh_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoRefresh;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RefreshIntervalSeconds;
	static const UECodeGen_Private::FNamePropertyParams NewProp_HandSocket;
	static const UECodeGen_Private::FNamePropertyParams NewProp_SupportHandSocket;
	static const UECodeGen_Private::FStrPropertyParams NewProp_HandMeshNameHint;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DefaultPickupDistance;
	static void NewProp_bApproachPickupTargets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bApproachPickupTargets;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxPickupApproachDistance;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachWalkSpeed;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachRunSpeed;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachRunDistance;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachTimeoutSeconds;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachRotationSpeed;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachRepathIntervalSeconds;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachStallSeconds;
	static const UECodeGen_Private::FIntPropertyParams NewProp_PickupApproachCandidateCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaximumPickupApproachRepathFailures;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachGroundFollowInterpSpeed;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachMaximumGroundStepUp;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupApproachMaximumGroundStepDown;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupFacingYawOffsetDegrees;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ThrowImpulse;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultPickupAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_GroundPickupAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_TablePickupAnimation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TablePickupMinimumHeight;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundPickupAnimationPlayRate;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TablePickupAnimationPlayRate;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundPickupEffectTriggerNormalizedTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TablePickupEffectTriggerNormalizedTime;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultDropAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultHeldIdleAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultHeldWalkAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultThrowAnimation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ItemAnimationProfiles_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ItemAnimationProfiles;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_PickupApproachWalkAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_PickupApproachRunAnimation;
	static void NewProp_bEnableAdaptivePickupIK_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableAdaptivePickupIK;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AdaptivePickupIKBlendWindow;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AdaptivePickupPelvisInfluence;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AdaptivePickupMaxPelvisOffset;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AdaptivePickupMaxArmStretch;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnWorldStateUpdated;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnWorldActionCompleted;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UNPCWorldStateAgentComponent constinit property declarations ***************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("DropHeldActor"), .Pointer = &UNPCWorldStateAgentComponent::execDropHeldActor },
		{ .NameUTF8 = UTF8TEXT("ExecuteWorldAction"), .Pointer = &UNPCWorldStateAgentComponent::execExecuteWorldAction },
		{ .NameUTF8 = UTF8TEXT("GetCurrentHeldIdleAnimation"), .Pointer = &UNPCWorldStateAgentComponent::execGetCurrentHeldIdleAnimation },
		{ .NameUTF8 = UTF8TEXT("GetHeldActor"), .Pointer = &UNPCWorldStateAgentComponent::execGetHeldActor },
		{ .NameUTF8 = UTF8TEXT("GetHeldWalkAnimation"), .Pointer = &UNPCWorldStateAgentComponent::execGetHeldWalkAnimation },
		{ .NameUTF8 = UTF8TEXT("GetPickupAnimationForTarget"), .Pointer = &UNPCWorldStateAgentComponent::execGetPickupAnimationForTarget },
		{ .NameUTF8 = UTF8TEXT("GetVisibleWorldObjects"), .Pointer = &UNPCWorldStateAgentComponent::execGetVisibleWorldObjects },
		{ .NameUTF8 = UTF8TEXT("GetWorldStateJson"), .Pointer = &UNPCWorldStateAgentComponent::execGetWorldStateJson },
		{ .NameUTF8 = UTF8TEXT("GetWorldStateText"), .Pointer = &UNPCWorldStateAgentComponent::execGetWorldStateText },
		{ .NameUTF8 = UTF8TEXT("IsWorldActionInProgress"), .Pointer = &UNPCWorldStateAgentComponent::execIsWorldActionInProgress },
		{ .NameUTF8 = UTF8TEXT("RefreshWorldState"), .Pointer = &UNPCWorldStateAgentComponent::execRefreshWorldState },
		{ .NameUTF8 = UTF8TEXT("ResumeHeldIdleAnimation"), .Pointer = &UNPCWorldStateAgentComponent::execResumeHeldIdleAnimation },
		{ .NameUTF8 = UTF8TEXT("SuspendHeldIdleAnimation"), .Pointer = &UNPCWorldStateAgentComponent::execSuspendHeldIdleAnimation },
		{ .NameUTF8 = UTF8TEXT("TryExecuteNaturalLanguageAction"), .Pointer = &UNPCWorldStateAgentComponent::execTryExecuteNaturalLanguageAction },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_DropHeldActor, "DropHeldActor" }, // 1109412635
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_ExecuteWorldAction, "ExecuteWorldAction" }, // 1176622941
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetCurrentHeldIdleAnimation, "GetCurrentHeldIdleAnimation" }, // 788753097
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldActor, "GetHeldActor" }, // 3721205780
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetHeldWalkAnimation, "GetHeldWalkAnimation" }, // 2915049033
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetPickupAnimationForTarget, "GetPickupAnimationForTarget" }, // 1886588787
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetVisibleWorldObjects, "GetVisibleWorldObjects" }, // 3110884374
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateJson, "GetWorldStateJson" }, // 2543448436
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_GetWorldStateText, "GetWorldStateText" }, // 3277560808
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_IsWorldActionInProgress, "IsWorldActionInProgress" }, // 4066902963
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_RefreshWorldState, "RefreshWorldState" }, // 2055611542
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_ResumeHeldIdleAnimation, "ResumeHeldIdleAnimation" }, // 1113136043
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_SuspendHeldIdleAnimation, "SuspendHeldIdleAnimation" }, // 3693695782
		{ &Z_Construct_UFunction_UNPCWorldStateAgentComponent_TryExecuteNaturalLanguageAction, "TryExecuteNaturalLanguageAction" }, // 670647272
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNPCWorldStateAgentComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics

// ********** Begin Class UNPCWorldStateAgentComponent Property Definitions ************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PerceptionRadius = { "PerceptionRadius", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PerceptionRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PerceptionRadius_MetaData), NewProp_PerceptionRadius_MetaData) };
void Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bAutoRefresh_SetBit(void* Obj)
{
	((UNPCWorldStateAgentComponent*)Obj)->bAutoRefresh = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bAutoRefresh = { "bAutoRefresh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNPCWorldStateAgentComponent), &Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bAutoRefresh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoRefresh_MetaData), NewProp_bAutoRefresh_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_RefreshIntervalSeconds = { "RefreshIntervalSeconds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, RefreshIntervalSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RefreshIntervalSeconds_MetaData), NewProp_RefreshIntervalSeconds_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_HandSocket = { "HandSocket", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, HandSocket), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HandSocket_MetaData), NewProp_HandSocket_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_SupportHandSocket = { "SupportHandSocket", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, SupportHandSocket), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SupportHandSocket_MetaData), NewProp_SupportHandSocket_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_HandMeshNameHint = { "HandMeshNameHint", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, HandMeshNameHint), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HandMeshNameHint_MetaData), NewProp_HandMeshNameHint_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultPickupDistance = { "DefaultPickupDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultPickupDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultPickupDistance_MetaData), NewProp_DefaultPickupDistance_MetaData) };
void Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bApproachPickupTargets_SetBit(void* Obj)
{
	((UNPCWorldStateAgentComponent*)Obj)->bApproachPickupTargets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bApproachPickupTargets = { "bApproachPickupTargets", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNPCWorldStateAgentComponent), &Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bApproachPickupTargets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bApproachPickupTargets_MetaData), NewProp_bApproachPickupTargets_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_MaxPickupApproachDistance = { "MaxPickupApproachDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, MaxPickupApproachDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxPickupApproachDistance_MetaData), NewProp_MaxPickupApproachDistance_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachWalkSpeed = { "PickupApproachWalkSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachWalkSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachWalkSpeed_MetaData), NewProp_PickupApproachWalkSpeed_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunSpeed = { "PickupApproachRunSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachRunSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachRunSpeed_MetaData), NewProp_PickupApproachRunSpeed_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunDistance = { "PickupApproachRunDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachRunDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachRunDistance_MetaData), NewProp_PickupApproachRunDistance_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachTimeoutSeconds = { "PickupApproachTimeoutSeconds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachTimeoutSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachTimeoutSeconds_MetaData), NewProp_PickupApproachTimeoutSeconds_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRotationSpeed = { "PickupApproachRotationSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachRotationSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachRotationSpeed_MetaData), NewProp_PickupApproachRotationSpeed_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRepathIntervalSeconds = { "PickupApproachRepathIntervalSeconds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachRepathIntervalSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachRepathIntervalSeconds_MetaData), NewProp_PickupApproachRepathIntervalSeconds_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachStallSeconds = { "PickupApproachStallSeconds", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachStallSeconds), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachStallSeconds_MetaData), NewProp_PickupApproachStallSeconds_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachCandidateCount = { "PickupApproachCandidateCount", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachCandidateCount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachCandidateCount_MetaData), NewProp_PickupApproachCandidateCount_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_MaximumPickupApproachRepathFailures = { "MaximumPickupApproachRepathFailures", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, MaximumPickupApproachRepathFailures), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaximumPickupApproachRepathFailures_MetaData), NewProp_MaximumPickupApproachRepathFailures_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachGroundFollowInterpSpeed = { "PickupApproachGroundFollowInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachGroundFollowInterpSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachGroundFollowInterpSpeed_MetaData), NewProp_PickupApproachGroundFollowInterpSpeed_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachMaximumGroundStepUp = { "PickupApproachMaximumGroundStepUp", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachMaximumGroundStepUp), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachMaximumGroundStepUp_MetaData), NewProp_PickupApproachMaximumGroundStepUp_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachMaximumGroundStepDown = { "PickupApproachMaximumGroundStepDown", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachMaximumGroundStepDown), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachMaximumGroundStepDown_MetaData), NewProp_PickupApproachMaximumGroundStepDown_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupFacingYawOffsetDegrees = { "PickupFacingYawOffsetDegrees", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupFacingYawOffsetDegrees), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupFacingYawOffsetDegrees_MetaData), NewProp_PickupFacingYawOffsetDegrees_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ThrowImpulse = { "ThrowImpulse", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, ThrowImpulse), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThrowImpulse_MetaData), NewProp_ThrowImpulse_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultPickupAnimation = { "DefaultPickupAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultPickupAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultPickupAnimation_MetaData), NewProp_DefaultPickupAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupAnimation = { "GroundPickupAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, GroundPickupAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GroundPickupAnimation_MetaData), NewProp_GroundPickupAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupAnimation = { "TablePickupAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, TablePickupAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TablePickupAnimation_MetaData), NewProp_TablePickupAnimation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupMinimumHeight = { "TablePickupMinimumHeight", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, TablePickupMinimumHeight), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TablePickupMinimumHeight_MetaData), NewProp_TablePickupMinimumHeight_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupAnimationPlayRate = { "GroundPickupAnimationPlayRate", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, GroundPickupAnimationPlayRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GroundPickupAnimationPlayRate_MetaData), NewProp_GroundPickupAnimationPlayRate_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupAnimationPlayRate = { "TablePickupAnimationPlayRate", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, TablePickupAnimationPlayRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TablePickupAnimationPlayRate_MetaData), NewProp_TablePickupAnimationPlayRate_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupEffectTriggerNormalizedTime = { "GroundPickupEffectTriggerNormalizedTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, GroundPickupEffectTriggerNormalizedTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GroundPickupEffectTriggerNormalizedTime_MetaData), NewProp_GroundPickupEffectTriggerNormalizedTime_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupEffectTriggerNormalizedTime = { "TablePickupEffectTriggerNormalizedTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, TablePickupEffectTriggerNormalizedTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TablePickupEffectTriggerNormalizedTime_MetaData), NewProp_TablePickupEffectTriggerNormalizedTime_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultDropAnimation = { "DefaultDropAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultDropAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultDropAnimation_MetaData), NewProp_DefaultDropAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultHeldIdleAnimation = { "DefaultHeldIdleAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultHeldIdleAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultHeldIdleAnimation_MetaData), NewProp_DefaultHeldIdleAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultHeldWalkAnimation = { "DefaultHeldWalkAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultHeldWalkAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultHeldWalkAnimation_MetaData), NewProp_DefaultHeldWalkAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultThrowAnimation = { "DefaultThrowAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, DefaultThrowAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultThrowAnimation_MetaData), NewProp_DefaultThrowAnimation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ItemAnimationProfiles_Inner = { "ItemAnimationProfiles", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile, METADATA_PARAMS(0, nullptr) }; // 1233827653
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ItemAnimationProfiles = { "ItemAnimationProfiles", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, ItemAnimationProfiles), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ItemAnimationProfiles_MetaData), NewProp_ItemAnimationProfiles_MetaData) }; // 1233827653
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachWalkAnimation = { "PickupApproachWalkAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachWalkAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachWalkAnimation_MetaData), NewProp_PickupApproachWalkAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunAnimation = { "PickupApproachRunAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, PickupApproachRunAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupApproachRunAnimation_MetaData), NewProp_PickupApproachRunAnimation_MetaData) };
void Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bEnableAdaptivePickupIK_SetBit(void* Obj)
{
	((UNPCWorldStateAgentComponent*)Obj)->bEnableAdaptivePickupIK = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bEnableAdaptivePickupIK = { "bEnableAdaptivePickupIK", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNPCWorldStateAgentComponent), &Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bEnableAdaptivePickupIK_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableAdaptivePickupIK_MetaData), NewProp_bEnableAdaptivePickupIK_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupIKBlendWindow = { "AdaptivePickupIKBlendWindow", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, AdaptivePickupIKBlendWindow), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AdaptivePickupIKBlendWindow_MetaData), NewProp_AdaptivePickupIKBlendWindow_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupPelvisInfluence = { "AdaptivePickupPelvisInfluence", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, AdaptivePickupPelvisInfluence), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AdaptivePickupPelvisInfluence_MetaData), NewProp_AdaptivePickupPelvisInfluence_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupMaxPelvisOffset = { "AdaptivePickupMaxPelvisOffset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, AdaptivePickupMaxPelvisOffset), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AdaptivePickupMaxPelvisOffset_MetaData), NewProp_AdaptivePickupMaxPelvisOffset_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupMaxArmStretch = { "AdaptivePickupMaxArmStretch", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, AdaptivePickupMaxArmStretch), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AdaptivePickupMaxArmStretch_MetaData), NewProp_AdaptivePickupMaxArmStretch_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_OnWorldStateUpdated = { "OnWorldStateUpdated", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, OnWorldStateUpdated), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnWorldStateUpdated_MetaData), NewProp_OnWorldStateUpdated_MetaData) }; // 2641193384
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_OnWorldActionCompleted = { "OnWorldActionCompleted", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateAgentComponent, OnWorldActionCompleted), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnWorldActionCompleted_MetaData), NewProp_OnWorldActionCompleted_MetaData) }; // 1034622484
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PerceptionRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bAutoRefresh,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_RefreshIntervalSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_HandSocket,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_SupportHandSocket,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_HandMeshNameHint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultPickupDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bApproachPickupTargets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_MaxPickupApproachDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachWalkSpeed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunSpeed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachTimeoutSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRotationSpeed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRepathIntervalSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachStallSeconds,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachCandidateCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_MaximumPickupApproachRepathFailures,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachGroundFollowInterpSpeed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachMaximumGroundStepUp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachMaximumGroundStepDown,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupFacingYawOffsetDegrees,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ThrowImpulse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultPickupAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupMinimumHeight,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupAnimationPlayRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupAnimationPlayRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_GroundPickupEffectTriggerNormalizedTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_TablePickupEffectTriggerNormalizedTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultDropAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultHeldIdleAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultHeldWalkAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_DefaultThrowAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ItemAnimationProfiles_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_ItemAnimationProfiles,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachWalkAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_PickupApproachRunAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_bEnableAdaptivePickupIK,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupIKBlendWindow,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupPelvisInfluence,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupMaxPelvisOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_AdaptivePickupMaxArmStretch,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_OnWorldStateUpdated,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::NewProp_OnWorldActionCompleted,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::PropPointers) < 2048);
// ********** End Class UNPCWorldStateAgentComponent Property Definitions **************************
UObject* (*const Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::ClassParams = {
	&UNPCWorldStateAgentComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::Class_MetaDataParams)
};
void UNPCWorldStateAgentComponent::StaticRegisterNativesUNPCWorldStateAgentComponent()
{
	UClass* Class = UNPCWorldStateAgentComponent::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::Funcs));
}
UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent()
{
	if (!Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.OuterSingleton, Z_Construct_UClass_UNPCWorldStateAgentComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCWorldStateAgentComponent.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCWorldStateAgentComponent);
UNPCWorldStateAgentComponent::~UNPCWorldStateAgentComponent() {}
// ********** End Class UNPCWorldStateAgentComponent ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCWorldStateAgentComponent, UNPCWorldStateAgentComponent::StaticClass, TEXT("UNPCWorldStateAgentComponent"), &Z_Registration_Info_UClass_UNPCWorldStateAgentComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCWorldStateAgentComponent), 2733448563U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h__Script_NaturalNPCWorldState_583306579{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateAgentComponent_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
