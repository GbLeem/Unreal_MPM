// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM_Grid.h"

// Sets default values
AMPM_Grid::AMPM_Grid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AMPM_Grid::AMPM_Grid(FVector3f dimension, FVector3f cellspacing, FVector3f leftBottom, CenterType centerType)
{

}

// Called when the game starts or when spawned
void AMPM_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPM_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

