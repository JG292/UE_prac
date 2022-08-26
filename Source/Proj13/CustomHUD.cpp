// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomHUD.h"

#include "Blueprint/UserWidget.h"

ACustomHUD::ACustomHUD() {

	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetObj(TEXT("/Game/TPS/HUD/HUD"));
	if (hudWidgetObj.Succeeded()) {
		hudWidgetClass = hudWidgetObj.Class;
	}
	else {
		// hudWidgetObj not found
		hudWidgetClass = nullptr;
	}
}

void ACustomHUD::BeginPlay() {
	Super::BeginPlay();

	if (hudWidgetClass) {
		// the player controller should be constructed by now so we can get a reference to it
		hudWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->hudWidgetClass);
		hudWidget->AddToViewport();
	}
}