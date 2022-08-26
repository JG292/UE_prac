// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

void UMyBlueprintFunctionLibrary::callcall() {

	//if (GEngine) {
		APlayerController *PlayerController =
			//GEngine->GetWorld()->GetFirstLocalPlayerFromController();
			GEngine->GetWorld()->GetFirstPlayerController();

		//GEngine->GetWorldInstance();
		//GEngine->Getplay
	//}

	const FString &Addr = "192.168.0.174";

	//APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController) {
		PlayerController->ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
	}
}