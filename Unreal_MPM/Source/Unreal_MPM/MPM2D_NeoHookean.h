// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM2D_NeoHookean.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM2D_NeoHookean : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM2D_NeoHookean();

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

	void UpdateParticles();
	void Simulate();

public:
	//for matrix calculation
	FMatrix2x2 Transpose(FMatrix2x2 originMatrix);
	FMatrix2x2 MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FMatrix2x2 MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FVector2f MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1);

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:
	struct Particle
	{
		FVector2f x; //pos
		FVector2f v; //vel
		FMatrix2x2 C; //affine momentum from APIC
		float mass;
		float volume_0;
	};

	struct Cell
	{
		FVector2f v;
		float mass; //일정하게 유지
	};

public:
	int NumParticles;
	const int grid_res = 64;
	const int NumCells = grid_res * grid_res;

	//const float dt = 0.1f;
	const float dt = 0.05f;
	const float iterations = (int)(1.0f / dt);
	//const float gravity = -0.1f;
	const float gravity = -0.6f;

	float elastic_lambda = 10.f;
	float elastic_mu = 20.f;
	/*float elastic_lambda = 1.f;
	float elastic_mu = 2.f;*/

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FMatrix2x2> Fs; //for deformation gradient
	TArray<FVector2f> TempPositions;
	TArray<FVector2f> weights;

};
