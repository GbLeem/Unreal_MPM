// Fill out your copyright notice in the Description page of Project Settings.


#include "Instanced_MPM.h"
//#include "MPM3D.h"

//#include "Async/ParallelFor.h"

UInstanced_MPM::UInstanced_MPM(const FObjectInitializer& Initializer)
{
}

void UInstanced_MPM::OnRegister()
{

}

void UInstanced_MPM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//{
	//	SCOPE_CYCLE_COUNTER(STAT_Step);

	//	for (int32 Idx = 0; Idx < SystemConfig.IterationCount; ++Idx)
	//	{
	//		FluidSystem->Step();
	//	}
	//}

	//{
	//	SCOPE_CYCLE_COUNTER(STAT_CopyBack);
	//	FluidParticles->CopyBackParticleTransforms(reinterpret_cast<float*>(PerInstanceSMData.GetData()));
	//}

	//// Force recreation of the render data when proxy is created
	//InstanceUpdateCmdBuffer.NumEdits++;

	MarkRenderStateDirty();
}
