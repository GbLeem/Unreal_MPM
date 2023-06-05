
#include "MPM2D_Fluid.h"

AMPM2D_Fluid::AMPM2D_Fluid()
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

void AMPM2D_Fluid::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FVector2f> TempPositions;

	const float spacing = 0.5f;
	const int box_x = 32;
	const int box_y = 32;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;

	for (float i = sx - box_x / 2; i < sx+box_x / 2; i += spacing)
	{
		for (float j = sx-box_y / 2; j < sx+box_y / 2; j += spacing)
		{
			auto Pos = FVector2f(i, j);
			TempPositions.Add(Pos);
		}
	}

	int po2_amnt = 1; 
	while (po2_amnt <= TempPositions.Num())
		po2_amnt <<= 1;
	NumParticles = po2_amnt >> 1;

	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = { 0.f,0.f };
		p->C = { 0.f, 0.f, 0.f, 0.f };
		p->mass = 1.f;

		m_pParticles.Add(p);
	}

	m_pGrid.Empty(NumCells);

	for (int i = 0; i < NumCells; ++i)
	{
		Cell* c = new Cell();
		c->v = { 0.f, 0.f };
		m_pGrid.Add(c);
	}

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
void AMPM2D_Fluid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*for (int i = 0; i < 5; ++i)
	{
		Simulate();
	}*/
	Simulate();
	UpdateParticles();
}

void AMPM2D_Fluid::ClearGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		cell->mass = 0;
		cell->v = { 0.f,0.f };

		m_pGrid[i] = cell;
	}
}

void AMPM2D_Fluid::P2G_1()
{
	weights.Empty(3);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FIntVector2 cell_idx = FIntVector2(p->x.X, p->x.Y);
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };

		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

		FMatrix2x2 C = p->C;

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X + weights[gy].Y;

				FIntVector2 cell_x = FIntVector2(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				FVector2f cell_dist = FVector2f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f);
				FVector2f Q = MultiplyMatrixAndFloat(C, cell_dist);

				float mass_contrib = weight * p->mass;

				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;
				Cell* cell = m_pGrid[cell_index];

				cell->mass += mass_contrib;
				cell->v += mass_contrib * (p->v + Q);

				m_pGrid[cell_index] = cell;
			}
		}
	}
}

void AMPM2D_Fluid::P2G_2()
{
	weights.Empty(3);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FIntVector2 cell_idx = FIntVector2(p->x.X, p->x.Y);
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };

		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

		float density = 0.f;

		//iterate 3x3 cells
		int gx, gy = 0;
		for (gx = 0; gx < 3; ++gx)
		{
			for (gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				//2d index to 1d index for grid
				int cell_index = ((int)cell_idx.X + gx - 1) * grid_res + ((int)cell_idx.Y + gy - 1);
				density += m_pGrid[cell_index]->mass * weight;
			}
		}

		float volume = p->mass / density;

		float pressure = FMath::Max(-0.1f, eos_stiffness * (pow((density / rest_density), eos_power) - 1));

		FMatrix2x2 stress = { -pressure, 0, 0, -pressure };

		FMatrix2x2 dudv = p->C;
		FMatrix2x2 strain = dudv;

		float a, b, c, d = 0;
		strain.GetMatrix(a, b, c, d);

		float trace = b + c;
		c = b = trace;

		FMatrix2x2 viscosity_term = ScalingMatrix(strain, dynamic_viscosity);
		stress = PlusMatrix(stress, viscosity_term);

		FMatrix2x2 eq_16_term_0 = ScalingMatrix(stress, -volume * 4 * dt);

		for (gx = 0; gx < 3; ++gx) {
			for (gy = 0; gy < 3; ++gy) {
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1 };
				FVector2f cell_dist = { (cell_x.X - p->x.X) + 0.5f, (cell_x.Y - p->x.Y) + 0.5f };

				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;
				Cell* cell = m_pGrid[cell_index];

				// fused force + momentum contribution from MLS-MPM
				float a1, b1, c1, d1 = 0;
				eq_16_term_0.GetMatrix(a1, b1, c1, d1);
				FVector2f momentum = { a * weight * cell_dist.X + b * weight * cell_dist.Y, c * weight * cell_dist.X + d * weight * cell_dist.Y };
				cell->v += momentum;

				m_pGrid[cell_index] = cell;
			}
		}
	}
}

void AMPM2D_Fluid::UpdateGrid()
{
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		if (cell->mass > 0)
		{
			cell->v /= cell->mass;
			cell->v += FVector2f(0, gravity * dt);

			int x = i / grid_res;
			int y = i % grid_res;
			if (x < 2 || x > grid_res - 3)
			{
				cell->v.X = 0;
			}
			if (y < 2 || y > grid_res - 3)
			{
				cell->v.Y = 0;
			}
			m_pGrid[i] = cell;
		}
	}
}

void AMPM2D_Fluid::G2P()
{
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

		FMatrix2x2 B = FMatrix2x2(0.f, 0.f, 0.f, 0.f);

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1 };
				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;

				FVector2f dist = { (cell_x.X - p->x.X) + 0.5f, (cell_x.Y - p->x.Y) + 0.5f };
				FVector2f weighted_velocity = m_pGrid[cell_index]->v * weight;

				//right calculation
				FMatrix2x2 term = FMatrix2x2(weighted_velocity.X * dist.X, weighted_velocity.X * dist.Y,
					weighted_velocity.Y * dist.X, weighted_velocity.Y * dist.Y);

				B = PlusMatrix(B, term);

				p->v += weighted_velocity;
			}
		}
		p->C = ScalingMatrix(B, 2);

		//advect particles
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);

		FVector2f x_n = p->x + p->v;
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

		m_pParticles[i] = p;
	}
}

void AMPM2D_Fluid::UpdateParticles()
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

void AMPM2D_Fluid::Simulate()
{
	ClearGrid();
	P2G_1(); //P2G
	P2G_2(); //P2G
	UpdateGrid(); //grid velocity update
	G2P(); //G2P
}


FMatrix2x2 AMPM2D_Fluid::Transpose(FMatrix2x2 originMatrix)
{
	FMatrix2x2 resultMatrix;
	float a, b, c, d = 0.f;
	originMatrix.GetMatrix(a, b, c, d);
	resultMatrix = { a,c,b,d };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_Fluid::MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;
	float a1, b1, c1, d1, a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	resultMatrix = { a1 - a2, b1 - b2, c1 - c2, d1 - d2 };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_Fluid::PlusMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;
	float a1, b1, c1, d1, a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	resultMatrix = { a1 + a2, b1 + b2, c1 + c2, d1 + d2 };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_Fluid::MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;

	float a1, b1, c1, d1 = 0.f;
	float a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);

	resultMatrix = { a1 * a2 + b1 * c2, a1 * b2 + b1 * d2, c1 * a2 + d1 * c2, c1 * b2 + d1 * d2 };

	return resultMatrix;
}

FVector2f AMPM2D_Fluid::MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1)
{
	FVector2f resultVector;

	float a, b, c, d = 0.f;
	m1.GetMatrix(a, b, c, d);

	resultVector = { a * v1.X + b * v1.Y, c * v1.X + d * v1.Y };

	return resultVector;
}

FMatrix2x2 AMPM2D_Fluid::TraceCalculation(FMatrix2x2 m1, float trace)
{
	float a, b, c, d;
	m1.GetMatrix(a, b, c, d);

	trace = b + c;
	c = b = trace;
	return FMatrix2x2();
}

