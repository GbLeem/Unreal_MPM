// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM2D_Fluid.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM2D_Fluid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM2D_Fluid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ClearGrid();
	void P2G_1();
	void P2G_2();
	void UpdateGrid();
	void G2P();

	void UpdateParticles();
	void Simulate();

public:
	//for matrix calculation
	FMatrix2x2 Transpose(FMatrix2x2 originMatrix);
	FMatrix2x2 MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FMatrix2x2 PlusMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FMatrix2x2 MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2);
	FVector2f MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1);
	
	FMatrix2x2 TraceCalculation(FMatrix2x2 m1, float trace);

	template<typename T>
	FMatrix2x2 ScalingMatrix(FMatrix2x2 m1, T scale);

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

public:
	int NumParticles;
	const int grid_res = 64;
	const int NumCells = grid_res * grid_res;

	const float dt = 0.2f;
	const float iterations = (int)(1.0f / dt);
	const float gravity = -0.3f;

	//fluid parameter
	const float rest_density = 4.f;
	const float dynamic_viscosity = 0.1f;

	//equation of state
	const float eos_stiffness = 10.f;
	const float eos_power = 4.f;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;
	TArray<FVector2f> weights;
};

template<typename T>
inline FMatrix2x2 AMPM2D_Fluid::ScalingMatrix(FMatrix2x2 m1, T scale)
{
	float a, b, c, d;
	m1.GetMatrix(a, b, c, d);
	return { a * scale, b * scale, c * scale, d * scale };
}
