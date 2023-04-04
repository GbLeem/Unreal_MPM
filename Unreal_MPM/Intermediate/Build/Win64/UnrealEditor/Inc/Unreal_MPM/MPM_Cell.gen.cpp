// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/MPM_Cell.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMPM_Cell() {}
// Cross Module References
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Cell_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Cell();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
// End Cross Module References
	void AMPM_Cell::StaticRegisterNativesAMPM_Cell()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMPM_Cell);
	UClass* Z_Construct_UClass_AMPM_Cell_NoRegister()
	{
		return AMPM_Cell::StaticClass();
	}
	struct Z_Construct_UClass_AMPM_Cell_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMPM_Cell_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM_Cell_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MPM_Cell.h" },
		{ "ModuleRelativePath", "MPM_Cell.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMPM_Cell_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMPM_Cell>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMPM_Cell_Statics::ClassParams = {
		&AMPM_Cell::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AMPM_Cell_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM_Cell_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMPM_Cell()
	{
		if (!Z_Registration_Info_UClass_AMPM_Cell.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMPM_Cell.OuterSingleton, Z_Construct_UClass_AMPM_Cell_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMPM_Cell.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AMPM_Cell>()
	{
		return AMPM_Cell::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMPM_Cell);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Cell_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Cell_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMPM_Cell, AMPM_Cell::StaticClass, TEXT("AMPM_Cell"), &Z_Registration_Info_UClass_AMPM_Cell, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMPM_Cell), 526302528U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Cell_h_967671835(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Cell_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Cell_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
