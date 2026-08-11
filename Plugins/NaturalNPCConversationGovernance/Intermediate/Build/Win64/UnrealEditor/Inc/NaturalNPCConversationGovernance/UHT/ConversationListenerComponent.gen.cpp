// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ConversationListenerComponent.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeConversationListenerComponent() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationListenerComponent();
NATURALNPCCONVERSATIONGOVERNANCE_API UClass* Z_Construct_UClass_UConversationListenerComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_NaturalNPCConversationGovernance();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UConversationListenerComponent Function CanHearActor *********************
struct Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics
{
	struct ConversationListenerComponent_eventCanHearActor_Parms
	{
		const AActor* SoundSource;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundSource_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function CanHearActor constinit property declarations **************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundSource;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CanHearActor constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CanHearActor Property Definitions *************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_SoundSource = { "SoundSource", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationListenerComponent_eventCanHearActor_Parms, SoundSource), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundSource_MetaData), NewProp_SoundSource_MetaData) };
void Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((ConversationListenerComponent_eventCanHearActor_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ConversationListenerComponent_eventCanHearActor_Parms), &Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_SoundSource,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::PropPointers) < 2048);
// ********** End Function CanHearActor Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationListenerComponent, nullptr, "CanHearActor", 	Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::ConversationListenerComponent_eventCanHearActor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::ConversationListenerComponent_eventCanHearActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationListenerComponent_CanHearActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationListenerComponent_CanHearActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationListenerComponent::execCanHearActor)
{
	P_GET_OBJECT(AActor,Z_Param_SoundSource);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->CanHearActor(Z_Param_SoundSource);
	P_NATIVE_END;
}
// ********** End Class UConversationListenerComponent Function CanHearActor ***********************

// ********** Begin Class UConversationListenerComponent Function GetDistanceToActor ***************
struct Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics
{
	struct ConversationListenerComponent_eventGetDistanceToActor_Parms
	{
		const AActor* OtherActor;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OtherActor_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetDistanceToActor constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OtherActor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetDistanceToActor constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetDistanceToActor Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::NewProp_OtherActor = { "OtherActor", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationListenerComponent_eventGetDistanceToActor_Parms, OtherActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OtherActor_MetaData), NewProp_OtherActor_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationListenerComponent_eventGetDistanceToActor_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::NewProp_OtherActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::PropPointers) < 2048);
// ********** End Function GetDistanceToActor Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationListenerComponent, nullptr, "GetDistanceToActor", 	Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::ConversationListenerComponent_eventGetDistanceToActor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::ConversationListenerComponent_eventGetDistanceToActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationListenerComponent::execGetDistanceToActor)
{
	P_GET_OBJECT(AActor,Z_Param_OtherActor);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->GetDistanceToActor(Z_Param_OtherActor);
	P_NATIVE_END;
}
// ********** End Class UConversationListenerComponent Function GetDistanceToActor *****************

// ********** Begin Class UConversationListenerComponent Function GetResolvedDisplayName ***********
struct Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics
{
	struct ConversationListenerComponent_eventGetResolvedDisplayName_Parms
	{
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResolvedDisplayName constinit property declarations ****************
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResolvedDisplayName constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResolvedDisplayName Property Definitions ***************************
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationListenerComponent_eventGetResolvedDisplayName_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::PropPointers) < 2048);
// ********** End Function GetResolvedDisplayName Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationListenerComponent, nullptr, "GetResolvedDisplayName", 	Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::ConversationListenerComponent_eventGetResolvedDisplayName_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::ConversationListenerComponent_eventGetResolvedDisplayName_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationListenerComponent::execGetResolvedDisplayName)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=P_THIS->GetResolvedDisplayName();
	P_NATIVE_END;
}
// ********** End Class UConversationListenerComponent Function GetResolvedDisplayName *************

// ********** Begin Class UConversationListenerComponent Function GetResolvedNPCID *****************
struct Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics
{
	struct ConversationListenerComponent_eventGetResolvedNPCID_Parms
	{
		FName ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetResolvedNPCID constinit property declarations **********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetResolvedNPCID constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetResolvedNPCID Property Definitions *********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ConversationListenerComponent_eventGetResolvedNPCID_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::PropPointers) < 2048);
// ********** End Function GetResolvedNPCID Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UConversationListenerComponent, nullptr, "GetResolvedNPCID", 	Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::ConversationListenerComponent_eventGetResolvedNPCID_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::Function_MetaDataParams), Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::ConversationListenerComponent_eventGetResolvedNPCID_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UConversationListenerComponent::execGetResolvedNPCID)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FName*)Z_Param__Result=P_THIS->GetResolvedNPCID();
	P_NATIVE_END;
}
// ********** End Class UConversationListenerComponent Function GetResolvedNPCID *******************

// ********** Begin Class UConversationListenerComponent *******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UConversationListenerComponent;
UClass* UConversationListenerComponent::GetPrivateStaticClass()
{
	using TClass = UConversationListenerComponent;
	if (!Z_Registration_Info_UClass_UConversationListenerComponent.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("ConversationListenerComponent"),
			Z_Registration_Info_UClass_UConversationListenerComponent.InnerSingleton,
			StaticRegisterNativesUConversationListenerComponent,
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
	return Z_Registration_Info_UClass_UConversationListenerComponent.InnerSingleton;
}
UClass* Z_Construct_UClass_UConversationListenerComponent_NoRegister()
{
	return UConversationListenerComponent::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UConversationListenerComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "ConversationGovernance" },
		{ "IncludePath", "ConversationListenerComponent.h" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NPCID_MetaData[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ListeningRadius_MetaData[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaximumQueuedResponses_MetaData[] = {
		{ "Category", "Conversation Governance|Participation" },
		{ "ClampMax", "32" },
		{ "ClampMin", "1" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSideCommentsEnabled_MetaData[] = {
		{ "Category", "Conversation Governance|Participation" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPermissionRequestsEnabled_MetaData[] = {
		{ "Category", "Conversation Governance|Participation" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEmergencyInterruptionEnabled_MetaData[] = {
		{ "Category", "Conversation Governance|Participation" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bListenerEnabled_MetaData[] = {
		{ "Category", "Conversation Governance|Listener" },
		{ "ModuleRelativePath", "Public/ConversationListenerComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UConversationListenerComponent constinit property declarations ***********
	static const UECodeGen_Private::FNamePropertyParams NewProp_NPCID;
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ListeningRadius;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaximumQueuedResponses;
	static void NewProp_bSideCommentsEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSideCommentsEnabled;
	static void NewProp_bPermissionRequestsEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPermissionRequestsEnabled;
	static void NewProp_bEmergencyInterruptionEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEmergencyInterruptionEnabled;
	static void NewProp_bListenerEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bListenerEnabled;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UConversationListenerComponent constinit property declarations *************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("CanHearActor"), .Pointer = &UConversationListenerComponent::execCanHearActor },
		{ .NameUTF8 = UTF8TEXT("GetDistanceToActor"), .Pointer = &UConversationListenerComponent::execGetDistanceToActor },
		{ .NameUTF8 = UTF8TEXT("GetResolvedDisplayName"), .Pointer = &UConversationListenerComponent::execGetResolvedDisplayName },
		{ .NameUTF8 = UTF8TEXT("GetResolvedNPCID"), .Pointer = &UConversationListenerComponent::execGetResolvedNPCID },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UConversationListenerComponent_CanHearActor, "CanHearActor" }, // 1494799556
		{ &Z_Construct_UFunction_UConversationListenerComponent_GetDistanceToActor, "GetDistanceToActor" }, // 1369773086
		{ &Z_Construct_UFunction_UConversationListenerComponent_GetResolvedDisplayName, "GetResolvedDisplayName" }, // 1046163982
		{ &Z_Construct_UFunction_UConversationListenerComponent_GetResolvedNPCID, "GetResolvedNPCID" }, // 1048261973
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UConversationListenerComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UConversationListenerComponent_Statics

// ********** Begin Class UConversationListenerComponent Property Definitions **********************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_NPCID = { "NPCID", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationListenerComponent, NPCID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NPCID_MetaData), NewProp_NPCID_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationListenerComponent, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_ListeningRadius = { "ListeningRadius", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationListenerComponent, ListeningRadius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ListeningRadius_MetaData), NewProp_ListeningRadius_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_MaximumQueuedResponses = { "MaximumQueuedResponses", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UConversationListenerComponent, MaximumQueuedResponses), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaximumQueuedResponses_MetaData), NewProp_MaximumQueuedResponses_MetaData) };
void Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bSideCommentsEnabled_SetBit(void* Obj)
{
	((UConversationListenerComponent*)Obj)->bSideCommentsEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bSideCommentsEnabled = { "bSideCommentsEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationListenerComponent), &Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bSideCommentsEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSideCommentsEnabled_MetaData), NewProp_bSideCommentsEnabled_MetaData) };
void Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bPermissionRequestsEnabled_SetBit(void* Obj)
{
	((UConversationListenerComponent*)Obj)->bPermissionRequestsEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bPermissionRequestsEnabled = { "bPermissionRequestsEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationListenerComponent), &Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bPermissionRequestsEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPermissionRequestsEnabled_MetaData), NewProp_bPermissionRequestsEnabled_MetaData) };
void Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bEmergencyInterruptionEnabled_SetBit(void* Obj)
{
	((UConversationListenerComponent*)Obj)->bEmergencyInterruptionEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bEmergencyInterruptionEnabled = { "bEmergencyInterruptionEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationListenerComponent), &Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bEmergencyInterruptionEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEmergencyInterruptionEnabled_MetaData), NewProp_bEmergencyInterruptionEnabled_MetaData) };
void Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bListenerEnabled_SetBit(void* Obj)
{
	((UConversationListenerComponent*)Obj)->bListenerEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bListenerEnabled = { "bListenerEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UConversationListenerComponent), &Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bListenerEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bListenerEnabled_MetaData), NewProp_bListenerEnabled_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UConversationListenerComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_NPCID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_ListeningRadius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_MaximumQueuedResponses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bSideCommentsEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bPermissionRequestsEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bEmergencyInterruptionEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UConversationListenerComponent_Statics::NewProp_bListenerEnabled,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationListenerComponent_Statics::PropPointers) < 2048);
// ********** End Class UConversationListenerComponent Property Definitions ************************
UObject* (*const Z_Construct_UClass_UConversationListenerComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCConversationGovernance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationListenerComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UConversationListenerComponent_Statics::ClassParams = {
	&UConversationListenerComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UConversationListenerComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UConversationListenerComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UConversationListenerComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UConversationListenerComponent_Statics::Class_MetaDataParams)
};
void UConversationListenerComponent::StaticRegisterNativesUConversationListenerComponent()
{
	UClass* Class = UConversationListenerComponent::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UConversationListenerComponent_Statics::Funcs));
}
UClass* Z_Construct_UClass_UConversationListenerComponent()
{
	if (!Z_Registration_Info_UClass_UConversationListenerComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UConversationListenerComponent.OuterSingleton, Z_Construct_UClass_UConversationListenerComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UConversationListenerComponent.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UConversationListenerComponent);
UConversationListenerComponent::~UConversationListenerComponent() {}
// ********** End Class UConversationListenerComponent *********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h__Script_NaturalNPCConversationGovernance_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UConversationListenerComponent, UConversationListenerComponent::StaticClass, TEXT("UConversationListenerComponent"), &Z_Registration_Info_UClass_UConversationListenerComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UConversationListenerComponent), 4074846754U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h__Script_NaturalNPCConversationGovernance_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h__Script_NaturalNPCConversationGovernance_2181232069{
	TEXT("/Script/NaturalNPCConversationGovernance"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCConversationGovernance_Source_NaturalNPCConversationGovernance_Public_ConversationListenerComponent_h__Script_NaturalNPCConversationGovernance_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
