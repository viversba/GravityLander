// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GravityLander/GravityLanderGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGravityLanderGameModeBase() {}
// Cross Module References
	GRAVITYLANDER_API UClass* Z_Construct_UClass_AGravityLanderGameModeBase_NoRegister();
	GRAVITYLANDER_API UClass* Z_Construct_UClass_AGravityLanderGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_GravityLander();
// End Cross Module References
	void AGravityLanderGameModeBase::StaticRegisterNativesAGravityLanderGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_AGravityLanderGameModeBase_NoRegister()
	{
		return AGravityLanderGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AGravityLanderGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGravityLanderGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_GravityLander,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGravityLanderGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "GravityLanderGameModeBase.h" },
		{ "ModuleRelativePath", "GravityLanderGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGravityLanderGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGravityLanderGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGravityLanderGameModeBase_Statics::ClassParams = {
		&AGravityLanderGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AGravityLanderGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGravityLanderGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGravityLanderGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGravityLanderGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGravityLanderGameModeBase, 1514122704);
	template<> GRAVITYLANDER_API UClass* StaticClass<AGravityLanderGameModeBase>()
	{
		return AGravityLanderGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGravityLanderGameModeBase(Z_Construct_UClass_AGravityLanderGameModeBase, &AGravityLanderGameModeBase::StaticClass, TEXT("/Script/GravityLander"), TEXT("AGravityLanderGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGravityLanderGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
