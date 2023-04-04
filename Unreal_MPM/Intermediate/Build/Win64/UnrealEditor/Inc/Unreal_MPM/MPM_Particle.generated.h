// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UNREAL_MPM_MPM_Particle_generated_h
#error "MPM_Particle.generated.h already included, missing '#pragma once' in MPM_Particle.h"
#endif
#define UNREAL_MPM_MPM_Particle_generated_h

#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_SPARSE_DATA
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_RPC_WRAPPERS
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMPM_Particle(); \
	friend struct Z_Construct_UClass_AMPM_Particle_Statics; \
public: \
	DECLARE_CLASS(AMPM_Particle, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Unreal_MPM"), NO_API) \
	DECLARE_SERIALIZER(AMPM_Particle)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_INCLASS \
private: \
	static void StaticRegisterNativesAMPM_Particle(); \
	friend struct Z_Construct_UClass_AMPM_Particle_Statics; \
public: \
	DECLARE_CLASS(AMPM_Particle, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Unreal_MPM"), NO_API) \
	DECLARE_SERIALIZER(AMPM_Particle)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMPM_Particle(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMPM_Particle) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMPM_Particle); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMPM_Particle); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMPM_Particle(AMPM_Particle&&); \
	NO_API AMPM_Particle(const AMPM_Particle&); \
public:


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMPM_Particle(AMPM_Particle&&); \
	NO_API AMPM_Particle(const AMPM_Particle&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMPM_Particle); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMPM_Particle); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMPM_Particle)


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_18_PROLOG
#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_SPARSE_DATA \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_RPC_WRAPPERS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_INCLASS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_SPARSE_DATA \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_INCLASS_NO_PURE_DECLS \
	FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UNREAL_MPM_API UClass* StaticClass<class AMPM_Particle>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Unreal_MPM_Source_Unreal_MPM_MPM_Particle_h


#define FOREACH_ENUM_PTYPE(op) \
	op(PTYPE::Inactive) \
	op(PTYPE::Elastic) \
	op(PTYPE::Snow) \
	op(PTYPE::Liquid) 

enum class PTYPE;
template<> UNREAL_MPM_API UEnum* StaticEnum<PTYPE>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
