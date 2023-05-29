// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"
#include "MatrixTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM2D.generated.h"


struct Particle
{
	FVector2f x; //pos
	FVector2f v; //vel
	FMatrix2x2 C; //affine momentum from APIC
	float Jp; //determinant of deformation gradient
	float mass;
	float volume_0;
};

struct Cell
{
	FVector2f v;
	float mass; //일정하게 유지
};

UCLASS()
class UNREAL_MPM_API AMPM2D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM2D();
	virtual ~AMPM2D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialize();
	void ClearGrid();
	void P2G();
	void UpdateGrid();
	void G2P();

	void PipeLine();
	void UpdateInstancedMesh();

	FMatrix2x2 Transpose(FMatrix2x2 originMatrix);
	FMatrix2x2 MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FMatrix2x2 MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2);

public:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:
	//const variables
	const int grid_res = 64;
	const int NumCells = grid_res * grid_res;
	
	const int division = 16;

	const float dt = 1.0f;
	const int iterations = static_cast<int>(1.0f / dt);
	const float gravity = -0.3f;

	float elastic_lambda = 10.f;
	float elastic_mu = 20.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumParticles = 0;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FMatrix2x2> Fs; //for deformation gradient
	TArray<FVector2f> TempPositions;
};
