// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractionBall.generated.h"

UCLASS()
class UNREAL_MPM_API AInteractionBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractionBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_pMesh;

	UPROPERTY(VisibleAnywhere)
	FVector3f m_MeshLocation;
};
