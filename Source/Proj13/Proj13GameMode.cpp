// Copyright Epic Games, Inc. All Rights Reserved.


#include "Proj13GameMode.h"
#include "Proj13Character.h"
#include "UObject/ConstructorHelpers.h"

#include "CustomHUD.h"
#include "GameFramework/HUD.h"

AProj13GameMode::AProj13GameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TPS/Character/BP_Person"));
	if (PlayerPawnBPClass.Class != NULL)									
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = ACustomHUD::StaticClass();

	//static ConstructorHelpers::FObjectFinder<UBlueprint> HUDObjectFinder(TEXT("WidgetBlueprint'/Game/TPS/HUD/HUD'"));
	//if (HUDObjectFinder.Object != nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("Hello"));
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("detect Sender"));

	//	HUDClass = (UClass*)HUDObjectFinder.Object->GeneratedClass;
	//}


	//static ConstructorHelpers::FClassFinder<AHUD> HUDObjectFinder(TEXT("WidgetBlueprint'/Game/TPS/HUD/HUD'"));
	////HUDClass = 
	//if (HUDObjectFinder.Class != nullptr) {
	//	HUDClass = HUDObjectFinder.Class;
	//}
}
