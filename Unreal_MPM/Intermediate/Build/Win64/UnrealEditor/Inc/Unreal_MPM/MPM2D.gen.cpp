// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Unreal_MPM/MPM2D.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMPM2D() {}
// Cross Module References
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM2D_NoRegister();
	UNREAL_MPM_API UClass* Z_Construct_UClass_AMPM2D();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_Unreal_MPM();
// End Cross Module References
	DEFINE_FUNCTION(AMPM2D::execEach_Simulation_Step)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->Each_Simulation_Step();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(AMPM2D::execInitialize)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->Initialize();
		P_NATIVE_END;
	}
	void AMPM2D::StaticRegisterNativesAMPM2D()
	{
		UClass* Class = AMPM2D::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "Each_Simulation_Step", &AMPM2D::execEach_Simulation_Step },
			{ "Initialize", &AMPM2D::execInitialize },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "MPM2D.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMPM2D, nullptr, "Each_Simulation_Step", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AMPM2D_Each_Simulation_Step()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMPM2D_Each_Simulation_Step_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AMPM2D_Initialize_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMPM2D_Initialize_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "MPM2D.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMPM2D_Initialize_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMPM2D, nullptr, "Initialize", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AMPM2D_Initialize_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AMPM2D_Initialize_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AMPM2D_Initialize()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMPM2D_Initialize_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMPM2D);
	UClass* Z_Construct_UClass_AMPM2D_NoRegister()
	{
		return AMPM2D::StaticClass();
	}
	struct Z_Construct_UClass_AMPM2D_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_num_particles_MetaData[];
#endif
		static const UECodeGen_Private::FUnsizedIntPropertyParams NewProp_num_particles;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMPM2D_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Unreal_MPM,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AMPM2D_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AMPM2D_Each_Simulation_Step, "Each_Simulation_Step" }, // 453644250
		{ &Z_Construct_UFunction_AMPM2D_Initialize, "Initialize" }, // 2123836539
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM2D_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MPM2D.h" },
		{ "ModuleRelativePath", "MPM2D.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMPM2D_Statics::NewProp_num_particles_MetaData[] = {
		{ "Category", "MPM2D" },
		{ "ModuleRelativePath", "MPM2D.h" },
	};
#endif
	const UECodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_AMPM2D_Statics::NewProp_num_particles = { "num_particles", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AMPM2D, num_particles), METADATA_PARAMS(Z_Construct_UClass_AMPM2D_Statics::NewProp_num_particles_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM2D_Statics::NewProp_num_particles_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AMPM2D_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMPM2D_Statics::NewProp_num_particles,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMPM2D_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMPM2D>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMPM2D_Statics::ClassParams = {
		&AMPM2D::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AMPM2D_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AMPM2D_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AMPM2D_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMPM2D_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMPM2D()
	{
		if (!Z_Registration_Info_UClass_AMPM2D.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMPM2D.OuterSingleton, Z_Construct_UClass_AMPM2D_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMPM2D.OuterSingleton;
	}
	template<> UNREAL_MPM_API UClass* StaticClass<AMPM2D>()
	{
		return AMPM2D::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMPM2D);
	struct Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMPM2D, AMPM2D::StaticClass, TEXT("AMPM2D"), &Z_Registration_Info_UClass_AMPM2D, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMPM2D), 1636220568U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_3267468785(TEXT("/Script/Unreal_MPM"),
		Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
