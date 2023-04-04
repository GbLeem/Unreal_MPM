// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MPM_Cell.h"
#include "MPM_Particle.h"
#include "MPM_Grid.h"

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM3D.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM3D : public AActor
{
	GENERATED_BODY()
	
public:	

	enum ParticleData
	{
		POSITION,
		VELOCITY,
		MASS,
		VOLUME
	};
	
	enum ParticleType
	{
		Inactive = 0,
		Elastic,
		Snow,
		Liquid
	};

	AMPM3D();
	virtual ~AMPM3D();
	void Initialize();
	void InitGrid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	struct Cell
	{

	};

	struct Grid
	{

	};

	//for cell
	float m_cMass;
	FVector3f m_cMv;
	FVector3f m_cVel;
	FVector3f m_cForce;

	FVector2f m_cPadding;

	//for particle
	ParticleType m_pType;
	float m_pMass;
	float m_pVolume;
	FVector3f m_pPos;
	FVector3f m_pVel;
	FMatrix m_pB;
	FMatrix m_pD;
	FMatrix m_pFe; //
	FMatrix m_pFp; //deformation gradient on particle p

	FVector3f m_Ppadding;

};
