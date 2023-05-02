// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Instanced_MPM.generated.h"

//DECLARE_STATS_GROUP(TEXT("SPH"), STATGROUP_SPH, STATCAT_Advanced);
//DECLARE_CYCLE_STAT(TEXT("Step"), STAT_Step, STATGROUP_SPH);
//DECLARE_CYCLE_STAT(TEXT("Copy Back"), STAT_CopyBack, STATGROUP_SPH);
//DECLARE_CYCLE_STAT(TEXT("Update Rendering"), STAT_UpdateRendering, STATGROUP_SPH);


UCLASS()
class UNREAL_MPM_API UInstanced_MPM : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UInstanced_MPM(const FObjectInitializer& Initializer);

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
