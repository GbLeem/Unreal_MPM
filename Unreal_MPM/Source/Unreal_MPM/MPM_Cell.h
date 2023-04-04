// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM_Cell.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM_Cell : public AActor
{
	GENERATED_BODY()
	
public:	
	AMPM_Cell();

public:
	float mass;
	FVector3d mv;
	FVector3d vel;
	FVector3d force;
	
	FVector2d padding;
};
