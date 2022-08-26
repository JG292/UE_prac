// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SpotLightComponent.h"

#include "Person.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMesh;

	GunMesh->SetCollisionResponseToAllChannels(ECR_Block);
	GunMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FlashlightMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FlashlightMesh"));
	FlashlightMesh->SetupAttachment(RootComponent);

	Spotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	Spotlight->SetupAttachment(RootComponent);

	HitComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitComp"));
	HitComp->SetupAttachment(RootComponent);
	HitComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	Spotlight->SetIntensity(0.f);

	// enable collision on server
	if(HasAuthority()) {
		HitComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HitComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		HitComp->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnSphereOverlap);
		HitComp->OnComponentEndOverlap.AddDynamic(this, &AGun::OnSphereEndOverlap);
	}

	PickupWidget->SetVisibility(false);
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::OnSphereOverlap(
	UPrimitiveComponent *OverlappedComp, 
	AActor *OtherActor, 
	UPrimitiveComponent *OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult) {

	APerson *OtherPerson = Cast<APerson>(OtherActor);
	if (OtherPerson) {
		//PickupWidget->SetVisibility(true);

		//OtherPerson->OverlappingGun = this;

		OtherPerson->SetOverlappingGun(this);
	}

}

void AGun::OnSphereEndOverlap(
	UPrimitiveComponent *OverlappedComp, 
	AActor *OtherActor, 
	UPrimitiveComponent *OtherComp, 
	int32 OtherBodyIndex) {
	APerson *OtherPerson = Cast<APerson>(OtherActor);
	if (OtherPerson) {
		OtherPerson->SetOverlappingGun(nullptr);
	}

}

void AGun::ShowPickupWidget(bool bShow) {
	PickupWidget->SetVisibility(bShow);
}

