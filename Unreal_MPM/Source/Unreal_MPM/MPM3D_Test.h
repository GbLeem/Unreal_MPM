// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM3D_Test.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM3D_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM3D_Test();

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

	FVector3f MultiplyMatrixAndVector(FMatrix m1, FVector3f v1);
	FMatrix ScalingMatrix(FMatrix m, float scale);
	FMatrix PlusMatrix(FMatrix m1, FMatrix m2);

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
		FMatrix C; //affine momentum matrix
		float mass;
	};

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	int NumParticles = 0;

	const int grid_res = 32;
	const int NumCells = grid_res * grid_res * grid_res;

	const float dt = 0.2f;
	const float iterations = (int)(1.f / dt);
	const float gravity = -0.05f;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	//TArray<FVector3f> TempPositions;

	TArray<FVector3f> weights;
};
