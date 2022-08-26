// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Person.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

	// hit
	HitComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitComp"));
	RootComponent = HitComp;

	//bReplicates = true;

	HitComp->InitSphereRadius(15.f);
	HitComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//HitComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	HitComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);


	// Movement
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	ProjectileMovementComp->SetUpdatedComponent(HitComp);
	ProjectileMovementComp->InitialSpeed = 3000.f;

	//ProjectileMovementComp->InitialSpeed = 1000.f;
	//ProjectileMovementComp->ProjectileGravityScale = 2.f;

	//ProjectileMovementComp->InitialSpeed = 0;
	ProjectileMovementComp->MaxSpeed = 3000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;
	//ProjectileMovementComp->ProjectileGravityScale = 0.f;

	// Mesh
	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	ProjectileMeshComp->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("'/Game/TPS/Sphere.Sphere'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TPS/Weapon/BulletMesh"));
	if (Mesh.Succeeded()) {
		ProjectileMeshComp->SetStaticMesh(Mesh.Object);
	}
	ProjectileMeshComp->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//if (HasAuthority()) {
		HitComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	//}

	//StaticMeshComp->OnComponentHit.AddDynamic(this, &ATargetBase::OnHit);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::FireInDirection(const FVector &ShootDirection) {
	ProjectileMovementComp->Velocity = ShootDirection * ProjectileMovementComp->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit) {
	//Destroy();

	//SetLifeSpan(0.02f);

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComp->Velocity * 100.f, Hit.ImpactPoint);
	}

	APerson *OtherPerson = Cast<APerson>(OtherActor);
	if (OtherPerson) {
		Destroy();
	}
	else {
		SetLifeSpan(0.02f);
	}
}

