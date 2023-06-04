// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM2D_NeoHookean.h"

AMPM2D_NeoHookean::AMPM2D_NeoHookean()
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

void AMPM2D_NeoHookean::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FVector2f> TempPositions;

	const float spacing = 0.5f;
	//const float spacing = 1.f;
	const int box_x = 32;
	const int box_y = 32;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;

	for (float i = -box_x / 2; i < box_x / 2; i += spacing)
	{
		for (float j = -box_y / 2; j < box_y / 2; j += spacing)
		{
			auto Pos = FVector2f(sx + i, sy + j);
			TempPositions.Add(Pos);
		}
	}

	NumParticles = TempPositions.Num();

	//m_pParticles.SetNum(NumParticles);
	//Fs.SetNum(NumParticles);
	m_pParticles.Empty(NumParticles);
	Fs.Empty(NumParticles);
	//weights.Empty(3);

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = new Particle();
		p->x = TempPositions[i];
		p->v = { 0.f,0.f };
		p->C = { 0.f, 0.f, 0.f, 0.f };
		p->mass = 1.f;

		m_pParticles.Add(p);
		Fs.Add(FMatrix2x2(1.f, 0.f, 0.f, 1.f));
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

	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

		FVector2f cell_idx = { (float)floor(p->x.X), (float)floor(p->x.Y) };
		FVector2f cell_diff = { (p->x.X - cell_idx.X) - 0.5f, (p->x.Y - cell_idx.Y) - 0.5f };

		weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

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

		m_pParticles[i] = p;
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

void AMPM2D_NeoHookean::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	for (int i = 0; i < 5; ++i)
	{
		Simulate();
	}
	//Simulate();
	UpdateParticles();
}

void AMPM2D_NeoHookean::ClearGrid()
{
	//reset grid
	
	for (int i = 0; i < NumCells; ++i)
	{
		Cell* cell = m_pGrid[i];

		cell->mass = 0;
		cell->v = { 0.f,0.f };

		m_pGrid[i] = cell;
	}
}

void AMPM2D_NeoHookean::P2G()
{
	/*float a, b, c, d = 0;
	float a1, b1, c1, d1 = 0;*/

	weights.Empty(3); //[TODO] 어디에 둬야 할까?
	for (int i = 0; i < NumParticles; ++i)
	{
		Particle* p = m_pParticles[i];

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

		//mpm course eq.48
		
		//F_minus_F_inv_T.GetMatrix(a, b, c, d);
		//FMatrix2x2 P_term_0 = { elastic_mu * a, elastic_mu * b, elastic_mu * c, elastic_mu * d };
		//->
		FMatrix2x2 P_term_0 = ScalingMatrix(F_minus_F_inv_T, elastic_mu);


		//F_inv_T.GetMatrix(a1, b1, c1, d1);
		//FMatrix2x2 P_term_1 = { elastic_lambda * log(J) * a1, elastic_lambda * log(J) * b1, elastic_lambda * log(J) * c1, elastic_lambda * log(J) * d1 };
		//->
		FMatrix2x2 P_term_1 = ScalingMatrix(F_inv_T, log(J) * elastic_lambda);

		//P_term_0.GetMatrix(a, b, c, d);
		//P_term_1.GetMatrix(a1, b1, c1, d1);
		//FMatrix2x2 P = { a + a1, b + b1, c + c1, d + d1 };
		//->
		FMatrix2x2 P = PlusMatrix(P_term_0, P_term_1);


		//mpm course eq.38
		//cauchy stress  =(1/det(F)) * p * F_T
		FMatrix2x2 temp = MultiplyMatrix(P, F_T);
		//temp.GetMatrix(a, b, c, d);
		//stress = { (1.f / J) * a, (1.f / J) * b, (1.f / J) * c, (1.f / J) * d };
		//->
		stress = ScalingMatrix(temp, (1.f / J));

		//Apic, mpm course p.42
		//(M_p)^-1 = 4
		//stress.GetMatrix(a, b, c, d);
		//FMatrix2x2 eq_16_term_0 = { -volume * 4 * a * dt, -volume * 4 * b * dt, -volume * 4 * c * dt, -volume * 4 * d * dt };
		//->
		FMatrix2x2 eq_16_term_0 = ScalingMatrix(stress, -volume * 4 * dt);

		//quadratic interpolation
		FIntVector2 cell_idx = FIntVector2(p->x.X, p->x.Y);
		FVector2f cell_diff = { p->x.X - cell_idx.X - 0.5f, p->x.Y - cell_idx.Y - 0.5f };

		//TArray<FVector2f> weights;
		//weights.Empty(3);
		weights.Add({ 0.5f * (float)pow(0.5f - cell_diff.X, 2), 0.5f * (float)pow(0.5f - cell_diff.Y, 2) });
		weights.Add({ 0.75f - (float)pow(cell_diff.X, 2), 0.75f - (float)pow(cell_diff.Y, 2) });
		weights.Add({ 0.5f * (float)pow(0.5f + cell_diff.X, 2), 0.5f * (float)pow(0.5f + cell_diff.Y, 2) });

		//for surrounding 3x3 cells
		for (int gx = 0; gx < 3; ++gx)
		{
			for (int gy = 0; gy < 3; ++gy)
			{
				float weight = weights[gx].X * weights[gy].Y;

				FIntVector2 cell_x = FIntVector2(cell_idx.X + gx - 1, cell_idx.Y + gy - 1);
				FVector2f cell_dist = FVector2f(cell_x.X - p->x.X + 0.5f, cell_x.Y - p->x.Y + 0.5f);
				FVector2f Q = MultiplyMatrixAndFloat(p->C, cell_dist);

				//scatter mass and momentum to the grid
				int cell_index = (int)cell_x.X * grid_res + (int)cell_x.Y;

				Cell* cell = m_pGrid[cell_index];

				//mpm course eq 172
				float weighted_mass = weight * p->mass;
				cell->mass += weighted_mass;

				//APIC p2g momentum contribution
				cell->v += weighted_mass * (p->v + Q);

				//fused force/momentum update from MLS_MPM
				//MLS paper eq.28 

				//[FIX] 5/30 if calculate once, particles move stable but calculate each steps particle moving a lot
				float a, b, c, d = 0;
				eq_16_term_0.GetMatrix(a, b, c, d);
				//eq_16_term_0 = { a * weight, b * weight, c * weight, d * weight };

				//eq_16_term_0.GetMatrix(a, b, c, d);
				FVector2f momentum = { a * weight * cell_dist.X + b * weight * cell_dist.Y, c * weight * cell_dist.X + d * weight * cell_dist.Y };
				//FVector2f momentum = MultiplyMatrixAndFloat(eq_16_term_0, cell_dist);
				cell->v += momentum;

				m_pGrid[cell_index] = cell;
			}
		}
	}
}

void AMPM2D_NeoHookean::UpdateGrid()
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

void AMPM2D_NeoHookean::G2P()
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

				/*float a1, b1, c1, d1;
				float a2, b2, c2, d2;
				B.GetMatrix(a1, b1, c1, d1);
				term.GetMatrix(a2, b2, c2, d2);

				B = { a1 + a2, b1 + b2, c1 + c2, d1 + d2 };*/
				B = PlusMatrix(B, term);

				p->v += weighted_velocity;
			}
		}
		/*float a, b, c, d;
		B.GetMatrix(a, b, c, d);
		p->C = { a * 4, b * 4, c * 4, d * 4 };*/
		p->C = ScalingMatrix(B, 2);

		//advect particles
		p->x += p->v * dt;

		p->x.X = FMath::Clamp(p->x.X, 1, grid_res - 2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);
		
		//deformation gradient update
		FMatrix2x2 Fp_new = { 1.f, 0.f, 0.f, 1.f };

		float a1, b1, c1, d1;
		float a, b, c, d;
		Fp_new.GetMatrix(a1, b1, c1, d1);
		p->C.GetMatrix(a, b, c, d);

		Fp_new = { a1 + a * dt, b1 + b * dt, c1 + c * dt, d1 + d * dt };
		Fs[i] = MultiplyMatrix(Fp_new, Fs[i]);

		m_pParticles[i] = p;
	}
}

void AMPM2D_NeoHookean::UpdateParticles()
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

void AMPM2D_NeoHookean::Simulate()
{
	ClearGrid();
	P2G(); //P2G
	UpdateGrid(); //grid velocity update
	G2P(); //G2P
}


FMatrix2x2 AMPM2D_NeoHookean::Transpose(FMatrix2x2 originMatrix)
{
	FMatrix2x2 resultMatrix;
	float a, b, c, d = 0.f;
	originMatrix.GetMatrix(a, b, c, d);
	resultMatrix = { a,c,b,d };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_NeoHookean::MinusMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;
	float a1, b1, c1, d1, a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	resultMatrix = { a1 - a2, b1 - b2, c1 - c2, d1 - d2 };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_NeoHookean::PlusMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;
	float a1, b1, c1, d1, a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);
	resultMatrix = { a1 + a2, b1 + b2, c1 + c2, d1 + d2 };

	return resultMatrix;
}

FMatrix2x2 AMPM2D_NeoHookean::MultiplyMatrix(FMatrix2x2 m1, FMatrix2x2 m2)
{
	FMatrix2x2 resultMatrix;

	float a1, b1, c1, d1 = 0.f;
	float a2, b2, c2, d2 = 0.f;
	m1.GetMatrix(a1, b1, c1, d1);
	m2.GetMatrix(a2, b2, c2, d2);

	resultMatrix = { a1 * a2 + b1 * c2, a1 * b2 + b1 * d2, c1 * a2 + d1 * c2, c1 * b2 + d1 * d2 };

	return resultMatrix;
}

FVector2f AMPM2D_NeoHookean::MultiplyMatrixAndFloat(FMatrix2x2 m1, FVector2f v1)
{
	FVector2f resultVector;

	float a, b, c, d = 0.f;
	m1.GetMatrix(a, b, c, d);

	resultVector = { a * v1.X + b * v1.Y, c * v1.X + d * v1.Y };

	return resultVector;
}