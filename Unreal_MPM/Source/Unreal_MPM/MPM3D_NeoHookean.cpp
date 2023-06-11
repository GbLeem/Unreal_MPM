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

	const float spacing = 1.f;
	const int box_x = 16;
	const int box_y = 16;
	const int box_z = 16;
	const float sx = grid_res / 2.f;
	const float sy = grid_res / 2.f;
	const float sz = grid_res / 2.f;

	for (float i = - box_x / 2; i < box_x / 2; i += spacing) //-16+16
	{
		for (float j = - box_y / 2; j < box_y / 2; j += spacing)
		{
			for (float k = - box_z / 2; k < box_z / 2; k += spacing)
			{
				auto Pos = FVector3f(sx + i, sy + j, sz + k);
				TempPositions.Add(Pos);
			}
		}
	}

	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles);
	Fs.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = { 0.f,0.f,0.f };
		p->C;
		p->mass = 1.f;

		m_pParticles.Add(p);

		PMatrix<float, 3, 3> temp(1, 1, 1);
		Fs.Add(temp);
	}

	m_pGrid.Empty(NumCells);

	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = new Cell();
		cell->v = { 0.f, 0.f, 0.f };
		m_pGrid.Add(cell);
	}

	P2G();

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FVector3f cell_idx = { (float)floor(p->x.X), (float)floor(p->x.Y) ,(float)floor(p->x.Z) };
		FVector3f cell_diff = { (p->x.X - cell_idx.X - 0.5f), (p->x.Y - cell_idx.Y - 0.5f), (p->x.Z - cell_idx.Z - 0.5f) };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2),0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2), 0.75f - (float)pow(cell_diff.Z, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2), 0.5f * (float)pow(0.5f + cell_diff.Z, 2) });

		float density = 0.f;

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					int cell_index = ((int)(cell_idx.X) + gx - 1) * grid_res * grid_res + ((int)cell_idx.Y + gy - 1) * grid_res + ((int)cell_idx.Z + gz - 1);
					density += m_pGrid[cell_index]->mass * weight;
				}
			}
		}
		float volume = p->mass / density;
		p->volume_0 = volume;

		m_pParticles[i] = p;
	}

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

	for (int i = 0; i < 5; ++i)
	{
		Simulate();
	}
	//Simulate();
	UpdateParticles();
}

void AMPM3D_NeoHookean::ClearGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		cell->mass = 0;
		cell->v = { 0.f,0.f,0.f };

		m_pGrid[i] = cell;
	}
}

void AMPM3D_NeoHookean::P2G()
{
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		PMatrix<float, 3, 3> stress(0, 0, 0, 0, 0, 0, 0, 0, 0);

		PMatrix<float, 3, 3> F = Fs[i];

		float J = F.Determinant();

		float volume = p->volume_0 * J;

		PMatrix<float, 3, 3> F_T = F.GetTransposed();
		PMatrix<float, 3, 3> F_inv_T = F_T.Inverse();
		PMatrix<float, 3, 3> F_minus_F_inv_T = F - F_inv_T;

		PMatrix<float, 3, 3> P_term_0 = elastic_mu * F_minus_F_inv_T;
		PMatrix<float, 3, 3> P_term_1 = elastic_lambda * log(J) * F_inv_T;
		PMatrix<float, 3, 3> P = P_term_0 + P_term_1;

		stress = (1.f / J) * (P * F_T);

		PMatrix<float, 3, 3> eq_16_term_0 = -volume * 2 * stress * dt;

		FIntVector cell_idx = FIntVector(p->x.X, p->x.Y, p->x.Z);
		FVector3f cell_diff = FVector3f(p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f, p->x.Z - cell_idx.Z - 0.5f);

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2),0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2), 0.75f - (float)pow(cell_diff.Z, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2), 0.5f * (float)pow(0.5f + cell_diff.Z, 2) });

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = FVector3f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f, cell_x.Z - p->x.Z + 0.5f);
					FVector3f Q = p->C * cell_dist;

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;
					
					Cell* cell = m_pGrid[cell_index];
					float weighted_mass = weight * p->mass;
					cell->mass += weighted_mass;

					cell->v += weighted_mass * (p->v + Q);

					FVector3f momentum = (eq_16_term_0 * weight) * cell_dist;
					cell->v += momentum;

					m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}


void AMPM3D_NeoHookean::UpdateGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		if (cell->mass > 0)
		{
			cell->v /= cell->mass;
			cell->v += dt * FVector3f(0, 0, gravity);

			int x = i / (grid_res * grid_res);
			int y = (i % (grid_res * grid_res)) / grid_res;
			int z = i % grid_res;

			if (x < 2 || x > grid_res - 3)
			{
				cell->v.X = 0;
			}
			if (y < 2 || y > grid_res - 3)
			{
				cell->v.Y = 0;
			}
			if (z < 2 || z > grid_res - 3)
			{
				cell->v.Z = 0;
			}
			m_pGrid[i] = cell;
		}
	}
}

void AMPM3D_NeoHookean::G2P()
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

		PMatrix<float,3,3> B;

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
					//PMatrix<float, 3, 3> term(weighted_velocity * dist.X, weighted_velocity * dist.Y, weighted_velocity * dist.Z);
					PMatrix<float, 3, 3> term;
					term.M[0][0] = (weighted_velocity.X * dist.X);
					term.M[1][0] = (weighted_velocity.Y * dist.X);
					term.M[2][0] = (weighted_velocity.Z * dist.X);

					term.M[0][1] = (weighted_velocity.X * dist.Y);
					term.M[1][1] = (weighted_velocity.Y * dist.Y);
					term.M[2][1] = (weighted_velocity.Z * dist.Y);

					term.M[0][2] = (weighted_velocity.X * dist.Z);
					term.M[1][2] = (weighted_velocity.Y * dist.Z);
					term.M[2][2] = (weighted_velocity.Z * dist.Z);

					B += term;
					p->v += weighted_velocity;
				}
			}
		}
		p->C = B * 2;
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);
		p->x.Z = FMath::Clamp(p->x.Z, 1, grid_res - 2);

		PMatrix<float, 3, 3> Fp_new(1, 0, 0, 0, 1, 0, 0, 0, 1);

		Fp_new += dt * p->C;
		Fs[i] = Fp_new * Fs[i];

		m_pParticles[i] = p;
	}
}

void AMPM3D_NeoHookean::Simulate()
{
	ClearGrid();
	P2G();
	UpdateGrid();
	G2P();
}

void AMPM3D_NeoHookean::UpdateParticles()
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
