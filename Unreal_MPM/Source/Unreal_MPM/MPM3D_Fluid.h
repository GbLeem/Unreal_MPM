
#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM3D_Fluid.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM3D_Fluid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM3D_Fluid();

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

	void Simulate();
	void UpdateParticles();

public:
	FVector3f MultiplyMatrixAndVector(FMatrix m, FVector3f v);

	template<typename T>
	FMatrix ScalingMatrix(FMatrix m, T scale);

	FMatrix ResetMatrix(FMatrix m);
	FVector3f Eq_16_Calculation(FMatrix eq_16_term, float weight, FVector3f cell_dist);
	FMatrix PlusMatrix(FMatrix m1, FMatrix m2);
	FMatrix MakeMatrixTerm(FVector3f v1, FVector3f v2);

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

	int NumParticles;

	const int grid_res = 32;
	const int NumCells = grid_res * grid_res * grid_res;

	const float dt = 0.02f;
	const float iterations = (int)(1.f / dt);

	const float gravity = -0.3f;
	const float rest_density = 4.0f;
	const float dynamic_viscosity = 0.1f;

	const float eos_stiffness = 10.f;
	const float eos_power = 4;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;

	TArray<FVector3f> weights;
};


template<typename T>
inline FMatrix AMPM3D_Fluid::ScalingMatrix(FMatrix m, T scale)
{
	m.M[0][0] *= scale;
	m.M[0][1] *= scale;
	m.M[0][2] *= scale;
	
	m.M[1][0] *= scale;
	m.M[1][1] *= scale;
	m.M[1][2] *= scale;
	
	m.M[2][0] *= scale;
	m.M[2][1] *= scale;
	m.M[2][2] *= scale;
	
	return m;	
}