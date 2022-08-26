// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Grenade.generated.h"


UCLASS()
class PROJ13_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Comp
	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent *HitComp;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent *ExplodeSphere;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent *ProjectileMovementComp;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent *ProjectileMeshComp;

	// Func
	void FireInDirection(const FVector &ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, FVector NormalImpulse,
			const FHitResult &Hit);

	// Var
	class APerson *Sender;

};
