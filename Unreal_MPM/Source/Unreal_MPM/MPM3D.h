// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "MPM_Cell.h"
//#include "MPM_Particle.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM3D.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM3D : public AActor
{
	GENERATED_BODY()
	
public:	
	AMPM3D();
	virtual ~AMPM3D();
	void Initialize();
	void InitGrid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:
	struct Cell
	{

	};
	
	struct Particle
	{
	};
	
};
