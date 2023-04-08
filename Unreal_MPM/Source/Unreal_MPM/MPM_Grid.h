// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM_Grid.generated.h"

UCLASS()
class UNREAL_MPM_API AMPM_Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	AMPM_Grid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
