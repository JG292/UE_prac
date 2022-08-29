// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBase.h"

#include "Person.h"
#include "Bullet.h"

// Sets default values
ATargetBase::ATargetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TargetSceneRoot"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	//StaticMeshComp->SetCollisionObjectType(ECC_WorldStatic);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhyCons"));
	PhysicsConstraint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATargetBase::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ATargetBase::OnHit);

}

// Called every frame
void ATargetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetBase::OnHit(
	UPrimitiveComponent *HitComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComponent,
	FVector NormalImpulse,
	const FHitResult &Hit) {


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("hit!"));

	FVector hitPosition = Hit.ImpactPoint;
	//FVector origin = GetActorLocation();

	FVector origin = StaticMeshComp->GetComponentLocation();
	//origin = StaticMeshComp->Bounds.GetSphere().Center;

	float d = FVector::Dist(hitPosition, origin);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	//	FString::Printf(TEXT("hit distance : %f"), d));

	ABullet *IncomingBullet = Cast<ABullet>(OtherActor);
	if (IncomingBullet && HasAuthority()) {
		if (IncomingBullet->Sender) {

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("detect Sender"));
			IncomingBullet->Sender->MulticastChangeScore(1);
		}
		
	}
}

