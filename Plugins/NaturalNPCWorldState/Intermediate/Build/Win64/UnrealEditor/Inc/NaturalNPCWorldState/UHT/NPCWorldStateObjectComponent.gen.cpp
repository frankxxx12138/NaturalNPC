// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldStateObjectComponent.h"
#include "NPCWorldStateTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldStateObjectComponent() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionDefinition();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UNPCWorldStateObjectComponent Function GetAvailableActions ***************
struct Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics
{
	struct NPCWorldStateObjectComponent_eventGetAvailableActions_Parms
	{
		AActor* Observer;
		TArray<FNPCWorldActionDefinition> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetAvailableActions constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Observer;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetAvailableActions constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetAvailableActions Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_Observer = { "Observer", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventGetAvailableActions_Parms, Observer), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldActionDefinition, METADATA_PARAMS(0, nullptr) }; // 1722355823
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventGetAvailableActions_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 1722355823
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_Observer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::PropPointers) < 2048);
// ********** End Function GetAvailableActions Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateObjectComponent, nullptr, "GetAvailableActions", 	Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NPCWorldStateObjectComponent_eventGetAvailableActions_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::NPCWorldStateObjectComponent_eventGetAvailableActions_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateObjectComponent::execGetAvailableActions)
{
	P_GET_OBJECT(AActor,Z_Param_Observer);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FNPCWorldActionDefinition>*)Z_Param__Result=P_THIS->GetAvailableActions(Z_Param_Observer);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateObjectComponent Function GetAvailableActions *****************

// ********** Begin Class UNPCWorldStateObjectComponent Function GetResolvedDisplayName ************
struct Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics
{
	struct NPCWorldStateObjectComponent_eventGetResolvedDisplayName_Parms
	{
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResolvedDisplayName constinit property declarations ****************
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResolvedDisplayName constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResolvedDisplayName Property Definitions ***************************
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventGetResolvedDisplayName_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::PropPointers) < 2048);
// ********** End Function GetResolvedDisplayName Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateObjectComponent, nullptr, "GetResolvedDisplayName", 	Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::NPCWorldStateObjectComponent_eventGetResolvedDisplayName_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::NPCWorldStateObjectComponent_eventGetResolvedDisplayName_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateObjectComponent::execGetResolvedDisplayName)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=P_THIS->GetResolvedDisplayName();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateObjectComponent Function GetResolvedDisplayName **************

// ********** Begin Class UNPCWorldStateObjectComponent Function GetResolvedObjectId ***************
struct Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics
{
	struct NPCWorldStateObjectComponent_eventGetResolvedObjectId_Parms
	{
		FName ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResolvedObjectId constinit property declarations *******************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResolvedObjectId constinit property declarations *********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResolvedObjectId Property Definitions ******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventGetResolvedObjectId_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::PropPointers) < 2048);
// ********** End Function GetResolvedObjectId Property Definitions ********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateObjectComponent, nullptr, "GetResolvedObjectId", 	Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::NPCWorldStateObjectComponent_eventGetResolvedObjectId_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::NPCWorldStateObjectComponent_eventGetResolvedObjectId_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateObjectComponent::execGetResolvedObjectId)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FName*)Z_Param__Result=P_THIS->GetResolvedObjectId();
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateObjectComponent Function GetResolvedObjectId *****************

// ********** Begin Class UNPCWorldStateObjectComponent Function RemoveWorldStateValue *************
struct Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics
{
	struct NPCWorldStateObjectComponent_eventRemoveWorldStateValue_Parms
	{
		FName Key;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveWorldStateValue constinit property declarations *****************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Key;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveWorldStateValue constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveWorldStateValue Property Definitions ****************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventRemoveWorldStateValue_Parms, Key), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::NewProp_Key,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::PropPointers) < 2048);
// ********** End Function RemoveWorldStateValue Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateObjectComponent, nullptr, "RemoveWorldStateValue", 	Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::NPCWorldStateObjectComponent_eventRemoveWorldStateValue_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::NPCWorldStateObjectComponent_eventRemoveWorldStateValue_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateObjectComponent::execRemoveWorldStateValue)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RemoveWorldStateValue(Z_Param_Key);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateObjectComponent Function RemoveWorldStateValue ***************

// ********** Begin Class UNPCWorldStateObjectComponent Function SetWorldStateValue ****************
struct Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics
{
	struct NPCWorldStateObjectComponent_eventSetWorldStateValue_Parms
	{
		FName Key;
		FString Value;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetWorldStateValue constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Key;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Value;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetWorldStateValue constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetWorldStateValue Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventSetWorldStateValue_Parms, Key), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateObjectComponent_eventSetWorldStateValue_Parms, Value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Value_MetaData), NewProp_Value_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NewProp_Value,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::PropPointers) < 2048);
// ********** End Function SetWorldStateValue Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateObjectComponent, nullptr, "SetWorldStateValue", 	Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NPCWorldStateObjectComponent_eventSetWorldStateValue_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::NPCWorldStateObjectComponent_eventSetWorldStateValue_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateObjectComponent::execSetWorldStateValue)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Key);
	P_GET_PROPERTY(FStrProperty,Z_Param_Value);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetWorldStateValue(Z_Param_Key,Z_Param_Value);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateObjectComponent Function SetWorldStateValue ******************

// ********** Begin Class UNPCWorldStateObjectComponent ********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCWorldStateObjectComponent;
UClass* UNPCWorldStateObjectComponent::GetPrivateStaticClass()
{
	using TClass = UNPCWorldStateObjectComponent;
	if (!Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCWorldStateObjectComponent"),
			Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.InnerSingleton,
			StaticRegisterNativesUNPCWorldStateObjectComponent,
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
	return Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent_NoRegister()
{
	return UNPCWorldStateObjectComponent::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "NPCWorldState" },
		{ "IncludePath", "NPCWorldStateObjectComponent.h" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectId_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Description_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
		{ "MultiLine", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Aliases_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actions_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bVisibleToNPCs_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAutoInferVRTemplateActions_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuntimeObjectId_MetaData[] = {
		{ "ModuleRelativePath", "Public/NPCWorldStateObjectComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UNPCWorldStateObjectComponent constinit property declarations ************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Category;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Description;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Aliases_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Aliases;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Actions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Actions;
	static const UECodeGen_Private::FStrPropertyParams NewProp_State_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_State_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_State;
	static void NewProp_bVisibleToNPCs_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bVisibleToNPCs;
	static void NewProp_bAutoInferVRTemplateActions_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoInferVRTemplateActions;
	static const UECodeGen_Private::FNamePropertyParams NewProp_RuntimeObjectId;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UNPCWorldStateObjectComponent constinit property declarations **************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetAvailableActions"), .Pointer = &UNPCWorldStateObjectComponent::execGetAvailableActions },
		{ .NameUTF8 = UTF8TEXT("GetResolvedDisplayName"), .Pointer = &UNPCWorldStateObjectComponent::execGetResolvedDisplayName },
		{ .NameUTF8 = UTF8TEXT("GetResolvedObjectId"), .Pointer = &UNPCWorldStateObjectComponent::execGetResolvedObjectId },
		{ .NameUTF8 = UTF8TEXT("RemoveWorldStateValue"), .Pointer = &UNPCWorldStateObjectComponent::execRemoveWorldStateValue },
		{ .NameUTF8 = UTF8TEXT("SetWorldStateValue"), .Pointer = &UNPCWorldStateObjectComponent::execSetWorldStateValue },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetAvailableActions, "GetAvailableActions" }, // 1423978007
		{ &Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedDisplayName, "GetResolvedDisplayName" }, // 1497707114
		{ &Z_Construct_UFunction_UNPCWorldStateObjectComponent_GetResolvedObjectId, "GetResolvedObjectId" }, // 1695986460
		{ &Z_Construct_UFunction_UNPCWorldStateObjectComponent_RemoveWorldStateValue, "RemoveWorldStateValue" }, // 1986861033
		{ &Z_Construct_UFunction_UNPCWorldStateObjectComponent_SetWorldStateValue, "SetWorldStateValue" }, // 1700297163
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNPCWorldStateObjectComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics

// ********** Begin Class UNPCWorldStateObjectComponent Property Definitions ***********************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, ObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectId_MetaData), NewProp_ObjectId_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Description = { "Description", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, Description), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Description_MetaData), NewProp_Description_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Aliases_Inner = { "Aliases", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Aliases = { "Aliases", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, Aliases), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Aliases_MetaData), NewProp_Aliases_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Actions_Inner = { "Actions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldActionDefinition, METADATA_PARAMS(0, nullptr) }; // 1722355823
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Actions = { "Actions", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, Actions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actions_MetaData), NewProp_Actions_MetaData) }; // 1722355823
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State_ValueProp = { "State", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State_Key_KeyProp = { "State_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, State), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_State_MetaData), NewProp_State_MetaData) };
void Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bVisibleToNPCs_SetBit(void* Obj)
{
	((UNPCWorldStateObjectComponent*)Obj)->bVisibleToNPCs = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bVisibleToNPCs = { "bVisibleToNPCs", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNPCWorldStateObjectComponent), &Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bVisibleToNPCs_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bVisibleToNPCs_MetaData), NewProp_bVisibleToNPCs_MetaData) };
void Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bAutoInferVRTemplateActions_SetBit(void* Obj)
{
	((UNPCWorldStateObjectComponent*)Obj)->bAutoInferVRTemplateActions = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bAutoInferVRTemplateActions = { "bAutoInferVRTemplateActions", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UNPCWorldStateObjectComponent), &Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bAutoInferVRTemplateActions_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAutoInferVRTemplateActions_MetaData), NewProp_bAutoInferVRTemplateActions_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_RuntimeObjectId = { "RuntimeObjectId", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UNPCWorldStateObjectComponent, RuntimeObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuntimeObjectId_MetaData), NewProp_RuntimeObjectId_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Description,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Aliases_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Aliases,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Actions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_Actions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_State,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bVisibleToNPCs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_bAutoInferVRTemplateActions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::NewProp_RuntimeObjectId,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::PropPointers) < 2048);
// ********** End Class UNPCWorldStateObjectComponent Property Definitions *************************
UObject* (*const Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::ClassParams = {
	&UNPCWorldStateObjectComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::Class_MetaDataParams)
};
void UNPCWorldStateObjectComponent::StaticRegisterNativesUNPCWorldStateObjectComponent()
{
	UClass* Class = UNPCWorldStateObjectComponent::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::Funcs));
}
UClass* Z_Construct_UClass_UNPCWorldStateObjectComponent()
{
	if (!Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.OuterSingleton, Z_Construct_UClass_UNPCWorldStateObjectComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCWorldStateObjectComponent.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCWorldStateObjectComponent);
UNPCWorldStateObjectComponent::~UNPCWorldStateObjectComponent() {}
// ********** End Class UNPCWorldStateObjectComponent **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCWorldStateObjectComponent, UNPCWorldStateObjectComponent::StaticClass, TEXT("UNPCWorldStateObjectComponent"), &Z_Registration_Info_UClass_UNPCWorldStateObjectComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCWorldStateObjectComponent), 2168177262U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h__Script_NaturalNPCWorldState_494621587{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateObjectComponent_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
