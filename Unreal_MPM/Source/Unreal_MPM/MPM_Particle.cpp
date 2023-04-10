#include "MPM_Particle.h"

// Sets default values
AMPM_Particle::AMPM_Particle()
	:NumParticles(0)
	,NumParticlesForInstancedStaticMesh(64)
	,grid_res(64)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InstancedParticle = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	SetRootComponent(InstancedParticle);

	InstancedParticle->SetMobility(EComponentMobility::Movable);
	InstancedParticle->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedParticle->SetGenerateOverlapEvents(false);
}

AMPM_Particle::~AMPM_Particle()
{

}

void AMPM_Particle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//if (InstancedParticle->GetInstanceCount() == 0)
	//{
	//	TArray<FTransform> Transforms;
	//
	//	Transforms.Empty(NumParticlesForInstancedStaticMesh);
	//	//TransformContainer.reserve(NumParticles);
	//	for (int index = 0; index < NumParticlesForInstancedStaticMesh / 8; ++index)
	//	{
	//		for (int index2 = 0; index2 < NumParticlesForInstancedStaticMesh / 8; ++index2)
	//		{
	//			Transforms.Add(FTransform(FVector(100.f * index, 100.f*index2, 0.f)));
	//		}
	//	}

	//	InstancedParticle->AddInstances(Transforms, false);
	//}

	//=================COLOR TEST============================
	/*TArray<float> CustomData;
	CustomData.Init(0.f, 3);

	for (int index = 0; index < NumParticles; ++index)
	{
		CustomData[0] = FMath::RandRange(0.f, 1.f);
		CustomData[1] = FMath::RandRange(0.f, 1.f);
		CustomData[2] = FMath::RandRange(0.f, 1.f);

		InstancedParticle->SetCustomData(index, CustomData, false);
	}
	InstancedParticle->MarkRenderStateDirty();*/
}

void AMPM_Particle::InitGrid()
{
	//TArray<FVector2f> TempPositions;
	const float spacing = 1.0f;
	const int box_x = 16;
	const int box_y = 16;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;

	for (float i = sx - box_x / 2; i < sx + box_x / 2; i += spacing) 
	{
		for (float j = sy - box_y / 2; j < sy + box_y / 2; j += spacing) 
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
		// random initial velocity
		float RandNum = FMath::RandRange(0.f, 1.f);
		p->v = FVector2f(RandNum - 0.5f, RandNum - 0.5f + 2.75f) * 0.5f;
		p->C = FMatrix2x2(1, 0, 0, 1); //identity matrix
		p->mass = 1.0f;

		m_pParticles.Add(p);
	}

	for (int i = 0; i < num_cells; ++i) 
	{
		Cell* TempCell = new Cell();
		m_pGrid.Add(TempCell);
	}

	UE_LOG(LogTemp, Log, TEXT("Particle Num: %d"), m_pParticles.Num());
	UE_LOG(LogTemp, Log, TEXT("Cell Num: %d"), m_pGrid.Num());
	for (int i = 0; i < NumParticles; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("%d's Particle Pos: %f %f"), i+1, m_pParticles[i]->x.X, m_pParticles[i]->x.Y);
	}
}

void AMPM_Particle::Simulate()
{
	// reset grid scratchpad
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->v = { 0.f,0.f };
	}

	// P2G
	for (auto& p : m_pParticles)
	{
		// quadratic interpolation weights
		FIntPoint cell_idx = static_cast<FIntPoint>(p->x.X, p->x.Y);
		FVector2f cell_diff = (p->x - FVector2f(cell_idx)) - 0.5f;
		m_weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) }); 
		m_weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2) });
		m_weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) });


		// for all surrounding 9 cells
		for (uint32 gx = 0; gx < 3; ++gx)
		{
			for (uint32 gy = 0; gy < 3; ++gy)
			{
				float weight = m_weights[gx][0] * m_weights[gy][1];
				
				FIntPoint cell_x = FIntPoint(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				FVector2D cell_dist = { (cell_x.X - p->x.X) + 0.5f ,(cell_x.Y - p->x.Y) + 0.5f };
				float a, b, c, d; //for matrix multiply
				p->C.GetMatrix(a, b, c, d);
				FVector2D Q = { a * cell_dist.X + b * cell_dist.Y, c * cell_dist.X + d * cell_dist.Y };

				// MPM course, equation 172
				float mass_contrib = weight * p->mass;

				// converting 2D index to 1D
				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;
				Cell* cell = m_pGrid[cell_index]; //[TODO] index error

				// scatter mass to the grid
				cell->mass += mass_contrib;

				cell->v += mass_contrib * FVector2f(p->v.X + Q.X, p->v.Y + Q.Y);
			}
			
		}
	}

	int indexOfCell = 0;

	//grid velocity update
	for (auto& c : m_pGrid)
	{
		if (c->mass > 0)
		{
			//convert momentum to velocity, apply gravity
			c->v /= c->mass;
			c->v += dt * FVector2f(0, m_gravity);

			//boundary conditions
			int x = indexOfCell / grid_res;
			int y = indexOfCell % grid_res;
			
			if (x<2 || x>grid_res - 3)
			{
				c->v.X = 0;
			}
			if (y<2 || y>grid_res - 3)
			{
				c->v.Y = 0;
			}
		}
		indexOfCell += 1;
	}

	//G2P
	for (auto& p : m_pParticles)
	{
		// reset particle velocity. we calculate it from scratch each step using the grid
		p->v = { 0.f,0.f };

		// quadratic interpolation weights
		FIntPoint cell_idx = static_cast<FIntPoint>(p->x.X, p->x.Y);
		FVector2f cell_diff = (p->x - cell_idx) - 0.5f;
		m_weights[0] = { 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) };
		m_weights[1] = { 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2) };
		m_weights[2] = { 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) };

		FMatrix2x2 B(0, 0, 0, 0);

		for (int gx = 0; gx < 3; ++gx) 
		{
			for (int gy = 0; gy < 3; ++gy) 
			{
				float weight = m_weights[gx].X * m_weights[gy].Y;

				FIntPoint cell_x = FIntPoint(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;

				//FVector2D cell_dist = { (cell_x.X - p->x.X) + 0.5f ,(cell_x.Y - p->x.Y) + 0.5f };
				FVector2f dist = (cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f);
				FVector2f weighted_velocity = m_pGrid[cell_index]->v * weight;

				// APIC paper equation 10, constructing inner term for B
				auto term = FMatrix2x2(weighted_velocity.X * dist.X, 0, 0, weighted_velocity.Y * dist.Y);

				float a1, b1, c1, d1;
				B.GetMatrix(a1, b1, c1, d1);

				float a2, b2, c2, d2;
				term.GetMatrix(a2, b2, c2, d2);

				B = FMatrix2x2(a1 + a2, b1 + b2, c1 + c2, d1 + d2);

				p->v += weighted_velocity;
			}
		}

		float a, b, c, d;
		B.GetMatrix(a, b, c, d);

		p->C = FMatrix2x2(a * 4, b * 4, c * 4, d * 4);

		//advect particles
		p->x += p->v * dt;

		//safety clamp to ensure particles don't exit simulation domain
		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);
	}
}

void AMPM_Particle::UpdateParticle()
{
	TArray<FTransform> Transforms;

	Transforms.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->x.X * 100.f, m_pParticles[i]->x.Y * 100.f, 0.0f));
		Transforms.Add(tempValue);
	}

	for (int i = 0; i < NumParticles; ++i)
	{
		InstancedParticle->UpdateInstanceTransform(i, Transforms[i], false, false, true);
	}

	InstancedParticle->MarkRenderStateDirty(); 
}


void AMPM_Particle::BeginPlay()
{
	Super::BeginPlay();

	InitGrid();

	if (InstancedParticle->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(NumParticles);

		for (int i = 0; i < NumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(TempPositions[i].X * 100.f, TempPositions[i].Y * 100.f, 0.0f));
			Transforms.Add(tempValue);
		}

		InstancedParticle->AddInstances(Transforms, false);
	}
}

void AMPM_Particle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Simulate();
	for (int i = 0; i < 10; ++i)
	{
		UpdateParticle();
	}
	//Simulate();
	//UpdateParticle();

	////[TODO] 모든 particle이 한번 수행 후 (1,1)로 가버림
	UE_LOG(LogTemp, Log, TEXT("%d's Particle Pos: %f %f"), 0, m_pParticles[0]->x.X, m_pParticles[0]->x.Y);

	/*UE_LOG(LogTemp, Log, TEXT("Particle Num: %d"), m_pParticles.Num());

	for (int i = 0; i < NumParticles; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("%d's Particle Pos: %f %f"), i + 1, m_pParticles[i]->x.X, m_pParticles[i]->x.Y);
	}*/
}


