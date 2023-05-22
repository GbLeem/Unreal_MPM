// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Multithread_MPM3D.generated.h"

UCLASS()
class UNREAL_MPM_API AMultithread_MPM3D : public AActor
{
	GENERATED_BODY()
	
public:	
	AMultithread_MPM3D();

protected:
	virtual void BeginPlay() override;

public:
	void StartAsyncTask();
	void StartAsyncTask_UsingAsyncTask();
};

class FMyAsyncTask : public FNonAbandonableTask
{
public:
	FMyAsyncTask(int32 InLoopCount) :LoopCount(InLoopCount) {}
	FORCEINLINE TStatId GetStatId() const 
	{ 
		RETURN_QUICK_DECLARE_CYCLE_STAT(FMyAsyncTask, STATGROUP_ThreadPoolAsyncTasks); 
	}
	void DoWork();
private:
	int32 LoopCount;
};
