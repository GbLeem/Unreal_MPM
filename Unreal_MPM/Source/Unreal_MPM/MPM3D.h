// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MPM3D.generated.h"


UCLASS()
class UNREAL_MPM_API AMPM3D : public AActor
{
	GENERATED_BODY()
	
public:	
	AMPM3D();
	virtual ~AMPM3D();

	UFUNCTION(BlueprintCallable)
	void Initialize(const int box_size);

	UFUNCTION(BlueprintCallable)
	void Initialize_second(const int box_size);

	UFUNCTION(BlueprintCallable)
	void UpdateParticle();

	UFUNCTION(BlueprintCallable)
	void SimulatingPipeLine(double timestep);

	//function step
	void ClearGrid();
	void P2GFirst();
	void P2GSecond(double timestep);
	void P2GScatterMLS();
	void P2GMLS(double timestep);
	void UpdateGrid(double timestep);
	void G2P(double timestep);
	void G2PMLS(double timestep);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	//for interaction with static mesh
	void MoveInteractionBall();

	//for check particle positions
	void CheckParticlePos();

	//for FMatrix multiply with Vector3f
	FVector3f MultiplyMatrixAndVector(FMatrix f, FVector3f v);
public:
	struct Cell
	{
		FVector3f Vel;
		float mass;
	};

	struct Particle
	{
		FVector3f Pos; 
		FVector3f Vel;
		FMatrix C; //affine momentum matrix
		float mass;
		float volume_0;
	};

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedInteractionMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_pMesh;

	//calculating variables
	const int grid_res = 16;
	//const int grid_res = 32;
	const int num_cells = grid_res * grid_res * grid_res;

	//simulation
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float dt = 1.0f;
	const float iterations = static_cast<int>(1.f / dt);

	const float gravity = -0.03f;
	//const float gravity = -0.1f;

	//fluid parameter
	const float rest_density = 4.0f;
	const float dynamic_viscosity = 0.1f;

	//lame parameters for stress-strain relationship
	const float elastic_lambda = 10.f;
	const float elastic_mu = 20.f;

	//equation of state
	const float eos_stiffness = 10.f;
	const float eos_power = 4;

	//Particle and Grid
	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FVector3f> TempPositions;
	TArray<FMatrix> m_DeformationGradient;
 
	int NumParticles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double timesteps = 1.0f; //timestep fit to tick?

	bool firstRound = true;


	
	//for interaction
	//FVector3f BallPosition = AInteractionBall::BallTransform;
	//float InteractionDistance = 100.f;
};
