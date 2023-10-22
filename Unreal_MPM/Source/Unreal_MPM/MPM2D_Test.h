// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM2D_Test.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM2D_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM2D_Test();

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
	FVector2f MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1);

public:
	struct Particle
	{
		FVector2f x; //pos
		FVector2f v; //vel
		FMatrix2x2 C; //affine momentum from APIC
		float mass;
	};

	struct Cell
	{
		FVector2f v;
		float mass; //일정하게 유지
	};

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	//const variables
	const int grid_res = 64;
	const int NumCells = grid_res * grid_res;
	const float dt = 1.0f;
	const int iterations = (int)(1.0f / dt);

	const float gravity = -0.05f;

	int NumParticles = 0;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FVector2f> TempPositions;

	TArray<FVector2f> weights;
};
