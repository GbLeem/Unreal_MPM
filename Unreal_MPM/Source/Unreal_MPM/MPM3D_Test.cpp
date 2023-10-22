#include "MPM3D_Test.h"


AMPM3D_Test::AMPM3D_Test()
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

void AMPM3D_Test::BeginPlay()
{
	Super::BeginPlay();

	//Initialize
	TArray<FVector3f> TempPositions;

	const float spacing = 1.0f;
	const int box_x = 16;
	const int box_y = 16;
	const int box_z = 16;
	const float sx = grid_res / 2.f;
	const float sy = grid_res / 2.f;
	const float sz = grid_res / 2.f;

	for (float i = sx - box_x / 2; i < sx + box_x / 2; i += spacing) 
	{
		for (float j = sy - box_y / 2; j < sy + box_y / 2; j += spacing)
		{
			for (float k = sz - box_z / 2; k < sz + box_z / 2; k += spacing)
			{
				auto Pos = FVector3f(i, j, k);
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
		p->v= FVector3f{ (FMath::FRandRange(0,1) - 0.5f) * 0.5f, (FMath::FRandRange(0,1) - 0.5f + 2.75f) * 0.5f , (FMath::FRandRange(0,1) - 0.5f-2.75f)*0.5f };
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
void AMPM3D_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Simulate();
	UpdateParticles();
}

void AMPM3D_Test::ClearGrid()
{
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->v = { 0.f,0.f,0.f };
	}
}

void AMPM3D_Test::P2G()
{
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FIntVector cell_idx = FIntVector(p->x.X, p->x.Y, p->x.Z);
		FVector3f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f, p->x.Z - cell_idx.Z - 0.5f };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2),0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2), 0.75f - (float)pow(cell_diff.Z, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2), 0.5f * (float)pow(0.5f + cell_diff.Z, 2) });

		FMatrix C = p->C;

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					FIntVector cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1 };
					FVector3f cell_dist = FVector3f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f, cell_x.Z - p->x.Z + 0.5f);
					FVector3f Q = MultiplyMatrixAndVector(C, cell_dist);

					float mass_contrib = weight * p->mass;

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;
					Cell* cell = m_pGrid[cell_index];

					cell->mass += mass_contrib;
					cell->v += mass_contrib * (p->v + Q);
					m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}

void AMPM3D_Test::UpdateGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* c = m_pGrid[i];

		if (c->mass > 0)
		{
			c->v /= c->mass;
			c->v += dt * FVector3f(0, gravity, 0);

			int x = i / (grid_res * grid_res);
			int y = (i % (grid_res * grid_res)) / grid_res;
			int z = i % grid_res;

			if (x < 2 || x > grid_res - 3)
			{
				c->v.X = 0;
			}
			if (y < 2 || y > grid_res - 3)
			{
				c->v.Y = 0;
			}
			if (z < 2 || z > grid_res - 3)
			{
				c->v.Z = 0;
			}

			m_pGrid[i] = c;
		}
		m_pGrid[i] = c;
	}
}

void AMPM3D_Test::G2P()
{
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		p->v = { 0.f, 0.f, 0.f };

		FIntVector cell_idx = FIntVector(p->x.X, p->x.Y, p->x.Z);
		FVector3f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f, p->x.Z - cell_idx.Z - 0.5f };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2), 0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2), 0.75f - (float)pow(cell_diff.Z, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2), 0.5f * (float)pow(0.5f + cell_diff.Z, 2) });

		FMatrix B;

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					FVector3f dist = { (cell_x.X - p->x.X) + 0.5f, (cell_x.Y - p->x.Y) + 0.5f, (cell_x.Z - p->x.Z) + 0.5f };
					FVector3f weighted_velocity = m_pGrid[cell_index]->v * weight;

					//right calculation
					FMatrix term;
					term.M[0][0] = (weighted_velocity.X * dist.X);
					term.M[1][0] = (weighted_velocity.Y * dist.X);
					term.M[2][0] = (weighted_velocity.Z * dist.X);

					term.M[0][1] = (weighted_velocity.X * dist.Y);
					term.M[1][1] = (weighted_velocity.Y * dist.Y);
					term.M[2][1] = (weighted_velocity.Z * dist.Y);

					term.M[0][2] = (weighted_velocity.X * dist.Z);
					term.M[1][2] = (weighted_velocity.Y * dist.Z);
					term.M[2][2] = (weighted_velocity.Z * dist.Z);

					B = PlusMatrix(term, B);
					p->v += weighted_velocity;
				}
			}
		}
		p->C = ScalingMatrix(B, 1);
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);
		p->x.Z = FMath::Clamp(p->x.Z, 1, grid_res - 2);

		m_pParticles[i] = p;
	}
}

void AMPM3D_Test::Simulate()
{
	ClearGrid();
	P2G();
	UpdateGrid();
	G2P();
}

void AMPM3D_Test::UpdateParticles()
{
	TArray<FTransform> Transforms;

	Transforms.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->x.X * 100.f, m_pParticles[i]->x.Y * 100.f, m_pParticles[i]->x.Z * 100.f));
		Transforms.Add(tempValue);
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, Transforms[i]);
	}
	InstancedStaticMeshComponent->MarkRenderStateDirty();
}

FVector3f AMPM3D_Test::MultiplyMatrixAndVector(FMatrix m, FVector3f v)
{
	float x = m.M[0][0] * v.X + m.M[0][1] * v.Y + m.M[0][2] * v.Z;
	float y = m.M[1][0] * v.X + m.M[1][1] * v.Y + m.M[1][2] * v.Z;
	float z = m.M[2][0] * v.X + m.M[2][1] * v.Y + m.M[2][2] * v.Z;

	return { x,y,z };
}

FMatrix AMPM3D_Test::ScalingMatrix(FMatrix m, float scale)
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

FMatrix AMPM3D_Test::PlusMatrix(FMatrix m1, FMatrix m2)
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
