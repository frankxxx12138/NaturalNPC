// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCAdaptivePickupAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCAdaptivePickupAnimInstance() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UAnimSingleNodeInstance();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCAdaptivePickupAnimInstance();
NATURALNPCWORLDSTATE_API UClass* Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_NoRegister();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UNPCAdaptivePickupAnimInstance *******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance;
UClass* UNPCAdaptivePickupAnimInstance::GetPrivateStaticClass()
{
	using TClass = UNPCAdaptivePickupAnimInstance;
	if (!Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("NPCAdaptivePickupAnimInstance"),
			Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.InnerSingleton,
			StaticRegisterNativesUNPCAdaptivePickupAnimInstance,
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
	return Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.InnerSingleton;
}
UClass* Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_NoRegister()
{
	return UNPCAdaptivePickupAnimInstance::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Plays a pickup sequence while adapting the contact pose to an object in the\n * world. The proxy keeps the feet planted, adjusts the pelvis and spine, then\n * solves the arm chains against the requested pickup point.\n */" },
#endif
		{ "HideCategories", "AnimInstance" },
		{ "IncludePath", "NPCAdaptivePickupAnimInstance.h" },
		{ "ModuleRelativePath", "Public/NPCAdaptivePickupAnimInstance.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Plays a pickup sequence while adapting the contact pose to an object in the\nworld. The proxy keeps the feet planted, adjusts the pelvis and spine, then\nsolves the arm chains against the requested pickup point." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UNPCAdaptivePickupAnimInstance constinit property declarations ***********
// ********** End Class UNPCAdaptivePickupAnimInstance constinit property declarations *************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNPCAdaptivePickupAnimInstance>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics
UObject* (*const Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAnimSingleNodeInstance,
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::ClassParams = {
	&UNPCAdaptivePickupAnimInstance::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::Class_MetaDataParams)
};
void UNPCAdaptivePickupAnimInstance::StaticRegisterNativesUNPCAdaptivePickupAnimInstance()
{
}
UClass* Z_Construct_UClass_UNPCAdaptivePickupAnimInstance()
{
	if (!Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.OuterSingleton, Z_Construct_UClass_UNPCAdaptivePickupAnimInstance_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance.OuterSingleton;
}
UNPCAdaptivePickupAnimInstance::UNPCAdaptivePickupAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UNPCAdaptivePickupAnimInstance);
UNPCAdaptivePickupAnimInstance::~UNPCAdaptivePickupAnimInstance() {}
// ********** End Class UNPCAdaptivePickupAnimInstance *********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCAdaptivePickupAnimInstance_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNPCAdaptivePickupAnimInstance, UNPCAdaptivePickupAnimInstance::StaticClass, TEXT("UNPCAdaptivePickupAnimInstance"), &Z_Registration_Info_UClass_UNPCAdaptivePickupAnimInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNPCAdaptivePickupAnimInstance), 4253817989U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCAdaptivePickupAnimInstance_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCAdaptivePickupAnimInstance_h__Script_NaturalNPCWorldState_1013476465{
	TEXT("/Script/NaturalNPCWorldState"),
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCAdaptivePickupAnimInstance_h__Script_NaturalNPCWorldState_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCAdaptivePickupAnimInstance_h__Script_NaturalNPCWorldState_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
