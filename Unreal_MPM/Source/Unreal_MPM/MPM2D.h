// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM2D.generated.h"


struct Particle
{
	FVector2f x;
	FVector2f v;
	FMatrix C;
	float mass; //일정하게 유지
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

	UFUNCTION()
	void Initialize();

	UFUNCTION()
	void Each_Simulation_Step();


	//const variables
	const int grid_res = 64;
	const int num_cells = grid_res * grid_res;
	const float dt = 1.0f;
	const int iterations = static_cast<int>(1.0f / dt);
	const float gravity = -0.05f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int num_particles = 0;

	TArray<Particle*> particles;
	TArray<Cell*> grid;
	TArray<FVector2f> Weights;
};
