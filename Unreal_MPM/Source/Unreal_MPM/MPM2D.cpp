// Fill out your copyright notice in the Description page of Project Settings.


#include "MPM2D.h"


// Sets default values
AMPM2D::AMPM2D()
{
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

AMPM2D::~AMPM2D()
{
}

// Called when the game starts or when spawned
void AMPM2D::BeginPlay()
{
	Super::BeginPlay();

	//Initialize();
}

// Called every frame
void AMPM2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*for (int i = 0; i < 1; ++i)
	{
		Each_Simulation_Step();
	}*/
}

void AMPM2D::Initialize()
{
	
	TArray<FVector2f> temp_positions;
	const float spacing = 1.0f;
	const int box_x = 16;
	const int box_y = 16;
	const float sx = grid_res / 2.0f;
	const float sy = grid_res / 2.0f;
	for (float i = sx - box_x / 2; i < sx + box_x / 2; i += spacing)
	{
		for (float j = sy - box_y / 2; i < sy + box_y / 2; j += spacing)
		{
			auto pos = FVector2f(i, j);
			temp_positions.Add(pos);
		}
	}
	num_particles = temp_positions.Num();

	particles.Reserve(num_particles);

	for (int i = 0; i < num_particles; ++i)
	{
		Particle* p = new Particle();
		p->x = temp_positions[i];
		float randNum = FMath::FRandRange(0.f, 1.f);
		p->v = FVector2f(randNum - 0.5f, randNum - 0.5f + 2.75f) * 0.5f;
		//p.C = 0;
		p->mass = 1.0f;
	}

	grid.Reserve(num_cells);

	for (int i = 0; i < num_cells; ++i)
	{
		grid[i] = new Cell();
	}

	//? SimRenderer 코드 분석 필요
	//sim_renderer = GameObject.FindObjectOfType<SimRenderer>();
	//sim_renderer.Initialise(num_particles, Marshal.SizeOf(new Particle()));
}

void AMPM2D::Each_Simulation_Step()
{
	//reset grid scratch-pad
	for (int i = 0; i < num_cells; ++i)
	{
		auto cell = grid[i];

		cell->mass = 0;
		cell->v = { 0.f,0.f };

		grid[i] = cell;
	}

	//p2g
	for (int i = 0; i < num_particles; ++i)
	{
		auto p = particles[i];

		FIntPoint cell_index = static_cast<FIntPoint>(p->x.X, p->x.Y);
		FVector2f cell_diff = (p->x - cell_index) - 0.5f;

		Weights[0] = (0.5f * FVector2f(FMath::Pow(0.5f - cell_diff.X, 2), FMath::Pow(0.5f - cell_diff.Y, 2)));//index 0
		Weights[1] = (0.75f * FVector2f(FMath::Pow(cell_diff.X, 2), FMath::Pow(cell_diff.Y, 2))); //index 1
		Weights[2] = (0.5f * FVector2f(FMath::Pow(0.5f + cell_diff.X, 2), FMath::Pow(0.5f + cell_diff.Y, 2)));//index 2

		//for all surrounding 9 cells
		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				float weight = Weights[gx][0] * Weights[gy][1];

				FIntPoint cell_x = FIntPoint(cell_index.X + gx - 1, cell_index.Y + gy - 1);
				FVector2f cell_dist = FVector2f(cell_x.X - p->x.X, cell_x.Y - p->x.Y) + 0.5;
				//FVector2f Q = FVector2D(p.C.GetColumn(0) * cell_dist.X ,p.C.GetColumn(0) * cell_dist.Y); // matrix 2*2 와 vector 곱
				FVector2f Q = 0;

				float mass_contrib = weight * p->mass;

				//converting 2D index to 1D
				int New_cell_index = static_cast<int>(cell_x.X) * grid_res + static_cast<int>(cell_x.Y);
				Cell* cell = grid[New_cell_index];

				//scatter mass to the grid
				cell->mass += mass_contrib;

				cell->v += mass_contrib * (p->v + Q);
				grid[New_cell_index] = cell;
			}
		}
	}


	//grid velocity update
	for (int i = 0; i < num_cells; ++i)
	{
		auto cell = grid[i];

		if (cell->mass > 0)
		{
			//convert momentum to velocity, apply gravity
			cell->v /= cell->mass;
			cell->v += dt * FVector2f(0, gravity);

			//boundary conditions
			int x = i / grid_res;
			int y = i % grid_res;
			if (x<2 || x>grid_res - 3)
			{
				cell->v.X = 0;
			}
			if (y<2 || y>grid_res - 3)
			{
				cell->v.Y = 0;
			}

			grid[i] = cell;
		}
		grid[i] = cell;
	}

	//G2P
	for (int i = 0; i < num_particles; ++i)
	{
		auto p = particles[i];

		//reset particle velocity. we calculate it from scratch each step using the grid
		p->v.Zero();

		//quadratic interpolation weights
		FIntPoint cell_index2 = static_cast<FIntPoint>(p->x.X, p->x.Y);
		FVector2f cell_diff2 = (p->x - cell_index2) - 0.5f;

		Weights[0] = 0.5f * FVector2f(FMath::Pow(0.5f - cell_diff2.X, 2), FMath::Pow(0.5f - cell_diff2.Y, 2));
		Weights[1] = 0.75f * FVector2f(FMath::Pow(cell_diff2.X, 2), FMath::Pow(cell_diff2.Y, 2));
		Weights[2] = 0.5f * FVector2f(FMath::Pow(0.5f + cell_diff2.X, 2), FMath::Pow(0.5f + cell_diff2.Y, 2));


		//APIC
		FMatrix2x2 B = {0,0,0,0};
		
		for (UINT gx = 0; gx < 3; ++gx)
		{
			for (UINT gy = 0; gy < 3; ++gy)
			{
				float weight = Weights[gx].X * Weights[gy].Y;

				FIntPoint cell_x = FIntPoint(cell_index2.X + gx - 1, cell_index2.Y + gy - 1);
				int cell_index = static_cast<int>(cell_x.X) * grid_res + static_cast<int>(cell_x.Y);

				FVector2f dist = (cell_x.X - p->x.X, cell_x.Y - p->x.Y) + 0.5f;
				FVector2f weighted_velocity = grid[cell_index]->v * weight;

				//APIC paper equation 10
				auto term = FMatrix2x2(weighted_velocity.X * dist.X, weighted_velocity.Y * dist.X, weighted_velocity.X * dist.Y, weighted_velocity.Y * dist.Y);
				B = term;

				p->v += weighted_velocity;
			}
		}
		float a, b, c, d;
		B.GetMatrix(a, b, c, d);

		//p.C = { a * 4, b * 4, c * 4, d * 4 };

		//adverty particles
		p->x += p->v * dt;

		//safety clamp
		p->x.X = FMath::Clamp(p->x.X, 1, grid_res-2);
		p->x.Y = FMath::Clamp(p->x.Y, 1, grid_res - 2);

		particles[i] = p;
	}
}

