// Fill out your copyright notice in the Description page of Project Settings.


#include "Person.h"


#include "Gun.h"
#include "Bullet.h"
#include "Grenade.h"

// Sets default values
APerson::APerson()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	TagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TagWidget"));
	TagWidget->SetupAttachment(RootComponent);

	ScoreWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoreWidget"));
	ScoreWidget->SetupAttachment(RootComponent);

	bThrowGrenade = false;
}

// Called when the game starts or when spawned
void APerson::BeginPlay()
{
	Super::BeginPlay();

	//ScoreWidget->Set

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APerson::OnHit);
	
}

// Called every frame
void APerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (OverlappingGun) {
	//	OverlappingGun->ShowPickupWidget(true);
	//}


}

// Called to bind functionality to input
void APerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APerson::MoveX);
	PlayerInputComponent->BindAxis("MoveRight", this, &APerson::MoveY);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APerson::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APerson::StopJump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &APerson::StartEquipGun);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APerson::TraceAndFire);
	PlayerInputComponent->BindAction("Spotlight", IE_Pressed, this, &APerson::SwitchSpotlight);
	//PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &APerson::TraceAndThrow);
	//PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &APerson::StartThrow);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &APerson::ServerStartThrow);


	PlayerInputComponent->BindTouch(IE_Pressed, this, &APerson::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &APerson::TouchStopped);

}

void APerson::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// only replicate to owner
	DOREPLIFETIME_CONDITION(APerson, OverlappingGun, COND_OwnerOnly);

	DOREPLIFETIME(APerson, EquippedGun);
}

void APerson::SwitchServer(const FString &Addr) {

	//const FString  &Addr = "192.168.0.174";

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
	//	FString::Printf(TEXT("switch server")));

	APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (false) {
		//if (PlayerController) {

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		//	FString::Printf(TEXT("get controller!")));

		//PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);

		const FString &name = GetWorld()->GetMapName();
		if (name.Len() == 11) {
			PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
		}

	}
	else {
		const FString &name = GetWorld()->GetMapName();
		if (name.Len() == 11) {
			UGameplayStatics::OpenLevel(this, *Addr);
		}
	}
}

void APerson::MoveX(float Value) {
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void APerson::MoveY(float Value) {
	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	AddMovementInput(Direction, Value);
}

void APerson::StartJump() {
	bPressedJump = true;
}

void APerson::StopJump() {
	bPressedJump = false;
}

void APerson::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
	//	FString::Printf(TEXT("touch and jump!!")));

	//const FString &Addr = "192.168.0.174";
	////UGameplayStatics::OpenLevel(this, *Addr);

	//APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	//if (PlayerController) {
	//	PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
	//}

	//Jump();
	//Fire();
	//ServerFire();

	TraceAndFire();
}


void APerson::SetOverlappingGun(AGun *Gun) {
	// manually remove tag for server, if it's set before
	if (OverlappingGun) {
		OverlappingGun->ShowPickupWidget(false);
	}

	OverlappingGun = Gun;
	// manually set flag for server, since only called by server
	if (IsLocallyControlled() && OverlappingGun != nullptr) {
		OverlappingGun->ShowPickupWidget(true);
	}
}

// arg: last value before ovrride
void APerson::OnRep_OverlappingGun(AGun *PrevWeapon) {
	if (PrevWeapon) {
		PrevWeapon->ShowPickupWidget(false);
	}
	if (OverlappingGun) {
		OverlappingGun->ShowPickupWidget(true);
	}
}

void APerson::StartEquipGun() {
	bUseControllerRotationYaw = true;

	if (HasAuthority()) {
		EquipGun(OverlappingGun);
	}
	else {
		ServerStartEquipGun();
	}
}

void APerson::EquipGun(class AGun *InGun) {
	if (InGun == nullptr) return;

	EquippedGun = InGun;
	const USkeletalMeshSocket *WeaponHand = GetMesh()->GetSocketByName(FName("GunHand"));
	if (WeaponHand) {
		WeaponHand->AttachActor(InGun, GetMesh());
	}
	EquippedGun->SetOwner(this);
	EquippedGun->ShowPickupWidget(false);
	EquippedGun->HitComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// comply with animation
	GetCharacterMovement()->JumpZVelocity = 420;
}

void APerson::ServerStartEquipGun_Implementation() {
	EquipGun(OverlappingGun);
}

void APerson::SwitchSpotlight() {
	ServerSwitchSpotlight();
}

void APerson::ServerSwitchSpotlight_Implementation() {
	MulticastSwitchSpotlight();
}

void APerson::MulticastSwitchSpotlight_Implementation() {
	if (EquippedGun) {
		float CrtIntensity = EquippedGun->Spotlight->Intensity;
		if (CrtIntensity > 100.f) {
			EquippedGun->Spotlight->SetIntensity(0.f);
		}
		else {
			EquippedGun->Spotlight->SetIntensity(40000.f);
		}
	}
}

void APerson::ServerStartThrow_Implementation() {
	MulticastStartThrow();
}

void APerson::MulticastStartThrow_Implementation() {
	StartThrow();
}

void APerson::StartThrow() {
	bThrowGrenade = true;
	//if (ThrowGrenadeAnim) {
	//	GetMesh()->PlayAnimation(ThrowGrenadeAnim, false);
	//}
}

void APerson::TraceAndThrow() {

	if (BulletClass) {
		FVector ActorLocation;
		FRotator ActorRotation;
		//ActorLocation = GetActorLocation();
		//ActorRotation = GetActorRotation();
		//MuzzleOffset.Set(100.f, 0.f, 0.f);

		GetActorEyesViewPoint(ActorLocation, ActorRotation);

		MuzzleOffset.Set(100.f, 0.f, 50.f);

		FVector MuzzleLocation = ActorLocation + FTransform(ActorRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = ActorRotation;
		//MuzzleRotation.Pitch += 10.f;

		ThrowInDirection(MuzzleLocation, MuzzleRotation);
	}
}

void APerson::ServerThrowInDirection_Implementation(const FVector &Location, const FRotator &Rotation) {
	MulticastFireInDirection(Location, Rotation);
}

void APerson::MulticastThrowInDirection_Implementation(const FVector &Location, const FRotator &Rotation) {
	FireInDirection(Location, Rotation);
}

void APerson::ThrowInDirection(const FVector &Location, const FRotator &Rotation) {

	UWorld *World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		AGrenade *Grenade = World->SpawnActor<AGrenade>(GrenadeClass, Location, Rotation, SpawnParams);
		Grenade->Sender = this;

		if (Grenade) {
			FVector LauchDirection = Rotation.Vector();
			Grenade->FireInDirection(LauchDirection);
		}

	}

	bThrowGrenade = false;
}

void APerson::TraceAndFire() {

	if (BulletClass) {
		FVector ActorLocation;
		FRotator ActorRotation;
		//ActorLocation = GetActorLocation();
		//ActorRotation = GetActorRotation();
		//MuzzleOffset.Set(100.f, 0.f, 0.f);

		GetActorEyesViewPoint(ActorLocation, ActorRotation);

		MuzzleOffset.Set(2.f, 0.f, 0.f);
		MuzzleOffset.Set(100.f, 0.f, 0.f);

		FVector MuzzleLocation = ActorLocation + FTransform(ActorRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = ActorRotation;
		//MuzzleRotation.Pitch += 10.f;

		// display crosshair
		FHitResult TraceHitRes;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitRes,
			MuzzleLocation,
			MuzzleLocation + FTransform(MuzzleRotation).TransformVector(FVector(8000.f, 0, 0)),
			ECollisionChannel::ECC_Visibility
		);
		if (TraceHitRes.bBlockingHit) {
			DrawDebugSphere(
				GetWorld(),
				TraceHitRes.ImpactPoint,
				12.f,
				12,
				FColor::Red
			);
		}

		ServerFireInDirection(MuzzleLocation, MuzzleRotation);

		//UWorld *World = GetWorld();
		//if (World) {
		//	FActorSpawnParameters SpawnParams;
		//	SpawnParams.Owner = this;
		//	SpawnParams.Instigator = GetInstigator();
		//	ABullet *Bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire %s"), "SS");
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hello %s"), *GetActorLocation().ToString()));
		//	//UE_LOG(LogTemp, Warning, TEXT("Text, %s"), ActorLocation.ToString());
		//	if (Bullet) {
		//		FVector LauchDirection = MuzzleRotation.Vector();
		//		Bullet->FireInDirection(LauchDirection);
		//	}

		//}


	}
}

void APerson::ServerFireInDirection_Implementation(const FVector &Location, const FRotator &Rotation) {
	MulticastFireInDirection(Location, Rotation);
}

void APerson::MulticastFireInDirection_Implementation(const FVector &Location, const FRotator &Rotation) {
	FireInDirection(Location, Rotation);
}

void APerson::FireInDirection(const FVector &Location, const FRotator &Rotation) {

	UWorld *World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = this;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		ABullet *Bullet = World->SpawnActor<ABullet>(BulletClass, Location, Rotation, SpawnParams);
		Bullet->Sender = this;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire %s"), "SS");
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hello %s"), *GetActorLocation().ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("Text, %s"), ActorLocation.ToString());
		if (Bullet) {
			APerson *Sender = Cast<APerson>(Bullet->GetOwner());
			FVector LauchDirection = Rotation.Vector();
			Bullet->FireInDirection(LauchDirection);
		}

	}
}


void APerson::Fire() {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire"));


	if (BulletClass) {
		FVector ActorLocation;
		FRotator ActorRotation;
		//ActorLocation = GetActorLocation();
		//ActorRotation = GetActorRotation();
		//MuzzleOffset.Set(100.f, 0.f, 0.f);

		GetActorEyesViewPoint(ActorLocation, ActorRotation);

		MuzzleOffset.Set(2.f, 0.f, 0.f);
		MuzzleOffset.Set(100.f, 0.f, 0.f);

		FVector MuzzleLocation = ActorLocation + FTransform(ActorRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = ActorRotation;
		//MuzzleRotation.Pitch += 10.f;

		// display crosshair
		FHitResult TraceHitRes;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitRes,
			MuzzleLocation,
			MuzzleLocation + FTransform(MuzzleRotation).TransformVector(FVector(8000.f, 0, 0)),
			ECollisionChannel::ECC_Visibility
		);
		if (TraceHitRes.bBlockingHit) {
			DrawDebugSphere(
				GetWorld(),
				TraceHitRes.ImpactPoint,
				12.f,
				12,
				FColor::Red
			);
		}

		UWorld *World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet *Bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire %s"), "SS");
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hello %s"), *GetActorLocation().ToString()));
			//UE_LOG(LogTemp, Warning, TEXT("Text, %s"), ActorLocation.ToString());
			if (Bullet) {
				FVector LauchDirection = MuzzleRotation.Vector();
				Bullet->FireInDirection(LauchDirection);
			}

		}


	}
}

//void APerson::Fire2() {
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire"));
//
//	if (BulletClass) {
//		FVector ActorLocation;
//		FRotator ActorRotation;
//		//ActorLocation = GetActorLocation();
//		//ActorRotation = GetActorRotation();
//		//MuzzleOffset.Set(100.f, 0.f, 0.f);
//
//		GetActorEyesViewPoint(ActorLocation, ActorRotation);
//
//		MuzzleOffset.Set(2.f, 0.f, 0.f);
//		MuzzleOffset.Set(100.f, 0.f, 0.f);
//
//		FVector MuzzleLocation = ActorLocation + FTransform(ActorRotation).TransformVector(MuzzleOffset);
//
//		FRotator MuzzleRotation = ActorRotation;
//		//MuzzleRotation.Pitch += 10.f;
//
//		// display crosshair
//		FHitResult TraceHitRes;
//		GetWorld()->LineTraceSingleByChannel(
//			TraceHitRes,
//			MuzzleLocation,
//			MuzzleLocation + FTransform(MuzzleRotation).TransformVector(FVector(8000.f, 0, 0)),
//			ECollisionChannel::ECC_Visibility
//		);
//		if (TraceHitRes.bBlockingHit) {
//			DrawDebugSphere(
//				GetWorld(),
//				TraceHitRes.ImpactPoint,
//				12.f,
//				12,
//				FColor::Red
//			);
//		}
//
//		UWorld *World = GetWorld();
//		if (World) {
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			SpawnParams.Instigator = GetInstigator();
//			ABullet *Bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//
//			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Fire %s"), "SS");
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hello %s"), *GetActorLocation().ToString()));
//			//UE_LOG(LogTemp, Warning, TEXT("Text, %s"), ActorLocation.ToString());
//			if (Bullet) {
//				FVector LauchDirection = MuzzleRotation.Vector();
//				Bullet->FireInDirection(LauchDirection);
//			}
//
//		}
//
//
//	}
//}

void APerson::ServerFire_Implementation() {
	MulticastFire();
}

void APerson::MulticastFire_Implementation() {
	Fire();
}

void APerson::MulticastChangeScore_Implementation(float Change) {
	Score += Change;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("add score"));


}

void APerson::OnHit(
	UPrimitiveComponent *HitComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComponent,
	FVector NormalImpulse,
	const FHitResult &Hit) {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("player hit!"));

	FVector hitPosition = Hit.ImpactPoint;
	//FVector origin = GetActorLocation();

	ABullet *IncomingBullet = Cast<ABullet>(OtherActor);

	if (IncomingBullet && IncomingBullet->GetActorEnableCollision()) {

		APerson *Sender = IncomingBullet->Sender;

		IncomingBullet->Destroy();
		IncomingBullet->SetActorEnableCollision(false);
		IncomingBullet->HitComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (HasAuthority()) {
			MulticastChangeScore(-1);

			if (Sender) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("add score"));
				Sender->MulticastChangeScore(1);
			}
		}

	}

	//if (IncomingBullet && HasAuthority()) {

	//	MulticastChangeScore(-1.f);

	//	//if (IncomingBullet->Sender) {

	//	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("detect Sender"));
	//	//	IncomingBullet->Sender->MulticastChangeScore(1.f);
	//	//}

	//	//APerson *Sender = Cast<APerson>(IncomingBullet->GetOwner());
	//	//if (Sender) {
	//	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("tb2 add score"));

	//	//	//Sender->MulticastChangeScore(1.f);
	//	//}

	//}

	//if (IncomingBullet) {
	//	IncomingBullet->Destroy();
	//	IncomingBullet->DisableComponentsSimulatePhysics();
	//}

	//if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
	//	OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.f, Hit.ImpactPoint);
	//}

	//Destroy();
}
