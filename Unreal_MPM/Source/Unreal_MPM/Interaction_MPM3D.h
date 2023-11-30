// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"
#include "Chaos/Matrix.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction_MPM3D.generated.h"

using namespace Chaos;

UCLASS()
class UNREAL_MPM_API AInteraction_MPM3D : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteraction_MPM3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ClearGrid();
	void P2G();
	void UpdateGrid();
	void G2P();

	void Simulate();
	void UpdateParticles();

public:
	struct Cell
	{
		FVector3f v;
		float mass;
	};

	struct Particle
	{
		FVector3f x;
		FVector3f v;
		PMatrix<float, 3, 3> C; //affine momentum matrix
		float mass;
		float volume_0;
	};

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	int NumParticles1 = 0;
	int NumParticles2 = 0;
	int NumParticles3 = 0;
	int TotalNumParticles = 0;

	const int grid_res = 32;
	const int NumCells = grid_res * grid_res * grid_res;

	const float dt = 0.1f;
	const float iterations = (int)(1.f / dt);
	const float gravity = -0.3f;

	const float elastic_lambda = 10.f;
	const float elastic_mu = 20.f;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<PMatrix<float, 3, 3>> Fs;
	TArray<FVector3f> weights;
};
