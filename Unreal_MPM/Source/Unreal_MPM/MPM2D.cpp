#include "MPM2D.h"

AMPM2D::AMPM2D()
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

AMPM2D::~AMPM2D()
{
	for (auto& c : m_pGrid)
	{
		delete c;
	}

	for (auto& p : m_pParticles)
	{
		delete p;
	}
}

// Called when the game starts or when spawned
void AMPM2D::BeginPlay()
{
	Super::BeginPlay();

	Initialize();

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

	/*FMatrix2x2 a = { 1,2,3,4 };
	FMatrix2x2 b = Transpose(a);
	float a1, b1, c1, d1;
	b.GetMatrix(a1, b1, c1, d1);
	UE_LOG(LogTemp, Log, TEXT("%f, %f, %f, %f"), a1, b1, c1, d1);*/
}

void AMPM2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*for (int i = 0; i < iterations; ++i)
	{
		PipeLine();
	}*/
	PipeLine();
	UpdateInstancedMesh();

	//UE_LOG(LogTemp, Log, TEXT("log Test %f"), log(10));
	//UE_LOG(LogTemp, Log, TEXT("particle 10 pos %f %f"), m_pParticles[10]->x.X, m_pParticles[10]->x.Y);
	//UE_LOG(LogTemp, Log, TEXT("particle 100 pos %f %f"), m_pParticles[100]->x.X, m_pParticles[100]->x.Y);
}

void AMPM2D::Initialize()
{
	const float spacing = 0.5f;
	const int box_x = 32;
	const int box_y = 32;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;
	
	for (float i = -box_x / 2; i < box_x / 2; i += spacing)
	{
		for (float j = -box_y / 2; j < box_y / 2; j += spacing)
		{
			FVector2f Pos = { sx + i, sy + j };

			TempPositions.Add(Pos);
		}
	}

	NumParticles = TempPositions.Num();
	m_pParticles.Empty(NumParticles);
	Fs.Empty(NumParticles);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = {0.f,0.f};
		p->C = { 0.f,0.f,0.f,0.f };
		p->mass = 1.f;
		m_pParticles.Add(p);

		Fs.Add({ 1.f, 0.f, 0.f, 1.f });
	}

	m_pGrid.Empty(NumCells);
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* c = new Cell();
		c->v = { 0.f, 0.f };
		m_pGrid.Add(c);
	}

	//scatter particle mass to grid
	P2G();

	for (auto& p : m_pParticles)
	{
		FVector2f cell_idx = { FMath::Floor(p->x.X), FMath::Floor(p->x.Y) };
		FVector2f cell_diff = { (p->x.X - cell_idx.X) - 0.5f,(p->x.Y - cell_idx.Y) - 0.5f };

		TArray<FVector2f> weights;
		weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) });

		float density = 0.f;

		//iterate 3x3 cells
		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				//2d index to 1d index for grid
				int cell_index = ((int)(cell_idx.X) + gx - 1) * grid_res + ((int)(cell_idx.Y) + gy - 1);
				density += m_pGrid[cell_index]->mass * weight;
			}
		}

		float volume = p->mass / density;
		p->volume_0 = volume;
		
		//UE_LOG(LogTemp, Log, TEXT("volume %f"), volume); 
	}

	//[TEST] 1024..
	//UE_LOG(LogTemp, Log, TEXT("Fs count %d"), Fs.Num());
	//UE_LOG(LogTemp, Log, TEXT("particle count %d"), m_pParticles.Num());

	/*float a, b, c, d;
	Fs[1000].GetMatrix(a,b,c,d);
	UE_LOG(LogTemp, Log, TEXT("Fs matrix %f %f %f %f"), a,b,c,d);*/

	//UE_LOG(LogTemp, Log, TEXT("NumParticles %d"), NumParticles);
}

void AMPM2D::ClearGrid()
{
	for (auto& c : m_pGrid)
	{
		c->mass = 0;
		c->v = { 0.f, 0.f };
	}
}


void AMPM2D::P2G()
{
	//TArray<FVector2f> weights;
	float a, b, c, d = 0;
	float a1, b1, c1, d1 = 0;

	int i = 0;
	for (auto& p : m_pParticles)
	{
		FMatrix2x2 stress = { 0.f, 0.f, 0.f, 0.f };

		//deformation gradient
		FMatrix2x2 F = Fs[i];

		float J = F.Determinant();

		//mpm course p.46
		float volume = p->volume_0 * J;

		//matrix for Neo-Hookean model
		FMatrix2x2 F_T = Transpose(F);
		FMatrix2x2 F_inv_T = F_T.Inverse();
		FMatrix2x2 F_minus_F_inv_T = MinusMatrix(F, F_inv_T);
		F_minus_F_inv_T.GetMatrix(a, b, c, d);

		//mpm course eq.48
		FMatrix2x2 P_term_0 = { elastic_mu * a, elastic_mu * b, elastic_mu * c, elastic_mu * d };
		F_inv_T.GetMatrix(a1, b1, c1, d1);
		FMatrix2x2 P_term_1 = { elastic_lambda * log(J) * a1, elastic_lambda * log(J) * b1, elastic_lambda * log(J) * c1, elastic_lambda * log(J) * d1 };
		P_term_0.GetMatrix(a, b, c, d);
		P_term_1.GetMatrix(a1, b1, c1, d1);
		FMatrix2x2 P = { a + a1, b + b1, c + c1, d + d1 };
		
		//mpm course eq.38
		//cauchy stress  =(1/det(F)) * p * F_T
		P = MultiplyMatrix(P, F_T);
		P.GetMatrix(a, b, c, d);
		stress = { (1.f / J) * a, (1.f / J) * b, (1.f / J) * c, (1.f / J) * d };;

		//Apic, mpm course p.42
		//(M_p)^-1 = 4
		stress.GetMatrix(a, b, c, d);
		FMatrix2x2 eq_16_term_0 = { -volume * 4 * dt * a, -volume * 4 * dt * b, -volume * 4 * dt * c, -volume * 4 * dt * d };

		//quadratic interpolation
		FIntVector2 cell_idx = { (int)(p->x.X), (int)(p->x.Y) };
		FVector2f cell_diff = { (p->x.X - cell_idx.X) - 0.5f, (p->x.Y - cell_idx.Y) - 0.5f };

		TArray<FVector2f> weights;
		weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) });

		//for surrounding 3x3 cells
		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = { (int)(cell_idx.X) + gx - 1, (int)(cell_idx.Y) + gy - 1 };
				FVector2f cell_dist = { ((cell_x.X - p->x.X) + 0.5f), ((cell_x.Y - p->x.Y) + 0.5f) };

				p->C.GetMatrix(a, b, c, d);
				FVector2f Q = { a * cell_dist.X + b * cell_dist.Y, c * cell_dist.X + d * cell_dist.Y };

				//scatter mass and momentum to the grid
				int cell_index = (int)(cell_idx.X) * grid_res + (int)(cell_idx.Y);

				Cell* cell = m_pGrid[cell_index];

				//mpm course eq 172
				float weighted_mass = weight * p->mass;
				cell->mass += weighted_mass;

				//APIC p2g momentum contribution
				cell->v += weighted_mass * (p->v + Q);

				//fused force/momentum update from MLS_MPM
				//MLS paper eq.28 

				//[FIX] 5/30 if calculate once, particles move stable but calculate each steps particle moving a lot
				eq_16_term_0.GetMatrix(a, b, c, d);
				eq_16_term_0 = { a * weight, b * weight, c * weight, d * weight };

				eq_16_term_0.GetMatrix(a, b, c, d);
				//FVector2f momentum = { a * weight * cell_dist.X + b * weight * cell_dist.Y, c * weight * cell_dist.X + d * weight * cell_dist.Y };
				FVector2f momentum = { a * cell_dist.X + b * cell_dist.Y, c * cell_dist.X + d * cell_dist.Y };
				cell->v += momentum;
			}
		}
		++i;
	}
}

void AMPM2D::UpdateGrid()
{
	int i = 0;
	for (auto& c : m_pGrid)
	{
		if (c->mass > 0)
		{
			c->v /= c->mass;
			c->v += dt * FVector2f(0, gravity);

			int x = i / grid_res;
			int y = i % grid_res;
			if (x<2 || x>grid_res - 3)
			{
				c->v.X = 0;
			}
			if (y<2 || y>grid_res - 3)
			{
				c->v.Y = 0;
			}
		}
		++i;
	}
}

void AMPM2D::G2P()
{
	int i = 0;

	for (auto& p : m_pParticles)
	{
		p->v = { 0.f,0.f };

		FIntVector2 cell_idx = { (int)p->x.X, (int)p->x.Y };
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };

		TArray<FVector2f> weights;
		weights.Add({ 0.5f * FMath::Pow(0.5f - cell_diff.X, 2), 0.5f * FMath::Pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - FMath::Pow(cell_diff.X, 2), 0.75f - FMath::Pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * FMath::Pow(0.5f + cell_diff.X, 2), 0.5f * FMath::Pow(0.5f + cell_diff.Y, 2) });

		FMatrix2x2 B = { 0.f, 0.f, 0.f, 0.f };
		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = { cell_idx.X + gx - 1, cell_idx.Y + gy - 1 };
				int cell_index = ((int)(cell_idx.X) * grid_res + (int)(cell_idx.Y));

				FVector2f dist = { (cell_x.X - p->x.X) + 0.5f, (cell_x.Y - p->x.Y) + 0.5f };
				FVector2f weighted_velocity = m_pGrid[cell_index]->v * weight;

				//APIC eq.10
				FMatrix2x2 term = { weighted_velocity.X * dist.X, weighted_velocity.X * dist.Y, 
					weighted_velocity.Y * dist.X, weighted_velocity.Y * dist.Y };


				float a, b, c, d = 0;
				float a1, b1, c1, d1 = 0;
				term.GetMatrix(a, b, c, d);
				B.GetMatrix(a1, b1, c1, d1);
				B = { a1 + a, b1 + b, c1 + c, d1 + d };

				p->v += weighted_velocity;
			}
		}
		float a, b, c, d = 0;
		B.GetMatrix(a, b, c, d);
		p->C = { a * 4, b * 4, c * 4, d * 4 };

		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);

		//Add force
		/*{
			FVector2f force = { 0.0f, 0.06f };
			p->v += force;
		}*/

		/*for (auto& f : Fs)
		{
			FMatrix2x2 Fp_new = { 1.f, 0.f, 0.f, 1.f };
			float a1, b1, c1, d1 = 0;
			Fp_new.GetMatrix(a1, b1, c1, d1);
			p->C.GetMatrix(a, b, c, d);

			Fp_new = { a1 + a * dt, b1 + b * dt, c1 + c * dt, d1 + d * dt };
			f = MultiplyMatrix(Fp_new, f);

		}*/

		FMatrix2x2 Fp_new = { 1.f, 0.f, 0.f, 1.f };

		float a1, b1, c1, d1;
		Fp_new.GetMatrix(a1, b1, c1, d1);
		p->C.GetMatrix(a, b, c, d);

		Fp_new = { a1 + a * dt, b1 + b * dt, c1 + c * dt, d1 + d * dt };
		Fs[i] = MultiplyMatrix(Fp_new, Fs[i]);

		++i;
	}
}

void AMPM2D::PipeLine()
{
	ClearGrid();
	P2G();
	UpdateGrid();
	G2P();
}

void AMPM2D::UpdateInstancedMesh()
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

FMatrix2x2 AMPM2D::Transpose(FMatrix2x2 originMatrix)
{
	FMatrix2x2 resultMatrix;
	float a, b, c, d = 0.f;
	originMatrix.GetMatrix(a, b, c, d);
	resultMatrix = { a,c,b,d };

	return resultMatrix;
}

FMatrix2x2 AMPM2D::MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;
	float a1, b1, c1, d1, a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	resultMatrix = { a1 - a2, b1 - b2, c1 - c2, d1 - d2 };

	return resultMatrix;
}

FMatrix2x2 AMPM2D::MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;

	float a1, b1, c1, d1 = 0.f;
	float a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	
	resultMatrix = { a1 * a2 + b1 * c2, a1 * b2 + b1 * d2, c1 * a2 + d1 * c2, c1 * b2 + d1 * d2 };

	return resultMatrix;
}

