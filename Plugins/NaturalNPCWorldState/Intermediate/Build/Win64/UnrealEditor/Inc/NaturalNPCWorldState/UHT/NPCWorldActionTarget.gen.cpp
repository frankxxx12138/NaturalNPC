// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldActionTarget.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldActionTarget() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldActionTarget();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCWorldActionTarget_NoRegister();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Interface UNPCWorldActionTarget Function ExecuteNPCWorldAction *****************
struct NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms
{
	AActor* NPC;
	FName ActionId;
	FString Parameters;
	FString ResultMessage;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms()
		: ReturnValue(false)
	{
	}
};
bool INPCWorldActionTarget::ExecuteNPCWorldAction(AActor* NPC, FName ActionId, const FString& Parameters, FString& ResultMessage)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_ExecuteNPCWorldAction instead.");
	NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms Parms;
	return Parms.ReturnValue;
}
static FName NAME_UNPCWorldActionTarget_ExecuteNPCWorldAction = FName(TEXT("ExecuteNPCWorldAction"));
bool INPCWorldActionTarget::Execute_ExecuteNPCWorldAction(UObject* O, AActor* NPC, FName ActionId, const FString& Parameters, FString& ResultMessage)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UNPCWorldActionTarget::StaticClass()));
	NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UNPCWorldActionTarget_ExecuteNPCWorldAction);
	if (Func)
	{
		Parms.NPC=std::move(NPC);
		Parms.ActionId=std::move(ActionId);
		Parms.Parameters=std::move(Parameters);
		Parms.ResultMessage=std::move(ResultMessage);
		O->ProcessEvent(Func, &Parms);
		ResultMessage=std::move(Parms.ResultMessage);
	}
	else if (auto I = (INPCWorldActionTarget*)(O->GetNativeInterfaceAddress(UNPCWorldActionTarget::StaticClass())))
	{
		Parms.ReturnValue = I->ExecuteNPCWorldAction_Implementation(NPC,ActionId,Parameters,ResultMessage);
	}
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldActionTarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameters_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ExecuteNPCWorldAction constinit property declarations *****************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NPC;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActionId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Parameters;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ResultMessage;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ExecuteNPCWorldAction constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ExecuteNPCWorldAction Property Definitions ****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_NPC = { "NPC", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms, NPC), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ActionId = { "ActionId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms, ActionId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_Parameters = { "Parameters", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms, Parameters), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameters_MetaData), NewProp_Parameters_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ResultMessage = { "ResultMessage", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms, ResultMessage), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms), &Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_NPC,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ActionId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_Parameters,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ResultMessage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::PropPointers) < 2048);
// ********** End Function ExecuteNPCWorldAction Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UNPCWorldActionTarget, nullptr, "ExecuteNPCWorldAction", 	Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::PropPointers), 
sizeof(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(NPCWorldActionTarget_eventExecuteNPCWorldAction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(INPCWorldActionTarget::execExecuteNPCWorldAction)
{
	P_GET_OBJECT(AActor,Z_Param_NPC);
	P_GET_PROPERTY(FNameProperty,Z_Param_ActionId);
	P_GET_PROPERTY(FStrProperty,Z_Param_Parameters);
	P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_ResultMessage);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ExecuteNPCWorldAction_Implementation(Z_Param_NPC,Z_Param_ActionId,Z_Param_Parameters,Z_Param_Out_ResultMessage);
	P_NATIVE_END;
}
// ********** End Interface UNPCWorldActionTarget Function ExecuteNPCWorldAction *******************

// ********** Begin Interface UNPCWorldActionTarget ************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCWorldActionTarget;
UClass* UNPCWorldActionTarget::GetPrivateStaticClass()
{
	using TClass = UNPCWorldActionTarget;
	if (!Z_Registration_Info_UClass_UNPCWorldActionTarget.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCWorldActionTarget"),
			Z_Registration_Info_UClass_UNPCWorldActionTarget.InnerSingleton,
			StaticRegisterNativesUNPCWorldActionTarget,
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
	return Z_Registration_Info_UClass_UNPCWorldActionTarget.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCWorldActionTarget_NoRegister()
{
	return UNPCWorldActionTarget::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCWorldActionTarget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NPCWorldActionTarget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Interface UNPCWorldActionTarget constinit property declarations ****************
// ********** End Interface UNPCWorldActionTarget constinit property declarations ******************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ExecuteNPCWorldAction"), .Pointer = &INPCWorldActionTarget::execExecuteNPCWorldAction },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UNPCWorldActionTarget_ExecuteNPCWorldAction, "ExecuteNPCWorldAction" }, // 3691686576
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<INPCWorldActionTarget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCWorldActionTarget_Statics
UObject* (*const Z_Construct_UClass_UNPCWorldActionTarget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldActionTarget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCWorldActionTarget_Statics::ClassParams = {
	&UNPCWorldActionTarget::StaticClass,
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
	0x001040A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCWorldActionTarget_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCWorldActionTarget_Statics::Class_MetaDataParams)
};
void UNPCWorldActionTarget::StaticRegisterNativesUNPCWorldActionTarget()
{
	UClass* Class = UNPCWorldActionTarget::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UNPCWorldActionTarget_Statics::Funcs));
}
UClass* Z_Construct_UClass_UNPCWorldActionTarget()
{
	if (!Z_Registration_Info_UClass_UNPCWorldActionTarget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCWorldActionTarget.OuterSingleton, Z_Construct_UClass_UNPCWorldActionTarget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCWorldActionTarget.OuterSingleton;
}
UNPCWorldActionTarget::UNPCWorldActionTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCWorldActionTarget);
// ********** End Interface UNPCWorldActionTarget **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCWorldActionTarget, UNPCWorldActionTarget::StaticClass, TEXT("UNPCWorldActionTarget"), &Z_Registration_Info_UClass_UNPCWorldActionTarget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCWorldActionTarget), 728040985U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h__Script_NaturalNPCWorldState_3592148637{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldActionTarget_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
