
#include "InteractionBall.h"


AInteractionBall::AInteractionBall()
{
	
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	SetRootComponent(m_pMesh);
	m_MeshLocation = { 0.f,0.f,0.f };

}


void AInteractionBall::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInteractionBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_MeshLocation.X += 10.f;
}

void AInteractionBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("moveforward", this, &AInteractionBall::MoveForward);
	PlayerInputComponent->BindAxis("moveright", this, &AInteractionBall::MoveRight);

}

void AInteractionBall::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		FVector Direction = FVector(1.f, 0.f, 0.f);
		AddMovementInput(Direction, Value);
	}
}

void AInteractionBall::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		FVector Direction = FVector(0.f, 1.f, 0.f);
		AddMovementInput(Direction, Value);
	}
}

