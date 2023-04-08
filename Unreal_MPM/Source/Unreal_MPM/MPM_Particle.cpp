#include "MPM_Particle.h"

// Sets default values
AMPM_Particle::AMPM_Particle()
	:NumParticles()
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

void AMPM_Particle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (InstancedParticle->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;
	
		Transforms.Empty(NumParticles);
		//TransformContainer.reserve(NumParticles);
		for (int index = 0; index < NumParticles / 8; ++index)
		{
			for (int index2 = 0; index2 < NumParticles / 8; ++index2)
			{
				Transforms.Add(FTransform(FVector(100.f * index, 100.f*index2, 0.f)));
			}
		}

		InstancedParticle->AddInstances(Transforms, false);
	}

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
	TArray<FVector2f> TempPositions;
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


	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		// random initial velocity
		float RandNum = FMath::RandRange(0.f, 1.f);
		p->v = FVector2f(RandNum - 0.5f, RandNum - 0.5f + 2.75f) * 0.5f;
		p->C = FMatrix2x2(1, 0, 0, 1); //identity matrix
		p->mass = 1.0f;
		//m_pParticles[i] = p; //[TODO] index error -> maybe need to reserve TArray's size
	}

	//for (int i = 0; i < num_cells; ++i) 
	//{
	//	//m_pGrid[i] = new Cell(); [TODO] index error
	//}
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
		FVector2f cell_diff = (p->x - cell_idx) - 0.5f;
		m_weights[0] = { 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) };
		m_weights[1] = { 0.75f * FMath::Pow(cell_diff.X, 2), 0.75f * FMath::Pow(cell_diff.Y, 2) };
		m_weights[2] = { 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) };


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
				Cell* cell = m_pGrid[cell_index];

				// scatter mass to the grid
				cell->mass += mass_contrib;

				cell->v += mass_contrib * FVector2f(p->v.X + Q.X, p->v.Y + Q.Y);
			}
			
		}
	}
}

void AMPM_Particle::BeginPlay()
{
	Super::BeginPlay();
	//InitGrid();
}

void AMPM_Particle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//=========MOVING TEST===========================
	/*TArray<FTransform> OffsetTransformContainer;
	OffsetTransformContainer.Empty(NumParticles);

	for (int index = 0; index < NumParticles / 8; ++index)
	{
		for (int index2 = 0; index2 < NumParticles / 8; ++index2)
		{
			const float Offset = 100.f * sin(2 * PI * 1.f * GetWorld()->GetTimeSeconds());

			FTransform OffsetTransform = FTransform(FVector(100.f * index + Offset, 100.f * index2 + Offset, 0.f));
			OffsetTransformContainer.Add(OffsetTransform);
		}
	}
	for (int i = 0; i < NumParticles; ++i)
	{
		InstancedParticle->UpdateInstanceTransform(i, OffsetTransformContainer[i], false, false, true);
	}
	InstancedParticle->MarkRenderStateDirty();*/
}


