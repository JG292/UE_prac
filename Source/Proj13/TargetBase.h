// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "TargetBase.generated.h"

UCLASS()
class PROJ13_API ATargetBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Comp
	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent *StaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent *BaseMesh;

	UPROPERTY(VisibleDefaultsOnly)
		UPhysicsConstraintComponent *PhysicsConstraint;


	// Func
	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, FVector NormalImpulse,
			const FHitResult &Hit);


	// Var
};
