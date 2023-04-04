// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/MPM3D.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMPM3D() {}
// Cross Module References
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM3D_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM3D();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
	ENGINE_API UClass* Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister();
// End Cross Module References
	void AMPM3D::StaticRegisterNativesAMPM3D()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMPM3D);
	UClass* Z_Construct_UClass_AMPM3D_NoRegister()
	{
		return AMPM3D::StaticClass();
	}
	struct Z_Construct_UClass_AMPM3D_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InstancedStaticMeshComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InstancedStaticMeshComponent;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMPM3D_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM3D_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MPM3D.h" },
		{ "ModuleRelativePath", "MPM3D.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM3D_Statics::NewProp_InstancedStaticMeshComponent_MetaData[] = {
		{ "Category", "MPM3D" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "MPM3D.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMPM3D_Statics::NewProp_InstancedStaticMeshComponent = { "InstancedStaticMeshComponent", nullptr, (EPropertyFlags)0x00200800000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AMPM3D, InstancedStaticMeshComponent), Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AMPM3D_Statics::NewProp_InstancedStaticMeshComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM3D_Statics::NewProp_InstancedStaticMeshComponent_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AMPM3D_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMPM3D_Statics::NewProp_InstancedStaticMeshComponent,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMPM3D_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMPM3D>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMPM3D_Statics::ClassParams = {
		&AMPM3D::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AMPM3D_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AMPM3D_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AMPM3D_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM3D_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMPM3D()
	{
		if (!Z_Registration_Info_UClass_AMPM3D.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMPM3D.OuterSingleton, Z_Construct_UClass_AMPM3D_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMPM3D.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AMPM3D>()
	{
		return AMPM3D::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMPM3D);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM3D_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM3D_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMPM3D, AMPM3D::StaticClass, TEXT("AMPM3D"), &Z_Registration_Info_UClass_AMPM3D, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMPM3D), 2264482279U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM3D_h_3522004595(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM3D_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM3D_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
