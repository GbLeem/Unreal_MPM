// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/MPM_Particle.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMPM_Particle() {}
// Cross Module References
	UNREAL_MPM_API UEnum* Z_Construct_UEnum_Unreal_MPM_PTYPE();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Particle_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM_Particle();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_PTYPE;
	static UEnum* PTYPE_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_PTYPE.OuterSingleton)
		{
			Z_Registration_Info_UEnum_PTYPE.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_Unreal_MPM_PTYPE, Z_Construct_UPackage__Script_Unreal_MPM(), TEXT("PTYPE"));
		}
		return Z_Registration_Info_UEnum_PTYPE.OuterSingleton;
	}
	template<> UNREAL_MPM_API UEnum* StaticEnum<PTYPE>()
	{
		return PTYPE_StaticEnum();
	}
	struct Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enumerators[] = {
		{ "PTYPE::Inactive", (int64)PTYPE::Inactive },
		{ "PTYPE::Elastic", (int64)PTYPE::Elastic },
		{ "PTYPE::Snow", (int64)PTYPE::Snow },
		{ "PTYPE::Liquid", (int64)PTYPE::Liquid },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enum_MetaDataParams[] = {
		{ "Elastic.Name", "PTYPE::Elastic" },
		{ "Inactive.Name", "PTYPE::Inactive" },
		{ "Liquid.Name", "PTYPE::Liquid" },
		{ "ModuleRelativePath", "MPM_Particle.h" },
		{ "Snow.Name", "PTYPE::Snow" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_Unreal_MPM,
		nullptr,
		"PTYPE",
		"PTYPE",
		Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_Unreal_MPM_PTYPE()
	{
		if (!Z_Registration_Info_UEnum_PTYPE.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_PTYPE.InnerSingleton, Z_Construct_UEnum_Unreal_MPM_PTYPE_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_PTYPE.InnerSingleton;
	}
	void AMPM_Particle::StaticRegisterNativesAMPM_Particle()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMPM_Particle);
	UClass* Z_Construct_UClass_AMPM_Particle_NoRegister()
	{
		return AMPM_Particle::StaticClass();
	}
	struct Z_Construct_UClass_AMPM_Particle_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMPM_Particle_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM_Particle_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MPM_Particle.h" },
		{ "ModuleRelativePath", "MPM_Particle.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMPM_Particle_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMPM_Particle>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMPM_Particle_Statics::ClassParams = {
		&AMPM_Particle::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AMPM_Particle_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM_Particle_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMPM_Particle()
	{
		if (!Z_Registration_Info_UClass_AMPM_Particle.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMPM_Particle.OuterSingleton, Z_Construct_UClass_AMPM_Particle_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMPM_Particle.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AMPM_Particle>()
	{
		return AMPM_Particle::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMPM_Particle);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::EnumInfo[] = {
		{ PTYPE_StaticEnum, TEXT("PTYPE"), &Z_Registration_Info_UEnum_PTYPE, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2485639255U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMPM_Particle, AMPM_Particle::StaticClass, TEXT("AMPM_Particle"), &Z_Registration_Info_UClass_AMPM_Particle, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMPM_Particle), 3286266459U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_3717982744(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::ClassInfo),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
