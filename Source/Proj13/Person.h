// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Person.generated.h"

UCLASS()
class PROJ13_API APerson : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APerson();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutProps) const override;

	// Comp

	UPROPERTY(BlueprintReadOnly)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent *TagWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent *ScoreWidget;

	// Func

	UFUNCTION(BlueprintCallable)
		void SwitchServer(const FString &Addr);

	UFUNCTION()
		void MoveX(float Value);

	UFUNCTION()
		void MoveY(float Value);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION()
	void SetOverlappingGun(AGun *Gun);

	UFUNCTION()
	void OnRep_OverlappingGun(AGun *PrevWeapon);

	UFUNCTION(BlueprintCallable)
	void StartEquipGun();

	UFUNCTION(BlueprintCallable)
	void EquipGun(class AGun *InGun);

	UFUNCTION(Server, Reliable)	// RPC for equip weapon
		void ServerStartEquipGun();

	//UFUNCTION()
	//	void Fire(const FVector &TargetLoc);

	//UFUNCTION(Server, Reliable, BlueprintCallable)
	//void ServerFire(const FVector &TargetLoc);

	//UFUNCTION(NetMulticast, Reliable)
	//	void MulticastFire(const FVector &TargetLoc);

	UFUNCTION(BlueprintCallable)
		void SwitchSpotlight();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ServerSwitchSpotlight();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSwitchSpotlight();

	UFUNCTION()
		void TraceAndThrow();

	UFUNCTION()
		void ThrowInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ServerThrowInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastThrowInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION()
		void TraceAndFire();

	UFUNCTION()
	void FireInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ServerFireInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFireInDirection(const FVector &Location, const FRotator &Rotation);

	UFUNCTION()
		void Fire();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeScore(float Change);

	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, FVector NormalImpulse,
			const FHitResult &Hit);

	// Var
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingGun)
	class AGun *OverlappingGun;

	UPROPERTY(Replicated)
		class AGun *EquippedGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AGrenade> GrenadeClass;
};
