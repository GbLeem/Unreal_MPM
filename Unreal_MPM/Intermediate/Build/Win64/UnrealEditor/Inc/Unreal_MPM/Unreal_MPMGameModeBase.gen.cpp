// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/Unreal_MPMGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUnreal_MPMGameModeBase() {}
// Cross Module References
	UNREAL_MPM_API UClass* Z_Construct_UClass_AUnreal_MPMGameModeBase_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AUnreal_MPMGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
// End Cross Module References
	void AUnreal_MPMGameModeBase::StaticRegisterNativesAUnreal_MPMGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AUnreal_MPMGameModeBase);
	UClass* Z_Construct_UClass_AUnreal_MPMGameModeBase_NoRegister()
	{
		return AUnreal_MPMGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Unreal_MPMGameModeBase.h" },
		{ "ModuleRelativePath", "Unreal_MPMGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUnreal_MPMGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::ClassParams = {
		&AUnreal_MPMGameModeBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AUnreal_MPMGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AUnreal_MPMGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AUnreal_MPMGameModeBase.OuterSingleton, Z_Construct_UClass_AUnreal_MPMGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AUnreal_MPMGameModeBase.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AUnreal_MPMGameModeBase>()
	{
		return AUnreal_MPMGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AUnreal_MPMGameModeBase);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_Unreal_MPMGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_Unreal_MPMGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AUnreal_MPMGameModeBase, AUnreal_MPMGameModeBase::StaticClass, TEXT("AUnreal_MPMGameModeBase"), &Z_Registration_Info_UClass_AUnreal_MPMGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AUnreal_MPMGameModeBase), 3645589195U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_Unreal_MPMGameModeBase_h_3494030483(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_Unreal_MPMGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_Unreal_MPMGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
