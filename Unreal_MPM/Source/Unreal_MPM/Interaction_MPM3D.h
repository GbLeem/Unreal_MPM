// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction_MPM3D.generated.h"

UCLASS()
class UNREAL_MPM_API AInteraction_MPM3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteraction_MPM3D();

	void Initialize();
	void SimulatingPipeLine(double timestep);
	void UpdateParticle();

	void ClearGrid();
	void P2GFirst();
	void UpdateGrid(double timestep);
	void G2P(double timestep);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
		FMatrix F; //Deformation gradient

		float mass;
		float volume_0;
	};

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:

	//simulation parameter
	const float gravity = -0.03f;
	double timesteps = 1.0f; 

	//grid data
	const int grid_res = 20;
	const int num_cells = grid_res * grid_res * grid_res;

	int NumParticles1 = 0;
	int NumParticles2 = 0;
	int TotalNumParticles = 0;

	//array
	TArray<Particle*> m_pParticles; //1+2 total particles
	TArray<Cell*> m_pGrid;
	TArray<FVector3f> TempPositions1;
	TArray<FVector3f> TempPositions2;
};

