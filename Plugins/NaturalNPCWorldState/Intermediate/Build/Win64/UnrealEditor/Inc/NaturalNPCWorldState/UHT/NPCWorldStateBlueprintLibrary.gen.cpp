// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldStateBlueprintLibrary.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldStateBlueprintLibrary() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateBlueprintLibrary();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_NoRegister();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UNPCWorldStateBlueprintLibrary Function EnsureNPCWorldStateAgent *********
struct Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics
{
	struct NPCWorldStateBlueprintLibrary_eventEnsureNPCWorldStateAgent_Parms
	{
		AActor* Actor;
		UNPCWorldStateAgentComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateBlueprintLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function EnsureNPCWorldStateAgent constinit property declarations **************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function EnsureNPCWorldStateAgent constinit property declarations ****************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function EnsureNPCWorldStateAgent Property Definitions *************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateBlueprintLibrary_eventEnsureNPCWorldStateAgent_Parms, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateBlueprintLibrary_eventEnsureNPCWorldStateAgent_Parms, ReturnValue), Z_Construct_UClass_UNPCWorldStateAgentComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NewProp_Actor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::PropPointers) < 2048);
// ********** End Function EnsureNPCWorldStateAgent Property Definitions ***************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateBlueprintLibrary, nullptr, "EnsureNPCWorldStateAgent", 	Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NPCWorldStateBlueprintLibrary_eventEnsureNPCWorldStateAgent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::NPCWorldStateBlueprintLibrary_eventEnsureNPCWorldStateAgent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateBlueprintLibrary::execEnsureNPCWorldStateAgent)
{
	P_GET_OBJECT(AActor,Z_Param_Actor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UNPCWorldStateAgentComponent**)Z_Param__Result=UNPCWorldStateBlueprintLibrary::EnsureNPCWorldStateAgent(Z_Param_Actor);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateBlueprintLibrary Function EnsureNPCWorldStateAgent ***********

// ********** Begin Class UNPCWorldStateBlueprintLibrary Function GetNPCWorldStateSubsystem ********
struct Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics
{
	struct NPCWorldStateBlueprintLibrary_eventGetNPCWorldStateSubsystem_Parms
	{
		const UObject* WorldContextObject;
		UNPCWorldStateSubsystem* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateBlueprintLibrary.h" },
		{ "WorldContext", "WorldContextObject" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldContextObject_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetNPCWorldStateSubsystem constinit property declarations *************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetNPCWorldStateSubsystem constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetNPCWorldStateSubsystem Property Definitions ************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateBlueprintLibrary_eventGetNPCWorldStateSubsystem_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldContextObject_MetaData), NewProp_WorldContextObject_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldStateBlueprintLibrary_eventGetNPCWorldStateSubsystem_Parms, ReturnValue), Z_Construct_UClass_UNPCWorldStateSubsystem_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NewProp_WorldContextObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::PropPointers) < 2048);
// ********** End Function GetNPCWorldStateSubsystem Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldStateBlueprintLibrary, nullptr, "GetNPCWorldStateSubsystem", 	Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NPCWorldStateBlueprintLibrary_eventGetNPCWorldStateSubsystem_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::NPCWorldStateBlueprintLibrary_eventGetNPCWorldStateSubsystem_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UNPCWorldStateBlueprintLibrary::execGetNPCWorldStateSubsystem)
{
	P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UNPCWorldStateSubsystem**)Z_Param__Result=UNPCWorldStateBlueprintLibrary::GetNPCWorldStateSubsystem(Z_Param_WorldContextObject);
	P_NATIVE_END;
}
// ********** End Class UNPCWorldStateBlueprintLibrary Function GetNPCWorldStateSubsystem **********

// ********** Begin Class UNPCWorldStateBlueprintLibrary *******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary;
UClass* UNPCWorldStateBlueprintLibrary::GetPrivateStaticClass()
{
	using TClass = UNPCWorldStateBlueprintLibrary;
	if (!Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCWorldStateBlueprintLibrary"),
			Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.InnerSingleton,
			StaticRegisterNativesUNPCWorldStateBlueprintLibrary,
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
	return Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_NoRegister()
{
	return UNPCWorldStateBlueprintLibrary::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "NPCWorldStateBlueprintLibrary.h" },
		{ "ModuleRelativePath", "Public/NPCWorldStateBlueprintLibrary.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UNPCWorldStateBlueprintLibrary constinit property declarations ***********
// ********** End Class UNPCWorldStateBlueprintLibrary constinit property declarations *************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("EnsureNPCWorldStateAgent"), .Pointer = &UNPCWorldStateBlueprintLibrary::execEnsureNPCWorldStateAgent },
		{ .NameUTF8 = UTF8TEXT("GetNPCWorldStateSubsystem"), .Pointer = &UNPCWorldStateBlueprintLibrary::execGetNPCWorldStateSubsystem },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_EnsureNPCWorldStateAgent, "EnsureNPCWorldStateAgent" }, // 1913066607
		{ &Z_Construct_UFunction_UNPCWorldStateBlueprintLibrary_GetNPCWorldStateSubsystem, "GetNPCWorldStateSubsystem" }, // 304764971
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNPCWorldStateBlueprintLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics
UObject* (*const Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::ClassParams = {
	&UNPCWorldStateBlueprintLibrary::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::Class_MetaDataParams)
};
void UNPCWorldStateBlueprintLibrary::StaticRegisterNativesUNPCWorldStateBlueprintLibrary()
{
	UClass* Class = UNPCWorldStateBlueprintLibrary::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::Funcs));
}
UClass* Z_Construct_UClass_UNPCWorldStateBlueprintLibrary()
{
	if (!Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.OuterSingleton, Z_Construct_UClass_UNPCWorldStateBlueprintLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary.OuterSingleton;
}
UNPCWorldStateBlueprintLibrary::UNPCWorldStateBlueprintLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCWorldStateBlueprintLibrary);
UNPCWorldStateBlueprintLibrary::~UNPCWorldStateBlueprintLibrary() {}
// ********** End Class UNPCWorldStateBlueprintLibrary *********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCWorldStateBlueprintLibrary, UNPCWorldStateBlueprintLibrary::StaticClass, TEXT("UNPCWorldStateBlueprintLibrary"), &Z_Registration_Info_UClass_UNPCWorldStateBlueprintLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCWorldStateBlueprintLibrary), 2153291904U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h__Script_NaturalNPCWorldState_3275997128{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateBlueprintLibrary_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
