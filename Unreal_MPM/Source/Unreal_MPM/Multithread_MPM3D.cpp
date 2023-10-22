// Fill out your copyright notice in the Description page of Project Settings.


#include "Multithread_MPM3D.h"

AMultithread_MPM3D::AMultithread_MPM3D()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMultithread_MPM3D::BeginPlay()
{
	Super::BeginPlay();
	StartAsyncTask();
	StartAsyncTask_UsingAsyncTask();
}

void AMultithread_MPM3D::StartAsyncTask()
{
	(new FAutoDeleteAsyncTask<FMyAsyncTask>(10))->StartBackgroundTask();
}

void AMultithread_MPM3D::StartAsyncTask_UsingAsyncTask()
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, []()
		{
			auto MyTask = new FAsyncTask<FMyAsyncTask>(10);
	MyTask->StartBackgroundTask();
	MyTask->EnsureCompletion();
	delete MyTask;
	UE_LOG(LogTemp, Log, TEXT("[MyLog] Stop: AsyncTask"));
		});
}

void FMyAsyncTask::DoWork()
{
	while (LoopCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[MyLog] Run : NonAbandonbaleTask : %d"), LoopCount--);
		FPlatformProcess::Sleep(0.5f);
	}
	UE_LOG(LogTemp, Log, TEXT("[MyLog] Stop - NonAbandonableTask"));
}
