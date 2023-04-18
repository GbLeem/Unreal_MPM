#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM_Particle.generated.h"

#define NUM_PARTICLE 64

UENUM()
enum class PTYPE
{
	Inactive = 0,
	Elastic,
	Snow,
	Liquid
};

UCLASS()
class UNREAL_MPM_API AMPM_Particle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM_Particle();
	~AMPM_Particle();
	virtual void OnConstruction(const FTransform& Transform) override;
	void InitGrid();

	UFUNCTION(BlueprintCallable)
	void Simulate();

	UFUNCTION(BlueprintCallable)
	void UpdateParticle();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	struct Particle
	{
		FVector2f x; // position
		FVector2f v; // velocity
		FMatrix2x2 C; // affine momentum matrix
		float mass;
		float padding; // just for performance
	};

	struct Cell
	{
		FVector2f v; // velocity
		float mass;
		float padding; // just for performance
	};

protected:
	//TArray<FTransform> Transforms;
	//UPROPERTY(Transient) //직렬화

	TArray<FVector2f> TempPositions;

	//float timestep;
	int NumParticles;
	int NumParticlesForInstancedStaticMesh;
	const float m_gravity = -0.05f;
	const float dt = 0.01f;
	const float iterations = 1.0f / dt;

	UPROPERTY(VisibleAnywhere)
	int grid_res;

	UPROPERTY(VisibleAnywhere)
	int num_cells = grid_res * grid_res;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedParticle;

	TArray<Particle*> m_pParticles;
	TArray<Cell*> m_pGrid;

	TArray<FVector2f> m_weights;

private:
	/*inline FMatrix2x2 operator*(const FVector2f& other)
	{
		
	}*/
};
