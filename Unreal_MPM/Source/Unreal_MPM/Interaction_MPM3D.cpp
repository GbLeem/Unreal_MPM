// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_MPM3D.h"

// Sets default values
AInteraction_MPM3D::AInteraction_MPM3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));

	SetRootComponent(InstancedStaticMeshComponent);

	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);
}

void AInteraction_MPM3D::Initialize()
{
	const float spacing = 1.0f;

	//make 4*4*4 mesh
	for (int i = 4; i < 9; i++)
	{
		for (int j = 4; j < 9; j++)
		{
			for (int k = 4; k < 9; k++)
			{
				auto Pos = FVector3f(i, j, k);
				TempPositions1.Add(Pos);
			}
		}
	}

	//make 4*4*4 mesh - upper mesh
	for (int i = 4; i < 9; i++)
	{
		for (int j = 10; j < 15; j++)
		{
			for (int k = 4; k < 9; k++)
			{
				auto Pos = FVector3f(i, j, k);
				TempPositions2.Add(Pos);
			}
		}
	}

	//create particles
	NumParticles1 = TempPositions1.Num();
	NumParticles2 = TempPositions2.Num();
	TotalNumParticles = NumParticles1 + NumParticles2;

	m_pParticles.Empty(TotalNumParticles); //make particle array

	for (int i = 0; i < NumParticles1; ++i)
	{
		Particle* p = new Particle();
		p->Pos = TempPositions1[i];
		p->Vel = { 0.f, 0.f, 0.f };
		p->C = FMatrix::Identity; //identity matrix
		p->mass = 1.f;
		p->First = true;
		m_pParticles.Add(p);
	}

	for (int i = 0; i < NumParticles2; ++i)
	{
		Particle* p = new Particle();
		p->Pos = TempPositions2[i];
		p->Vel = { 0.f, 0.f, 0.f };
		p->C = FMatrix::Identity; //identity matrix
		p->mass = 1.f;
		p->First = false;
		m_pParticles.Add(p);
	}

	//fill grid
	m_pGrid.Empty(num_cells);//make grid array
	for (int i = 0; i < num_cells; ++i)
	{
		Cell* TempCell = new Cell();
		TempCell->Vel = { 0.f, 0.f, 0.f };
		m_pGrid.Add(TempCell);
	}
}

void AInteraction_MPM3D::SimulatingPipeLine(double timestep)
{
	ClearGrid();
	P2GFirst();
	UpdateGrid(timestep);
	G2P(timestep);
}

void AInteraction_MPM3D::UpdateParticle()
{
	TArray<FTransform> Transforms;

	//Transforms.Empty(TotalNumParticles);
	Transforms.Empty(TotalNumParticles);

	for (int i = 0; i < TotalNumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->Pos.X * 100.f, m_pParticles[i]->Pos.Y * 100.f, m_pParticles[i]->Pos.Z * 100.f));
		Transforms.Add(tempValue);
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, Transforms[i]);
	}
	InstancedStaticMeshComponent->MarkRenderStateDirty();
}

void AInteraction_MPM3D::AddForce()
{
	for (int i = 0; i < NumParticles1; ++i)
	{
		m_pParticles[i]->Vel += FVector3f{ 0.f, 0.02f, 0.f };
	}
}

void AInteraction_MPM3D::ClearGrid()
{
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->Vel = { 0.f,0.f,0.f }; 
	}
}

void AInteraction_MPM3D::P2GFirst()
{
	TArray<FVector3f> m_weights;

	for (auto& p : m_pParticles)
	{
		//calculate weights for 3x3 neighbor cells that surrouding particle pos -> by Eulerian Interpolating Function (course - 8.1)
		m_weights.Empty(3);
		FIntVector cell_idx = static_cast<FIntVector>(p->Pos.X, p->Pos.Y, p->Pos.Z);
		FVector3f cell_diff = { (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };
		m_weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		FMatrix C = p->C;

		//UE_LOG(LogTemp, Log, TEXT("matrix C's value: %f, %f,%f,%f"), C.M[0][0], C.M[1][1], C.M[2][2], C.M[3][3]);

		//Scatter particle's momentum -> by Eulerian interpolation function
		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				for (UINT gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = { (cell_x.X - p->Pos.X) + 0.5f, (cell_x.Y - p->Pos.Y) + 0.5f, (cell_x.Z - p->Pos.Z) + 0.5f };

					//[FIX] - new version
					FVector3d Q = { C.M[0][0] * cell_dist.X, C.M[1][1] * cell_dist.Y, C.M[2][2] * cell_dist.Z };
					float mass_contrib = weight * p->mass;

					//convert index to 1d
					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					//[FIX] - 5/2 
					for (auto& cell : m_pGrid)
					{
						cell->mass += mass_contrib;
						cell->Vel += mass_contrib * FVector3f{ (p->Vel.X + (float)Q.X),(p->Vel.Y + (float)Q.Y),(p->Vel.Z + (float)Q.Z) };
					}
				}
			}
		}
	}
}

void AInteraction_MPM3D::UpdateGrid(double timestep)
{
	//calculate grid velocities
	int gridIndex = 0;

	for (auto& c : m_pGrid)
	{
		if (c->mass > 0)
		{
			//calculate grid velocity based on P2G's momentum (Cell->Velocity = momentum)
			//convert momentum to velocity, apply gravity

			c->Vel /= c->mass; //make real velocity -> mass*velocity = momentum
			c->Vel += timestep * FVector3f(0, gravity, 0);

			//boundary conditions
			int x = gridIndex / (grid_res * grid_res);
			int y = (gridIndex % (grid_res * grid_res)) / grid_res;
			int z = gridIndex % grid_res;

			if (x < 2 || x > grid_res - 3)
			{
				c->Vel.X = 0;
			}
			if (y < 2 || y > grid_res - 3)
			{
				c->Vel.Y = 0;
			}
			if (z < 2 || z > grid_res - 3)
			{
				c->Vel.Z = 0;
			}
		}
		++gridIndex;
	}
}

void AInteraction_MPM3D::G2P(double timestep)
{
	TArray<FVector3f> m_weights;
	
	int particleIdx = 0;

	//[TEST] for mesh check
	int firstCnt = 0;
	int secondCnt = 0;
	int i = 0;

	//for (auto& p : m_pParticles)
	for (i = 0; i < TotalNumParticles; ++i)
	{
		//reset particle velocity
		m_pParticles[i]->Vel = {0.f,0.f,0.f};

		//calculate neighbouring cell weights
		FIntVector cell_idx = static_cast<FIntVector>(m_pParticles[i]->Pos.X, m_pParticles[i]->Pos.Y, m_pParticles[i]->Pos.Z);
		FVector3f cell_diff = { (m_pParticles[i]->Pos.X - cell_idx.X - 0.5f),(m_pParticles[i]->Pos.Y - cell_idx.Y - 0.5f),(m_pParticles[i]->Pos.Z - cell_idx.Z - 0.5f)};

		//using interpolation function
		m_weights.Empty(3);
		m_weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		//calculate new particle velocity
		FMatrix B;

		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				for (UINT gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					FVector3f dist = { (cell_x.X - m_pParticles[i]->Pos.X) + 0.5f, (cell_x.Y - m_pParticles[i]->Pos.Y) + 0.5f, (cell_x.Z - m_pParticles[i]->Pos.Z) + 0.5f };
					FVector3f weighted_velocity = m_pGrid[cell_index]->Vel * weight;

					auto term = FMatrix::Identity;
					term.M[0][0] = weighted_velocity.X * dist.X;
					term.M[1][0] = weighted_velocity.X * dist.X;
					term.M[2][0] = weighted_velocity.X * dist.X;

					term.M[0][1] = weighted_velocity.Y * dist.Y;
					term.M[1][1] = weighted_velocity.Y * dist.Y;
					term.M[2][1] = weighted_velocity.Y * dist.Y;

					term.M[0][2] = weighted_velocity.Z * dist.Z;
					term.M[1][2] = weighted_velocity.Z * dist.Z;
					term.M[2][2] = weighted_velocity.Z * dist.Z;

					B += term;

					m_pParticles[i]->Vel += weighted_velocity;
				}
			}
		}
		m_pParticles[i]->C = B * 4;

		//advect particles
		m_pParticles[i]->Pos += m_pParticles[i]->Vel * timestep;

		//safety clamp 
		m_pParticles[i]->Pos.X = FMath::Clamp(m_pParticles[i]->Pos.X, 1, grid_res - 2);
		m_pParticles[i]->Pos.Y = FMath::Clamp(m_pParticles[i]->Pos.Y, 1, grid_res - 2);
		m_pParticles[i]->Pos.Z = FMath::Clamp(m_pParticles[i]->Pos.Z, 1, grid_res - 2);
		
		/*{
			FVector3f force = { 0.0f, 0.1f, 0.f };
			m_pParticles[i]->Vel += force;
		}*/
		//AddForce();

		//boundaries
		FVector3f x_n = m_pParticles[i]->Pos + m_pParticles[i]->Vel;

		const float wall_min = 1;
		//float wall_max = (float)(grid_res - 1) - wall_min;
		float wall_max = (float)(32 - 1) - wall_min; //[TEST] 5/16 for boundary condition
		
		if (x_n.X < wall_min)
			m_pParticles[i]->Vel.X += wall_min - x_n.X;
		if (x_n.X > wall_max)
			m_pParticles[i]->Vel.X += wall_max - x_n.X;

		if (x_n.Y < wall_min)
			m_pParticles[i]->Vel.Y += wall_min - x_n.Y;
		if (x_n.Y > wall_max)
			m_pParticles[i]->Vel.Y += wall_max - x_n.Y;

		if (x_n.Z < wall_min)
			m_pParticles[i]->Vel.Z += wall_min - x_n.Z;
		if (x_n.Z > wall_max)
			m_pParticles[i]->Vel.Z += wall_max - x_n.Z;
		
		particleIdx++;
	}

	//int j = 0;
	//for (j = NumParticles1; j < TotalNumParticles; ++j)
	//{
	//	//reset particle velocity
	//	m_pParticles[j]->Vel = { 0.f,0.f,0.f };

	//	//calculate neighbouring cell weights
	//	FIntVector cell_idx = static_cast<FIntVector>(m_pParticles[j]->Pos.X, m_pParticles[j]->Pos.Y, m_pParticles[j]->Pos.Z);
	//	FVector3f cell_diff = { (m_pParticles[j]->Pos.X - cell_idx.X - 0.5f),(m_pParticles[j]->Pos.Y - cell_idx.Y - 0.5f),(m_pParticles[j]->Pos.Z - cell_idx.Z - 0.5f) };

	//	//using interpolation function
	//	m_weights.Empty(3);
	//	m_weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
	//	m_weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
	//	m_weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

	//	//calculate new particle velocity
	//	FMatrix B;

	//	for (UINT gx = 0; gx < 3; ++gx)
	//	{
	//		for (UINT gy = 0; gy < 3; ++gy)
	//		{
	//			for (UINT gz = 0; gz < 3; ++gz)
	//			{
	//				float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

	//				FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
	//				int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

	//				FVector3f dist = { (cell_x.X - m_pParticles[j]->Pos.X) + 0.5f, (cell_x.Y - m_pParticles[j]->Pos.Y) + 0.5f, (cell_x.Z - m_pParticles[j]->Pos.Z) + 0.5f };
	//				FVector3f weighted_velocity = m_pGrid[cell_index]->Vel * weight;

	//				auto term = FMatrix::Identity;
	//				term.M[0][0] = weighted_velocity.X * dist.X;
	//				term.M[1][0] = weighted_velocity.X * dist.X;
	//				term.M[2][0] = weighted_velocity.X * dist.X;

	//				term.M[0][1] = weighted_velocity.Y * dist.Y;
	//				term.M[1][1] = weighted_velocity.Y * dist.Y;
	//				term.M[2][1] = weighted_velocity.Y * dist.Y;

	//				term.M[0][2] = weighted_velocity.Z * dist.Z;
	//				term.M[1][2] = weighted_velocity.Z * dist.Z;
	//				term.M[2][2] = weighted_velocity.Z * dist.Z;

	//				B += term;

	//				m_pParticles[j]->Vel += weighted_velocity;
	//			}
	//		}
	//	}
	//	m_pParticles[j]->C = B * 4;

	//	//advect particles
	//	m_pParticles[j]->Pos += m_pParticles[j]->Vel * timestep;

	//	//safety clamp 
	//	m_pParticles[j]->Pos.X = FMath::Clamp(m_pParticles[j]->Pos.X, 1, grid_res - 2);
	//	m_pParticles[j]->Pos.Y = FMath::Clamp(m_pParticles[j]->Pos.Y, 1, grid_res - 2);
	//	m_pParticles[j]->Pos.Z = FMath::Clamp(m_pParticles[j]->Pos.Z, 1, grid_res - 2);

	//	{
	//		FVector3f force = { 0.0f, -0.1f, 0.f };
	//		m_pParticles[j]->Vel += force;
	//	}


	//	//boundaries
	//	FVector3f x_n = m_pParticles[j]->Pos + m_pParticles[j]->Vel;

	//	const float wall_min = 1;
	//	//float wall_max = (float)(grid_res - 1) - wall_min;
	//	float wall_max = (float)(32 - 1) - wall_min; //[TEST] 5/16 for boundary condition

	//	if (x_n.X < wall_min)
	//		m_pParticles[j]->Vel.X += wall_min - x_n.X;
	//	if (x_n.X > wall_max)
	//		m_pParticles[j]->Vel.X += wall_max - x_n.X;

	//	if (x_n.Y < wall_min)
	//		m_pParticles[j]->Vel.Y += wall_min - x_n.Y;
	//	if (x_n.Y > wall_max)
	//		m_pParticles[j]->Vel.Y += wall_max - x_n.Y;

	//	if (x_n.Z < wall_min)
	//		m_pParticles[j]->Vel.Z += wall_min - x_n.Z;
	//	if (x_n.Z > wall_max)
	//		m_pParticles[j]->Vel.Z += wall_max - x_n.Z;

	//	particleIdx++;
	//}

	//UE_LOG(LogTemp, Log, TEXT("first mesh : %d / second mesh : %d"), firstCnt, secondCnt);
}

// Called when the game starts or when spawned
void AInteraction_MPM3D::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();

	if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(TotalNumParticles);

		for (int i = 0; i < TotalNumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(m_pParticles[i]->Pos.X * 100.f, m_pParticles[i]->Pos.Y * 100.f, m_pParticles[i]->Pos.Z * 100.f));
			Transforms.Add(tempValue);
		}
		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}
}

// Called every frame
void AInteraction_MPM3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SimulatingPipeLine(timesteps);
	UpdateParticle();

	/*UE_LOG(LogTemp, Log, TEXT("Particle Nums %d / %d / %d"), NumParticles1, NumParticles2, TotalNumParticles);
	UE_LOG(LogTemp, Log, TEXT("m_pParticles %d"), m_pParticles.Num());*/
	//UE_LOG(LogTemp, Log, TEXT("ball location: %f / %f / %f"), m_pMesh->GetComponentLocation().X, m_pMesh->GetComponentLocation().Y, m_pMesh->GetComponentLocation().Y);

}