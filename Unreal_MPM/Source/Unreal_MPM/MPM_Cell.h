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
	FVector3d m_velocity;
	float m_mass;
	float m_padding;

	UPROPERTY(VisibleAnywhere)
	int m_gridResolution;

	const int m_numCells = m_gridResolution * m_gridResolution;
};
