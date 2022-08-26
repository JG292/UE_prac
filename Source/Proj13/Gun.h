// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//#include "Components/SkeletalMeshComponent.h"

#include "Gun.generated.h"

UCLASS()
class PROJ13_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Comp
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *GunMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *FlashlightMesh;

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent *Spotlight;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent *HitComp;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent *PickupWidget;

	// FUNC
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent *OverlappedComp,
			AActor *OtherActor,
			UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult
		);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent *OverlappedComp,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex
	);

	void ShowPickupWidget(bool bShow);

};
