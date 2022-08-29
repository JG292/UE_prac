// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Person.h"

// Sets default values
AGrenade::AGrenade()
{ 
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// hit
	HitComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitComp"));
	RootComponent = HitComp;

	HitComp->InitSphereRadius(15.f);
	HitComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//HitComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	ExplodeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplodeSphere"));
	ExplodeSphere->SetupAttachment(RootComponent);
	ExplodeSphere->InitSphereRadius(100.f);
	ExplodeSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ExplodeSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Movement
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetUpdatedComponent(HitComp);

	ProjectileMovementComp->InitialSpeed = 1000.f;
	ProjectileMovementComp->ProjectileGravityScale = 2.f;
	ProjectileMovementComp->MaxSpeed = 3000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.5f;

	// Mesh
	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	ProjectileMeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TPS/Weapon/BulletMesh"));
	if (Mesh.Succeeded()) {
		ProjectileMeshComp->SetStaticMesh(Mesh.Object);
	}
	ProjectileMeshComp->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	HitComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::FireInDirection(const FVector &ShootDirection) {
	ProjectileMovementComp->Velocity = ShootDirection * ProjectileMovementComp->InitialSpeed;
}

void AGrenade::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit) {
	//Destroy();

	//SetLifeSpan(0.02f);

	//if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
	//	OtherComponent->AddImpulseAtLocation(ProjectileMovementComp->Velocity * 100.f, Hit.ImpactPoint);
	//}

	//APerson *OtherPerson = Cast<APerson>(OtherActor);
	//if (OtherPerson) {
	//	Destroy();
	//}
	//else {
	//	SetLifeSpan(0.2f);
	//}

	TArray<AActor*> OverlappingActors;
	ExplodeSphere->SetSphereRadius(500.f);
	ExplodeSphere->GetOverlappingActors(OverlappingActors);
	for (AActor *Actor : OverlappingActors) {
		UStaticMeshComponent *OtherMesh = Cast<UStaticMeshComponent>(Actor->GetRootComponent());
		if (OtherMesh) {
			Actor->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("detect expl"));


			OtherMesh->AddImpulse(
				(OtherActor->GetActorLocation() - GetActorLocation()) * 1000.f);
		}
	}

	//Destroy();
}
