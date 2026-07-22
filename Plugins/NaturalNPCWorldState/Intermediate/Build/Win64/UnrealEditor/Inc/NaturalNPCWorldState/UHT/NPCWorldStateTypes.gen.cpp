// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NPCWorldStateTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeNPCWorldStateTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UAnimSequence_NoRegister();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionDefinition();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionResult();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile();
NATURALNPCWORLDSTATE_API UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldObjectState();
UPackage* Z_Construct_UPackage__Script_NaturalNPCWorldState();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FNPCWorldItemAnimationProfile *************************************
struct Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FNPCWorldItemAnimationProfile); }
	static inline consteval int16 GetStructAlignment() { return alignof(FNPCWorldItemAnimationProfile); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HeldIdleAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DropAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThrowAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PickupEffectTriggerNormalizedTime_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "-1.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCenterObjectBetweenHands_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FNPCWorldItemAnimationProfile constinit property declarations *****
	static const UECodeGen_Private::FNamePropertyParams NewProp_Category;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_PickupAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_HeldIdleAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DropAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ThrowAnimation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PickupEffectTriggerNormalizedTime;
	static void NewProp_bCenterObjectBetweenHands_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCenterObjectBetweenHands;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FNPCWorldItemAnimationProfile constinit property declarations *******
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNPCWorldItemAnimationProfile>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile;
class UScriptStruct* FNPCWorldItemAnimationProfile::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile, (UObject*)Z_Construct_UPackage__Script_NaturalNPCWorldState(), TEXT("NPCWorldItemAnimationProfile"));
	}
	return Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.OuterSingleton;
	}

// ********** Begin ScriptStruct FNPCWorldItemAnimationProfile Property Definitions ****************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_PickupAnimation = { "PickupAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, PickupAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupAnimation_MetaData), NewProp_PickupAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_HeldIdleAnimation = { "HeldIdleAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, HeldIdleAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HeldIdleAnimation_MetaData), NewProp_HeldIdleAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_DropAnimation = { "DropAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, DropAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DropAnimation_MetaData), NewProp_DropAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_ThrowAnimation = { "ThrowAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, ThrowAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThrowAnimation_MetaData), NewProp_ThrowAnimation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_PickupEffectTriggerNormalizedTime = { "PickupEffectTriggerNormalizedTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldItemAnimationProfile, PickupEffectTriggerNormalizedTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PickupEffectTriggerNormalizedTime_MetaData), NewProp_PickupEffectTriggerNormalizedTime_MetaData) };
void Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_bCenterObjectBetweenHands_SetBit(void* Obj)
{
	((FNPCWorldItemAnimationProfile*)Obj)->bCenterObjectBetweenHands = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_bCenterObjectBetweenHands = { "bCenterObjectBetweenHands", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FNPCWorldItemAnimationProfile), &Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_bCenterObjectBetweenHands_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCenterObjectBetweenHands_MetaData), NewProp_bCenterObjectBetweenHands_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_PickupAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_HeldIdleAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_DropAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_ThrowAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_PickupEffectTriggerNormalizedTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewProp_bCenterObjectBetweenHands,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FNPCWorldItemAnimationProfile Property Definitions ******************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
	nullptr,
	&NewStructOps,
	"NPCWorldItemAnimationProfile",
	Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::PropPointers),
	sizeof(FNPCWorldItemAnimationProfile),
	alignof(FNPCWorldItemAnimationProfile),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.InnerSingleton, Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile.InnerSingleton);
}
// ********** End ScriptStruct FNPCWorldItemAnimationProfile ***************************************

// ********** Begin ScriptStruct FNPCWorldActionDefinition *****************************************
struct Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FNPCWorldActionDefinition); }
	static inline consteval int16 GetStructAlignment() { return alignof(FNPCWorldActionDefinition); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActionId_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Description_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
		{ "MultiLine", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UtteranceHints_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxDistance_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachSocket_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachLocationOffset_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttachRotationOffset_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActionAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HeldIdleAnimation_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AnimationPlayRate_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ClampMin", "0.01" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EffectTriggerNormalizedTime_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPlayAnimationInReverse_MetaData[] = {
		{ "Category", "NPC World State|Animation" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FNPCWorldActionDefinition constinit property declarations *********
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActionId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Description;
	static const UECodeGen_Private::FStrPropertyParams NewProp_UtteranceHints_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_UtteranceHints;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxDistance;
	static const UECodeGen_Private::FNamePropertyParams NewProp_AttachSocket;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AttachLocationOffset;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AttachRotationOffset;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ActionAnimation;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_HeldIdleAnimation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_AnimationPlayRate;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_EffectTriggerNormalizedTime;
	static void NewProp_bPlayAnimationInReverse_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPlayAnimationInReverse;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FNPCWorldActionDefinition constinit property declarations ***********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNPCWorldActionDefinition>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition;
class UScriptStruct* FNPCWorldActionDefinition::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FNPCWorldActionDefinition, (UObject*)Z_Construct_UPackage__Script_NaturalNPCWorldState(), TEXT("NPCWorldActionDefinition"));
	}
	return Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.OuterSingleton;
	}

// ********** Begin ScriptStruct FNPCWorldActionDefinition Property Definitions ********************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_ActionId = { "ActionId", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, ActionId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActionId_MetaData), NewProp_ActionId_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_Description = { "Description", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, Description), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Description_MetaData), NewProp_Description_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_UtteranceHints_Inner = { "UtteranceHints", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_UtteranceHints = { "UtteranceHints", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, UtteranceHints), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UtteranceHints_MetaData), NewProp_UtteranceHints_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_MaxDistance = { "MaxDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, MaxDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxDistance_MetaData), NewProp_MaxDistance_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachSocket = { "AttachSocket", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, AttachSocket), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachSocket_MetaData), NewProp_AttachSocket_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachLocationOffset = { "AttachLocationOffset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, AttachLocationOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachLocationOffset_MetaData), NewProp_AttachLocationOffset_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachRotationOffset = { "AttachRotationOffset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, AttachRotationOffset), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttachRotationOffset_MetaData), NewProp_AttachRotationOffset_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_ActionAnimation = { "ActionAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, ActionAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActionAnimation_MetaData), NewProp_ActionAnimation_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_HeldIdleAnimation = { "HeldIdleAnimation", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, HeldIdleAnimation), Z_Construct_UClass_UAnimSequence_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HeldIdleAnimation_MetaData), NewProp_HeldIdleAnimation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AnimationPlayRate = { "AnimationPlayRate", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, AnimationPlayRate), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AnimationPlayRate_MetaData), NewProp_AnimationPlayRate_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_EffectTriggerNormalizedTime = { "EffectTriggerNormalizedTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionDefinition, EffectTriggerNormalizedTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EffectTriggerNormalizedTime_MetaData), NewProp_EffectTriggerNormalizedTime_MetaData) };
void Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_bPlayAnimationInReverse_SetBit(void* Obj)
{
	((FNPCWorldActionDefinition*)Obj)->bPlayAnimationInReverse = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_bPlayAnimationInReverse = { "bPlayAnimationInReverse", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FNPCWorldActionDefinition), &Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_bPlayAnimationInReverse_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPlayAnimationInReverse_MetaData), NewProp_bPlayAnimationInReverse_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_ActionId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_Description,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_UtteranceHints_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_UtteranceHints,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_MaxDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachSocket,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachLocationOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AttachRotationOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_ActionAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_HeldIdleAnimation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_AnimationPlayRate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_EffectTriggerNormalizedTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewProp_bPlayAnimationInReverse,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FNPCWorldActionDefinition Property Definitions **********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
	nullptr,
	&NewStructOps,
	"NPCWorldActionDefinition",
	Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::PropPointers),
	sizeof(FNPCWorldActionDefinition),
	alignof(FNPCWorldActionDefinition),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionDefinition()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.InnerSingleton, Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition.InnerSingleton);
}
// ********** End ScriptStruct FNPCWorldActionDefinition *******************************************

// ********** Begin ScriptStruct FNPCWorldObjectState **********************************************
struct Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FNPCWorldObjectState); }
	static inline consteval int16 GetStructAlignment() { return alignof(FNPCWorldObjectState); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectId_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Description_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actor_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Distance_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AvailableActions_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FNPCWorldObjectState constinit property declarations **************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Category;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Description;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Distance;
	static const UECodeGen_Private::FStrPropertyParams NewProp_State_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_State_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_State;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AvailableActions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_AvailableActions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FNPCWorldObjectState constinit property declarations ****************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNPCWorldObjectState>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FNPCWorldObjectState;
class UScriptStruct* FNPCWorldObjectState::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FNPCWorldObjectState, (UObject*)Z_Construct_UPackage__Script_NaturalNPCWorldState(), TEXT("NPCWorldObjectState"));
	}
	return Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.OuterSingleton;
	}

// ********** Begin ScriptStruct FNPCWorldObjectState Property Definitions *************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, ObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectId_MetaData), NewProp_ObjectId_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Description = { "Description", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, Description), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Description_MetaData), NewProp_Description_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0114000000000014, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actor_MetaData), NewProp_Actor_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Distance = { "Distance", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, Distance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Distance_MetaData), NewProp_Distance_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State_ValueProp = { "State", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State_Key_KeyProp = { "State_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, State), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_State_MetaData), NewProp_State_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_AvailableActions_Inner = { "AvailableActions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FNPCWorldActionDefinition, METADATA_PARAMS(0, nullptr) }; // 1722355823
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_AvailableActions = { "AvailableActions", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldObjectState, AvailableActions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AvailableActions_MetaData), NewProp_AvailableActions_MetaData) }; // 1722355823
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Description,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Actor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_Distance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_State,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_AvailableActions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewProp_AvailableActions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FNPCWorldObjectState Property Definitions ***************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
	nullptr,
	&NewStructOps,
	"NPCWorldObjectState",
	Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::PropPointers),
	sizeof(FNPCWorldObjectState),
	alignof(FNPCWorldObjectState),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldObjectState()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.InnerSingleton, Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FNPCWorldObjectState.InnerSingleton);
}
// ********** End ScriptStruct FNPCWorldObjectState ************************************************

// ********** Begin ScriptStruct FNPCWorldActionResult *********************************************
struct Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FNPCWorldActionResult); }
	static inline consteval int16 GetStructAlignment() { return alignof(FNPCWorldActionResult); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSuccess_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectId_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActionId_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Message_MetaData[] = {
		{ "Category", "NPC World State" },
		{ "ModuleRelativePath", "Public/NPCWorldStateTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FNPCWorldActionResult constinit property declarations *************
	static void NewProp_bSuccess_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSuccess;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActionId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Message;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FNPCWorldActionResult constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNPCWorldActionResult>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FNPCWorldActionResult;
class UScriptStruct* FNPCWorldActionResult::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FNPCWorldActionResult, (UObject*)Z_Construct_UPackage__Script_NaturalNPCWorldState(), TEXT("NPCWorldActionResult"));
	}
	return Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.OuterSingleton;
	}

// ********** Begin ScriptStruct FNPCWorldActionResult Property Definitions ************************
void Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_bSuccess_SetBit(void* Obj)
{
	((FNPCWorldActionResult*)Obj)->bSuccess = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_bSuccess = { "bSuccess", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FNPCWorldActionResult), &Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_bSuccess_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSuccess_MetaData), NewProp_bSuccess_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionResult, ObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectId_MetaData), NewProp_ObjectId_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_ActionId = { "ActionId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionResult, ActionId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActionId_MetaData), NewProp_ActionId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FNPCWorldActionResult, Message), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Message_MetaData), NewProp_Message_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_bSuccess,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_ActionId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewProp_Message,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FNPCWorldActionResult Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_NaturalNPCWorldState,
	nullptr,
	&NewStructOps,
	"NPCWorldActionResult",
	Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::PropPointers),
	sizeof(FNPCWorldActionResult),
	alignof(FNPCWorldActionResult),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FNPCWorldActionResult()
{
	if (!Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.InnerSingleton, Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FNPCWorldActionResult.InnerSingleton);
}
// ********** End ScriptStruct FNPCWorldActionResult ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateTypes_h__Script_NaturalNPCWorldState_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FNPCWorldItemAnimationProfile::StaticStruct, Z_Construct_UScriptStruct_FNPCWorldItemAnimationProfile_Statics::NewStructOps, TEXT("NPCWorldItemAnimationProfile"),&Z_Registration_Info_UScriptStruct_FNPCWorldItemAnimationProfile, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FNPCWorldItemAnimationProfile), 2274357887U) },
		{ FNPCWorldActionDefinition::StaticStruct, Z_Construct_UScriptStruct_FNPCWorldActionDefinition_Statics::NewStructOps, TEXT("NPCWorldActionDefinition"),&Z_Registration_Info_UScriptStruct_FNPCWorldActionDefinition, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FNPCWorldActionDefinition), 1722355823U) },
		{ FNPCWorldObjectState::StaticStruct, Z_Construct_UScriptStruct_FNPCWorldObjectState_Statics::NewStructOps, TEXT("NPCWorldObjectState"),&Z_Registration_Info_UScriptStruct_FNPCWorldObjectState, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FNPCWorldObjectState), 871788507U) },
		{ FNPCWorldActionResult::StaticStruct, Z_Construct_UScriptStruct_FNPCWorldActionResult_Statics::NewStructOps, TEXT("NPCWorldActionResult"),&Z_Registration_Info_UScriptStruct_FNPCWorldActionResult, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FNPCWorldActionResult), 3945505905U) },
	};
}; // Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateTypes_h__Script_NaturalNPCWorldState_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateTypes_h__Script_NaturalNPCWorldState_2813682411{
	TEXT("/Script/NaturalNPCWorldState"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateTypes_h__Script_NaturalNPCWorldState_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE_Projects_NaturalNPC_Plugins_NaturalNPCWorldState_Source_NaturalNPCWorldState_Public_NPCWorldStateTypes_h__Script_NaturalNPCWorldState_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
