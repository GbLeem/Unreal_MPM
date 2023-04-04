// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/MPM_Grid.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMPM_Grid() {}
// Cross Module References
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Grid_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Grid();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
// End Cross Module References
	void AMPM_Grid::StaticRegisterNativesAMPM_Grid()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMPM_Grid);
	UClass* Z_Construct_UClass_AMPM_Grid_NoRegister()
	{
		return AMPM_Grid::StaticClass();
	}
	struct Z_Construct_UClass_AMPM_Grid_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMPM_Grid_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM_Grid_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MPM_Grid.h" },
		{ "ModuleRelativePath", "MPM_Grid.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMPM_Grid_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMPM_Grid>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMPM_Grid_Statics::ClassParams = {
		&AMPM_Grid::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AMPM_Grid_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM_Grid_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMPM_Grid()
	{
		if (!Z_Registration_Info_UClass_AMPM_Grid.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMPM_Grid.OuterSingleton, Z_Construct_UClass_AMPM_Grid_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMPM_Grid.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AMPM_Grid>()
	{
		return AMPM_Grid::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMPM_Grid);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Grid_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Grid_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMPM_Grid, AMPM_Grid::StaticClass, TEXT("AMPM_Grid"), &Z_Registration_Info_UClass_AMPM_Grid, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMPM_Grid), 1226240005U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Grid_h_622939661(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Grid_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Grid_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
