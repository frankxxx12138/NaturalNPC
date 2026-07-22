// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldStateSubsystem.h"
#include "NPCWorldStateTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldStateSubsystem() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UWorldSubsystem();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateSubsystem();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister();
NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldObjectState();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FNPCWorldObjectEvent **************************************************
struct Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms
	{
		FName ObjectId;
		AActor* Actor;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FNPCWorldObjectEvent constinit property declarations ******************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FNPCWorldObjectEvent constinit property declarations ********************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FNPCWorldObjectEvent Property Definitions *****************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms, ObjectId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::NewProp_Actor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FNPCWorldObjectEvent Property Definitions *******************************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_NaturalNPCWorldState, nullptr, "NPCWorldObjectEvent__DelegateSignature", 	Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::_Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FNPCWorldObjectEvent_DelegateWrapper(const FMulticastScriptDelegate& NPCWorldObjectEvent, FName ObjectId, AActor* Actor)
{
	struct _Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms
	{
		FName ObjectId;
		AActor* Actor;
	};
	_Script_NaturalNPCWorldState_eventNPCWorldObjectEvent_Parms Parms;
	Parms.ObjectId=ObjectId;
	Parms.Actor=Actor;
	NPCWorldObjectEvent.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FNPCWorldObjectEvent ****************************************************

// ********** Begin Class UNPCWorldStateSubsystem Function BuildWorldStateJson *********************
struct Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics
{
	struct NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms
	{
		AActor* Observer;
		float Radius;
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "CPP_Default_Radius", "0.000000" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function BuildWorldStateJson constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Observer;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function BuildWorldStateJson constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function BuildWorldStateJson Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_Observer = { "Observer", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms, Observer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms, Radius), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_Observer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_Radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::PropPointers) < 2048);
// ********** End Function BuildWorldStateJson Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateSubsystem, nullptr, "BuildWorldStateJson", 	Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::NPCWorldStateSubsystem_eventBuildWorldStateJson_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateSubsystem::execBuildWorldStateJson)
{
	P_GET_OBJECT(AActor,Z_Param_Observer);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Radius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=P_THIS->BuildWorldStateJson(Z_Param_Observer,Z_Param_Radius);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateSubsystem Function BuildWorldStateJson ***********************

// ********** Begin Class UNPCWorldStateSubsystem Function BuildWorldStateText *********************
struct Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics
{
	struct NPCWorldStateSubsystem_eventBuildWorldStateText_Parms
	{
		AActor* Observer;
		float Radius;
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "CPP_Default_Radius", "0.000000" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function BuildWorldStateText constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Observer;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function BuildWorldStateText constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function BuildWorldStateText Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_Observer = { "Observer", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateText_Parms, Observer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateText_Parms, Radius), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventBuildWorldStateText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_Observer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_Radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::PropPointers) < 2048);
// ********** End Function BuildWorldStateText Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateSubsystem, nullptr, "BuildWorldStateText", 	Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NPCWorldStateSubsystem_eventBuildWorldStateText_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::NPCWorldStateSubsystem_eventBuildWorldStateText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateSubsystem::execBuildWorldStateText)
{
	P_GET_OBJECT(AActor,Z_Param_Observer);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Radius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=P_THIS->BuildWorldStateText(Z_Param_Observer,Z_Param_Radius);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateSubsystem Function BuildWorldStateText ***********************

// ********** Begin Class UNPCWorldStateSubsystem Function FindWorldObject *************************
struct Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics
{
	struct NPCWorldStateSubsystem_eventFindWorldObject_Parms
	{
		FName ObjectId;
		UNPCWorldStateObjectComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function FindWorldObject constinit property declarations ***********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function FindWorldObject constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function FindWorldObject Property Definitions **********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventFindWorldObject_Parms, ObjectId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventFindWorldObject_Parms, ReturnValue), Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::PropPointers) < 2048);
// ********** End Function FindWorldObject Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateSubsystem, nullptr, "FindWorldObject", 	Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NPCWorldStateSubsystem_eventFindWorldObject_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::NPCWorldStateSubsystem_eventFindWorldObject_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateSubsystem::execFindWorldObject)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_ObjectId);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UNPCWorldStateObjectComponent**)Z_Param__Result=P_THIS->FindWorldObject(Z_Param_ObjectId);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateSubsystem Function FindWorldObject ***************************

// ********** Begin Class UNPCWorldStateSubsystem Function GetWorldState ***************************
struct Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics
{
	struct NPCWorldStateSubsystem_eventGetWorldState_Parms
	{
		AActor* Observer;
		float Radius;
		TArray<FNPCWorldObjectState> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "CPP_Default_Radius", "0.000000" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetWorldState constinit property declarations *************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Observer;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetWorldState constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetWorldState Property Definitions ************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_Observer = { "Observer", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventGetWorldState_Parms, Observer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventGetWorldState_Parms, Radius), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldObjectState, METADATA_PARAMS(0, nullptr) }; // 871788507
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateSubsystem_eventGetWorldState_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 871788507
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_Observer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_Radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::PropPointers) < 2048);
// ********** End Function GetWorldState Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateSubsystem, nullptr, "GetWorldState", 	Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NPCWorldStateSubsystem_eventGetWorldState_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::NPCWorldStateSubsystem_eventGetWorldState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateSubsystem::execGetWorldState)
{
	P_GET_OBJECT(AActor,Z_Param_Observer);
	P_GET_PROPERTY(FFloatProperty,Z_Param_Radius);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FNPCWorldObjectState>*)Z_Param__Result=P_THIS->GetWorldState(Z_Param_Observer,Z_Param_Radius);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateSubsystem Function GetWorldState *****************************

// ********** Begin Class UNPCWorldStateSubsystem Function RefreshAutoDiscovery ********************
struct Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RefreshAutoDiscovery constinit property declarations ******************
// ********** End Function RefreshAutoDiscovery constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateSubsystem, nullptr, "RefreshAutoDiscovery", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateSubsystem::execRefreshAutoDiscovery)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RefreshAutoDiscovery();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateSubsystem Function RefreshAutoDiscovery **********************

// ********** Begin Class UNPCWorldStateSubsystem **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCWorldStateSubsystem;
UClass* UNPCWorldStateSubsystem::GetPrivateStaticClass()
{
	using TClass = UNPCWorldStateSubsystem;
	if (!Z_Registration_Info_UClass_UNPCWorldStateSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCWorldStateSubsystem"),
			Z_Registration_Info_UClass_UNPCWorldStateSubsystem.InnerSingleton,
			StaticRegisterNativesUNPCWorldStateSubsystem,
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
	return Z_Registration_Info_UClass_UNPCWorldStateSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister()
{
	return UNPCWorldStateSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCWorldStateSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "NPCWorldStateSubsystem.h" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnWorldObjectAdded_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnWorldObjectRemoved_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnWorldObjectChanged_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UNPCWorldStateSubsystem constinit property declarations ******************
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnWorldObjectAdded;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnWorldObjectRemoved;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnWorldObjectChanged;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UNPCWorldStateSubsystem constinit property declarations ********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("BuildWorldStateJson"), .Pointer = &UNPCWorldStateSubsystem::execBuildWorldStateJson },
		{ .NameUTF8 = UTF8TEXT("BuildWorldStateText"), .Pointer = &UNPCWorldStateSubsystem::execBuildWorldStateText },
		{ .NameUTF8 = UTF8TEXT("FindWorldObject"), .Pointer = &UNPCWorldStateSubsystem::execFindWorldObject },
		{ .NameUTF8 = UTF8TEXT("GetWorldState"), .Pointer = &UNPCWorldStateSubsystem::execGetWorldState },
		{ .NameUTF8 = UTF8TEXT("RefreshAutoDiscovery"), .Pointer = &UNPCWorldStateSubsystem::execRefreshAutoDiscovery },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateJson, "BuildWorldStateJson" }, // 1799589214
		{ &Z_Construct_UFunction_UNPCWorldStateSubsystem_BuildWorldStateText, "BuildWorldStateText" }, // 2620436416
		{ &Z_Construct_UFunction_UNPCWorldStateSubsystem_FindWorldObject, "FindWorldObject" }, // 1045593000
		{ &Z_Construct_UFunction_UNPCWorldStateSubsystem_GetWorldState, "GetWorldState" }, // 1975426273
		{ &Z_Construct_UFunction_UNPCWorldStateSubsystem_RefreshAutoDiscovery, "RefreshAutoDiscovery" }, // 2106262012
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNPCWorldStateSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCWorldStateSubsystem_Statics

// ********** Begin Class UNPCWorldStateSubsystem Property Definitions *****************************
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectAdded = { "OnWorldObjectAdded", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateSubsystem, OnWorldObjectAdded), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnWorldObjectAdded_MetaData), NewProp_OnWorldObjectAdded_MetaData) }; // 66872457
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectRemoved = { "OnWorldObjectRemoved", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateSubsystem, OnWorldObjectRemoved), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnWorldObjectRemoved_MetaData), NewProp_OnWorldObjectRemoved_MetaData) }; // 66872457
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectChanged = { "OnWorldObjectChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateSubsystem, OnWorldObjectChanged), Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnWorldObjectChanged_MetaData), NewProp_OnWorldObjectChanged_MetaData) }; // 66872457
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectAdded,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectRemoved,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::NewProp_OnWorldObjectChanged,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::PropPointers) < 2048);
// ********** End Class UNPCWorldStateSubsystem Property Definitions *******************************
UObject* (*const Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWorldSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::ClassParams = {
	&UNPCWorldStateSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::Class_MetaDataParams)
};
void UNPCWorldStateSubsystem::StaticRegisterNativesUNPCWorldStateSubsystem()
{
	UClass* Class = UNPCWorldStateSubsystem::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::Funcs));
}
UClass* Z_Construct_UClass_UNPCWorldStateSubsystem()
{
	if (!Z_Registration_Info_UClass_UNPCWorldStateSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCWorldStateSubsystem.OuterSingleton, Z_Construct_UClass_UNPCWorldStateSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCWorldStateSubsystem.OuterSingleton;
}
UNPCWorldStateSubsystem::UNPCWorldStateSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCWorldStateSubsystem);
UNPCWorldStateSubsystem::~UNPCWorldStateSubsystem() {}
// ********** End Class UNPCWorldStateSubsystem ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCWorldStateSubsystem, UNPCWorldStateSubsystem::StaticClass, TEXT("UNPCWorldStateSubsystem"), &Z_Registration_Info_UClass_UNPCWorldStateSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCWorldStateSubsystem), 1998658707U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h__Script_NaturalNPCWorldState_1385695653{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateSubsystem_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
