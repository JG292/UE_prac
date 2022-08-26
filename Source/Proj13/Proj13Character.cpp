// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proj13Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AProj13Character

AProj13Character::AProj13Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProj13Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProj13Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProj13Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProj13Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProj13Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProj13Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProj13Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProj13Character::OnResetVR);
}

void AProj13Character::OnResetVR()
{
	// If Proj13 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Proj13.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProj13Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		FString::Printf(TEXT("touch and jump!!")));

	//const FString &Addr = "192.168.0.174";
	////UGameplayStatics::OpenLevel(this, *Addr);

	//APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	//if (PlayerController) {
	//	PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
	//}

	Jump();
}

void AProj13Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProj13Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProj13Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProj13Character::MoveForward(float Value)
{

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProj13Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AProj13Character::CallCall(const FString &Addr) {

	//const FString &Addr = "192.168.0.174";

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
		FString::Printf(TEXT("char call call")));

	APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (false) {
	//if (PlayerController) {

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
			FString::Printf(TEXT("get controller!")));

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

//void AProj13Character::CallOpenLevel(const FString &Addr) {
//
//
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
//	//	FString::Printf(TEXT("distance Warn")));
//
//	UGameplayStatics::OpenLevel(this, *Addr);
//}

//void AProj13Character::CallClientTravel(const FString &Addr) {
//	APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
//	if (PlayerController) {
//		PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
//	}
//}

void AProj13Character::BeginPlay() {
	Super::BeginPlay();

	////const FString &Addr = "192.168.0.174";
	////UGameplayStatics::OpenLevel(this, *Addr);

	//const FString &name = GetWorld()->GetMapName();

	//if (name.Len() == 11) {

	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, name);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("begin play %d"), name.Len()));

	//}


	//if (name.Len() == 15) {

	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
	//		FString::Printf(TEXT("begin play")));

		//APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		//if (PlayerController) {
		//	//PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
		//	PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
		//}

		//UGameplayStatics::OpenLevel(this, *Addr);

	//}

	//if (!HasAuthority()) {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
	//		FString::Printf(TEXT("client")));

	//	APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	//	if (PlayerController) {
	//		PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
	//	}

	//}

	//if (!HasAuthority()) {

	//	//CallClientTravel("192.168.0.174");
	//	UGameplayStatics::OpenLevel(this, *Addr);
	//}
}

