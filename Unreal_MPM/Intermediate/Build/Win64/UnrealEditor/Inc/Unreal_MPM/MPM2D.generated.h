// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UNREAL_MPM_MPM2D_generated_h
#error "MPM2D.generated.h already included, missing '#pragma once' in MPM2D.h"
#endif
#define UNREAL_MPM_MPM2D_generated_h

#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_SPARSE_DATA
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execEach_Simulation_Step); \
	DECLARE_FUNCTION(execInitialize);


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execEach_Simulation_Step); \
	DECLARE_FUNCTION(execInitialize);


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMPM2D(); \
	friend struct Z_Construct_UClass_AMPM2D_Statics; \
public: \
	DECLARE_CLASS(AMPM2D, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Unreal_MPM"), NO_API) \
	DECLARE_SERIALIZER(AMPM2D)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_INCLASS \
private: \
	static void StaticRegisterNativesAMPM2D(); \
	friend struct Z_Construct_UClass_AMPM2D_Statics; \
public: \
	DECLARE_CLASS(AMPM2D, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Unreal_MPM"), NO_API) \
	DECLARE_SERIALIZER(AMPM2D)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMPM2D(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMPM2D) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMPM2D); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMPM2D); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMPM2D(AMPM2D&&); \
	NO_API AMPM2D(const AMPM2D&); \
public:


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMPM2D(AMPM2D&&); \
	NO_API AMPM2D(const AMPM2D&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMPM2D); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMPM2D); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMPM2D)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_24_PROLOG
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_SPARSE_DATA \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_RPC_WRAPPERS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_INCLASS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_SPARSE_DATA \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_INCLASS_NO_PURE_DECLS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h_27_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UNREAL_MPM_API UClass* StaticClass<class AMPM2D>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Unreal_MPM_Source_Unreal_MPM_MPM2D_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
