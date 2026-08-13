// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNaturalNPCWorldState_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");	NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature();
	NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature();
	NATURALNPCWORLDSTATE_API UFunction* Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_NaturalNPCWorldState;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState()
	{
		if (!Z_Registration_Info_UPackage__Script_NaturalNPCWorldState.OuterSingleton)
		{
		static UObject* (*const SingletonFuncArray[])() = {
			(UObject* (*)())Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldActionCompletedEvent__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldObjectEvent__DelegateSignature,
			(UObject* (*)())Z_Construct_UDelegateFunction_NaturalNPCWorldState_NPCWorldStateUpdatedEvent__DelegateSignature,
		};
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/NaturalNPCWorldState",
			SingletonFuncArray,
			UE_ARRAY_COUNT(SingletonFuncArray),
			PKG_CompiledIn | 0x00000000,
			0x364A39C9,
			0x4938EB64,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_NaturalNPCWorldState.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_NaturalNPCWorldState.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_NaturalNPCWorldState(Z_Construct_UPackage__Script_NaturalNPCWorldState, TEXT("/Script/NaturalNPCWorldState"), Z_Registration_Info_UPackage__Script_NaturalNPCWorldState, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x364A39C9, 0x4938EB64));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
