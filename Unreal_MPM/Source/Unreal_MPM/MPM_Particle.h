// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM_Particle.generated.h"

UENUM()
enum class PTYPE
{
	Inactive = 0,
	Elastic,
	Snow,
	Liquid
};

UCLASS()
class UNREAL_MPM_API AMPM_Particle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM_Particle();

public:
	PTYPE type;
	float mass;
	float volume;

	FVector3d pos;
	FVector3d vel;
	FMatrix B;
	FMatrix D;
	FMatrix Fe;
	FMatrix Fp;
	
	FVector3d padding;

	FVector3d Pos = pos;
	
};
