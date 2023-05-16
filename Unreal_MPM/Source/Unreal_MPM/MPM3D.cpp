#include "MPM3D.h"

AMPM3D::AMPM3D()
	:NumParticles(0)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	InstancedInteractionMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InteractiveInstancedMesh"));

	SetRootComponent(InstancedStaticMeshComponent);
	
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponent->SetGenerateOverlapEvents(false);

	//Interactive test 2: instanced mesh
	InstancedInteractionMesh->SetMobility(EComponentMobility::Static);
	InstancedInteractionMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedInteractionMesh->SetGenerateOverlapEvents(false);


	//Interactive test 1: static mesh
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractionBall"));
}

AMPM3D::~AMPM3D()
{
	//destroy
	for (auto& p : m_pParticles)
	{
		delete p;
	}

	for (auto& c : m_pGrid)
	{
		delete c;
	}
}

void AMPM3D::Initialize()
{
	//init grid and fill grids with (grid_res * grid_res) cells
	const float spacing = 1.0f;
	/*const int box_x = 16;
	const int box_y = 16;
	const int box_z = 16;*/
	const int box_x = 8;
	const int box_y = 8;
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
	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles); //make particle array
	m_DeformationGradient.Empty(NumParticles); //deformation gradient

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->Pos = TempPositions[i];
		p->Vel = { 0.f, 0.f, 0.f };
		p->C = FMatrix::Identity; //identity matrix
		p->mass = 1.f;
		m_pParticles.Add(p);

		//deformation gradient init
		m_DeformationGradient.Add(FMatrix::Identity);
	}

	//fill grid
	m_pGrid.Empty(num_cells);//make grid array
	for (int i = 0; i < num_cells; ++i)
	{
		Cell* TempCell = new Cell();
		TempCell->Vel = { 0.f, 0.f, 0.f };
		m_pGrid.Add(TempCell);
	}

	//[TEST] Neo-Hookean
	//P2GScatterMLS();
}

void AMPM3D::UpdateParticle()
{
	TArray<FTransform> Transforms;

	Transforms.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		FTransform tempValue = FTransform(FVector(m_pParticles[i]->Pos.X * 100.f, m_pParticles[i]->Pos.Y * 100.f, m_pParticles[i]->Pos.Z * 100.f));
		Transforms.Add(tempValue);
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, Transforms[i]);
	}
	InstancedStaticMeshComponent->MarkRenderStateDirty();
	UE_LOG(LogTemp, Log, TEXT("tempValue 0 : %f %f %f"), Transforms[0].GetLocation().X, Transforms[0].GetLocation().Y, Transforms[0].GetLocation().Z);
	UE_LOG(LogTemp, Log, TEXT("tempValue 100 : %f %f %f"), Transforms[1].GetLocation().X, Transforms[1].GetLocation().Y, Transforms[1].GetLocation().Z);
}

void AMPM3D::SimulatingPipeLine(double timestep)
{
	ClearGrid();
	P2GFirst();
	//P2GSecond(timestep);
	//P2GMLS(timestep);
	UpdateGrid(timestep);
	G2P(timestep);
	//G2PMLS(timestep);
}

void AMPM3D::ClearGrid()
{
	//reset grid scratch-pad
	for (auto& c : m_pGrid)
	{
		c->mass = 0; //cell's zero mass
		c->Vel = { 0.f,0.f,0.f }; //cell's velocity zero
	}
}

void AMPM3D::P2GFirst()
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

					//[TODO] Matrix and Float Multiply - Original Code : //float3 Q = math.mul(C, cell_dist);
					//C is Identity matrix

					//[FIX] 5/2
					//C.M[3][3] = 1;
					//FVector4d temp_cell_dist = FVector4f(cell_dist, 1.f);
					//FVector4 Q = C.TransformFVector4(temp_cell_dist);

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
	firstRound = false;
	//UE_LOG(LogTemp, Log, TEXT("grid mass and vel of x: %f, %f"), m_pGrid[10]->mass, m_pGrid[10]->Vel.X);
}

void AMPM3D::P2GSecond(double timestep)
{
	for (auto& p : m_pParticles)
	{
		FIntVector cell_idx = static_cast<FIntVector>((int)p->Pos.X, (int)p->Pos.Y, (int)p->Pos.Z);
		FVector3f cell_diff = FVector3f{ (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };
		TArray<FVector3f> m_weights;

		m_weights.Empty(3);
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });
		
		//estimating particle volume by summing up neighbourhood's weighted mass contribution
		//MPM course eq 152
		float density = 0.0f;
		int gx, gy, gz = 0;
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
		if (density != 0.f)
			volume = p->mass / density;
		else
			volume = p->mass;

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
		float trace = strain.M[0][0] + strain.M[1][1] + strain.M[2][2];
		strain.M[2][2] = strain.M[1][1] = strain.M[0][0] = trace;

		FMatrix viscosity_term = strain.ApplyScale(dynamic_viscosity);
		stress += viscosity_term;

		//-volume * 4 * stress * dt;
		auto eq_16_term_0 = stress.ApplyScale(-volume * 4) * timestep;

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

					for (auto& c : m_pGrid)
					{
						FVector3f momentum = FVector3f{ static_cast<float>(eq_16_term_0.ApplyScale(weight).M[0][0]) * cell_dist.X,
												 static_cast<float>(eq_16_term_0.ApplyScale(weight).M[1][1]) * cell_dist.Y,
												 static_cast<float>(eq_16_term_0.ApplyScale(weight).M[2][2]) * cell_dist.Z };
						c->Vel += momentum;
					}

					//Cell* cell = m_pGrid[cell_index];

					////[TODO] clear m_pGrid??
					//m_pGrid.Empty();

					//// fused force + momentum contribution from MLS-MPM
					////[TODO] it is not diagonal matrix maybe...
					//FVector3f momentum = FVector3f{ static_cast<float>(eq_16_term_0.ApplyScale(weight).M[0][0]) * cell_dist.X,
					//						 static_cast<float>(eq_16_term_0.ApplyScale(weight).M[1][1]) * cell_dist.Y,
					//						 static_cast<float>(eq_16_term_0.ApplyScale(weight).M[2][2]) * cell_dist.Z };
					//cell->Vel += momentum;

					//m_pGrid[cell_index] = cell;
				}
			}
		}
	}
}

void AMPM3D::P2GScatterMLS()
{
	TArray<FVector3f> m_weights;
	for (auto& p : m_pParticles)
	{
		FIntVector cell_idx = static_cast<FIntVector>((int)p->Pos.X, (int)p->Pos.Y, (int)p->Pos.Z);
		FVector3f cell_diff = FVector3f{ (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };

		m_weights.Empty(3);
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		float density = 0.f;

		//iterate surrounding 3x3x3 cells
		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					int cell_index = (int)(cell_idx.X + gx - 1) * grid_res * grid_res + (int)(cell_idx.Y + gy - 1) * grid_res + (int)(cell_idx.Z + gz - 1);
					density += m_pGrid[cell_index]->mass * weight;
				}
			}
		}

		float volume = p->mass / density;
		p->volume_0 = volume;
	}
}

void AMPM3D::P2GMLS(double timestep)
{
	TArray<FVector3f> m_weights;
	m_weights.Empty(3);

	int particleIndex = 0;
	for (auto& p : m_pParticles)
	{
		FMatrix stress;

		//deformation gradient
		auto F = m_DeformationGradient[particleIndex];

		auto J = F.Determinant();

		//mpm course p.46
		auto volume = p->volume_0 * J;

		//Neo-hookean
		FMatrix F_T = F.GetTransposed();
		FMatrix F_inv_T = F_T.Inverse();
		F_inv_T.ApplyScale(-1.f); // for minus calculation
		FMatrix F_minus_F_inv_T = F + F_inv_T;

		//mpm course eq.48
		auto P_term_0 = F_minus_F_inv_T.ApplyScale(elastic_mu);
		auto P_term_1 = (F_inv_T.ApplyScale(elastic_lambda)).ApplyScale(FMath::Loge(J));
		auto P = P_term_0 + P_term_1;

		stress = (P * F_T).ApplyScale(1.f / J);

		auto tempValue = -volume * 4 * timestep;
		FMatrix eq_16_term_0 = stress.ApplyScale(tempValue);

		FIntVector cell_idx = static_cast<FIntVector>((int)p->Pos.X, (int)p->Pos.Y, (int)p->Pos.Z);
		FVector3f cell_diff = FVector3f{ (p->Pos.X - cell_idx.X) - 0.5f ,(p->Pos.Y - cell_idx.Y) - 0.5f ,(p->Pos.Z - cell_idx.Z) - 0.5f };

		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2),0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add(FVector3f{ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				for (int gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					FVector3f cell_dist = { (cell_x.X - p->Pos.X) + 0.5f, (cell_x.Y - p->Pos.Y) + 0.5f, (cell_x.Z - p->Pos.Z) + 0.5f };

					FVector3d Q = { p->C.M[0][0] * cell_dist.X, p->C.M[1][1] * cell_dist.Y, p->C.M[2][2] * cell_dist.Z };

					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					//[FIX] - 5/2 
					for (auto& cell : m_pGrid)
					{
						//eq.172
						float weighted_mass = weight * p->mass;
						cell->mass += weighted_mass;

						//APIC P2G momentum contribution
						cell->Vel += weighted_mass * FVector3f{ (p->Vel.X + (float)Q.X),(p->Vel.Y + (float)Q.Y),(p->Vel.Z + (float)Q.Z) };

						//fused force/momentum update from MLS-MPM
						//MLS-MPM eq.28
						
						eq_16_term_0.ApplyScale(weight);
						//float2 momentum = math.mul(eq_16_term_0 * weight, cell_dist);
						auto momentum = MultiplyMatrixAndVector(eq_16_term_0, cell_dist);
						cell->Vel += momentum;
					}
				}
			}
		}

		++particleIndex;
	}
}

void AMPM3D::UpdateGrid(double timestep)
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
			c->Vel += timestep * FVector3f(0, gravity, 0); //[TODO] why y axis gravity?

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
		//UE_LOG(LogTemp, Log, TEXT("gridIndex: %d"), gridIndex);
	}
}

void AMPM3D::G2P(double timestep)
{
	TArray<FVector3f> m_weights;

	for (auto& p : m_pParticles)
	{
		//reset particle velocity
		p->Vel = { 0.f,0.f,0.f };

		//[TODO] update paticle's deformation gradient MLS_MPM eq 17

		//calculate neighbouring cell weights
		FIntVector cell_idx = static_cast<FIntVector>(p->Pos.X, p->Pos.Y, p->Pos.Z);
		FVector3f cell_diff = { (p->Pos.X - cell_idx.X - 0.5f),(p->Pos.Y - cell_idx.Y - 0.5f),(p->Pos.Z - cell_idx.Z - 0.5f) };

		//using interpolation function
		m_weights.Empty(3);
		m_weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Z, 2) });
		m_weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2), 0.75f - FMath::Pow(cell_diff.Z, 2) });
		m_weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Z, 2) });

		//calculate new particle velocity
		FMatrix B;// FMatrix::Identity;
		/*B.M[0][0] = 0;
		B.M[1][1] = 0;
		B.M[2][2] = 0;*/
		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				for (UINT gz = 0; gz < 3; ++gz)
				{
					float weight = m_weights[gx].X * m_weights[gy].Y * m_weights[gz].Z;

					FIntVector cell_x = FIntVector(cell_idx.X + gx - 1, cell_idx.Y + gy - 1, cell_idx.Z + gz - 1);
					int cell_index = (int)cell_x.X * grid_res * grid_res + (int)cell_x.Y * grid_res + (int)cell_x.Z;

					FVector3f dist = { (cell_x.X - p->Pos.X) + 0.5f, (cell_x.Y - p->Pos.Y) + 0.5f, (cell_x.Z - p->Pos.Z) + 0.5f };
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
					//UE_LOG(LogTemp, Log, TEXT("(0,0) : %f (1,1) : %f"), B.M[0][0], B.M[1][1]);

					p->Vel += weighted_velocity;
				}
			}
		}
		p->C = B * 4;

		//advect particles
		p->Pos += p->Vel * timestep;

		//safety clamp 
		p->Pos.X = FMath::Clamp(p->Pos.X, 1, grid_res - 2);
		p->Pos.Y = FMath::Clamp(p->Pos.Y, 1, grid_res - 2);
		p->Pos.Z = FMath::Clamp(p->Pos.Z, 1, grid_res - 2);

		//force test [TEST] 5/16
		/*{
			FVector3f force = { 0.5f, 0.0f, 0.f };
			p->Vel += force;
		}*/

		//interaction with static mesh
		FVector dist_sphere = FVector{ p->Pos.X * 100.f - m_pMesh->GetComponentLocation().X,
				p->Pos.Y * 100.f - m_pMesh->GetComponentLocation().Y,
				p->Pos.Z * 100.f - m_pMesh->GetComponentLocation().Z };

		/*UE_LOG(LogTemp, Log, TEXT("ball location: %f / %f / %f"), m_pMesh->GetComponentLocation().X,
			m_pMesh->GetComponentLocation().Y, m_pMesh->GetComponentLocation().Z);
		UE_LOG(LogTemp, Log, TEXT("================="));
		UE_LOG(LogTemp, Log, TEXT("particle location: %f / %f / %f"), p->Pos.X * 100.f, p->Pos.Y * 100.f, p->Pos.Z * 100.f);*/

		auto dotproduct_res = sqrt(dist_sphere.X * dist_sphere.X + dist_sphere.Y * dist_sphere.Y + dist_sphere.Z * dist_sphere.Z);
		
		//UE_LOG(LogTemp, Log, TEXT("dot: %f"), dotproduct_res);

		//if (dotproduct_res < 20) //[TEST]do not interaction
		
		//[TEST] 5/16 delete for MLS test
		//if (dotproduct_res < 2200) 
		//{
		//	auto force = dist_sphere.GetSafeNormal() * 1.f / 10.f;
		//	//UE_LOG(LogTemp, Log, TEXT("force: %f / %f / %f"), force.X, force.Y, force.Z);

		//	p->Vel.X += force.X;
		//	p->Vel.Y += force.Y;
		//	p->Vel.Z += force.Z;
		//}
			
		
		//boundaries
		FVector3f x_n = p->Pos + p->Vel;
		/*const float wall_min = 3;
		float wall_max = (float)grid_res - 4;*/
		const float wall_min = 1;
		//float wall_max = (float)(grid_res - 1) - wall_min;
		float wall_max = (float)(32 - 1) - wall_min; //[TEST] 5/16 for boundary condition

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

		//first step end
		//UE_LOG(LogTemp, Log, TEXT("==particle=="));
	}
}

void AMPM3D::G2PMLS(double timestep)
{
	for (auto& p : m_pParticles)
	{
		//reset particle velocity
		p->Vel = { 0.f,0.f,0.f };

		//calculate neighbouring cell weights
		FIntVector cell_idx = static_cast<FIntVector>(p->Pos.X, p->Pos.Y, p->Pos.Z);
		FVector3f cell_diff = { (p->Pos.X - cell_idx.X - 0.5f),(p->Pos.Y - cell_idx.Y - 0.5f),(p->Pos.Z - cell_idx.Z - 0.5f) };

		//using interpolation function
		TArray<FVector3f> m_weights;
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

					FVector3f dist = { (cell_x.X - p->Pos.X) + 0.5f, (cell_x.Y - p->Pos.Y) + 0.5f, (cell_x.Z - p->Pos.Z) + 0.5f };
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
					//UE_LOG(LogTemp, Log, TEXT("(0,0) : %f (1,1) : %f"), B.M[0][0], B.M[1][1]);

					p->Vel += weighted_velocity;
				}
			}
		}
		p->C = B * 4;

		//advect particles
		p->Pos += p->Vel * timestep;

		//safety clamp 
		p->Pos.X = FMath::Clamp(p->Pos.X, 1, grid_res - 2);
		p->Pos.Y = FMath::Clamp(p->Pos.Y, 1, grid_res - 2);
		p->Pos.Z = FMath::Clamp(p->Pos.Z, 1, grid_res - 2);


		auto Fp_new = FMatrix::Identity;

		Fp_new += p->C.ApplyScale(timestep);
		
		for (auto& d : m_DeformationGradient)
		{
			d = Fp_new * d;
		}
	}
}


void AMPM3D::BeginPlay()
{
	Super::BeginPlay();

	//static mesh
	m_pMesh->SetWorldLocation(FVector(-420, -1260, 200));

	Initialize();

	if (InstancedStaticMeshComponent->GetInstanceCount() == 0)
	{
		TArray<FTransform> Transforms;

		Transforms.Empty(NumParticles);

		for (int i = 0; i < NumParticles; ++i)
		{
			FTransform tempValue = FTransform(FVector(m_pParticles[i]->Pos.X * 100.f, m_pParticles[i]->Pos.Y * 100.f, m_pParticles[i]->Pos.Z * 100.f));
			Transforms.Add(tempValue);
		}
		InstancedStaticMeshComponent->AddInstances(Transforms, false);
	}
	
	//instanced mesh 
	FTransform InstancedInteractiveMeshTransform = FTransform(FVector(-420, -1260, 200));
	InstancedInteractionMesh->AddInstance(InstancedInteractiveMeshTransform, false);

	//UE_LOG(LogTemp, Log, TEXT("particle xpos: %f, ypos : %f"), m_pParticles[0]->Pos.X, m_pParticles[0]->Pos.Y);
	//UE_LOG(LogTemp, Log, TEXT("particle xpos: %f, ypos : %f"), m_pParticles[NumParticles-1]->Pos.X, m_pParticles[NumParticles-1]->Pos.Y);
}

void AMPM3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SimulatingPipeLine(timesteps);
	UpdateParticle();

	//MoveInteractionBall();
	//CheckParticlePos();
	//UE_LOG(LogTemp, Log, TEXT("ball location: %f / %f / %f"), m_pMesh->GetComponentLocation().X, m_pMesh->GetComponentLocation().Y, m_pMesh->GetComponentLocation().Y);
}

void AMPM3D::MoveInteractionBall()
{
	FVector location = m_pMesh->GetComponentLocation();
	location.X += 10;
	m_pMesh->SetWorldLocation(location);
}

void AMPM3D::CheckParticlePos()
{
	int num_of_zero_particles = 0;
	for (auto& p : m_pParticles)
	{
		if (p->Pos.X == 0.f || p->Pos.Y == 0.f || p->Pos.Z == 0.f)
		{
			num_of_zero_particles += 1;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("zero particles: %d"), num_of_zero_particles);
}

FVector3f AMPM3D::MultiplyMatrixAndVector(FMatrix f, FVector3f v)
{
	float xvalue = f.M[0][0] * v.X + f.M[0][1] * v.Y + f.M[0][2] * v.Z;
	float yvalue = f.M[1][0] * v.X + f.M[1][1] * v.Y + f.M[1][2] * v.Z;
	float zvalue = f.M[2][0] * v.X + f.M[2][1] * v.Y + f.M[2][2] * v.Z;

	return { xvalue, yvalue, zvalue };
}
