#include "MPM_Particle.h"

// Sets default values
AMPM_Particle::AMPM_Particle()
	:type(PTYPE::Inactive)
	, mass(0)
	, volume(0)
	, pos()
	, vel()
	, NumParticles(8*8)
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

void AMPM_Particle::BeginPlay()
{
	Super::BeginPlay();

	/*float CustomData = 0.f;
	InstancedParticle->SetCustomDataValue(0, 0, CustomData, false);*/
}

void AMPM_Particle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FTransform> OffsetTransformContainer;
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
	InstancedParticle->MarkRenderStateDirty();
}


