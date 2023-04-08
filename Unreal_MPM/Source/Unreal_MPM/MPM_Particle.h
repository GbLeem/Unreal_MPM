#pragma once

#include "Components/InstancedStaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPM_Particle.generated.h"

UENUM()
enum class PTYPE
{
	Inactive = 0,
	Elastic,
	Snow,
	Liquid
};

UCLASS()
class UNREAL_MPM_API AMPM_Particle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPM_Particle();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

protected:
	PTYPE type;
	float mass;
	float volume;

	FVector3d pos;
	FVector3d vel;

	int NumParticles;
	/*FMatrix B;
	FMatrix D;
	FMatrix Fe;
	FMatrix Fp;*/
	
	/*FVector3d padding;

	FVector3d Pos = pos;*/
	
	//UPROPERTY(Transient) //직렬화
	//TArray<FTransform> Transforms;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedParticle;
};
