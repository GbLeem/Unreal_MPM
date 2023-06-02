// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM2D_Test.h"

// Sets default values
AMPM2D_Test::AMPM2D_Test()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));

	SetRootComponent(InstancedStaticMeshComponent);

	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void AMPM2D_Test::BeginPlay()
{
	Super::BeginPlay();
	
	//initialize
	const float spacing = 1.0f;
	const int box_x = 16;
	const int box_y = 16;
	const float sx = grid_res / 2.f;
	const float sy = grid_res / 2.f;

	for (float i = sx - box_x / 2; i < sx + box_x; i += spacing)
	{
		for (float j = sy - box_y / 2; j < sy + box_y; j += spacing)
		{
			auto Pos = FVector2f(i, j);
			TempPositions.Add(Pos);
		}
	}
	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = FVector2f{ (FMath::FRandRange(0,1) - 0.5f) * 0.5f, (FMath::FRandRange(0,1) - 0.5f + 2.75f) * 0.5f };
		p->C = { 0, 0, 0, 0 };
		p->mass = 1.f;

		m_pParticles.Add(p);
	}
	
	m_pGrid.Empty(NumParticles);
	
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = new Cell();
		m_pGrid.Add(cell);
	}

	
	//make mesh
	if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(NumParticles);

		for (int i = 0; i < NumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(m_pParticles[i]->x.X * 100.f, m_pParticles[i]->x.Y * 100.f, 0));
			Transforms.Add(tempValue);
		}
		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}
}

// Called every frame
void AMPM2D_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < iterations; ++i)
	{
		Simulate();
	}
	UpdateParticles();
}

void AMPM2D_Test::Simulate()
{
	//reset grid
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->v = { 0.f, 0.f };
	}

	//P2G
	for (auto& p : m_pParticles)
	{
		FIntVector2 cell_idx = FIntVector2(p->x.X, p->x.Y);
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };
		weights.Empty(3);
		//항상 여기서 인덱스가 문제임;
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X + weights[gy].Y;

				FIntVector2 cell_x = FIntVector2(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				FVector2f cell_dist = FVector2f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f);
				FVector2f Q = MultiplyMatrixAndFloat(p->C, cell_dist);

				float mass_contrib = weight * p->mass;

				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;

				Cell* cell = m_pGrid[cell_index];
				cell->mass += mass_contrib;
				cell->v += mass_contrib * (p->v + Q);
				m_pGrid[cell_index] = cell;
			}
		}
	}

	//grid velocity update
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];
		if (cell->mass > 0)
		{
			cell->v /= cell->mass;
			cell->v += dt * FVector2f(0, gravity);

			int x = i / grid_res;
			int y = i % grid_res;
			if (x<2 || x>grid_res - 3)
			{
				cell->v.X = 0;
			}
			if (y<2 || y>grid_res - 3)
			{
				cell->v.Y = 0;
			}
			m_pGrid[i] = cell;
		}
		m_pGrid[i] = cell;
	}

	//G2P
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		p->v = { 0.f,0.f };

		FIntVector2 cell_idx = FIntVector2(p->x.X, p->x.Y);
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };
		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

		FMatrix2x2 B = { 0.f, 0.f, 0.f, 0.f };

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = FIntVector2(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;

				FVector2f dist = { (cell_x.X - p->x.X) + 0.5f,(cell_x.Y - p->x.Y) + 0.5f };
				FVector2f weighted_velocity = m_pGrid[cell_index]->v * weight;

				//right calculation
				FMatrix2x2 term = FMatrix2x2(weighted_velocity.X * dist.X, weighted_velocity.X * dist.Y, 
					weighted_velocity.Y * dist.X, weighted_velocity.Y * dist.Y);

				float a1, b1, c1, d1;
				float a2, b2, c2, d2;
				B.GetMatrix(a1, b1, c1, d1);
				term.GetMatrix(a2, b2, c2, d2);
				
				B = { a1 + a2, b1 + b2,c1 + c2,d1 + d2 };
				p->v += weighted_velocity;
			}
		}
		float a, b, c, d;
		B.GetMatrix(a, b, c, d);
		p->C = { a * 4,b * 4,c * 4,d * 4 };
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);

		m_pParticles[i] = p;
	}
}

void AMPM2D_Test::UpdateParticles()
{
	TArray<FTransform> Transforms;

	Transforms.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->x.X * 100.f, m_pParticles[i]->x.Y * 100.f, 0));
		Transforms.Add(tempValue);
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, Transforms[i]);
	}
	InstancedStaticMeshComponent->MarkRenderStateDirty();
}

FVector2f AMPM2D_Test::MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1)
{
	FVector2f resultVector;

	float a, b, c, d = 0.f;
	m1.GetMatrix(a, b, c, d);

	resultVector = { a * v1.X + b * v1.Y, c * v1.X + d * v1.Y };

	return resultVector;
}