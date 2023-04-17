// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM3D.h"

// Sets default values
AMPM3D::AMPM3D()
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

AMPM3D::~AMPM3D()
{
	//destroy
}

void AMPM3D::Initialize()
{
	//init Grid
	const float spacing = 0.5f;
	/*const int box_x = 16;
	const int box_y = 16;
	const int box_z = 16;*/
	const int box_x = 8;
	const int box_y = 8;
	const int box_z = 8;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;
	const float sz = grid_res / 2.0f;

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

	//[TODO] understand this code
	/*int po2_amnt = 1; 
	while (po2_amnt <= TempPositions.Num())
		po2_amnt <<= 1;
	NumParticles = po2_amnt >> 1;*/

	NumParticles = TempPositions.Num();

	m_pParticles.Empty(NumParticles); //make particle array

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->Pos = TempPositions[i];
		p->Vel = { 0.f, 0.f, 0.f };
		p->C = FMatrix();
		p->mass = 1.f;
		m_pParticles.Add(p);
	}

	m_pGrid.Empty(num_cells);//make grid array

	for (int i = 0; i < num_cells; ++i)
	{
		Cell* TempCell = new Cell();
		TempCell->Vel = { 0.f, 0.f, 0.f };
		m_pGrid.Add(TempCell);
	}
}


void AMPM3D::SimulateUpdate()
{
	//reset grid scratch-pad
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->Vel = { 0.f,0.f,0.f };
	}

	//P2G_1
	P2GFirst();

	//grid velocity update
	int gridIndex = 0;
	for (auto& c : m_pGrid)
	{
		if (c->mass > 0)
		{
			//convert momentum to velocity, apply gravity
			c->Vel /= c->mass;
			c->Vel += dt * FVector3f(0, gravity, 0);

			//boundary conditions
			int x = gridIndex / (grid_res * grid_res);
			int y = (gridIndex % (grid_res * grid_res)) / grid_res;
			int z = gridIndex % gridIndex;
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
		gridIndex += 1;
	}

	//G2P
	int particleIndex = 0;

	for (auto& p : m_pParticles)
	{
		//reset particle velocity
		p->Vel = { 0.f,0.f,0.f };

		FIntVector cell_idx = static_cast<FIntVector>(p->Pos.X, p->Pos.Y, p->Pos.Z);
		FVector3f cell_diff = { (p->Pos.X - cell_idx.X),(p->Pos.Y - cell_idx.Y),(p->Pos.Z - cell_idx.Z) };

		m_weights[0] = { (0.5f * FMath::Pow(0.5f - cell_diff.X,2)), (0.5f * FMath::Pow(0.5f - cell_diff.Y,2)), (0.5f * FMath::Pow(0.5f - cell_diff.Z,2)) };
		m_weights[1] = { (0.75f - FMath::Pow(cell_diff.X,2)), (0.75f - FMath::Pow(cell_diff.Y,2)), (0.75f - FMath::Pow(cell_diff.Z,2)) };
		m_weights[2] = { (0.5f * FMath::Pow(0.5f + cell_diff.X,2)), (0.5f * FMath::Pow(0.5f + cell_diff.Y,2)), (0.5f * FMath::Pow(0.5f + cell_diff.Z,2)) };

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

					FVector3f dist = { (cell_x.X - p->Pos.X) + 0.5f,(cell_x.Y - p->Pos.Y) + 0.5f,(cell_x.Z - p->Pos.Z) + 0.5f };
					FVector3f weighted_velocity = m_pGrid[cell_index]->Vel * weight;
					
					
					auto term = FMatrix::Identity;
					term.M[0][0] = weighted_velocity.X * dist.X;
					term.M[1][1] = weighted_velocity.Y * dist.Y;
					term.M[2][2] = weighted_velocity.Z * dist.Z;

					B += term;

					p->Vel += weighted_velocity;
				}
			}
		}

		p->C = B * 4;

		//advect particles
		p->Pos += p->Vel * dt;

		//safety clamp 
		p->Pos.X = FMath::Clamp(p->Pos.X, 1, grid_res - 2);
		p->Pos.Y = FMath::Clamp(p->Pos.Y, 1, grid_res - 2);
		p->Pos.Z= FMath::Clamp(p->Pos.Z, 1, grid_res - 2);

		//update index
		particleIndex += 1;

		//boundary check??
	}
}

void AMPM3D::UpdateParticle()
{
	TArray<FTransform> Transforms;

	//Transforms.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->Pos.X * 100.f, m_pParticles[i]->Pos.Y * 100.f, m_pParticles[i]->Pos.Z * 100.f));
		Transforms.Add(tempValue);
	}

	for (int i = 0; i < NumParticles; ++i)
	{
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, Transforms[i]);
	}

	InstancedStaticMeshComponent->MarkRenderStateDirty();
}

void AMPM3D::SimulatingPipeLine()
{
	ClearGrid();
	P2GFirst();
	UpdateGrid();
	G2P();
}

void AMPM3D::ClearGrid()
{
	//reset grid scratch-pad
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->Vel = { 0.f,0.f,0.f };
	}
}

void AMPM3D::P2GFirst()
{
	for (auto& p : m_pParticles)
	{
		FIntVector cell_idx = static_cast<FIntVector>((int)p->Pos.X, (int)p->Pos.Y, (int)p->Pos.Z);
		FVector3f cell_diff = FVector3f{ (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		FMatrix C = p->C;

		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				for (UINT gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = FVector3f{ (cell_x.X - p->Pos.X) + 0.5f,(cell_x.Y - p->Pos.Y) + 0.5f,(cell_x.Z - p->Pos.Z) + 0.5f };

					//[TODO] Matrix and Float Multiply - Original Code : //float3 Q = math.mul(C, cell_dist);
					C.M[3][3] = 1;
					//UE_LOG(LogTemp, Log, TEXT("matrix : %f,%f,%f,%f"), C.M[0][0], C.M[1][1], C.M[2][2], C.M[3][3]);

					FVector4d temp_cell_dist = FVector4f(cell_dist, 1.f);
					FVector4 Q = C.TransformFVector4(temp_cell_dist);
					//UE_LOG(LogTemp, Log, TEXT("After Calculate: %lf, %lf, %lf, %lf"), Q.X, Q.Y, Q.Z, Q.W);

					// MPM course, equation 172
					float mass_contrib = weight * p->mass;

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;
					Cell* cell = m_pGrid[cell_index];

					// mass and momentum update
					cell->mass += mass_contrib;

					cell->Vel += mass_contrib * FVector3f{ (p->Vel.X + (float)Q.X),(p->Vel.Y + (float)Q.Y),(p->Vel.Z + (float)Q.Z) };

					m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}

void AMPM3D::P2GSecond()
{
	for (auto& p : m_pParticles)
	{
		FIntVector cell_idx = static_cast<FIntVector>((int)p->Pos.X, (int)p->Pos.Y, (int)p->Pos.Z);
		FVector3f cell_diff = FVector3f{ (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		
		//estimating particle volume by summing up neighbourhood's weighted mass contribution
		//MPM course eq 152
		float density = 0.0f;
		int gx, gy, gz;
		for (gx = 0; gx < 3; ++gx)
		{
			for (gy = 0; gy < 3; ++gy)
			{
				for (gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;
					int cell_index = (int)(cell_idx.X + gx - 1) * grid_res * grid_res + (int)(cell_idx.Y + gy - 1) * grid_res + (int)(cell_idx.Z + gz - 1);
					density += m_pGrid[cell_index]->mass * weight;
				}
			}
		}
		float volume;
		if(density!=0.f)
			volume = p->mass / density;

		// end goal, constitutive equation for isotropic fluid
		// stress = -pressure*I + viscosity *(velocity_gradient + velocity_gradient_transposed)

		// tait Equation of state
		// clamp
		float pressure = FMath::Max(-0.1f, eos_stiffness * FMath::Pow(density / rest_density, eos_power) - 1);

		FMatrix stress;
		stress.M[0][0] = -pressure;
		stress.M[1][1] = -pressure;
		stress.M[2][2] = -pressure;

		//velocity gradient - CPIC eq 17
		FMatrix dudv = p->C;
		FMatrix strain = dudv;

		//column 3, 2, 1
		//1행 3열
		//[row][col]
		float trace = strain.M[0][2] + strain.M[1][1] + strain.M[2][0];
		strain.M[0][2] = strain.M[1][1] = strain.M[0][2] = trace;

		FMatrix viscosity_term = strain.ApplyScale(dynamic_viscosity);
		stress += viscosity_term;

		//-volume * 4 * stress * dt;
		auto eq_16_term_0 = stress.ApplyScale(-volume * 4 * dt);

		for (gx = 0; gx < 3; ++gx)
		{
			for (gy = 0; gy < 3; ++gy)
			{
				for (gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = FVector3f{ (cell_x.X - p->Pos.X) + 0.5f,(cell_x.Y - p->Pos.Y) + 0.5f,(cell_x.Z - p->Pos.Z) + 0.5f };

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;
					Cell* cell = m_pGrid[cell_index];

					// fused force + momentum contribution from MLS-MPM
					FVector3f momentum;// = math.mul(eq_16_term_0 * weight, cell_dist);
					cell->Vel += momentum;

					m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}

void AMPM3D::UpdateGrid()
{
	//grid velocity update
	int gridIndex = 0;
	for (auto& c : m_pGrid)
	{
		if (c->mass > 0)
		{
			//convert momentum to velocity, apply gravity
			c->Vel /= c->mass;
			c->Vel += dt * FVector3f(0, gravity, 0);

			//boundary conditions
			int x = gridIndex / (grid_res * grid_res);
			int y = (gridIndex % (grid_res * grid_res)) / grid_res;
			int z = gridIndex % gridIndex;
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

			m_pGrid[gridIndex] = c;
		}

		gridIndex += 1;
	}
}

void AMPM3D::G2P()
{
	int particleIndex = 0;

	for (auto& p : m_pParticles)
	{
		//reset particle velocity
		p->Vel = { 0.f,0.f,0.f };

		FIntVector cell_idx = static_cast<FIntVector>(p->Pos.X, p->Pos.Y, p->Pos.Z);
		FVector3f cell_diff = { (p->Pos.X - cell_idx.X),(p->Pos.Y - cell_idx.Y),(p->Pos.Z - cell_idx.Z) };

		m_weights[0] = { (0.5f * FMath::Pow(0.5f - cell_diff.X,2)), (0.5f * FMath::Pow(0.5f - cell_diff.Y,2)), (0.5f * FMath::Pow(0.5f - cell_diff.Z,2)) };
		m_weights[1] = { (0.75f - FMath::Pow(cell_diff.X,2)), (0.75f - FMath::Pow(cell_diff.Y,2)), (0.75f - FMath::Pow(cell_diff.Z,2)) };
		m_weights[2] = { (0.5f * FMath::Pow(0.5f + cell_diff.X,2)), (0.5f * FMath::Pow(0.5f + cell_diff.Y,2)), (0.5f * FMath::Pow(0.5f + cell_diff.Z,2)) };

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

					FVector3f dist = { (cell_x.X - p->Pos.X) + 0.5f,(cell_x.Y - p->Pos.Y) + 0.5f,(cell_x.Z - p->Pos.Z) + 0.5f };
					FVector3f weighted_velocity = m_pGrid[cell_index]->Vel * weight;

					auto term = FMatrix::Identity;
					term.M[0][0] = weighted_velocity.X * dist.X;
					term.M[1][1] = weighted_velocity.Y * dist.Y;
					term.M[2][2] = weighted_velocity.Z * dist.Z;

					B += term;

					p->Vel += weighted_velocity;
				}
			}
		}

		p->C = B * 4;

		//advect particles
		p->Pos += p->Vel * dt;

		//safety clamp 
		p->Pos.X = FMath::Clamp(p->Pos.X, 1, grid_res - 2);
		p->Pos.Y = FMath::Clamp(p->Pos.Y, 1, grid_res - 2);
		p->Pos.Z = FMath::Clamp(p->Pos.Z, 1, grid_res - 2);


		//boundaries
		FVector3f x_n = p->Pos + p->Vel;
		const float wall_min = 3;
		float wall_max = (float)grid_res - 4;

		if (x_n.X < wall_min)
			p->Vel.X += wall_min - x_n.X;
		if (x_n.X > wall_max)
			p->Vel.X += wall_max - x_n.X;

		if (x_n.Y < wall_min)
			p->Vel.Y += wall_min - x_n.Y;
		if (x_n.Y > wall_max)
			p->Vel.Y += wall_max - x_n.Y;

		if (x_n.Z < wall_min)
			p->Vel.Z += wall_min - x_n.Z;
		if (x_n.Z > wall_max)
			p->Vel.Z += wall_max - x_n.Z;

		m_pParticles[particleIndex] = p;

		//update index
		particleIndex += 1;
	}
}



void AMPM3D::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();

	if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(NumParticles);

		for (int i = 0; i < NumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(TempPositions[i].X * 100.f, TempPositions[i].Y * 100.f, TempPositions[i].Z * 100.f));
			Transforms.Add(tempValue);
		}

		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}
}

void AMPM3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Log, TEXT("particle: %d"), NumParticles);
	//SimulateUpdate();
	//UpdateParticle();

	/*for (int i = 0; i < iterations; ++i)
	{
		SimulateUpdate();
		UpdateParticle();
	}*/
}

