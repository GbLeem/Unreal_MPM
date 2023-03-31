// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	const int grid_res = 0;
	const int num_cells = 0;
	const float dt = 0;
	const int iterations = 0;
	const float gravity = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int num_particles = 0;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<Particle> particles;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<Cell> grid;

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FVector2f> Weights;
};
