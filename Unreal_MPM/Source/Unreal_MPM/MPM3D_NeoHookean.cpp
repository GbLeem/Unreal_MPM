// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM3D_NeoHookean.h"

AMPM3D_NeoHookean::AMPM3D_NeoHookean()
	:NumParticles(0)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));

	SetRootComponent(InstancedStaticMeshComponent);

	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);
}

void AMPM3D_NeoHookean::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FVector3f> TempPositions;

	const float spacing = 1.0f;
	const int box_x = 16;
	const int box_y = 16;
	const int box_z = 16;
	const float sx = grid_res / 2.f;
	const float sy = grid_res / 2.f;
	const float sz = grid_res / 2.f;

	for (float i = -box_x / 2; i < box_x / 2; i += spacing) //32-8 < 32+8 -> 16-8 < <16+8 : 8~24
	{
		for (float j = -box_y / 2; j < box_y / 2; j += spacing)
		{
			for (float k = -box_z / 2; k < box_z / 2; k += spacing)
			{
				auto Pos = FVector3f(sx + i, sy + j, sz + k);
				TempPositions.Add(Pos);
			}
		}
	}

	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = { 0.f,0.f,0.f };
		//p->C = { 0, 0, 0, 0 };
		p->mass = 1.f;
		m_pParticles.Add(p);
	}

	m_pGrid.Empty(NumCells);

	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = new Cell();
		cell->v = { 0.f, 0.f, 0.f };
		m_pGrid.Add(cell);
	}

	//make mesh
	if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(NumParticles);

		for (int i = 0; i < NumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(m_pParticles[i]->x.X * 100.f, m_pParticles[i]->x.Y * 100.f, m_pParticles[i]->x.Z * 100.f));
			Transforms.Add(tempValue);
		}
		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}
}

// Called every frame
void AMPM3D_NeoHookean::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMPM3D_NeoHookean::ClearGrid()
{
}

void AMPM3D_NeoHookean::P2G_1()
{
}

void AMPM3D_NeoHookean::P2G_2()
{
}

void AMPM3D_NeoHookean::UpdateGrid()
{
}

void AMPM3D_NeoHookean::G2P()
{
}

void AMPM3D_NeoHookean::Simulate()
{
}

void AMPM3D_NeoHookean::UpdateParticles()
{
}

FVector3f AMPM3D_NeoHookean::MultiplyMatrixAndVector(FMatrix m1, FVector3f v1)
{
	float x = m1.M[0][0] * v1.X + m1.M[0][1] * v1.Y + m1.M[0][2] * v1.Z;
	float y = m1.M[1][0] * v1.X + m1.M[1][1] * v1.Y + m1.M[1][2] * v1.Z;
	float z = m1.M[2][0] * v1.X + m1.M[2][1] * v1.Y + m1.M[2][2] * v1.Z;

	return { x,y,z };
}

FMatrix AMPM3D_NeoHookean::ScalingMatrix(FMatrix m, float scale)
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

FMatrix AMPM3D_NeoHookean::PlusMatrix(FMatrix m1, FMatrix m2)
{
	FMatrix resultMatrix;
	resultMatrix.M[0][0] = m1.M[0][0] + m2.M[0][0];
	resultMatrix.M[0][1] = m1.M[0][1] + m2.M[0][1];
	resultMatrix.M[0][2] = m1.M[0][2] + m2.M[0][2];

	resultMatrix.M[1][0] = m1.M[1][0] + m2.M[1][0];
	resultMatrix.M[1][1] = m1.M[1][1] + m2.M[1][1];
	resultMatrix.M[1][2] = m1.M[1][2] + m2.M[1][2];

	resultMatrix.M[2][0] = m1.M[2][0] + m2.M[2][0];
	resultMatrix.M[2][0] = m1.M[2][1] + m2.M[2][1];
	resultMatrix.M[2][0] = m1.M[2][2] + m2.M[2][2];

	return resultMatrix;
}

