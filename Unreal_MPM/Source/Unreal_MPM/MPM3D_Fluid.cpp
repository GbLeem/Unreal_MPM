
#include "MPM3D_Fluid.h"

AMPM3D_Fluid::AMPM3D_Fluid()
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

void AMPM3D_Fluid::BeginPlay()
{
	Super::BeginPlay();

	TArray<FVector3f> TempPositions;

	const float spacing = 0.5f;
	const int box_x = 16;
	const int box_y = 16;
	const int box_z = 8;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;
	const float sz = grid_res / 2.0f;

	for (float i = sx - box_x / 2; i < sx + box_x / 2; i += spacing) //4~12 
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

	//round number of particles
	int po2_amnt = 1;
	while (po2_amnt <= TempPositions.Num())
		po2_amnt <<= 1;
	NumParticles = po2_amnt >> 1;

	//create particles
	//NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles); //make particle array

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = { 0.f, 0.f, 0.f };
		p->C = (0, 0, 0, 0, 0, 0, 0, 0, 0);
		p->mass = 1.f;
		m_pParticles.Add(p);
	}

	m_pGrid.Empty(NumCells);//make grid array

	for (int i = 0; i < NumCells; ++i)
	{
		Cell* TempCell = new Cell();
		TempCell->v = { 0.f, 0.f, 0.f };

		m_pGrid.Add(TempCell);
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
void AMPM3D_Fluid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < iterations; ++i)
	{
		Simulate();
	}
	//Simulate();
	UpdateParticles();
}

void AMPM3D_Fluid::ClearGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		cell->mass = 0;
		cell->v = { 0.f,0.f,0.f };

		m_pGrid[i] = cell;
	}
}

void AMPM3D_Fluid::P2G_1()
{
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FIntVector cell_idx = FIntVector(p->x.X, p->x.Y, p->x.Z);
		FVector3f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f, p->x.Z - cell_idx.Z - 0.5f };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2), 0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2), 0.75f - (float)pow(cell_diff.Z, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2), 0.5f * (float)pow(0.5f + cell_diff.Z, 2) });

		PMatrix<float,3,3> C = p->C;

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = FVector3f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f, cell_x.Z - p->x.Z + 0.5f);
					FVector3f Q = C * cell_dist;

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

void AMPM3D_Fluid::P2G_2()
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

		float density = 0.f;
		int gx, gy, gz;

		for (gx = 0; gx < 3; ++gx)
		{
			for (gy = 0; gy < 3; ++gy)
			{
				for (gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;
					int cell_index = (int)(cell_idx.X + gx - 1) * grid_res * grid_res + (int)(cell_idx.Y + gy - 1) * grid_res + (int)(cell_idx.Z + gz - 1);
					density += m_pGrid[cell_index]->mass * weight;
				}
			}
		}

		float volume = p->mass / density;

		float pressure = FMath::Max(-0.1f, eos_stiffness * (pow(density / rest_density, eos_power) - 1));
		
		PMatrix<float, 3, 3> stress(-pressure, 0, 0, 0, -pressure, 0, 0, 0, -pressure);

		PMatrix<float, 3, 3> dudv = p->C;
		PMatrix<float, 3, 3> strain = dudv;

		float trace = strain.M[0][2] + strain.M[1][1] + strain.M[2][0];
		strain.M[2][0] = strain.M[1][1] = strain.M[0][2] = trace;

		PMatrix<float, 3, 3> viscosity_term = dynamic_viscosity * strain;
		stress += viscosity_term;

		PMatrix<float, 3, 3> eq_16_term_0 = -volume * 1 * stress * dt;

		for (gx = 0; gx < 3; ++gx)
		{
			for (gy = 0; gy < 3; ++gy)
			{
				for (gz = 0; gz < 3; ++gz)
				{
					float weight = weights[gx].X * weights[gy].Y * weights[gz].Z;

					FIntVector cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1 };
					FVector3f cell_dist = FVector3f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f, cell_x.Z - p->x.Z + 0.5f);

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;
					Cell* cell = m_pGrid[cell_index];

					FVector3f momentum = (eq_16_term_0 * weight) * cell_dist;
					cell->v += momentum;

					m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}

void AMPM3D_Fluid::UpdateGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* c = m_pGrid[i];

		if (c->mass > 0)
		{
			c->v /= c->mass;
			c->v += dt * FVector3f(0, 0, gravity);

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
	}
}

void AMPM3D_Fluid::G2P()
{
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		p->v = { 0.f,0.f,0.f };

		FIntVector cell_idx = FIntVector(p->x.X, p->x.Y, p->x.Z);
		FVector3f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f, p->x.Z - cell_idx.Z - 0.5f };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2),0.5f * (float)pow(0.5f - cell_diff.Z, 2) });
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

					FIntVector cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1 };
					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					FVector3f dist = { cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f, cell_x.Z - p->x.Z + 0.5f };
					FVector3f weighted_velocity = m_pGrid[cell_index]->v * weight;

					PMatrix<float,3,3> term;
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
		p->C = B * 1;
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);
		p->x.Z = FMath::Clamp(p->x.Z, 1, grid_res - 2);


		/*{
			FVector3f force = { 0.f, 0.1f, 0.f };
			p->v += force;
		}*/

		FVector3f x_n = p->x + p->v;
		const float wall_min = 3;
		float wall_max = (float)grid_res - 4;

		if (x_n.X < wall_min)
			p->v.X += wall_min - x_n.X;
		if (x_n.X > wall_max)
			p->v.X += wall_max - x_n.X;

		if (x_n.Y < wall_min)
			p->v.Y += wall_min - x_n.Y;
		if (x_n.Y > wall_max)
			p->v.Y += wall_max - x_n.Y;

		if (x_n.Z < wall_min)
			p->v.Z += wall_min - x_n.Z;
		if (x_n.Z > wall_max)
			p->v.Z += wall_max - x_n.Z;

		m_pParticles[i] = p;
	}
}

void AMPM3D_Fluid::Simulate()
{
	ClearGrid();
	P2G_1();
	P2G_2();
	UpdateGrid();
	G2P();
}

void AMPM3D_Fluid::UpdateParticles()
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

