// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst_Person.h"

#include "Person.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInst_Person::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	ThisPerson = Cast<APerson>(TryGetPawnOwner());
}

void UAnimInst_Person::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	if (ThisPerson == nullptr) {
		ThisPerson = Cast<APerson>(TryGetPawnOwner());
	}
	if (ThisPerson == nullptr)
		return;

	FVector Velocity = ThisPerson->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bInAir = ThisPerson->GetCharacterMovement()->IsFalling();
	bAccel = (ThisPerson->GetCharacterMovement()->GetCurrentAcceleration().Size()) > 0.f;
	bEquipGun = ThisPerson->EquippedGun != nullptr;

	FRotator PersonRotation = ThisPerson->GetActorRotation();
	FVector LocalVelo = FTransform(PersonRotation).InverseTransformVector(Velocity);
	VeloX = LocalVelo.X;
	VeloY = LocalVelo.Y;

	bIsThrowingGrenade = ThisPerson->bThrowGrenade;
}

