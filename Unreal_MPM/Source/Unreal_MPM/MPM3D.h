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

	void Initialize();

	UFUNCTION(BlueprintCallable)
	void SimulateUpdate();

	UFUNCTION(BlueprintCallable)
	void UpdateParticle();

	UFUNCTION(BlueprintCallable)
	void SimulatingPipeLine();

	//function step
	void ClearGrid();
	void P2GFirst();
	void P2GSecond();
	void UpdateGrid();
	void G2P();

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
		float mass;
	};

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	//calculating variables
	//const int grid_res = 32;
	const int grid_res = 16;
	const int num_cells = grid_res * grid_res * grid_res;
	//const int division = 128; //batch size ?

	//simulation
	//const float dt = 0.2f;
	const float dt = 0.00002f;
	const float iterations = static_cast<int>(1.f / dt);

	const float gravity = -0.3f;

	//fluid parameter
	const float rest_density = 4.0f;
	const float dynamic_viscosity = 0.1f;

	//equation of state
	const float eos_stiffness = 10.f;
	const float eos_power = 4;

	//Particle and Grid
	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FVector3f> m_weights;
	TArray<FVector3f> TempPositions;

	int NumParticles;
	const float timestep = 0.2f; //[TODO] what timestep can fit to tick?
};
