// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM3D.h"

// Sets default values
AMPM3D::AMPM3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	//InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	//SetRootComponent(InstancedStaticMeshComponent);
	
}

AMPM3D::~AMPM3D()
{
}

void AMPM3D::Initialize()
{
	//init square


}

void AMPM3D::InitGrid()
{
	
	
}

// Called when the game starts or when spawned
void AMPM3D::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPM3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

